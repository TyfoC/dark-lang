#include <dark-preprocessor.hpp>
#include <iostream>

size_t Dark::Preprocessor::Find(const std::vector<Macro> macros, const std::string name) {
	size_t count = macros.size();
	for (size_t i = 0; i < count; i++) if(macros[i].GetName() == name) return i;
	return std::string::npos;
}

std::vector<Dark::Token> Dark::Preprocessor::Preprocess(
	const std::vector<Token> tokens,
	std::vector<Message>& messages,
	std::vector<Macro>& macros,
	std::string file_directory_path,
	std::vector<std::string> include_paths,
	const std::vector<Token> lexemes
) {
	std::vector<Token> result = {}, tmp = {}, cleaned = Lexer::RemoveUseless(tokens);
	size_t count = cleaned.size();
	size_t token_type;
	std::string token_value = "";

	std::string include_path = "";
	std::ifstream input_file;
	bool found;

	size_t tmp_index;
	Macro macro = {};
	for (size_t i = 0; i < count; i++) {
		token_type = cleaned[i].GetType();
		token_value = cleaned[i].GetValue();

		if (token_type == TOKEN_TYPE_OPERATOR_PREPROCESSOR_DIRECTIVE) {
			if (++i >= count) break;

			token_type = cleaned[i].GetType();
			token_value = cleaned[i].GetValue();

			if (token_value == "set") {
				if (++i >= count || cleaned[i].GetType() != TOKEN_TYPE_IDENTIFIER) {
					messages.push_back(Message(Message::TYPE_ERROR, "missing macro name"));
					return {};
				}

				macro.SetName(cleaned[i].GetValue());
				
				if (++i >= count) {
					messages.push_back(Message(Message::TYPE_ERROR, "end of macro definition not found"));
					return {};
				}

				token_type = cleaned[i].GetType();
				token_value = cleaned[i].GetValue();

				size_t token_index;
				if (token_type == TOKEN_TYPE_OPERATOR_OPENING_PARENTHESIS) {
					macro.SetType(Macro::TYPE_EXTENDED);
				
					if (++i >= count) {
						messages.push_back(Message(Message::TYPE_ERROR, "missing closing parenthesis"));
						return {};
					}

					token_type = cleaned[i].GetType();

					tmp_index = Lexer::FindNested(cleaned, TOKEN_TYPE_OPERATOR_OPENING_PARENTHESIS, TOKEN_TYPE_OPERATOR_CLOSING_PARENTHESIS, i);
					if (tmp_index == std::string::npos) {
						messages.push_back(Message(Message::TYPE_ERROR, "missing closing parenthesis"));
						return {};
					}

					for (; i < tmp_index; i++) {
						switch(cleaned[i].GetType()) {
							case TOKEN_TYPE_IDENTIFIER:
								macro.AddArguments({ cleaned[i] });
								break;
							case TOKEN_TYPE_OPERATOR_COMMA:
								if (cleaned[i - 1].GetType() == TOKEN_TYPE_OPERATOR_COMMA) {
									messages.push_back(Message(Message::TYPE_ERROR, "missing argument in function macro definition"));
									return {};
								}
								break;
							default:
								messages.push_back(Message(Message::TYPE_ERROR, "the given expression is not an identifier"));
								return {};
						}
					}

					++i;

					if (cleaned[i].GetType() == TOKEN_TYPE_OPERATOR_OPENING_BRACE) {
						token_index = Lexer::FindNested(cleaned, TOKEN_TYPE_OPERATOR_OPENING_BRACE, TOKEN_TYPE_OPERATOR_CLOSING_BRACE, ++i);
						if (token_index == std::string::npos) {
							messages.push_back(Message(Message::TYPE_ERROR, "macro expression not completed"));
							return {};
						}
					}
					else {
						token_index = Token::Find(cleaned, TOKEN_TYPE_OPERATOR_SEMICOLON, i);
						if (token_index == std::string::npos) {
							messages.push_back(Message(Message::TYPE_ERROR, "macro expression not completed"));
							return {};
						}
					}

					for (; i < token_index; i++) macro.AddExpression({ cleaned[i] });
				}
				else {
					macro.SetType(Macro::TYPE_STANDARD);

					if (token_type == TOKEN_TYPE_OPERATOR_OPENING_BRACE) {
						token_index = Lexer::FindNested(cleaned, TOKEN_TYPE_OPERATOR_OPENING_BRACE, TOKEN_TYPE_OPERATOR_CLOSING_BRACE, ++i);
						if (token_index == std::string::npos) {
							messages.push_back(Message(Message::TYPE_ERROR, "macro expression not completed"));
							return {};
						}
					}
					else {
						token_index = Token::Find(cleaned, TOKEN_TYPE_OPERATOR_SEMICOLON, i);
						if (token_index == std::string::npos) {
							messages.push_back(Message(Message::TYPE_ERROR, "macro expression not completed"));
							return {};
						}
					}

					for (; i < token_index; i++) macro.AddExpression({ cleaned[i] });
				}
				macros.push_back(macro);
				macro = {};
			}
			else if (token_value == "sysinc") {
				if (++i >= count) break;

				token_type = cleaned[i].GetType();
				token_value = cleaned[i].GetValue();

				if (token_type != TOKEN_TYPE_STRING_LITERAL) {
					messages.push_back(Message(Message::TYPE_ERROR, "missing string literal (file path)"));
					return {};
				}

				token_value = token_value.substr(1, token_value.length() - 2);

				found = false;
				for (const std::string& directory_path : include_paths) {
					include_path = directory_path + "/" + token_value;
					input_file = std::ifstream(include_path);
					if (input_file.is_open()) {
						found = true;
						break;
					}
				}

				if (!found) {
					messages.push_back(Message(Message::TYPE_ERROR, "cannot open file: " + token_value));
					return {};
				}

				tmp = Preprocess(
					Lexer::RemoveUseless(Lexer::Lex(ReadEntireFile(input_file), lexemes)),
					messages, macros, file_directory_path, include_paths, lexemes
				);

				result.insert(result.end(), tmp.begin(), tmp.end());
			}
			else {
				messages.push_back(Message(Message::TYPE_ERROR, "unknown directive name"));
				return {};
			}
		}
		else if (token_type == TOKEN_TYPE_IDENTIFIER) {
			tmp_index = Find(macros, token_value);
			if (tmp_index == std::string::npos) {
				result.push_back(cleaned[i]);
				continue;
			}
		}
		else result.push_back(cleaned[i]);
	}

	return result;
}
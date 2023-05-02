#include <dark-preprocessor.hpp>
#include <iostream>

std::vector<std::vector<Dark::Token> > Dark::Preprocessor::GetMacroArguments(const std::vector<Token> tokens, size_t macro_opening_parenthesis_index) {
	std::vector<std::vector<Dark::Token> > macro_arguments = {};
	std::vector<Token> tmp = {};
	size_t tmp_index;

	do {
		tmp_index = Lexer::FindNested(tokens, TOKEN_TYPE_OPERATOR_COMMA, ++macro_opening_parenthesis_index);
		if (tmp_index == std::string::npos) {
			tmp_index = Lexer::FindNested(tokens, TOKEN_TYPE_OPERATOR_CLOSING_PARENTHESIS, macro_opening_parenthesis_index);
			if (tmp_index != std::string::npos) {
				for (; macro_opening_parenthesis_index < tmp_index; macro_opening_parenthesis_index++) tmp.push_back(tokens[macro_opening_parenthesis_index]);
				macro_arguments.push_back(tmp);
			}

			break;
		}

		for (; macro_opening_parenthesis_index < tmp_index; macro_opening_parenthesis_index++) tmp.push_back(tokens[macro_opening_parenthesis_index]);
		macro_arguments.push_back(tmp);
		tmp.clear();
	} while(tmp_index != std::string::npos);

	return macro_arguments;
}

std::vector<Dark::Token> Dark::Preprocessor::Expand(size_t index, const std::vector<std::vector<Token> > input_arguments, const std::vector<Macro> macros) {
	if (index >= macros.size()) return {};
	std::vector<Token> result = {}, tmp = {};
	std::vector<std::vector<Token> > macro_arguments = {};

	std::vector<Token> expression = macros[index].GetExpression();
	std::vector<Token> arguments = macros[index].GetArguments();
	size_t expression_length = expression.size();

	std::string expr_name;
	size_t tmp_index;
	for (size_t i = 0; i < expression_length; i++) {
		expr_name = expression[i].GetValue();
		switch(expression[i].GetType()) {
			case TOKEN_TYPE_IDENTIFIER:
				tmp_index = Macro::Find(macros, expr_name);
				if (tmp_index == std::string::npos) {
					tmp_index = Token::Find(arguments, expr_name);
					if (tmp_index == std::string::npos) {
						result.push_back(expression[i]);
						break;
					}

					result.insert(result.end(), input_arguments[tmp_index].begin(), input_arguments[tmp_index].end());
					break;
				}

				if (macros[tmp_index].GetType() == Macro::TYPE_STANDARD) tmp = Expand(tmp_index, input_arguments, macros);
				else {
					if (++i >= expression_length || expression[i].GetType() != TOKEN_TYPE_OPERATOR_OPENING_PARENTHESIS) return {};
					macro_arguments = GetMacroArguments(expression, i);
					if (macro_arguments.size()) {
						for (const std::vector<Token>& arg_group : macro_arguments) i += arg_group.size();
						i += macro_arguments.size();
					}
					else ++i;

					if (i >= expression_length || expression[i].GetType() != TOKEN_TYPE_OPERATOR_CLOSING_PARENTHESIS) return {};

					tmp = Expand(tmp_index, macro_arguments, macros);
				}
				
				result.insert(result.end(), tmp.begin(), tmp.end());
				break;
			default:
				result.push_back(expression[i]);
				break;
		}
	}

	return result;
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
	std::vector<std::vector<Token> > macro_arguments = {};

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

					tmp_index = Lexer::FindNestingEnd(cleaned, TOKEN_TYPE_OPERATOR_OPENING_PARENTHESIS, TOKEN_TYPE_OPERATOR_CLOSING_PARENTHESIS, i);
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
									messages.push_back(Message(Message::TYPE_ERROR, "missing argument in extended macro definition"));
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
						token_index = Lexer::FindNestingEnd(cleaned, TOKEN_TYPE_OPERATOR_OPENING_BRACE, TOKEN_TYPE_OPERATOR_CLOSING_BRACE, ++i);
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
						token_index = Lexer::FindNestingEnd(cleaned, TOKEN_TYPE_OPERATOR_OPENING_BRACE, TOKEN_TYPE_OPERATOR_CLOSING_BRACE, ++i);
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
			tmp_index = Macro::Find(macros, token_value);
			if (tmp_index == std::string::npos) {
				result.push_back(cleaned[i]);
				continue;
			}

			if (macros[tmp_index].GetType() == Macro::TYPE_STANDARD) {
				tmp = Expand(tmp_index, {}, macros);
				result.insert(result.end(), tmp.begin(), tmp.end());
			}
			else {
				if (++i >= count || cleaned[i].GetType() != TOKEN_TYPE_OPERATOR_OPENING_PARENTHESIS) {
					messages.push_back(Message(Message::TYPE_ERROR, "extended macro is used as standard"));
					return {};
				}

				macro_arguments = GetMacroArguments(cleaned, i);
				if (macro_arguments.size()) {
					for (const std::vector<Token>& arg_group : macro_arguments) i += arg_group.size();
					i += macro_arguments.size();
				}
				else ++i;

				if (i >= count || cleaned[i].GetType() != TOKEN_TYPE_OPERATOR_CLOSING_PARENTHESIS) {
					messages.push_back(Message(Message::TYPE_ERROR, "missing closing parenthesis"));
					return {};
				}

				tmp = Expand(tmp_index, macro_arguments, macros);
				result.insert(result.end(), tmp.begin(), tmp.end());
			}
		}
		else result.push_back(cleaned[i]);
	}

	return result;
}
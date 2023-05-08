#include <dark-preprocessor.hpp>
#include <iostream>

const std::vector<Dark::AlgorithmOperator> Dark::Preprocessor::Operators = {
	{ Dark::TOKEN_TYPE_OPERATOR_LOGICAL_NOT, 1 },
	{ Dark::TOKEN_TYPE_OPERATOR_BITWISE_COMPLEMENT, 1 },
	{ Dark::TOKEN_TYPE_OPERATOR_MUL, 2 },
	{ Dark::TOKEN_TYPE_OPERATOR_DIV, 2 },
	{ Dark::TOKEN_TYPE_OPERATOR_MOD, 2 },
	{ Dark::TOKEN_TYPE_OPERATOR_ADD, 3 },
	{ Dark::TOKEN_TYPE_OPERATOR_SUB, 3 },
	{ Dark::TOKEN_TYPE_OPERATOR_SHIFT_LEFT, 4 },
	{ Dark::TOKEN_TYPE_OPERATOR_SHIFT_RIGHT, 4 },
	{ Dark::TOKEN_TYPE_OPERATOR_LESS_THAN, 4 },
	{ Dark::TOKEN_TYPE_OPERATOR_GREATER_THAN, 4 },
	{ Dark::TOKEN_TYPE_OPERATOR_LESS_OR_EQUAL, 4 },
	{ Dark::TOKEN_TYPE_OPERATOR_GREATER_OR_EQUAL, 4 },
	{ Dark::TOKEN_TYPE_OPERATOR_EQUAL_TO, 5 },
	{ Dark::TOKEN_TYPE_OPERATOR_NOT_EQUAL_TO, 5 },
	{ Dark::TOKEN_TYPE_OPERATOR_BITWISE_AND, 6 },
	{ Dark::TOKEN_TYPE_OPERATOR_BITWISE_XOR, 7 },
	{ Dark::TOKEN_TYPE_OPERATOR_BITWISE_OR, 8 },
	{ Dark::TOKEN_TYPE_OPERATOR_LOGICAL_AND, 9 },
	{ Dark::TOKEN_TYPE_OPERATOR_LOGICAL_OR, 10 },
};

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
	std::vector<Token> preprocessed = Lexer::RemoveUseless(tokens), cleaned = {}, tmp = {}, sub_tmp = {};
	std::vector<std::vector<Token> > macro_arguments = {};

	size_t count = preprocessed.size();
	size_t token_type;
	std::string token_value = "";

	std::string include_path = "";
	std::ifstream input_file;
	bool found;

	size_t tmp_index;
	Macro macro = {};

	//	macro def & undef
	for (size_t i = 0; i < count; i++) {
		token_type = preprocessed[i].GetType();
		token_value = preprocessed[i].GetValue();

		if (token_type == TOKEN_TYPE_OPERATOR_PREPROCESSOR_DIRECTIVE) {
			if (++i >= count) break;

			token_type = preprocessed[i].GetType();
			token_value = preprocessed[i].GetValue();

			if (token_value == "set") {
				if (++i >= count || preprocessed[i].GetType() != TOKEN_TYPE_IDENTIFIER) {
					messages.push_back(Message(Message::TYPE_ERROR, "missing macro name"));
					return {};
				}

				macro.SetName(preprocessed[i].GetValue());
				
				if (++i >= count) {
					messages.push_back(Message(Message::TYPE_ERROR, "end of macro definition not found"));
					return {};
				}

				token_type = preprocessed[i].GetType();
				token_value = preprocessed[i].GetValue();

				size_t token_index;
				if (token_type == TOKEN_TYPE_OPERATOR_OPENING_PARENTHESIS) {
					macro.SetType(Macro::TYPE_EXTENDED);
				
					if (++i >= count) {
						messages.push_back(Message(Message::TYPE_ERROR, "missing closing parenthesis"));
						return {};
					}

					token_type = preprocessed[i].GetType();

					tmp_index = Lexer::FindNestingEnd(preprocessed, TOKEN_TYPE_OPERATOR_OPENING_PARENTHESIS, TOKEN_TYPE_OPERATOR_CLOSING_PARENTHESIS, i);
					if (tmp_index == std::string::npos) {
						messages.push_back(Message(Message::TYPE_ERROR, "missing closing parenthesis"));
						return {};
					}

					for (; i < tmp_index; i++) {
						switch(preprocessed[i].GetType()) {
							case TOKEN_TYPE_IDENTIFIER:
								macro.AddArguments({ preprocessed[i] });
								break;
							case TOKEN_TYPE_OPERATOR_COMMA:
								if (preprocessed[i - 1].GetType() == TOKEN_TYPE_OPERATOR_COMMA) {
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

					if (preprocessed[i].GetType() == TOKEN_TYPE_OPERATOR_OPENING_BRACE) {
						token_index = Lexer::FindNestingEnd(preprocessed, TOKEN_TYPE_OPERATOR_OPENING_BRACE, TOKEN_TYPE_OPERATOR_CLOSING_BRACE, ++i);
						if (token_index == std::string::npos) {
							messages.push_back(Message(Message::TYPE_ERROR, "macro expression not completed"));
							return {};
						}
					}
					else {
						token_index = Token::Find(preprocessed, TOKEN_TYPE_OPERATOR_SEMICOLON, i);
						if (token_index == std::string::npos) {
							messages.push_back(Message(Message::TYPE_ERROR, "macro expression not completed"));
							return {};
						}
					}

					for (; i < token_index; i++) macro.AddExpression({ preprocessed[i] });
				}
				else {
					macro.SetType(Macro::TYPE_STANDARD);

					if (token_type == TOKEN_TYPE_OPERATOR_OPENING_BRACE) {
						token_index = Lexer::FindNestingEnd(preprocessed, TOKEN_TYPE_OPERATOR_OPENING_BRACE, TOKEN_TYPE_OPERATOR_CLOSING_BRACE, ++i);
						if (token_index == std::string::npos) {
							messages.push_back(Message(Message::TYPE_ERROR, "macro expression not completed"));
							return {};
						}
					}
					else {
						token_index = Token::Find(preprocessed, TOKEN_TYPE_OPERATOR_SEMICOLON, i);
						if (token_index == std::string::npos) {
							messages.push_back(Message(Message::TYPE_ERROR, "macro expression not completed"));
							return {};
						}
					}

					for (; i < token_index; i++) macro.AddExpression({ preprocessed[i] });
				}

				tmp_index = Macro::Find(macros, macro.GetName());
				if (tmp_index != std::string::npos) macros.erase(macros.begin() + tmp_index);

				macros.push_back(macro);
				macro = {};
			}
			else if (token_value == "unset") {
				if (++i >= count || preprocessed[i].GetType() != TOKEN_TYPE_IDENTIFIER) {
					messages.push_back(Message(Message::TYPE_ERROR, "missing macro name"));
					return {};
				}

				tmp_index = Macro::Find(macros, preprocessed[i].GetValue());
				if (tmp_index == std::string::npos) {
					messages.push_back(Message(Message::TYPE_WARNING, "this macro is not defined"));
					continue;
				}

				macros.erase(macros.begin() + tmp_index);
			}
			else if (token_value == "exist") {
				if (++i >= count || preprocessed[i].GetType() != TOKEN_TYPE_IDENTIFIER) {
					messages.push_back(Message(Message::TYPE_ERROR, "missing macro name"));
					return {};
				}

				tmp_index = Macro::Find(macros, preprocessed[i].GetValue());
				cleaned.push_back(preprocessed[i]);
				cleaned.back().SetType(TOKEN_TYPE_DEC_LITERAL);
				cleaned.back().SetValue(tmp_index == std::string::npos ? "0" : "1");
			}
			else {
				cleaned.push_back(preprocessed[i - 1]);
				cleaned.push_back(preprocessed[i]);
			}
		}
		else if (token_type == TOKEN_TYPE_IDENTIFIER) {
			tmp_index = Macro::Find(macros, token_value);
			if (tmp_index == std::string::npos) {
				cleaned.push_back(preprocessed[i]);
				continue;
			}

			if (macros[tmp_index].GetType() == Macro::TYPE_STANDARD) {
				tmp = Expand(tmp_index, {}, macros);
				cleaned.insert(cleaned.end(), tmp.begin(), tmp.end());
			}
			else {
				if (++i >= count || preprocessed[i].GetType() != TOKEN_TYPE_OPERATOR_OPENING_PARENTHESIS) {
					messages.push_back(Message(Message::TYPE_ERROR, "extended macro is used as standard"));
					return {};
				}

				macro_arguments = GetMacroArguments(preprocessed, i);
				if (macro_arguments.size()) {
					for (const std::vector<Token>& arg_group : macro_arguments) i += arg_group.size();
					i += macro_arguments.size();
				}
				else ++i;

				if (i >= count || preprocessed[i].GetType() != TOKEN_TYPE_OPERATOR_CLOSING_PARENTHESIS) {
					messages.push_back(Message(Message::TYPE_ERROR, "missing closing parenthesis"));
					return {};
				}

				tmp = Expand(tmp_index, macro_arguments, macros);
				cleaned.insert(cleaned.end(), tmp.begin(), tmp.end());
			}
		}
		else cleaned.push_back(preprocessed[i]);
	}

	preprocessed = {};
	count = cleaned.size();
	bool is_condition_true = false;
	size_t tmp_count;
	for (size_t i = 0; i < count; i++) {
		token_type = cleaned[i].GetType();
		token_value = cleaned[i].GetValue();
		if (token_type == TOKEN_TYPE_OPERATOR_PREPROCESSOR_DIRECTIVE) {
			if (++i >= count) break;
			
			token_type = cleaned[i].GetType();
			token_value = cleaned[i].GetValue();

			if (token_value == "specinc") {
				if (++i >= count) {
					messages.push_back(Message(Message::TYPE_ERROR, "missing string literal (file path) / macro name"));
					return {};
				}

				token_type = cleaned[i].GetType();
				token_value = cleaned[i].GetValue();
				
				if (token_type != TOKEN_TYPE_STRING_LITERAL) {
					messages.push_back(Message(Message::TYPE_ERROR, "missing string literal (file path) / macro name"));
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

				input_file.close();
				preprocessed.insert(preprocessed.end(), tmp.begin(), tmp.end());
			}
			else if (token_value == "locinc") {
				if (++i >= count) {
					messages.push_back(Message(Message::TYPE_ERROR, "missing string literal (file path) / macro name"));
					return {};
				}

				token_type = cleaned[i].GetType();
				token_value = cleaned[i].GetValue();
				
				if (token_type != TOKEN_TYPE_STRING_LITERAL) {
					messages.push_back(Message(Message::TYPE_ERROR, "missing string literal (file path) / macro name"));
					return {};
				}

				token_value = token_value.substr(1, token_value.length() - 2);
				include_path = file_directory_path + "/" + token_value;

				input_file = std::ifstream(include_path);
				if (!input_file.is_open()) {
					messages.push_back(Message(Message::TYPE_ERROR, "cannot open file: " + include_path));
					return {};
				}

				tmp = Preprocess(
					Lexer::RemoveUseless(Lexer::Lex(ReadEntireFile(input_file), lexemes)),
					messages, macros, file_directory_path, include_paths, lexemes
				);

				input_file.close();
				preprocessed.insert(preprocessed.end(), tmp.begin(), tmp.end());
			}
			else if (token_value == "specraw") {
				if (++i >= count) {
					messages.push_back(Message(Message::TYPE_ERROR, "missing string literal (file path) / macro name"));
					return {};
				}

				token_type = cleaned[i].GetType();
				token_value = cleaned[i].GetValue();
				
				if (token_type != TOKEN_TYPE_STRING_LITERAL) {
					messages.push_back(Message(Message::TYPE_ERROR, "missing string literal (file path) / macro name"));
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

				preprocessed.push_back(Token(TOKEN_TYPE_RAW_DATA, ReadEntireFile(input_file)));
				input_file.close();
			}
			else if (token_value == "locraw") {
				if (++i >= count) {
					messages.push_back(Message(Message::TYPE_ERROR, "missing string literal (file path) / macro name"));
					return {};
				}

				token_type = cleaned[i].GetType();
				token_value = cleaned[i].GetValue();
				
				if (token_type != TOKEN_TYPE_STRING_LITERAL) {
					messages.push_back(Message(Message::TYPE_ERROR, "missing string literal (file path) / macro name"));
					return {};
				}

				token_value = token_value.substr(1, token_value.length() - 2);
				include_path = file_directory_path + "/" + token_value;

				input_file = std::ifstream(include_path);
				if (!input_file.is_open()) {
					messages.push_back(Message(Message::TYPE_ERROR, "cannot open file: " + include_path));
					return {};
				}

				preprocessed.push_back(Token(TOKEN_TYPE_RAW_DATA, ReadEntireFile(input_file)));
				input_file.close();
			}
			else if (token_value == "if") {
				if (++i >= count) {
					messages.push_back(Message(Message::TYPE_ERROR, "condition not specified"));
					return {};
				}

				token_type = cleaned[i].GetType();
				token_value = cleaned[i].GetValue();

				if (token_type != TOKEN_TYPE_OPERATOR_OPENING_PARENTHESIS) {
					messages.push_back(Message(Message::TYPE_ERROR, "condition not specified"));
					return {};
				}

				tmp_index = Lexer::FindNested(cleaned, TOKEN_TYPE_OPERATOR_CLOSING_PARENTHESIS, ++i);
				if (tmp_index == std::string::npos) {
					messages.push_back(Message(Message::TYPE_ERROR, "missing closing parenthesis"));
					return {};
				}

				tmp.clear();
				for (; i < tmp_index; i++) tmp.push_back(cleaned[i]);
				tmp = InfixToPostfix(tmp, Operators);

				is_condition_true = EvaluatePostfix(tmp);

				if (++i >= count) {
					messages.push_back(Message(Message::TYPE_ERROR, "condition not completed"));
					return {};
				}

				token_type = cleaned[i].GetType();
				token_value = cleaned[i].GetValue();

				if (token_type == TOKEN_TYPE_OPERATOR_OPENING_BRACE) {
					tmp_index = Lexer::FindNestingEnd(cleaned, TOKEN_TYPE_OPERATOR_OPENING_BRACE, TOKEN_TYPE_OPERATOR_CLOSING_BRACE, ++i);
					if (tmp_index == std::string::npos) {
						messages.push_back(Message(Message::TYPE_ERROR, "condition not completed"));
						return {};
					}
				}
				else {
					tmp_index = Token::Find(cleaned, TOKEN_TYPE_OPERATOR_SEMICOLON, i);
					if (tmp_index == std::string::npos) {
						messages.push_back(Message(Message::TYPE_ERROR, "condition not completed"));
						return {};
					}
				}

				if (is_condition_true) {
					tmp.clear();
					for (; i < tmp_index; i++) tmp.push_back(cleaned[i]);
					tmp = Preprocess(tmp, messages, macros, file_directory_path, include_paths, lexemes);
					preprocessed.insert(preprocessed.end(), tmp.begin(), tmp.end());
				}
				else i = tmp_index;
			}
			else if (token_value == "else") {
				if (++i >= count) {
					messages.push_back(Message(Message::TYPE_ERROR, "condition not completed"));
					return {};
				}

				token_type = cleaned[i].GetType();
				token_value = cleaned[i].GetValue();

				if (token_type == TOKEN_TYPE_OPERATOR_OPENING_BRACE) {
					tmp_index = Lexer::FindNestingEnd(cleaned, TOKEN_TYPE_OPERATOR_OPENING_BRACE, TOKEN_TYPE_OPERATOR_CLOSING_BRACE, ++i);
					if (tmp_index == std::string::npos) {
						messages.push_back(Message(Message::TYPE_ERROR, "condition not completed"));
						return {};
					}
				}
				else {
					tmp_index = Token::Find(cleaned, TOKEN_TYPE_OPERATOR_SEMICOLON, i);
					if (tmp_index == std::string::npos) {
						messages.push_back(Message(Message::TYPE_ERROR, "condition not completed"));
						return {};
					}
				}
				
				if (!is_condition_true) {
					tmp.clear();
					for (; i < tmp_index; i++) tmp.push_back(cleaned[i]);
					tmp = Preprocess(tmp, messages, macros, file_directory_path, include_paths, lexemes);
					preprocessed.insert(preprocessed.end(), tmp.begin(), tmp.end());
				}
				else i = tmp_index;
			}
			else if (token_value == "repeat") {
				if (++i >= count) {
					messages.push_back(Message(Message::TYPE_ERROR, "missing number of times"));
					return {};
				}

				if (!IsNumber(cleaned[i])) {
					messages.push_back(Message(Message::TYPE_ERROR, "missing number of times"));
					return {};
				}

				tmp_count = GetUlong(cleaned[i]);

				if (++i >= count) {
					messages.push_back(Message(Message::TYPE_ERROR, "missing expression"));
					return {};
				}

				token_type = cleaned[i].GetType();
				token_value = cleaned[i].GetValue();

				if (token_type == TOKEN_TYPE_OPERATOR_OPENING_BRACE) {
					tmp_index = Lexer::FindNestingEnd(cleaned, TOKEN_TYPE_OPERATOR_OPENING_BRACE, TOKEN_TYPE_OPERATOR_CLOSING_BRACE, ++i);
					if (tmp_index == std::string::npos) {
						messages.push_back(Message(Message::TYPE_ERROR, "expression not completed"));
						return {};
					}
				}
				else {
					tmp_index = Token::Find(cleaned, TOKEN_TYPE_OPERATOR_SEMICOLON, i);
					if (tmp_index == std::string::npos) {
						messages.push_back(Message(Message::TYPE_ERROR, "expression not completed"));
						return {};
					}
				}

				tmp.clear();
				for (; i < tmp_index; i++) tmp.push_back(cleaned[i]);

				for (; tmp_count; tmp_count--) preprocessed.insert(preprocessed.end(), tmp.begin(), tmp.end());
			}
			else {
				messages.push_back(Message(Message::TYPE_ERROR, "unknown directive name"));
				return {};
			}
		}
		else preprocessed.push_back(cleaned[i]);
	}

	return preprocessed;
}
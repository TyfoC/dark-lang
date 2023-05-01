#include <dark-lexer.hpp>

const std::string Dark::Lexer::NewLineExpression = R"((\r\n|\n\r|\n|\r))";

std::vector<Dark::Token> Dark::Lexer::RemoveUseless(const std::vector<Token> tokens) {
	size_t token_type;
	std::vector<Token> result = {};
	for (const Token& token : tokens) {
		token_type = token.GetType();
		if (token_type != TOKEN_TYPE_SPACE && token_type != TOKEN_TYPE_NEW_LINE && token_type != TOKEN_TYPE_COMMENT_BLOCK) {
			result.push_back(token);
		}
	}

	return result;
}

size_t Dark::Lexer::FindNested(const std::vector<Token> tokens, size_t type_start, size_t type_end, size_t start_index) {
	size_t count = tokens.size(), nested_level = 0, token_type;
	for (; start_index < count; start_index++) {
		token_type = tokens[start_index].GetType();
		if (token_type == type_start) ++nested_level;
		else if (token_type == type_end) {
			if (nested_level) --nested_level;
			else return start_index;
		}
	}

	return std::string::npos;
}

std::vector<Dark::Token> Dark::Lexer::Lex(const std::string source, const std::vector<Token> lexemes) {
	size_t source_length = source.length(), position = 0, value_length;
	std::string destination = source;
	std::vector<Token> result = {};
	std::smatch expression_match;
	std::regex expression;
	bool expression_found;
	while (position < source_length) {
		expression_found = false;
		for (const Token& lexeme : lexemes) {
			expression = std::regex(lexeme.GetValue());
			if (std::regex_search(destination, expression_match, expression) && !expression_match.position()) {
				result.push_back({ lexeme.GetType(), expression_match.str() });
				value_length = result.back().GetValue().length();
				position += value_length;
				expression_found = true;
				break;
			}
		}

		if (!expression_found) {
			value_length = 1;
			result.push_back({ std::string::npos, std::to_string(position) });
			++position;
		}

		destination = destination.substr(expression_found ? value_length : 1);
	}

	return result;
}
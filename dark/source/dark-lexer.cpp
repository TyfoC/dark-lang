#include <dark-lexer.hpp>

const std::string Dark::Lexer::NewLineExpression = R"((\r\n|\n\r|\n|\r))";

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
				result.push_back({ lexeme.GetTypeIndex(), expression_match.str() });
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
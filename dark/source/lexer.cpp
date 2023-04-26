#include <lexer.hpp>

std::vector<Dark::Lexer::Token_t> Dark::Lexer::Lex(const std::string& source, const std::vector<Lexeme_t>& lexemes) {
	const size_t source_length = source.length();
	std::vector<Token_t> tokens = {};
	std::string copied_source(source);
	
	size_t position = 0;
	bool expr_found;
	std::regex reg_expr;
	std::smatch reg_match;
	while (position < source_length) {
		expr_found = false;
		for (const  Lexeme_t& lexeme : lexemes) {
			reg_expr = std::regex(lexeme.Value);
			if (std::regex_search(copied_source, reg_match, reg_expr) && !reg_match.position()) {
				tokens.push_back({ lexeme.Type, reg_match.str() });
				position += tokens.back().Value.length();
				expr_found = true;
				break;
			}
		}

		if (!expr_found) {
			tokens.push_back({ TokenTypeUndefined, std::to_string(position) });
			++position;
		}

		copied_source = copied_source.substr(expr_found ? tokens.back().Value.length() : 1);
	}

	return tokens;
}
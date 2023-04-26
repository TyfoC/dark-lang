#include <lexer.hpp>

size_t Dark::Lexer::Token_t::GetIndexExcludingTypes(const std::vector<Token_t>& values, const std::vector<size_t> types, size_t start_index) {
	bool type_found;
	const size_t values_count = values.size(), types_count = types.size();
	for (size_t i = start_index; i < values_count; i++) {
		type_found = false;
		for (size_t j = 0; j < types_count; j++) if (values[i].Type == types[j]) {
			type_found = true;
			break;
		}

		if (!type_found) return i;
	}

	return std::string::npos;
}

size_t Dark::Lexer::Token_t::GetIndexByTypes(const std::vector<Token_t>& values, const std::vector<size_t> types, size_t start_index) {
	const size_t values_count = values.size(), types_count = types.size();
	for (size_t i = start_index; i < values_count; i++) for (size_t j = 0; j < types_count; j++) if (values[i].Type == types[j]) return i;
	return std::string::npos;
}

std::vector<Dark::Lexer::Token_t> Dark::Lexer::Lex(const std::string& source, const std::vector<Lexeme_t>& lexemes) {
	const size_t source_length = source.length();
	std::vector<Token_t> tokens = {};
	std::string copied_source(source);
	
	size_t position = 0, value_length;
	bool expr_found;
	std::regex reg_expr;
	std::smatch reg_match;
	const std::regex new_line_expr = std::regex(R"((\r\n|\n\r|\r|\n))");
	std::smatch new_line_match;
	while (position < source_length) {
		expr_found = false;
		for (const  Lexeme_t& lexeme : lexemes) {
			reg_expr = std::regex(lexeme.Pattern);
			if (std::regex_search(copied_source, reg_match, reg_expr) && !reg_match.position()) {
				tokens.push_back({ lexeme.Type, reg_match.str() });
				value_length = tokens.back().Value.length();
				position += value_length;
				expr_found = true;
				break;
			}
		}

		if (!expr_found) {
			value_length = 1;
			tokens.push_back({ TokenTypeUndefined, std::to_string(position) });
			++position;
		}

		copied_source = copied_source.substr(expr_found ? value_length : 1);
	}

	return tokens;
}
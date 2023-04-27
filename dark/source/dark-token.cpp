#include <dark-token.hpp>

Dark::Token::Token() {
	m_type_index = std::string::npos;
	m_value = "";
}

Dark::Token::Token(size_t type_index, const std::string value) {
	m_type_index = type_index;
	m_value = value;
}

Dark::Token& Dark::Token::SetTypeIndex(size_t type_index) {
	m_type_index = type_index;
	return *this;
}

Dark::Token& Dark::Token::SetValue(const std::string value) {
	m_value = value;
	return *this;
}

size_t Dark::Token::GetTypeIndex() const {
	return m_type_index;
}

std::string Dark::Token::GetValue() const {
	return m_value;
}

std::vector<Dark::Token> Dark::Token::Concat(const std::vector<Token> first, const std::vector<Token> second) {
	std::vector<Token> result = {};
	result.insert(result.end(), first.begin(), first.end());
	result.insert(result.end(), second.begin(), second.end());
	return result;
}

size_t Dark::Token::GetIndexFromVector(const std::vector<Token> tokens, const Token search_token, bool check_value, bool check_type, size_t start_index) {
	if (!check_value && !check_type) return std::string::npos;
	const size_t tokens_count = tokens.size();

	if (check_value && check_type) {
		for (; start_index < tokens_count; start_index++) {
			if (tokens[start_index].m_value == search_token.m_value && tokens[start_index].m_type_index == search_token.m_type_index) return start_index;
		}
	}
	else if (check_value) for (; start_index < tokens_count; start_index++) {
		if (tokens[start_index].m_value == search_token.m_value) return start_index;
	}
	else for (; start_index < tokens_count; start_index++) if (tokens[start_index].m_type_index == search_token.m_type_index) return start_index;

	return std::string::npos;
}

/**
 * @param tokens				input tokens
 * @param excluded_type_indexes	types that are excluded from the search
 * @param included_type_indexes	types that are included in the search (included_type_indexes[0] == std::string::npos => all types included)
 * @param start_index			the index of the token from which the search starts
*/
size_t Dark::Token::GetNextIndex(const std::vector<Token> tokens, const std::vector<size_t> excluded_type_indexes, const std::vector<size_t> included_type_indexes, size_t start_index) {
	//	it is impossible to search for characters, if all of them are excluded
	if (excluded_type_indexes[0] == std::string::npos) return std::string::npos;

	bool found;
	size_t j, tokens_count = tokens.size(), included_types_count = included_type_indexes.size(), excluded_types_count = excluded_type_indexes.size();
	if (included_types_count == std::string::npos) {			//	all type indexes included
		for (; start_index < tokens_count; start_index++) {
			found = false;
			for (j = 0; j < excluded_types_count; j++) if (excluded_type_indexes[j] == tokens[start_index].m_type_index) {
				found = true;
				break;
			}

			if (!found) return start_index;
		}
	}
	else {
		for (; start_index < tokens_count; start_index++) {
			found = false;
			for (j = 0; j < excluded_types_count; j++) if (excluded_type_indexes[j] == tokens[start_index].m_type_index) {
				found = true;
				break;
			}

			if (found) continue;

			found = false;
			for (j = 0; j < included_types_count; j++) if (included_type_indexes[j] == tokens[start_index].m_type_index) {
				found = true;
				break;
			}

			if (found) return start_index;
		}
	}

	return std::string::npos;
}
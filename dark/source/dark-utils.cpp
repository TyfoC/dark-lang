#include <dark-utils.hpp>

size_t Dark::GetFileSize(std::ifstream& input_file) {
	input_file.seekg(0, std::ios::end);
	size_t result = input_file.tellg();
	input_file.seekg(0, std::ios::beg);
	return result;
}

std::string Dark::ReadEntireFile(std::ifstream& input_file) {
	size_t file_size = GetFileSize(input_file);
	std::string result;
	result.reserve(file_size);
	result.assign((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
	return result;
}

std::string Dark::GetFileDirectory(const std::string input_path) {
	std::string result = input_path;
	size_t position = result.rfind('/');
	if (position == std::string::npos) {
		position = result.rfind('\\');
		if (position == std::string::npos) return "";
	}

	return result.substr(0, position);
}

std::string Dark::Format(const std::vector<Token> tokens) {
	std::string result = "", token_value = "";
	size_t token_type, tab_level = 0, count = tokens.size();
	for (size_t i = 0; i < count; i++) {
		token_type = tokens[i].GetType();
		token_value = tokens[i].GetValue();

		if (token_type == TOKEN_TYPE_OPERATOR_OPENING_BRACE) {
			result += token_value + '\n';
			++tab_level;
			for (size_t i = 0; i < tab_level; i++) result += '\t';
		}
		else if (token_type == TOKEN_TYPE_OPERATOR_CLOSING_BRACE) {
			result += token_value + '\n';
			if (tab_level) --tab_level;
			for (size_t i = 0; i < tab_level; i++) result += '\t';
		}
		else if (token_type == TOKEN_TYPE_OPERATOR_OPENING_PARENTHESIS ||
			token_type == TOKEN_TYPE_OPERATOR_OPENING_BRACKET) {
				if (i && tokens[i - 1].GetType() == TOKEN_TYPE_IDENTIFIER) result.erase(result.end() - 1);
				result += token_value;
			}
		else if (token_type == TOKEN_TYPE_OPERATOR_CLOSING_PARENTHESIS ||
			token_type == TOKEN_TYPE_OPERATOR_CLOSING_BRACKET) {
			if (i && result.back() == ' ') result.erase(result.end() - 1);
			result += token_value + ' ';
		}
		else if (token_type == TOKEN_TYPE_OPERATOR_SEMICOLON) {
			if (i && result.back() == ' ') result.erase(result.end() - 1);
			result += token_value + '\n';
			for (size_t i = 0; i < tab_level; i++) result += '\t';
		}
		else if (token_type == TOKEN_TYPE_OPERATOR_COMMA) result += ", ";
		else result += token_value + ' ';

		if (i < count - 1 && tokens[i + 1].GetType() == TOKEN_TYPE_OPERATOR_CLOSING_BRACE) result.erase(result.end() - 1);
	}

	return result;
}
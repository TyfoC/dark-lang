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
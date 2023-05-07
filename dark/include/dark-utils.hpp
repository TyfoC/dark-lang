#pragma once
#ifndef DARK_UTILS_HPP
#define DARK_UTILS_HPP

#include <vector>
#include <string>
#include <fstream>

namespace Dark {
	template<typename T> std::vector<T> Find(const std::vector<T> values, const T value, size_t start_index = 0) {
		const size_t count = values.size();
		for (; start_index < count; start_index++) if (values[start_index] == value) return start_index;
		return std::string::npos;
	}

	template<typename T> bool Contains(const std::vector<T> values, const T value, size_t start_index = 0) {
		const size_t count = values.size();
		for (; start_index < count; start_index++) if (values[start_index] == value) return true;
		return false;
	}

	size_t GetFileSize(std::ifstream& input_file);
	std::string ReadEntireFile(std::ifstream& input_file);
	std::string GetFileDirectory(const std::string input_path);
}

#endif
#pragma once
#ifndef DARK_UTILS_HPP
#define DARK_UTILS_HPP

#include <vector>
#include <string>
#include <fstream>

namespace Dark {
	template<typename T> std::vector<T> Find(const std::vector<T> values, const T value) {
		const size_t count = values.size();
		for (size_t i = 0; i < count; i++) if (values[i] == value) return i;
		return std::string::npos;
	}

	template<typename T> bool Contains(const std::vector<T> values, const T value) {
		const size_t count = values.size();
		for (size_t i = 0; i < count; i++) if (values[i] == value) return true;
		return false;
	}

	size_t GetFileSize(std::ifstream& input_file);
	std::string ReadEntireFile(std::ifstream& input_file);
}

#endif
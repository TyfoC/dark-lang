#pragma once
#ifndef DARK_TOKEN_HPP
#define DARK_TOKEN_HPP

#include <string>
#include <vector>

namespace Dark {
	class Token {
		public:
		Token();
		Token(size_t type_index = std::string::npos, const std::string value = "");
		Token& SetTypeIndex(size_t type_index = std::string::npos);
		Token& SetValue(const std::string value = "");
		size_t GetTypeIndex() const;
		std::string GetValue() const;

		static std::vector<Token> Concat(const std::vector<Token> first, const std::vector<Token> second);
		static size_t GetIndexFromVector(const std::vector<Token> tokens, const Token search_token, bool check_value = true, bool check_type = true, size_t start_index = 0);
		static size_t GetNextIndex(const std::vector<Token> tokens, const std::vector<size_t> excluded_type_indexes = {}, const std::vector<size_t> included_type_indexes = { std::string::npos }, size_t start_index = 0);
		protected:
		size_t		m_type_index;
		std::string	m_value;
	};
}

#endif
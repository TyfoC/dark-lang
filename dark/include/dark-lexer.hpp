#pragma once
#ifndef DARK_LEXER_HPP
#define DARK_LEXER_HPP

#include <regex>
#include "dark-config.hpp"

namespace Dark {
	class Lexer {
		public:
		static const std::string NewLineExpression;

		static std::vector<Token> RemoveUseless(const std::vector<Token> tokens);
		static size_t FindNested(const std::vector<Token> tokens, size_t type_start, size_t type_end, size_t start_index = 0);
		static std::vector<Token> Lex(const std::string source, const std::vector<Token> lexemes = Lexemes);
	};
}

#endif
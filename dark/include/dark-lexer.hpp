#pragma once
#ifndef DARK_LEXER_HPP
#define DARK_LEXER_HPP

#include <regex>
#include "dark-config.hpp"

namespace Dark {
	class Lexer {
		public:
		static const std::string NewLineExpression;

		static std::vector<Token> Lex(const std::string source, const std::vector<Token> lexemes = Lexemes);
	};
}

#endif
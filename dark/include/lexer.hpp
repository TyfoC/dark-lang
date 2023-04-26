#pragma once
#ifndef __LEXER__
#define __LEXER__

#include <string>
#include <vector>
#include <regex>

namespace Dark {
	class Lexer {
		public:
		static constexpr size_t TokenTypeUndefined = std::string::npos;

		typedef struct Token_t {
			size_t		Type;		//	user-defined (except token_type_undefined)
			std::string	Value;		//	regex
		} Token_t, Lexeme_t;

		static std::vector<Token_t> Lex(const std::string& source, const std::vector<Lexeme_t>& lexemes);
	};
}

#endif
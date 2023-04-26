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

		typedef struct Lexeme_t {
			size_t		Type;
			std::string	Pattern;
		} Lexeme_t;

		typedef struct Token_t {
			static size_t GetIndexExcludingTypes(const std::vector<Token_t>& values, const std::vector<size_t> types, size_t start_index = 0);
			static size_t GetIndexByTypes(const std::vector<Token_t>& values, const std::vector<size_t> types, size_t start_index = 0);

			size_t		Type;		//	user-defined (except token_type_undefined)
			std::string	Value;		//	regex
		} Token_t;

		static std::vector<Token_t> Lex(const std::string& source, const std::vector<Lexeme_t>& lexemes);
	};
}

#endif
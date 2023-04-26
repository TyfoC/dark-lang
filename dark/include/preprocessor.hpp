#pragma once
#ifndef __PREPROCESSOR__
#define __PREPROCESSOR__

#include "common.hpp"

namespace Dark {	
	class Preprocessor {
		public:
		enum SymbolType_t : size_t {
			MACRO_CONSTANT,
			MACRO_FUNCTION
		};

		typedef struct Symbol_t {
			SymbolType_t	Type;
			std::string		Name;
			std::string		Value;
		} Symbol_t;

		static std::vector<Lexer::Token_t> Preprocess(std::vector<Message_t>& messages, const std::vector<Lexer::Token_t>& tokens, std::vector<Symbol_t>& symbols = EmptySymTable, const std::vector<Lexer::Lexeme_t>& lexemes = Lexemes);
		static std::vector<Symbol_t> EmptySymTable;
	};
}

#endif
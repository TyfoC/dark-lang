#pragma once
#ifndef DARK_PREPROCESSOR_HPP
#define DARK_PREPROCESSOR_HPP

#define DARK_PREPROCESSOR_VERSION	110	//	1.1.0

#include "dark-lexer.hpp"
#include "dark-algorithms.hpp"
#include "dark-utils.hpp"

namespace Dark {
	class Preprocessor {
		public:
		static const std::vector<AlgorithmOperator> Operators;

		static std::vector<std::vector<Token> > GetMacroArguments(const std::vector<Token> tokens, size_t macro_opening_parenthesis_index);
		static std::vector<Token> Expand(size_t index, const std::vector<std::vector<Token> > input_arguments, const std::vector<Macro> macros);

		static std::vector<Token> Preprocess(
			const std::vector<Token> tokens,
			std::vector<Message>& messages,
			std::vector<Macro>& macros,
			std::string file_directory_path = "",
			std::vector<std::string> include_paths = {},
			const std::vector<Token> lexemes = Lexemes
		);
	};
}

#endif
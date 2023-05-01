#pragma once
#ifndef DARK_PREPROCESSOR_HPP
#define DARK_PREPROCESSOR_HPP

#include "dark-utils.hpp"
#include "dark-config.hpp"
#include "dark-lexer.hpp"

namespace Dark {
	class Preprocessor {
		public:
		static size_t Find(const std::vector<Macro> macros, const std::string name);

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
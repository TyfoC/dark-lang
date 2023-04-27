#pragma once
#ifndef DARK_PREPROCESSOR_HPP
#define DARK_PREPROCESSOR_HPP

#include "dark-message.hpp"
#include "dark-config.hpp"

namespace Dark {
	class Preprocessor {
		public:
		static std::vector<Token> Preprocess(
			const std::vector<Token> tokens,
			std::vector<Message>& messages,
			std::string file_directory_path = "",
			std::vector<std::string> system_include_paths = {},
			const std::vector<Token> lexemes = Lexemes
		);
	};
}

#endif
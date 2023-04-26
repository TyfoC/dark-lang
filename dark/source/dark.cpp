#include <dark.hpp>

int main(int argc, char** argv) {
	if (argc == 1) {
		if (!strcmp("help", argv[1])) {
			std::cout << "Dark programking language ver. 04-25-2023. Usage:" << std::endl;
			std::cout << "\t" << "dark help - show help" << std::endl;
			std::cout << "\t" << "dark preprocess <file in> <file out>" << std::endl;
		}
	}
	else if (argc >= 2) {
		if (!strcmp("preprocess", argv[1])) {
			std::ifstream file_in(argv[2]);
			if (!file_in.is_open()) {
				std::cout << "Failed to open input file `" << argv[2] << "`!" << std::endl;
				return 1;
			}

			std::string source_in;
			file_in.seekg(0, std::ios::end);   
			source_in.reserve(file_in.tellg());
			file_in.seekg(0, std::ios::beg);
			source_in.assign((std::istreambuf_iterator<char>(file_in)), std::istreambuf_iterator<char>());

			std::cout << "Tokens:" << std::endl;
			std::vector<Dark::Lexer::Token_t> tokens = Dark::Lexer::Lex(source_in, Dark::Lexemes);
			for (const Dark::Lexer::Token_t& token : tokens) {
				std::cout << "Type: " << token.Type << ", Value: `" << token.Value << '`' <<  std::endl;
			}

			file_in.close();
		}
	}
	else std::cout << "Wrong command line arguments! Run \"dark help\" to get info." << std::endl;
}
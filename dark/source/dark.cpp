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

			std::vector<Dark::Message_t> messages = {};
			std::vector<Dark::Lexer::Token_t> tokens = Dark::Lexer::Lex(source_in, Dark::Lexemes);
			tokens = Dark::Preprocessor::Preprocess(messages, tokens);

			bool errors_exist = false;
			std::cout << "Preprocessor messages:" << std::endl;
			for (const Dark::Message_t& message : messages) {
				if (message.Type == Dark::ERROR) {
					std::cout << "Error ";
					errors_exist = true;
				}
				else if (message.Type == Dark::WARNING) std::cout << "Warning ";
				else std::cout << "Information ";
				std::cout << "(line " << message.Line << "): " << message.Text << std::endl;
			}

			std::cout << "Tokens:" << std::endl;
			for (const Dark::Lexer::Token_t& token : tokens) {
				std::cout << "Type: " << token.Type << ", Value: `" << token.Value << '`' <<  std::endl;
			}

			if (errors_exist) {
				std::cout << "Preprocessing terminated" << std::endl;
				return 2;
			}

			/*
			std::cout << "Preprocessor symbol table:" << std::endl;
			for (const Dark::Preprocessor::Symbol_t& symbol : Dark::Preprocessor::EmptySymTable) {
				std::cout << "Type: " << symbol.Type << std::endl;
				std::cout << "Name: " << symbol.Name << std::endl;
				std::cout << "Value: " << symbol.Value << std::endl;
			}
			*/

			file_in.close();
		}
	}
	else std::cout << "Wrong command line arguments! Run \"dark help\" to get info." << std::endl;
}
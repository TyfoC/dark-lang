#include <iostream>
#include <fstream>
#include <shell-argument.hpp>
#include <dark-utils.hpp>
#include <dark-preprocessor.hpp>

int main(int argc, char** argv) {
	std::string raw_arguments = "";
	for (size_t i = 1; i < (size_t)argc; i++) raw_arguments += std::string(&argv[i][0]) + " ";
	std::vector<Shell::Argument> arguments = Shell::Argument::Process(raw_arguments);

	char operation = ' ';
	size_t argument_type;
	std::string argument_name = "";
	std::vector<std::string> argument_values = {};
	std::vector<std::string> input_paths = {};
	std::vector<std::string> include_paths = {};
	std::vector<Dark::Message> messages = {};
	std::vector<Dark::Macro> macros = {};
	for (const Shell::Argument& argument : arguments) {
		argument_type = argument.GetType();
		argument_name = argument.GetName();
		argument_values = argument.GetValues();

		if (argument_type == Shell::Argument::TYPE_PARAMETER) {
			if (argument_name == "I") for (const std::string& entry : argument_values) include_paths.push_back(entry);
		}
		else if (argument_type == Shell::Argument::TYPE_OPTION) {
			if (argument_name == "p") {
				if (operation != ' ') std::cout << "Warning: operation already selected: `-" << operation << "`!" << std::endl;
				else operation = argument_name[0];
			}
		}
		else input_paths.push_back(argument_name);
	}

	if (operation == 'p') {
		for (const std::string& input_path : input_paths) {
			std::ifstream input_file(input_path);
			if (!input_file.is_open()) {
				std::cout << "Error: failed to open `" << input_path << "`!" << std::endl;
				return 2;
			}

			std::vector<Dark::Token> tokens = Dark::Lexer::Lex(Dark::ReadEntireFile(input_file), Dark::Lexemes);
			tokens = Dark::Preprocessor::Preprocess(tokens, messages, macros, Dark::GetFileDirectory(input_path), include_paths, Dark::Lexemes);

			if (messages.size()) {
				bool found_error = false;
				std::cout << "Messages:" << std::endl;
				for (const Dark::Message& message : messages) {
					std::cout << message.FormString() << std::endl;
					if (message.GetType() == Dark::Message::TYPE_ERROR && !found_error) found_error = true;
				}

				if (found_error) {
					std::cout << "Errors found => action terminated!" << std::endl;
					input_file.close();
				}
			}

			std::cout << "Tokens:" << std::endl;
			for (const Dark::Token& token : tokens) std::cout << "Name: " << token.GetValue() << ", Type: " << token.GetType() << std::endl;
			input_file.close();
		}
	}
	else {
		std::cout << "Error: unknown operation: `-" << argument_name << "`!" << std::endl;
		return 1;
	}
}
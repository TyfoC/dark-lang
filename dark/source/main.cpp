#include <iostream>
#include <shell-argument.hpp>

int main(int argc, char** argv) {
	std::string raw_arguments = "";
	for (size_t i = 0; i < (size_t)argc; i++) raw_arguments += std::string(&argv[i][0]) + " ";
	std::vector<Shell::Argument> arguments = Shell::Argument::Process(raw_arguments);
}
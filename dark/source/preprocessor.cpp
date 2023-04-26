#include <preprocessor.hpp>

std::vector<Dark::Preprocessor::Symbol_t> Dark::Preprocessor::EmptySymTable = {};

std::vector<Dark::Lexer::Token_t> Dark::Preprocessor::Preprocess(std::vector<Message_t>& messages, const std::vector<Lexer::Token_t>& tokens, std::vector<Symbol_t>& symbols, const std::vector<Lexer::Lexeme_t>& lexemes) {
	std::vector<Lexer::Token_t> result = {}, tmp = {};
	Symbol_t symbol = {};
	size_t index, line = 1;
	const size_t tokens_count = tokens.size();
	for (size_t i = 0; i < tokens_count; i++) {
		if (tokens[i].Type == SPACE || tokens[i].Type == NEW_LINE || tokens[i].Type == COMMENTARY) {
			++line;
			continue;
		}
		else if (tokens[i].Type == OPERATOR_PREPROCESSOR_DIRECTIVE) {
			index = Lexer::Token_t::GetIndexExcludingTypes(tokens, { SPACE, NEW_LINE }, i + 1);
			if (index == std::string::npos || tokens[index].Type != IDENTIFIER) {
				messages.push_back({ ERROR, line, "Preprocessor directive omitted" });
				continue;
			}
			i = index;

			if (tokens[i].Value == "set") {
				index = Lexer::Token_t::GetIndexExcludingTypes(tokens, { SPACE }, i + 1);
				if (index == std::string::npos) {
					messages.push_back({ ERROR, line, "Preprocessor directive `set` not completed" });
					continue;
				}
				i = index;

				symbol.Type = MACRO_CONSTANT;
				symbol.Name = tokens[i].Value;
				symbol.Value = "";
				index = Lexer::Token_t::GetIndexByTypes(tokens, { NEW_LINE }, index);
				if (index == std::string::npos) index = tokens_count;
				for (size_t j = i + 1; j < index; j++) tmp.push_back(tokens[j]);
				tmp = Preprocess(messages, tmp, symbols, lexemes);
				for (const Lexer::Token_t& token : tmp) symbol.Value += token.Value;
				i += tmp.size();
				tmp.clear();
				symbols.push_back(symbol);
			}
			else {
				messages.push_back({ ERROR, line, "Unknown preprocessor directive" });
				continue;
			}
		}
		else result.push_back(tokens[i]);
	}

	return result;
}
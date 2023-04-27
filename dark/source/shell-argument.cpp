#include <shell-argument.hpp>

const std::vector<Dark::Token> Shell::Argument::Lexemes = {
	{ Shell::Argument::TOKEN_TYPE_OPERATOR_SPACE, R"([ \t\v\r\n\f]+)" },
	{ Shell::Argument::TOKEN_TYPE_OPERATOR_PRIORITY, R"(-)" },
	{ Shell::Argument::TOKEN_TYPE_OPERATOR_ASSIGN, R"(=)" },
	{ Shell::Argument::TOKEN_TYPE_OPERATOR_COMMA, R"(,)" },
	{ Shell::Argument::TOKEN_TYPE_VALUE, R"([\w~`!@"#№\$;%\^\:&\?\*\(\)-\+\[\]\{\}'\|\\\/<>.]+)" },
	{ Shell::Argument::TOKEN_TYPE_EXTENDED_VALUE, R"("(\\[\s\S]|[^\"])*")" },
};

Shell::Argument::Argument(Type type, const std::string name, const std::vector<std::string> values) {
	m_type = type;
	m_name = name;
	m_values = values;
}

Shell::Argument& Shell::Argument::SetType(Type type) {
	m_type = type;
	return *this;
}

Shell::Argument& Shell::Argument::SetName(const std::string name) {
	m_name = name;
	return *this;
}

Shell::Argument& Shell::Argument::SetValues(const std::vector<std::string> values) {
	m_values = values;
	return *this;
}

Shell::Argument& Shell::Argument::AppendValues(const std::vector<std::string> values) {
	m_values.insert(m_values.end(), values.begin(), values.end());
	return *this;
}

Shell::Argument::Type Shell::Argument::GetType() const {
	return m_type;
}

std::string Shell::Argument::GetName() const {
	return m_name;
}

std::vector<std::string> Shell::Argument::GetValues() const {
	return m_values;
}

std::vector<Shell::Argument> Shell::Argument::Process(const std::string source, const std::vector<Dark::Token> lexemes) {
	std::vector<Dark::Token> tokens = Dark::Lexer::Lex(source, lexemes);
	size_t tokens_count = tokens.size(), index = 0;
	std::vector<Argument> result = {};
	Argument argument;
	bool unknown_token;

	for (; index < tokens_count; index++) {									//	remove space & new line symbols
		if (tokens[index].GetTypeIndex() == TOKEN_TYPE_OPERATOR_SPACE) {
			tokens.erase(tokens.begin() + index);
			--index;
			--tokens_count;
		}
	}

	index = 0;
	while (index < tokens_count) {
		if(tokens[index].GetTypeIndex() == TOKEN_TYPE_OPERATOR_PRIORITY) {
			++index;
			if (index >= tokens_count) break;

			argument.SetType(tokens[index].GetTypeIndex() == TOKEN_TYPE_OPERATOR_PRIORITY ? TYPE_SUBOPTION : TYPE_OPTION);
			while (tokens[index].GetTypeIndex() == TOKEN_TYPE_OPERATOR_PRIORITY && index < tokens_count) ++index;
			if (index >= tokens_count) break;

			if (tokens[index].GetTypeIndex() == TOKEN_TYPE_VALUE) argument.SetName(tokens[index++].GetValue());
			if (index >= tokens_count) {
				result.push_back(argument);
				break;
			}

			if (tokens[index].GetTypeIndex() != TOKEN_TYPE_OPERATOR_ASSIGN) {
				result.push_back(argument);
				continue;
			}

			while (tokens[index].GetTypeIndex() == TOKEN_TYPE_OPERATOR_ASSIGN && index < tokens_count) ++index;
			if (index >= tokens_count) {
				result.push_back(argument);
				break;
			}

			unknown_token = false;
			while (index < tokens_count) {
				switch (tokens[index].GetTypeIndex()) {
					case TOKEN_TYPE_VALUE:
						argument.AppendValues({ tokens[index].GetValue() });
						break;
					case TOKEN_TYPE_EXTENDED_VALUE:
						argument.AppendValues({ tokens[index].GetValue().substr(1, tokens[index].GetValue().length() - 2) });
						break;
					case TOKEN_TYPE_OPERATOR_COMMA:
						break;
					default:
						unknown_token = true;
						break;
				}

				if (unknown_token) break;
				++index;
			}

			result.push_back(argument);
			argument.SetValues({});
			if (index >= tokens_count) break;
			continue;
		}
		else if (tokens[index].GetTypeIndex() == TOKEN_TYPE_VALUE) {
			argument.SetType(TYPE_VALUE);
			argument.SetName(tokens[index].GetValue());
			argument.SetValues({});
			result.push_back(argument);
		}
		else if (tokens[index].GetTypeIndex() == TOKEN_TYPE_EXTENDED_VALUE) {
			argument.SetType(TYPE_VALUE);
			argument.SetName(tokens[index].GetValue().substr(1, tokens[index].GetValue().length() - 2));
			argument.SetValues({});
			result.push_back(argument);
		}
		
		++index;
	}

	return result;
}
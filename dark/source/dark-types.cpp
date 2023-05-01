#include <dark-types.hpp>

Dark::Message::Message(size_t type, const std::string value) {
	m_type = type;
	m_value = value;
}

Dark::Message& Dark::Message::SetType(size_t type) {
	m_type = type;
	return *this;
}

Dark::Message& Dark::Message::SetValue(const std::string value) {
	m_value = value;
	return *this;
}

size_t Dark::Message::GetType() const {
	return m_type;
}

std::string Dark::Message::GetValue() const {
	return m_value;
}

std::string Dark::Message::FormString() const {
	std::string result ="";
	switch(m_type) {
		case TYPE_INFORMATION:
			result += "Information: ";
			break;
		case TYPE_WARNING:
			result += "Warning: ";
			break;
		case TYPE_ERROR:
			result += "Error: ";
			break;
	}

	result += m_value;
	return result;
}

Dark::Token::Token(size_t type, const std::string value) {
	m_type = type;
	m_value = value;
}

Dark::Token& Dark::Token::SetType(size_t type) {
	m_type = type;
	return *this;
}

Dark::Token& Dark::Token::SetValue(const std::string value) {
	m_value = value;
	return *this;
}

size_t Dark::Token::GetType() const {
	return m_type;
}

std::string Dark::Token::GetValue() const {
	return m_value;
}

size_t Dark::Token::Find(const std::vector<Token> tokens, size_t type, size_t start_index) {
	const size_t count = tokens.size();
	for (; start_index < count; start_index++) if (tokens[start_index].GetType() == type) return start_index;
	return std::string::npos;
}

Dark::Macro::Macro(size_t type, const std::string name, const std::vector<Token> expression, const std::vector<Token> arguments) {
	m_type = type;
	m_name = name;
	m_expression = expression;
	m_arguments = arguments;
}

Dark::Macro& Dark::Macro::SetType(size_t type) {
	m_type = type;
	return *this;
}

Dark::Macro& Dark::Macro::SetName(const std::string name) {
	m_name = name;
	return *this;
}

Dark::Macro& Dark::Macro::SetExpression(const std::vector<Token> expression) {
	m_expression = expression;
	return *this;
}

Dark::Macro& Dark::Macro::SetArguments(const std::vector<Token> arguments) {
	m_arguments = arguments;
	return *this;
}

size_t Dark::Macro::GetType() const {
	return m_type;
}

std::string Dark::Macro::GetName() const {
	return m_name;
}

std::vector<Dark::Token> Dark::Macro::GetExpression() const {
	return m_expression;
}

std::vector<Dark::Token> Dark::Macro::GetArguments() const {
	return m_arguments;
}

Dark::Macro& Dark::Macro::AddExpression(const std::vector<Token> expression) {
	m_expression.insert(m_expression.end(), expression.begin(), expression.end());
	return *this;
}

Dark::Macro& Dark::Macro::AddArguments(const std::vector<Token> arguments) {
	m_arguments.insert(m_arguments.end(), arguments.begin(), arguments.end());
	return *this;
}
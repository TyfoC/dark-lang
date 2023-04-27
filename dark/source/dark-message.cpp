#include <dark-message.hpp>

Dark::Message::Message(Type type, const std::string value) {
	m_type = type;
	m_value = value;
}

Dark::Message& Dark::Message::SetType(Type type) {
	m_type = type;
	return *this;
}

Dark::Message& Dark::Message::SetValue(const std::string value) {
	m_value = value;
	return *this;
}

Dark::Message::Type Dark::Message::GetType() const {
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
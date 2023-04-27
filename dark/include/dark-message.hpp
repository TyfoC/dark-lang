#pragma once
#ifndef DARK_MESSAGES_HPP
#define DARK_MESSAGES_HPP

#include <string>

namespace Dark {
	class Message {
		public:
		enum Type : size_t {
			TYPE_INFORMATION,
			TYPE_WARNING,
			TYPE_ERROR,
		};

		Message(Type type = TYPE_INFORMATION, const std::string value = "");
		Message& SetType(Type type = TYPE_INFORMATION);
		Message& SetValue(const std::string value = "");
		Type GetType() const;
		std::string GetValue() const;
		std::string FormString() const;
		protected:
		Type		m_type;
		std::string	m_value;
	};
}

#endif
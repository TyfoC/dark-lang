#pragma once
#ifndef DARK_TYPES_HPP
#define DARK_TYPES_HPP

#include <string>
#include <vector>

namespace Dark {
	class Message {
		public:
		enum Type {
			TYPE_INFORMATION,
			TYPE_WARNING,
			TYPE_ERROR,
		};

		Message(size_t type = TYPE_INFORMATION, const std::string value = "");
		Message& SetType(size_t type = TYPE_INFORMATION);
		Message& SetValue(const std::string value = "");
		size_t GetType() const;
		std::string GetValue() const;
		std::string FormString() const;
		protected:
		size_t		m_type;
		std::string	m_value;
	};

	class Token {
		public:
		Token(size_t type = std::string::npos, const std::string value = "");
		Token& SetType(size_t type = std::string::npos);
		Token& SetValue(const std::string value = "");
		size_t GetType() const;
		std::string GetValue() const;

		static size_t Find(const std::vector<Token> tokens, size_t type, size_t start_index = 0);
		static size_t Find(const std::vector<Token> tokens, const std::string value, size_t start_index = 0);
		static std::string Format(const std::vector<Token> tokens);
		protected:
		size_t		m_type;
		std::string	m_value;
	};

	class Macro {
		public:
		enum Type {
			TYPE_STANDARD,
			TYPE_EXTENDED,
		};

		Macro(size_t type = std::string::npos, const std::string name = "", const std::vector<Token> expression = {}, const std::vector<Token> arguments = {});
		Macro& SetType(size_t type = std::string::npos);
		Macro& SetName(const std::string name = "");
		Macro& SetExpression(const std::vector<Token> expression = {});
		Macro& SetArguments(const std::vector<Token> arguments = {});
		size_t GetType() const;
		std::string GetName() const;
		std::vector<Token> GetExpression() const;
		std::vector<Token> GetArguments() const;

		Macro& AddExpression(const std::vector<Token> expression);
		Macro& AddArguments(const std::vector<Token> arguments);

		static size_t Find(const std::vector<Macro> macros, const std::string name);
		protected:
		size_t				m_type;
		std::string			m_name;
		std::vector<Token>	m_expression;
		std::vector<Token>	m_arguments;
	};
}

#endif
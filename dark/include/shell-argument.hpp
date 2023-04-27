#pragma once
#ifndef SHELL_ARGUMENT_HPP
#define SHELL_ARGUMENT_HPP

#include "dark-lexer.hpp"

namespace Shell {
	class Argument {
		public:
		enum Type : size_t {
			TYPE_VALUE,			//	askfakjsf/file.ext/21361273/...
			TYPE_OPTION,		//	-option
			TYPE_SUBOPTION,		//	--suboption
			TYPE_PARAMETER,		//	-parameter=value
			TYPE_SUBPARAMETER,	//	--subparameter=value
		};

		enum TokenType : size_t {
			TOKEN_TYPE_OPERATOR_SPACE,			//	' '|\t|\v|\f|\a|\r|\n
			TOKEN_TYPE_OPERATOR_PRIORITY,		//	-
			TOKEN_TYPE_OPERATOR_ASSIGN,			//	=
			TOKEN_TYPE_OPERATOR_COMMA,			//	,
			TOKEN_TYPE_VALUE,					//	someValue
			TOKEN_TYPE_EXTENDED_VALUE,			//	"\"extended\" value!"
		};

		static const std::vector<Dark::Token> Lexemes;

		Argument(Type type = TYPE_VALUE, const std::string name = "", const std::vector<std::string> values = {});
		Argument& SetType(Type type = TYPE_VALUE);
		Argument& SetName(const std::string name = "");
		Argument& SetValues(const std::vector<std::string> values = {});
		Argument& AppendValues(const std::vector<std::string> values = {});
		Type GetType() const;
		std::string GetName() const;
		std::vector<std::string> GetValues() const;

		static std::vector<Argument> Process(const std::string source, const std::vector<Dark::Token> lexemes = Lexemes);
		protected:
		Type						m_type;
		std::string					m_name;
		std::vector<std::string>	m_values;
	};
}

#endif
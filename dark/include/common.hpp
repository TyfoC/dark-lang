#pragma once
#ifndef __COMMON__
#define __COMMON__

#include <iostream>
#include <cstring>
#include <fstream>
#include "lexer.hpp"

namespace Dark {
	typedef enum : size_t {
		SPACE,
		NEW_LINE,
		COMMENTARY,

		LITERAL_STRING,
		LITERAL_HEX,
		LITERAL_OCT,
		LITERAL_BIN,
		LITERAL_DEC,

		PARENTHESIS_LEFT,
		PARENTHESIS_RIGHT,
		BRACKET_LEFT,
		BRACKET_RIGHT,
		BRACE_LEFT,
		BRACE_RIGHT,

		OPERATOR_ADD,
		OPERATOR_SUB,
		OPERATOR_MUL,
		OPERATOR_DIV,
		OPERATOR_MOD,

		OPERATOR_INCREMENT,
		OPERATOR_DECREMENT,

		OPERATOR_ASSIGN,
		OPERATOR_ADD_ASSIGN,
		OPERATOR_SUB_ASSIGN,
		OPERATOR_MUL_ASSIGN,
		OPERATOR_DIV_ASSIGN,
		OPERATOR_MOD_ASSIGN,
		OPERATOR_AND_ASSIGN,
		OPERATOR_OR_ASSIGN,
		OPERATOR_XOR_ASSIGN,
		OPERATOR_SHIFT_RIGHT_ASSIGN,
		OPERATOR_SHIFT_LEFT_ASSIGN,

		OPERATOR_EQUAL_TO,
		OPERATOR_NOT_EQUAL,
		OPERATOR_GREATER_THAN,
		OPERATOR_LESS_THAN,
		OPERATOR_GREATER_THAN_OR_EQUAL_TO,
		OPERATOR_LESS_THAN_OR_EQUAL_TO,

		OPERATOR_RETURN,

		OPERATOR_LOGICAL_AND,
		OPERATOR_LOGICAL_OR,
		OPERATOR_LOGICAL_NOT,

		OPERATOR_DOT,
		OPERATOR_COMMA,
		OPERATOR_COLON,
		OPERATOR_SEMICOLON,
		OPERATOR_PREPROCESSOR_DIRECTIVE,

		OPERATOR_SIZE_IN_BYTES,
		OPERATOR_SIZE_IN_BITS,
		OPERATOR_IF,
		OPERATOR_ELIF,
		OPERATOR_ELSE,
		OPERATOR_BREAK,
		OPERATOR_CONTINUE,
		OPERATOR_FOR,
		OPERATOR_DO,
		OPERATOR_WHILE,
		OPERATOR_UNTIL,
		
		IDENTIFIER,
	} TokenType_t, LexemeType_t;

	enum MessageType_t : size_t {
		INFORMATION,
		WARNING,
		ERROR
	};

	typedef struct Message_t {
		MessageType_t	Type;
		size_t			Line;
		std::string		Text;
	} Message_t;

	const std::vector<Lexer::Lexeme_t> Lexemes = {
		{ SPACE, R"([ \t\f])" },
		{ NEW_LINE, R"((\r\n|\n\r|\r|\n))" },
		{ COMMENTARY, R"(<!([\s\S]*?)!>)" },					//	<! ... !>

		{ LITERAL_STRING, R"("(\\[\s\S]|[^\"])*")" },
		{ LITERAL_HEX, R"(\b0[xX][0-9a-fA-F]+\b)" },			//	0xDEADBEEF				0XDEADBEEF
		{ LITERAL_OCT, R"(\b0[oO]?[0-7]+\b)" },					//	0124217124				0o12367423			0O3564174
		{ LITERAL_BIN, R"(\b0[bB][01]+\b)" },					//	0b11010100				0B11101011
		{ LITERAL_DEC, R"(\b[0-9]+\b)" },						//	28763273

		{ PARENTHESIS_LEFT, R"(\()" },
		{ PARENTHESIS_RIGHT, R"(\))" },
		{ BRACKET_LEFT, R"(\[)" },
		{ BRACKET_RIGHT, R"(\])" },
		{ BRACE_LEFT, R"(\{)" },
		{ BRACE_RIGHT, R"(\})" },

		{ OPERATOR_ADD, R"(\+)" },
		{ OPERATOR_SUB, R"(-)" },
		{ OPERATOR_MUL, R"(\*)" },
		{ OPERATOR_DIV, R"(/)" },
		{ OPERATOR_MOD, R"(%)" },
		
		{ OPERATOR_INCREMENT, R"(\+\+)" },
		{ OPERATOR_DECREMENT, R"(--)" },
		
		{ OPERATOR_ASSIGN, R"(=)" },
		{ OPERATOR_ADD_ASSIGN, R"(\+=)" },
		{ OPERATOR_SUB_ASSIGN, R"(-=)" },
		{ OPERATOR_MUL_ASSIGN, R"(\*=)" },
		{ OPERATOR_DIV_ASSIGN, R"(/=)" },
		{ OPERATOR_MOD_ASSIGN, R"(%=)" },
		{ OPERATOR_AND_ASSIGN, R"(&=)" },
		{ OPERATOR_OR_ASSIGN, R"(\|=)" },
		{ OPERATOR_XOR_ASSIGN, R"(\^=)" },
		{ OPERATOR_SHIFT_RIGHT_ASSIGN, R"(<<=)" },
		{ OPERATOR_SHIFT_LEFT_ASSIGN, R"(>>=)" },
		
		{ OPERATOR_EQUAL_TO, R"(==)" },
		{ OPERATOR_NOT_EQUAL, R"(!=)" },
		{ OPERATOR_GREATER_THAN, R"(>)" },
		{ OPERATOR_LESS_THAN, R"(<)" },
		{ OPERATOR_GREATER_THAN_OR_EQUAL_TO, R"(>=)" },
		{ OPERATOR_LESS_THAN_OR_EQUAL_TO, R"(<=)" },

		{ OPERATOR_RETURN, R"(~>)" },

		{ OPERATOR_LOGICAL_AND, R"(&)" },
		{ OPERATOR_LOGICAL_OR, R"(\|)" },
		{ OPERATOR_LOGICAL_NOT, R"(~)" },

		{ OPERATOR_DOT, R"(\.)" },
		{ OPERATOR_COMMA, R"(,)" },
		{ OPERATOR_COLON, R"(:)" },
		{ OPERATOR_SEMICOLON, R"(;)" },
		{ OPERATOR_PREPROCESSOR_DIRECTIVE, R"(@)" },

		{ OPERATOR_SIZE_IN_BYTES, "R(szbytes)" },
		{ OPERATOR_SIZE_IN_BITS, "R(szbits)" },
		{ OPERATOR_IF, R"(if)" },
		{ OPERATOR_ELIF, R"(elif)" },
		{ OPERATOR_ELSE, R"(else)" },
		{ OPERATOR_CONTINUE, R"(continue)" },
		{ OPERATOR_BREAK, R"(break)" },
		{ OPERATOR_FOR, R"(for)" },
		{ OPERATOR_DO, R"(do)" },
		{ OPERATOR_WHILE, R"(while)" },
		{ OPERATOR_UNTIL, R"(until)" },

		{ IDENTIFIER, R"(\b([_a-zA-Z]\w*)\b)" },			//	_aWholeIdentifier1st
	};
}

#endif
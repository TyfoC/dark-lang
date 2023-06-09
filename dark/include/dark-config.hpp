#pragma once
#ifndef DARK_CONFIG_HPP
#define DARK_CONFIG_HPP

#include "dark-types.hpp"

namespace Dark {
	enum TokenType : size_t {
		TOKEN_TYPE_SPACE,								//	' '|\t|\v|\f
		TOKEN_TYPE_NEW_LINE,							//	"\r\n"|"\n\r"|"\r"|"\n"
		TOKEN_TYPE_COMMENT_BLOCK,						//	<! ... !>

		TOKEN_TYPE_OPERATOR_SHIFT_RIGHT_ASSIGN,			//	>>=
		TOKEN_TYPE_OPERATOR_SHIFT_LEFT_ASSIGN,			//	<<=
		TOKEN_TYPE_OPERATOR_ADD_ASSIGN,					//	+=
		TOKEN_TYPE_OPERATOR_SUB_ASSIGN,					//	-=
		TOKEN_TYPE_OPERATOR_MUL_ASSIGN,					//	*=
		TOKEN_TYPE_OPERATOR_DIV_ASSIGN,					//	/=
		TOKEN_TYPE_OPERATOR_MOD_ASSIGN,					//	%=
		TOKEN_TYPE_OPERATOR_AND_ASSIGN,					//	&=
		TOKEN_TYPE_OPERATOR_OR_ASSIGN,					//	|=
		TOKEN_TYPE_OPERATOR_XOR_ASSIGN,					//	^=

		TOKEN_TYPE_OPERATOR_EQUAL_TO,					//	==
		TOKEN_TYPE_OPERATOR_NOT_EQUAL_TO,				//	!=
		TOKEN_TYPE_OPERATOR_GREATER_OR_EQUAL,			//	>=
		TOKEN_TYPE_OPERATOR_LESS_OR_EQUAL,				//	<=

		TOKEN_TYPE_OPERATOR_EXPAND,						//	->

		TOKEN_TYPE_OPERATOR_SHIFT_RIGHT,				//	>>
		TOKEN_TYPE_OPERATOR_SHIFT_LEFT,					//	<<

		TOKEN_TYPE_OPERATOR_INC,						//	++
		TOKEN_TYPE_OPERATOR_DEC,						//	--
		TOKEN_TYPE_OPERATOR_ADD,						//	+
		TOKEN_TYPE_OPERATOR_SUB,						//	-
		TOKEN_TYPE_OPERATOR_MUL,						//	*
		TOKEN_TYPE_OPERATOR_DIV,						//	/
		TOKEN_TYPE_OPERATOR_MOD,						//	%

		TOKEN_TYPE_OPERATOR_LOGICAL_AND,				//	&&
		TOKEN_TYPE_OPERATOR_LOGICAL_OR,					//	||
		TOKEN_TYPE_OPERATOR_LOGICAL_NOT,				//	!

		TOKEN_TYPE_OPERATOR_BITWISE_AND,				//	&
		TOKEN_TYPE_OPERATOR_BITWISE_OR,					//	|
		TOKEN_TYPE_OPERATOR_BITWISE_XOR,				//	^
		TOKEN_TYPE_OPERATOR_BITWISE_COMPLEMENT,			//	~

		TOKEN_TYPE_OPERATOR_PREPROCESSOR_DIRECTIVE,		//	@

		TOKEN_TYPE_OPERATOR_ASSIGN,						//	=

		TOKEN_TYPE_OPERATOR_GREATER_THAN,				//	>
		TOKEN_TYPE_OPERATOR_LESS_THAN,					//	<

		TOKEN_TYPE_OPERATOR_OPENING_PARENTHESIS,		//	(
		TOKEN_TYPE_OPERATOR_CLOSING_PARENTHESIS,		//	)
		TOKEN_TYPE_OPERATOR_OPENING_BRACKET,			//	[
		TOKEN_TYPE_OPERATOR_CLOSING_BRACKET,			//	]
		TOKEN_TYPE_OPERATOR_OPENING_BRACE,				//	{
		TOKEN_TYPE_OPERATOR_CLOSING_BRACE,				//	}

		TOKEN_TYPE_OPERATOR_DOT,						//	.
		TOKEN_TYPE_OPERATOR_COMMA,						//	,
		TOKEN_TYPE_OPERATOR_COLON,						//	:
		TOKEN_TYPE_OPERATOR_SEMICOLON,					//	;

		TOKEN_TYPE_OPERATOR_RETURN,						//	return
		TOKEN_TYPE_OPERATOR_TYPESZ,						//	typesz
		TOKEN_TYPE_OPERATOR_IF,							//	if x == y -> ... ;
		TOKEN_TYPE_OPERATOR_ELIF,						//	elif x == z && y != z -> ... ;
		TOKEN_TYPE_OPERATOR_ELSE,						//	else ... ;
		TOKEN_TYPE_OPERATOR_BREAK,						//	break
		TOKEN_TYPE_OPERATOR_CONTINUE,					//	continue
		TOKEN_TYPE_OPERATOR_FOR,						//	for declaration->condition->statement -> ... ;
		TOKEN_TYPE_OPERATOR_DO,							//	do ... while condition; do ... until condition;
		TOKEN_TYPE_OPERATOR_WHILE,						//	while condition -> ...;
		TOKEN_TYPE_OPERATOR_UNTIL,						//	until condition -> ...;

		TOKEN_TYPE_STRING_LITERAL,						//	"This is a \"string\"!"
		TOKEN_TYPE_HEX_LITERAL,							//	0xDEADBEEF
		TOKEN_TYPE_OCT_LITERAL,							//	01234567
		TOKEN_TYPE_BIN_LITERAL,							//	0b11011001
		TOKEN_TYPE_DEC_LITERAL,							//	1000000025

		TOKEN_TYPE_IDENTIFIER,							//	_aWh0l31d3nt1f13r

		TOKEN_TYPE_RAW_DATA,
	};

	const std::vector<Token> Lexemes = {
		{ TOKEN_TYPE_SPACE, R"([ \t\f\v])" },
		{ TOKEN_TYPE_NEW_LINE, R"((\r\n|\n\r|\n|\r))" },
		{ TOKEN_TYPE_COMMENT_BLOCK, R"(<!([\s\S]*?)!>)" },

		{ TOKEN_TYPE_STRING_LITERAL, R"("(\\[\s\S]|[^\"])*")" },
		{ TOKEN_TYPE_HEX_LITERAL, R"([+-]?0[xX][0-9a-fA-F]+\b)" },
		{ TOKEN_TYPE_OCT_LITERAL, R"([+-]?0[oO]?[0-7]+\b)" },
		{ TOKEN_TYPE_BIN_LITERAL, R"([+-]?0[bB][01]+\b)" },
		{ TOKEN_TYPE_DEC_LITERAL, R"([+-]?[0-9]+\b)" },

		{ TOKEN_TYPE_OPERATOR_SHIFT_RIGHT_ASSIGN, R"(>>=)" },
		{ TOKEN_TYPE_OPERATOR_SHIFT_LEFT_ASSIGN, R"(<<=)" },
		{ TOKEN_TYPE_OPERATOR_ADD_ASSIGN, R"(\+=)" },
		{ TOKEN_TYPE_OPERATOR_SUB_ASSIGN, R"(-=)" },
		{ TOKEN_TYPE_OPERATOR_MUL_ASSIGN, R"(\*=)" },
		{ TOKEN_TYPE_OPERATOR_DIV_ASSIGN, R"(/=)" },
		{ TOKEN_TYPE_OPERATOR_MOD_ASSIGN, R"(%=)" },
		{ TOKEN_TYPE_OPERATOR_AND_ASSIGN, R"(&=)" },
		{ TOKEN_TYPE_OPERATOR_OR_ASSIGN, R"(\|=)" },
		{ TOKEN_TYPE_OPERATOR_XOR_ASSIGN, R"(\^=)" },

		{ TOKEN_TYPE_OPERATOR_EQUAL_TO, R"(==)" },
		{ TOKEN_TYPE_OPERATOR_NOT_EQUAL_TO, R"(!=)" },
		{ TOKEN_TYPE_OPERATOR_GREATER_OR_EQUAL, R"(>=)" },
		{ TOKEN_TYPE_OPERATOR_LESS_OR_EQUAL, R"(<=)" },

		{ TOKEN_TYPE_OPERATOR_EXPAND, R"(->)" },

		{ TOKEN_TYPE_OPERATOR_SHIFT_RIGHT, R"(>>)" },
		{ TOKEN_TYPE_OPERATOR_SHIFT_LEFT, R"(<<)" },
		{ TOKEN_TYPE_OPERATOR_INC, R"(\+\+)" },
		{ TOKEN_TYPE_OPERATOR_DEC, R"(\-\-)" },
		{ TOKEN_TYPE_OPERATOR_ADD, R"(\+)" },
		{ TOKEN_TYPE_OPERATOR_SUB, R"(\-)" },
		{ TOKEN_TYPE_OPERATOR_MUL, R"(\*)" },
		{ TOKEN_TYPE_OPERATOR_DIV, R"(/)" },
		{ TOKEN_TYPE_OPERATOR_MOD, R"(%)" },

		{ TOKEN_TYPE_OPERATOR_LOGICAL_AND, R"(&&)" },
		{ TOKEN_TYPE_OPERATOR_LOGICAL_OR, R"(\|\|)" },
		{ TOKEN_TYPE_OPERATOR_LOGICAL_NOT, R"(!)" },

		{ TOKEN_TYPE_OPERATOR_BITWISE_AND, R"(&)" },
		{ TOKEN_TYPE_OPERATOR_BITWISE_OR, R"(\|)" },
		{ TOKEN_TYPE_OPERATOR_BITWISE_XOR, R"(\^)" },
		{ TOKEN_TYPE_OPERATOR_BITWISE_COMPLEMENT, R"(~)" },

		{ TOKEN_TYPE_OPERATOR_PREPROCESSOR_DIRECTIVE, R"(@)" },

		{ TOKEN_TYPE_OPERATOR_ASSIGN, R"(=)" },

		{ TOKEN_TYPE_OPERATOR_GREATER_THAN, R"(>)" },
		{ TOKEN_TYPE_OPERATOR_LESS_THAN, R"(<)" },

		{ TOKEN_TYPE_OPERATOR_OPENING_PARENTHESIS, R"(\()" },
		{ TOKEN_TYPE_OPERATOR_CLOSING_PARENTHESIS, R"(\))" },
		{ TOKEN_TYPE_OPERATOR_OPENING_BRACKET, R"(\[)" },
		{ TOKEN_TYPE_OPERATOR_CLOSING_BRACKET, R"(\])" },
		{ TOKEN_TYPE_OPERATOR_OPENING_BRACE, R"(\{)" },
		{ TOKEN_TYPE_OPERATOR_CLOSING_BRACE, R"(\})" },
		{ TOKEN_TYPE_OPERATOR_DOT, R"(\.)" },
		{ TOKEN_TYPE_OPERATOR_COMMA, R"(,)" },
		{ TOKEN_TYPE_OPERATOR_COLON, R"(\:)" },
		{ TOKEN_TYPE_OPERATOR_SEMICOLON, R"(;)" },

		{ TOKEN_TYPE_OPERATOR_RETURN, R"(\breturn\b)" },
		{ TOKEN_TYPE_OPERATOR_TYPESZ, R"(\btypesz\b)" },

		{ TOKEN_TYPE_OPERATOR_IF, R"(\bif\b)" },
		{ TOKEN_TYPE_OPERATOR_ELIF, R"(\belif\b)" },
		{ TOKEN_TYPE_OPERATOR_ELSE, R"(\belse\b)" },

		{ TOKEN_TYPE_OPERATOR_BREAK, R"(\bbreak\b)" },
		{ TOKEN_TYPE_OPERATOR_CONTINUE, R"(\bcontinue\b)" },
		{ TOKEN_TYPE_OPERATOR_FOR, R"(\bfor\b)" },
		{ TOKEN_TYPE_OPERATOR_DO, R"(\bdo\b)" },
		{ TOKEN_TYPE_OPERATOR_WHILE, R"(\bwhile\b)" },
		{ TOKEN_TYPE_OPERATOR_UNTIL, R"(\buntil\b)" },

		{ TOKEN_TYPE_IDENTIFIER, R"(\b[_a-zA-Z]\w*\b)" },
	};
}

#endif
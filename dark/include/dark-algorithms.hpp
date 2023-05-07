#pragma once
#ifndef DARK_ALGORITHMS_HPP
#define DARK_ALGORITHMS_HPP

#include <stack>
#include <cmath>
#include <sstream>
#include "dark-config.hpp"
#include "dark-utils.hpp"

namespace Dark {
	struct AlgorithmOperator {
		static size_t Find(const std::vector<AlgorithmOperator> operators, size_t type, size_t start_index = 0);

		size_t	Type;
		size_t	Priority;			//	0 - highest
	};

	std::vector<Token> InfixToPostfix(const std::vector<Token> tokens, const std::vector<AlgorithmOperator> operators);
	ptrdiff_t EvaluatePostfix(const std::vector<Token> tokens);
}

#endif
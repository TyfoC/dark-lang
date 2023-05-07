#include <dark-algorithms.hpp>

size_t Dark::AlgorithmOperator::Find(const std::vector<AlgorithmOperator> operators, size_t type, size_t start_index) {
	size_t count = operators.size();
	for (; start_index < count; start_index++) if (operators[start_index].Type == type) return start_index;
	return std::string::npos;
}

std::vector<Dark::Token> Dark::InfixToPostfix(const std::vector<Token> tokens, const std::vector<AlgorithmOperator> operators) {
	std::vector<Token> result = {};
	std::stack<Token> local_stack;
	size_t tokens_count = tokens.size();

	size_t tmp_index;
	std::string tmp_value = "";
	for (size_t i = 0; i < tokens_count; i++) {
		tmp_index = tokens[i].GetType();
		switch(tmp_index) {
			case TOKEN_TYPE_OPERATOR_OPENING_PARENTHESIS:
				local_stack.push(tokens[i]);
				break;
			case TOKEN_TYPE_OPERATOR_CLOSING_PARENTHESIS:
				while (local_stack.top().GetType() != TOKEN_TYPE_OPERATOR_OPENING_PARENTHESIS) {
					result.push_back(local_stack.top());
                	local_stack.pop();
				}
				local_stack.pop();
				break;
			case TOKEN_TYPE_HEX_LITERAL:
			case TOKEN_TYPE_OCT_LITERAL:
			case TOKEN_TYPE_BIN_LITERAL:
			case TOKEN_TYPE_DEC_LITERAL:
			case TOKEN_TYPE_IDENTIFIER:
				result.push_back(tokens[i]);
				break;
			default:
				tmp_value = tokens[i].GetValue();
				tmp_index = AlgorithmOperator::Find(operators, tmp_index);
				if (tmp_index == std::string::npos) result.push_back(tokens[i]);
				else {
					tmp_index = AlgorithmOperator::Find(operators, tokens[i].GetType());
					while (!local_stack.empty() && tmp_index >= AlgorithmOperator::Find(operators, local_stack.top().GetType())) {
						result.push_back(local_stack.top());
						local_stack.pop();
					}
					local_stack.push(tokens[i]);
				}
				break;
		}
	}

    while(!local_stack.empty()){
		result.push_back(local_stack.top());
		local_stack.pop();
    }

	return result;
}

ptrdiff_t Dark::EvaluatePostfix(const std::vector<Token> tokens) {
	ptrdiff_t first, second;
	std::stack<ptrdiff_t> local_stack;
	size_t count = tokens.size();
	std::stringstream conv_buff;

	size_t token_type;
	std::string token_value = "";
	for (size_t i = 0; i < count; i++) {
		token_type = tokens[i].GetType();
		token_value = tokens[i].GetValue();

		if (token_type == TOKEN_TYPE_HEX_LITERAL) local_stack.push(std::stoll(token_value, 0, 16));
		else if (token_type == TOKEN_TYPE_OCT_LITERAL) local_stack.push(std::stoll(token_value, 0, 8));
		else if (token_type == TOKEN_TYPE_BIN_LITERAL) local_stack.push(std::stoll(token_value, 0, 2));
		else if (token_type == TOKEN_TYPE_DEC_LITERAL) local_stack.push(std::stof(token_value));
		else {
			first = local_stack.top();
			local_stack.pop();

			if (token_type == TOKEN_TYPE_OPERATOR_LOGICAL_NOT) first = !first;
			else if (token_type == TOKEN_TYPE_OPERATOR_BITWISE_COMPLEMENT) first = ~first;
			else {
				second = first;
				first = local_stack.top();
				local_stack.pop();

				if (token_type == TOKEN_TYPE_OPERATOR_ADD) first += second;
				if (token_type == TOKEN_TYPE_OPERATOR_SUB) first -= second;
				if (token_type == TOKEN_TYPE_OPERATOR_MUL) first *= second;
				if (token_type == TOKEN_TYPE_OPERATOR_DIV) first /= second;
				if (token_type == TOKEN_TYPE_OPERATOR_MOD) first %= second;
				if (token_type == TOKEN_TYPE_OPERATOR_SHIFT_LEFT) first <<= second;
				if (token_type == TOKEN_TYPE_OPERATOR_SHIFT_RIGHT) first >>= second;
				if (token_type == TOKEN_TYPE_OPERATOR_BITWISE_AND) first &= second;
				if (token_type == TOKEN_TYPE_OPERATOR_BITWISE_OR) first |= second;
				if (token_type == TOKEN_TYPE_OPERATOR_BITWISE_XOR) first ^= second;
				if (token_type == TOKEN_TYPE_OPERATOR_LESS_THAN) first = (ptrdiff_t)(first < second);
				if (token_type == TOKEN_TYPE_OPERATOR_GREATER_THAN) first = (ptrdiff_t)(first > second);
				if (token_type == TOKEN_TYPE_OPERATOR_LESS_OR_EQUAL) first = (ptrdiff_t)(first <= second);
				if (token_type == TOKEN_TYPE_OPERATOR_GREATER_OR_EQUAL) first = (ptrdiff_t)(first >= second);
				if (token_type == TOKEN_TYPE_OPERATOR_EQUAL_TO) first = (ptrdiff_t)(first == second);
				if (token_type == TOKEN_TYPE_OPERATOR_NOT_EQUAL_TO) first = (ptrdiff_t)(first != second);
			}

			local_stack.push(first);
		}
	}

	return (ptrdiff_t)local_stack.top();
}
#pragma once

#include <array>
#include <cstddef>
#include <expr.hpp>
#include <memory>
#include <scanner.hpp>
#include <utility>
#include <variant.hpp>
#include <vector>

namespace cbycpp {
namespace parser {
class Parser {
  public:
	using pExpr = parser::pExpr;
	pExpr parse(std::unique_ptr<scanner::Scanner::vTokens> tokens);

  private:
	pExpr term();
	pExpr factor();
	pExpr unary();
	pExpr primary();

	std::size_t current{};
	scanner::Scanner::vTokens *tokens_pointer;
	inline scanner::Token cur_token() { return tokens_pointer->at(current); }
	inline scanner::Token pre_token() {
		return tokens_pointer->at(current - 1);
	}
	inline scanner::Token cur_advance() {
		return tokens_pointer->at(current++);
	}
	inline bool is_end() {
		if (current < tokens_pointer->size())
			return false;
		else
			return true;
	}
	inline bool match(scanner::TokenTypes type) {
		if (!is_end() && type == cur_token().type) {
			current++;
			return true;
		} else
			return false;
	}
	inline bool match(std::vector<scanner::TokenTypes> types) {
		for (auto &type : types) {
			if (match(type)) {
				return true;
			}
		}
		return false;
	}
};
} // namespace parser
} // namespace cbycpp

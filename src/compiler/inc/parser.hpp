#pragma once

#include <array>
#include <cstddef>
#include <expr.hpp>
#include <memory>
#include <scanner.hpp>
#include <utility>
#include <variant.hpp>
#include <vector>
#include <statement.hpp>

namespace cbycpp {
namespace parser {
class Parser {
  public:
	pStmt parse(std::unique_ptr<scanner::Scanner::vTokens> tokens);
	pExpr parse_expr(std::unique_ptr<scanner::Scanner::vTokens> tokens);

  private:
    pStmt statement();
	pStmt writeStmt();
	pStmt exprStmt();

	pExpr expression();
	pExpr equal();
	pExpr comparison();
	pExpr term();
	pExpr factor();
	pExpr unary();
	pExpr primary();

	std::size_t current{};
	scanner::Scanner::vTokens *tokens_pointer;
	inline scanner::Token cur_token() const {
		return tokens_pointer->at(current);
	}
	inline scanner::Token pre_token() const {
		return tokens_pointer->at(current - 1);
	}
	inline scanner::Token cur_advance() {
		return tokens_pointer->at(current++);
	}
	inline bool is_end() const {
		if (current < tokens_pointer->size() - 1) // EOF
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
	inline scanner::Token expect(scanner::TokenTypes type, std::string msg);
};
} // namespace parser
} // namespace cbycpp

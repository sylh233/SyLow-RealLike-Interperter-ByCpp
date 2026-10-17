#include <parser.hpp>

namespace cbycpp {
namespace parser {
Parser::pExpr Parser::parse(std::unique_ptr<scanner::Scanner::vTokens> tokens) {
	tokens_pointer = tokens.get();
	return term(); // 编译器优先尝试move，不用再写
}

Parser::pExpr Parser::term() {
	pExpr left = factor();
	while (match({scanner::TokenTypes::PLUS, scanner::TokenTypes::MINUS})) {
		scanner::Token op = pre_token();
		pExpr right = factor();
		pExpr expr = std::make_unique<Expr>(
		    Binary(std::move(left), std::move(right), op));
		left = std::move(expr);
	}

	return left;
}

Parser::pExpr Parser::factor() {
	pExpr left = unary();
	while (match({scanner::TokenTypes::STAR, scanner::TokenTypes::SLASH})) {
		scanner::Token op = pre_token();
		pExpr right = unary();
		pExpr expr = std::make_unique<Expr>(
		    Binary(std::move(left), std::move(right), op));
		left = std::move(expr);
	}

	return left;
}

Parser::pExpr Parser::unary() {
	if (match(scanner::MINUS)) {
		scanner::Token op = pre_token();
		pExpr opd = unary();
		pExpr expr = std::make_unique<Expr>(Unary(op, std::move(opd)));
		return expr;
	}
	return primary();
}

Parser::pExpr Parser::primary() {
	if (match(scanner::LEFT_PAREN)) {
		pExpr expr = term();
		pExpr grouping = std::make_unique<Expr>(Grouping(std::move(expr)));
		match(scanner::RIGHT_PAREN);
		return grouping;
	}
	if (match({scanner::INT, scanner::FLOAT})) {
		scanner::Token number = pre_token();
		return std::make_unique<Expr>(Literal(number));
	}

	auto fake = scanner::Token(0, scanner::INT, "0"); // 之后再做错误处理
	return std::make_unique<Expr>(Literal(fake));
}

} // namespace parser
} // namespace cbycpp

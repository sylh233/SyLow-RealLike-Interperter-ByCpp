#include <exception.hpp>
#include <iostream>
#include <parser.hpp>

namespace cbycpp {
namespace parser {
pStmt Parser::parse(std::unique_ptr<scanner::Scanner::vTokens> tokens) {
	tokens_pointer = tokens.get();
	pStmt stmt{};
	try {
		stmt = statement();
	} catch (exception::ParseException pe) {
		std::cout << pe.ExceptionMsg() << std::endl;
	}
	return stmt;
}

pExpr Parser::parse_expr(std::unique_ptr<scanner::Scanner::vTokens> tokens) {
	tokens_pointer = tokens.get();
	pExpr expr;
	try {
		expr = expression();
	} catch (exception::ParseException pe) {
		std::cout << pe.ExceptionMsg() << std::endl;
	}
	return expr;
}

pStmt Parser::statement() {
	if (match(scanner::WRITE))
		return writeStmt();
	return exprStmt();
}

pStmt Parser::writeStmt() {
	pExpr expr = expression();
	return std::make_unique<Statement>(WriteStmt(std::move(expr)));
}

pStmt Parser::exprStmt() {
	pExpr expr = expression();
	return std::make_unique<Statement>(ExprStmt(std::move(expr)));
}

pExpr Parser::expression() { return equal(); }

pExpr Parser::equal() {
	pExpr left = comparison();
	while (match(
	    {scanner::TokenTypes::EQUAL_EQUAL, scanner::TokenTypes::BANG_EQUAL})) {
		scanner::Token op = pre_token();
		pExpr right = comparison();
		pExpr expr = std::make_unique<Expr>(
		    Binary(std::move(left), std::move(right), op));
		left = std::move(expr);
	}

	return left;
}

pExpr Parser::comparison() {
	pExpr left = term();
	while (match({scanner::TokenTypes::LESS, scanner::TokenTypes::LESS_EQUAL,
	              scanner::GREATER, scanner::GREATER_EQUAL})) {
		scanner::Token op = pre_token();
		pExpr right = term();
		pExpr expr = std::make_unique<Expr>(
		    Binary(std::move(left), std::move(right), op));
		left = std::move(expr);
	}

	return left;
}

pExpr Parser::term() {
	pExpr left = factor();
	while (match({scanner::TokenTypes::PLUS, scanner::TokenTypes::MINUS})) {
		scanner::Token op = pre_token();
		pExpr right = factor();
		pExpr expr = std::make_unique<Expr>(
		    Binary(std::move(left), std::move(right), op));
		left = std::move(expr);
	}

	return left; // 编译器对return优先尝试move，不用再写
}

pExpr Parser::factor() {
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

pExpr Parser::unary() {
	if (match({scanner::MINUS, scanner::BANG})) {
		scanner::Token op = pre_token();
		pExpr opd = unary();
		pExpr expr = std::make_unique<Expr>(Unary(op, std::move(opd)));
		return expr;
	}
	return primary();
}

pExpr Parser::primary() {
	if (match(scanner::LEFT_PAREN)) {
		pExpr expr = term();
		pExpr grouping = std::make_unique<Expr>(Grouping(std::move(expr)));
		expect(scanner::RIGHT_PAREN, "Expect ')' after Grouping");
		return grouping;
	}
	if (match({scanner::INT, scanner::FLOAT, scanner::CHAR, scanner::STRING,
	           scanner::TRUE, scanner::FALSE, scanner::NIL})) {
		scanner::Token value = pre_token();
		return std::make_unique<Expr>(Literal(value));
	}

	throw exception::ParseException(cur_token(), "Unexpect Token");
}

inline scanner::Token Parser::expect(scanner::TokenTypes type,
                                     std::string msg) {
	if (match(type)) {
		return pre_token();
	} else {
		throw exception::ParseException(cur_token(), msg);
	}
}

} // namespace parser
} // namespace cbycpp

/*auto fake = scanner::Token(0, scanner::INT, "0"); // 之后再做错误处理
      return std::make_unique<Expr>(Literal(fake));*/

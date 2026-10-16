#pragma once

#include <cstddef>
#include <memory>
#include <scanner.hpp>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant.hpp>
// #include <cstdlib>

namespace cbycpp {
namespace parser {
class ExprVisitor;

enum VisitorReturnType {
	VARIANT,
	STRING,
	NOUN,
};

const std::unordered_map<VisitorReturnType, std::size_t> VRT_size_map = {
    {VARIANT, sizeof(scanner::Variant)},
    {STRING, sizeof(std::string)},
    {NOUN, 0}};

class VisitorReturn {
  public:
	const VisitorReturnType type;
	void *pointer;
	VisitorReturn(VisitorReturnType type, void *value) : type(type) {
		switch (type) {
		case VARIANT:
			pointer = new scanner::Variant(*(scanner::Variant *)value);
			break;
		case STRING:
			pointer = new std::string(*(std::string *)value);
			break;
		case NOUN:
			pointer = nullptr;
			break;
		}
	}
	~VisitorReturn() {
		switch (type) {
		case VARIANT:
			delete (scanner::Variant *)pointer;
			break;
		case STRING:
			delete (std::string *)pointer;
			break;
		case NOUN:
			break;
		}
	}
};

class Expr {
  public:
	virtual VisitorReturn accept(ExprVisitor &visitor) = 0;
	virtual ~Expr() = default;
};

class Binary : public Expr {
  public:
	const std::unique_ptr<Expr> left, right;
	const scanner::Token op;

	VisitorReturn accept(ExprVisitor &visitor) override;
	Binary(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right,
	       scanner::Token &op)
	    : left(std::move(left)), right(std::move(right)), op(op) {}
};

class Literal : public Expr {
  public:
	const scanner::Token literal;

	VisitorReturn accept(ExprVisitor &visitor) override;
	Literal(scanner::Token &literal) : literal(literal) {}
};

class Grouping : public Expr {
  public:
	std::unique_ptr<Expr> expr;

	VisitorReturn accept(ExprVisitor &visitor) override;
	Grouping(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}
};

class Unary : public Expr {
  public:
	const scanner::Token op;
	const std::unique_ptr<Expr> opd;

	VisitorReturn accept(ExprVisitor &visitor) override;
	Unary(scanner::Token &op, std::unique_ptr<Expr> opd)
	    : op(op), opd(std::move(opd)) {}
};
} // namespace parser
} // namespace cbycpp

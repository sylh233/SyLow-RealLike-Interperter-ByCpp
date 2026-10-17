#pragma once

#include <cstddef>
#include <memory>
#include <scanner.hpp>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant.hpp>
#include <variant>
// #include <cstdlib>

namespace cbycpp {
namespace parser {
using VisitorReturn =
    std::variant<std::monostate, scanner::Variant, std::string>;

class Binary;
class Literal;
class Grouping;
class Unary;

using Expr = std::variant<Binary, Literal, Grouping, Unary>;
using pExpr = std::unique_ptr<Expr>;

class Binary {
  public:
	pExpr left, right;
	scanner::Token op;

	Binary(pExpr left, pExpr right, scanner::Token &op)
	    : left(std::move(left)), right(std::move(right)), op(op) {}
};

class Literal {
  public:
	scanner::Token literal;

	Literal(scanner::Token &literal) : literal(literal) {}
};

class Grouping {
  public:
	pExpr expr;

	Grouping(pExpr expr) : expr(std::move(expr)) {}
};

class Unary {
  public:
	scanner::Token op;
	pExpr opd;

	Unary(scanner::Token &op, pExpr opd) : op(op), opd(std::move(opd)) {}
};
} // namespace parser
} // namespace cbycpp

/*enum VisitorReturnType {
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
}; */

/*class Expr {
public:
virtual VisitorReturn accept(ExprVisitor &visitor) = 0;
virtual ~Expr() = default;
};*/

#pragma once

#include <cstdint>
#include <expr.hpp>
#include <functional>
#include <variant.hpp>
#include <variant>

namespace cbycpp {
namespace parser {
class Evalator {
  public:
	VisitorReturn visit(Expr &expr);

  private:
	scanner::Variant visitExpr(Expr &expr);
	scanner::Variant visitBinary(Binary &binary);
	scanner::Variant visitLiteral(Literal &literal);
	scanner::Variant visitGrouping(Grouping &grouping);
	scanner::Variant visitUnary(Unary &unary);

	inline double ifIntToDouble(scanner::Variant &var) {
		if (std::holds_alternative<int>(var)) {
			return static_cast<double>(std::get<int>(var));
		} else {
			return std::get<double>(var);
		}
	}

	template <typename Op>
	inline scanner::Variant numBinary(scanner::Variant left,
	                                  scanner::Variant right, Op op) {
		if (std::holds_alternative<int>(left) &&
		    std::holds_alternative<int>(right)) {
			return op(std::get<int>(left), std::get<int>(right));
		} else {
			return op(ifIntToDouble(left), ifIntToDouble(right));
		}
	}
};
} // namespace parser
} // namespace cbycpp

/*template <typename N, typename Op>
    N numBinary(scanner::Variant *p_left, scanner::Variant *p_right, Op op) {
        N l_v{}, r_v{};
        p_left->get_value(&l_v);
        p_right->get_value(&r_v);
        return op(l_v, r_v);
        }*/

/*class ExprVisitor {
  public:
    virtual VisitorReturn visit(Binary &binary) = 0;
    virtual VisitorReturn visit(Literal &literal) = 0;
    virtual VisitorReturn visit(Grouping &grouping) = 0;
    virtual VisitorReturn visit(Unary &unary) = 0;

    virtual ~ExprVisitor() = default;
    };*/

#pragma once

#include <expr.hpp>
#include <functional>
#include <variant.hpp>
#include <cstdint>

namespace cbycpp {
namespace parser {
class ExprVisitor {
  public:
	virtual VisitorReturn visit(Binary &binary) = 0;
	virtual VisitorReturn visit(Literal &literal) = 0;
	virtual VisitorReturn visit(Grouping &grouping) = 0;
	virtual VisitorReturn visit(Unary &unary) = 0;

	virtual ~ExprVisitor() = default;
};

class Evalator : public ExprVisitor {
  public:
	VisitorReturn visit(Binary &binary) override;
	VisitorReturn visit(Literal &literal) override;
	VisitorReturn visit(Grouping &grouping) override;
	VisitorReturn visit(Unary &unary) override;

  private:
	template <typename N, typename Op>
	N numBinary(scanner::Variant *p_left, scanner::Variant *p_right, Op op) {
		N l_v{}, r_v{};
		p_left->get_value(&l_v);
		p_right->get_value(&r_v);
		return op(l_v, r_v);
	}
};
} // namespace parser
} // namespace cbycpp

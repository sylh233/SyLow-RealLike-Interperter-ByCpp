#include <expr_visitor.hpp>

namespace cbycpp {
namespace parser {

VisitorReturn Binary::accept(ExprVisitor &visitor) {
	return visitor.visit(*this);
}

VisitorReturn Literal::accept(ExprVisitor &visitor) {
	return visitor.visit(*this);
}

VisitorReturn Grouping::accept(ExprVisitor &visitor) {
	return visitor.visit(*this);
}

VisitorReturn Unary::accept(ExprVisitor &visitor) {
	return visitor.visit(*this);
}
} // namespace parser

} // namespace cbycpp

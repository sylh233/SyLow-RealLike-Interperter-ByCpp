#pragma once

#include <expr_visitor.hpp>
#include <scanner.hpp>
#include <variant.hpp>

namespace cbycpp {
namespace parser {
class Expr {
  public:
	virtual scanner::Variant accept() = 0;
	virtual ~Expr() = default;
};

class Binary : public Expr {
  public:
	scanner::Variant accept() override;
};
} // namespace parser
} // namespace cbycpp

#pragma once

#include <expr.hpp>
#include <expr_visitor.hpp>
#include <memory>
#include <variant>

namespace cbycpp {
namespace parser {
class WriteStmt;
class ExprStmt;

using Statement = std::variant<WriteStmt, ExprStmt>;
using pStmt = std::unique_ptr<Statement>;

class WriteStmt {
  public:
	pExpr expr;
	WriteStmt(pExpr expr) : expr(std::move(expr)) {}
};

class ExprStmt {
  public:
	pExpr expr;
	ExprStmt(pExpr expr) : expr(std::move(expr)) {}
};

class Executor {
  public:
	Executor() { eval.reset(new Evalator{}); }
	Executor(std::unique_ptr<Evalator> eval) : eval(std::move(eval)) {}
	void visit(Statement &stmt);

  private:
	std::unique_ptr<Evalator> eval;
	void visitStmt(Statement &stmt);
	void visitWriteStmt(WriteStmt &stmt);
	void visitExprStmt(ExprStmt &stmt);
};
} // namespace parser

} // namespace cbycpp

#include <iostream>
#include <statement.hpp>
#include <string>
#include <variant.hpp>

namespace cbycpp {
namespace parser {
void Executor::visit(Statement &stmt) {
	visitStmt(stmt);
	return;
}

void Executor::visitStmt(Statement &stmt) {
	if (std::holds_alternative<WriteStmt>(stmt)) {
		visitWriteStmt(std::get<WriteStmt>(stmt));
	} else if (std::holds_alternative<ExprStmt>(stmt)) {
		visitExprStmt(std::get<ExprStmt>(stmt));
	}
	return;
}

void Executor::visitWriteStmt(WriteStmt &stmt) {
	auto var = eval->visit(*stmt.expr);
	std::cout << scanner::var_to_string(var) << std::endl;
	// 这个var_to_string函数在variant.cpp这个源文件中定义
	// 而又在一个源文件中使用，所以库的链接顺序会对链接能否成功产生影响
	// 前面的库依赖后面的库，所以variant所在库必须在该库的后面
}

void Executor::visitExprStmt(ExprStmt &stmt) {
	auto expr = eval->visit(*stmt.expr);
}

} // namespace parser

} // namespace cbycpp

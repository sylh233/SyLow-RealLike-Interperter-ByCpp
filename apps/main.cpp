#include <expr.hpp>
#include <expr_visitor.hpp>
#include <iostream>
#include <parser.hpp>
#include <scanner.hpp>
#include <statement.hpp>
#include <string>

int main() {
	using namespace cbycpp;
	scanner::Scanner s1{};
	// std::string bytes = "233.33+1/(19+66)";
	bool quit_flag = true;
	while (quit_flag) {
		std::cout << ">>> ";
		std::string bytes{};
		std::getline(std::cin, bytes);
		if (bytes.length() == 0 || bytes.at(0) == 'e')
			break;
		auto t = s1.scan(bytes);
		// std::cout << "TOKENS:" << std::endl;
		// for (auto &token : *t) {
		// 	std::cout << token.toString() << std::endl;
		// }
		
		// std::unique_ptr<parser::Evalator> visitor;
		// visitor.reset(new parser::Evalator());

		parser::Parser p1{};
		auto stmt = p1.parse(std::move(t));

		auto visitor = parser::Executor();

		if (stmt != nullptr) {
			visitor.visit(*stmt);
		}

		// parser::AST_printer astp{};

		// std::cout << astp.visit(*expr) << std::endl;

		// if (expr != nullptr) {
		// 	auto var = visitor->visit(*expr);
		// 	std::cout << "Result = " << scanner::VTStr_with_quote(var)
		// 	          << std::endl;
		// }
	}

	return 0;
}
// if (returnT.type == cbycpp::parser::VARIANT) {
// 	auto var = (scanner::Variant *)returnT.pointer;
// 	switch (var->get_type()) {
// 	case cbycpp::scanner::Variant::Type::INT: {
// 		int re{};
// 		var->get_value(&re);
// 		std::cout << "Result = " << re << std::endl;
// 	} break;
// 	case cbycpp::scanner::Variant::Type::DOUBLE: {
// 		double re{};
// 		var->get_value(&re);
// 		std::cout << "Result = " << re << std::endl;
// 	} break;
// 	default:
// 		break;
// 	}
// }

// std::string b1 = "123+233+20";
// auto ts(s1.scan(b1));
// for (auto &token : *ts) {
// 	std::cout << token.toString() << std::endl;
// }

// std::unique_ptr<parser::Expr> left;
// left.reset(new parser::Literal(ts->at(0)));

// scanner::Token op(ts->at(1));

// std::unique_ptr<parser::Expr> right;
// right.reset(new parser::Literal(ts->at(2)));

// std::unique_ptr<parser::Expr> binary;
// binary.reset(new parser::Binary(std::move(left), std::move(right), op));

// std::unique_ptr<parser::ExprVisitor> visitor;
// visitor.reset(new parser::Evalator());

// parser::Parser p1{};
// auto binary = p1.parse(std::move(ts));

// auto returnT = binary->accept(*visitor);

// if (returnT.type == cbycpp::parser::VARIANT) {
// 	auto var = (scanner::Variant *)returnT.pointer;
// 	int re{};
// 	var->get_value(&re);
// 	std::cout << re << std::endl;
// }

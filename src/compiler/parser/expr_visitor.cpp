#include <expr_visitor.hpp>

namespace cbycpp {
namespace parser {
VisitorReturn Evalator::visit(Binary &binary) {
	VisitorReturn left = binary.left->accept(*this);
	VisitorReturn right = binary.right->accept(*this);

	auto get_value = [&]() -> scanner::Variant {
		if (left.type == VARIANT && right.type == VARIANT) {
			switch (binary.op.type) {
			case scanner::PLUS: {
				auto p_left = (scanner::Variant *)left.pointer;
				auto p_right = (scanner::Variant *)right.pointer;
				if (p_left->get_type() == scanner::Variant::Type::I32 &&
				    p_right->get_type() == scanner::Variant::Type::I32) {
					std::int32_t result = numBinary<std::int32_t>(
					    p_left, p_right, std::plus<std::int32_t>{});
					return scanner::Variant(&result,
					                        scanner::Variant::Type::I32);
				} else if (p_left->get_type() == scanner::Variant::Type::F64 ||
				           p_right->get_type() == scanner::Variant::Type::F64) {
					double result =
					    numBinary<double>(p_left, p_right, std::plus<double>{});
					return scanner::Variant(&result,
					                        scanner::Variant::Type::F64);
				}
			}
			case scanner::MINUS: {
				auto p_left = (scanner::Variant *)left.pointer;
				auto p_right = (scanner::Variant *)right.pointer;
				if (p_left->get_type() == scanner::Variant::Type::I32 &&
				    p_right->get_type() == scanner::Variant::Type::I32) {
					std::int32_t result = numBinary<std::int32_t>(
					    p_left, p_right, std::minus<std::int32_t>{});
					return scanner::Variant(&result,
					                        scanner::Variant::Type::I32);
				} else if (p_left->get_type() == scanner::Variant::Type::F64 ||
				           p_right->get_type() == scanner::Variant::Type::F64) {
					double result = numBinary<double>(p_left, p_right,
					                                  std::minus<double>{});
					return scanner::Variant(&result,
					                        scanner::Variant::Type::F64);
				}
			}
			case scanner::STAR: {
				auto p_left = (scanner::Variant *)left.pointer;
				auto p_right = (scanner::Variant *)right.pointer;
				if (p_left->get_type() == scanner::Variant::Type::I32 &&
				    p_right->get_type() == scanner::Variant::Type::I32) {
					std::int32_t result = numBinary<std::int32_t>(
					    p_left, p_right, std::multiplies<std::int32_t>{});
					return scanner::Variant(&result,
					                        scanner::Variant::Type::I32);
				} else if (p_left->get_type() == scanner::Variant::Type::F64 ||
				           p_right->get_type() == scanner::Variant::Type::F64) {
					double result = numBinary<double>(
					    p_left, p_right, std::multiplies<double>{});
					return scanner::Variant(&result,
					                        scanner::Variant::Type::F64);
				}
			}
			case scanner::SLASH: {
				auto p_left = (scanner::Variant *)left.pointer;
				auto p_right = (scanner::Variant *)right.pointer;
				if (p_left->get_type() == scanner::Variant::Type::I32 &&
				    p_right->get_type() == scanner::Variant::Type::I32) {
					std::int32_t result = numBinary<std::int32_t>(
					    p_left, p_right, std::divides<std::int32_t>{});
					return scanner::Variant(&result,
					                        scanner::Variant::Type::I32);
				} else if (p_left->get_type() == scanner::Variant::Type::F64 ||
				           p_right->get_type() == scanner::Variant::Type::F64) {
					double result = numBinary<double>(p_left, p_right,
					                                  std::divides<double>{});
					return scanner::Variant(&result,
					                        scanner::Variant::Type::F64);
				}
			}

			default:
				break;
			}
		}
		int t = 0;
		return scanner::Variant(&t, scanner::Variant::Type::I32);
	};

	scanner::Variant tv = get_value();
	VisitorReturn result = VisitorReturn(VisitorReturnType::VARIANT, &tv);
	return result;
}

VisitorReturn Evalator::visit(Literal &literal) {
	switch (literal.literal.type) {
	case scanner::NUM: {
		std::int32_t number =
		    static_cast<std::int32_t>(std::stoi(literal.literal.lexeme));
		auto v = scanner::Variant(&number, scanner::Variant::I32);
		return VisitorReturn(VARIANT, &v);
	}
	case scanner::FLOAT: {
		double number = std::stod(literal.literal.lexeme);
		auto v = scanner::Variant(&number, scanner::Variant::F64);
		return VisitorReturn(VARIANT, &v);
	}
	default:
		break;
	}

	int t = 0;
	auto v = scanner::Variant(&t, scanner::Variant::Type::I32);
	return VisitorReturn(VARIANT, &v);
}

VisitorReturn Evalator::visit(Grouping &grouping) {
	return grouping.expr->accept(*this);
}

VisitorReturn Evalator::visit(Unary &unary) {
	auto opd = unary.opd->accept(*this);

	auto get_value = [&]() -> scanner::Variant {
		if (opd.type == VARIANT) {
			auto var = (scanner::Variant *)opd.pointer;
			switch (unary.op.type) {
			case scanner::MINUS:
				switch (var->get_type()) {
				case scanner::Variant::I32: {
					int32_t v{};
					var->get_value(&v);
					v = -v;
					return scanner::Variant(&v, scanner::Variant::I32);
				}
				case scanner::Variant::F64: {
					double v{};
					var->get_value(&v);
					v = -v;
					return scanner::Variant(&v, scanner::Variant::F64);
				}
				default:
					break;
				}
			default:
				break;
			}
		}
		int v = 0;
		return scanner::Variant(&v, scanner::Variant::I32);
	};

	auto var = get_value();
	return VisitorReturn(VARIANT, &var);
}
} // namespace parser
} // namespace cbycpp

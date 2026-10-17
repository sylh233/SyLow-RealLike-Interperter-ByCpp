#include <expr_visitor.hpp>

namespace cbycpp {
namespace parser {
VisitorReturn Evalator::visit(Expr &expr) { return visitExpr(expr); }

scanner::Variant Evalator::visitExpr(Expr &expr) {
	if (std::holds_alternative<Binary>(expr)) {
		return visitBinary(std::get<Binary>(expr));
	} else if (std::holds_alternative<Literal>(expr)) {
		return visitLiteral(std::get<Literal>(expr));
	} else if (std::holds_alternative<Grouping>(expr)) {
		return visitGrouping(std::get<Grouping>(expr));
	} else if (std::holds_alternative<Unary>(expr)) {
		return visitUnary(std::get<Unary>(expr));
	}
	return scanner::Variant();
}

scanner::Variant Evalator::visitBinary(Binary &binary) {
	auto left = visitExpr(*binary.left);
	auto right = visitExpr(*binary.right);
	auto op = binary.op;

	switch (op.type) {
	case scanner::PLUS:
		return numBinary(left, right, std::plus<>{});
	case scanner::MINUS:
		return numBinary(left, right, std::minus<>{});
	case scanner::STAR:
		return numBinary(left, right, std::multiplies<>{});
	case scanner::SLASH:
		return numBinary(left, right, std::divides<>{});
	default:
		break;
	}

	return scanner::Variant();
}

scanner::Variant Evalator::visitLiteral(Literal &literal) {
	auto lit = literal.literal;
	switch (lit.type) {
	case scanner::INT:
		return std::stoi(lit.lexeme);
	case scanner::FLOAT:
		return std::stod(lit.lexeme);
	default:
		break;
	}
	return scanner::Variant();
}
scanner::Variant Evalator::visitGrouping(Grouping &grouping) {
	return visitExpr(*grouping.expr);
}
scanner::Variant Evalator::visitUnary(Unary &unary) {
	auto opd = visitExpr(*unary.opd);
	auto op = unary.op;

	switch (op.type) {
	case scanner::MINUS:
		if (std::holds_alternative<int>(opd)) {
			return -std::get<int>(opd);
		} else {
			return -ifIntToDouble(opd);
		}
	default:
		break;
	}
	return scanner::Variant();
}

} // namespace parser
} // namespace cbycpp

/*VisitorReturn Evalator::visit(Binary &binary) {
    VisitorReturn left = binary.left->accept(*this);
    VisitorReturn right = binary.right->accept(*this);

    auto get_value = [&]() -> scanner::Variant {
        if (left.type == VARIANT && right.type == VARIANT) {
            switch (binary.op.type) {
            case scanner::PLUS: {
                auto p_left = (scanner::Variant *)left.pointer;
                auto p_right = (scanner::Variant *)right.pointer;
                if (p_left->get_type() == scanner::Variant::Type::INT &&
                    p_right->get_type() == scanner::Variant::Type::INT) {
                    std::int32_t result = numBinary<std::int32_t>(
                        p_left, p_right, std::plus<std::int32_t>{});
                    return scanner::Variant(&result,
                                            scanner::Variant::Type::INT);
                } else if (p_left->get_type() ==
                               scanner::Variant::Type::DOUBLE ||
                           p_right->get_type() ==
                               scanner::Variant::Type::DOUBLE) {
                    double result =
                        numBinary<double>(p_left, p_right, std::plus<double>{});
                    return scanner::Variant(&result,
                                            scanner::Variant::Type::DOUBLE);
                }
            }
            case scanner::MINUS: {
                auto p_left = (scanner::Variant *)left.pointer;
                auto p_right = (scanner::Variant *)right.pointer;
                if (p_left->get_type() == scanner::Variant::Type::INT &&
                    p_right->get_type() == scanner::Variant::Type::INT) {
                    std::int32_t result = numBinary<std::int32_t>(
                        p_left, p_right, std::minus<std::int32_t>{});
                    return scanner::Variant(&result,
                                            scanner::Variant::Type::INT);
                } else if (p_left->get_type() ==
                               scanner::Variant::Type::DOUBLE ||
                           p_right->get_type() ==
                               scanner::Variant::Type::DOUBLE) {
                    double result = numBinary<double>(p_left, p_right,
                                                      std::minus<double>{});
                    return scanner::Variant(&result,
                                            scanner::Variant::Type::DOUBLE);
                }
            }
            case scanner::STAR: {
                auto p_left = (scanner::Variant *)left.pointer;
                auto p_right = (scanner::Variant *)right.pointer;
                if (p_left->get_type() == scanner::Variant::Type::INT &&
                    p_right->get_type() == scanner::Variant::Type::INT) {
                    std::int32_t result = numBinary<std::int32_t>(
                        p_left, p_right, std::multiplies<std::int32_t>{});
                    return scanner::Variant(&result,
                                            scanner::Variant::Type::INT);
                } else if (p_left->get_type() ==
                               scanner::Variant::Type::DOUBLE ||
                           p_right->get_type() ==
                               scanner::Variant::Type::DOUBLE) {
                    double result = numBinary<double>(
                        p_left, p_right, std::multiplies<double>{});
                    return scanner::Variant(&result,
                                            scanner::Variant::Type::DOUBLE);
                }
            }
            case scanner::SLASH: {
                auto p_left = (scanner::Variant *)left.pointer;
                auto p_right = (scanner::Variant *)right.pointer;
                if (p_left->get_type() == scanner::Variant::Type::INT &&
                    p_right->get_type() == scanner::Variant::Type::INT) {
                    std::int32_t result = numBinary<std::int32_t>(
                        p_left, p_right, std::divides<std::int32_t>{});
                    return scanner::Variant(&result,
                                            scanner::Variant::Type::INT);
                } else if (p_left->get_type() ==
                               scanner::Variant::Type::DOUBLE ||
                           p_right->get_type() ==
                               scanner::Variant::Type::DOUBLE) {
                    double result = numBinary<double>(p_left, p_right,
                                                      std::divides<double>{});
                    return scanner::Variant(&result,
                                            scanner::Variant::Type::DOUBLE);
                }
            }

            default:
                break;
            }
        }
        int t = 0;
        return scanner::Variant(&t, scanner::Variant::Type::INT);
    };

    scanner::Variant tv = get_value();
    VisitorReturn result = VisitorReturn(VisitorReturnType::VARIANT, &tv);
    return result;
}

VisitorReturn Evalator::visit(Literal &literal) {
    switch (literal.literal.type) {
    case scanner::INT: {
        std::int32_t number =
            static_cast<std::int32_t>(std::stoi(literal.literal.lexeme));
        auto v = scanner::Variant(&number, scanner::Variant::Type::INT);
        return VisitorReturn(VARIANT, &v);
    }
    case scanner::FLOAT: {
        double number = std::stod(literal.literal.lexeme);
        auto v = scanner::Variant(&number, scanner::Variant::Type::DOUBLE);
        return VisitorReturn(VARIANT, &v);
    }
    default:
        break;
    }

    int t = 0;
    auto v = scanner::Variant(&t, scanner::Variant::Type::INT);
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
                case scanner::Variant::Type::INT: {
                    int32_t v{};
                    var->get_value(&v);
                    v = -v;
                    return scanner::Variant(&v, scanner::Variant::Type::INT);
                }
                case scanner::Variant::Type::DOUBLE: {
                    double v{};
                    var->get_value(&v);
                    v = -v;
                    return scanner::Variant(&v, scanner::Variant::Type::DOUBLE);
                }
                default:
                    break;
                }
            default:
                break;
            }
        }
        int v = 0;
        return scanner::Variant(&v, scanner::Variant::Type::INT);
    };

    auto var = get_value();
    return VisitorReturn(VARIANT, &var);
}*/

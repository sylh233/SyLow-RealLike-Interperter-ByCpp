#include <expr_visitor.hpp>
#include <unicode.hpp>

namespace cbycpp {
namespace parser {
scanner::Variant Evalator::visit(Expr &expr) { return visitExpr(expr); }

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

	if (std::holds_alternative<std::string>(left) ||
	    std::holds_alternative<std::string>(right)) {
		switch (op.type) {
		case scanner::PLUS:
			return strBinary(left, right, std::plus<>{});
		case scanner::LESS:
			return strBinary(left, right, std::less<>{});
		case scanner::LESS_EQUAL:
			return strBinary(left, right, std::less_equal<>{});
		case scanner::GREATER:
			return strBinary(left, right, std::greater<>{});
		case scanner::GREATER_EQUAL:
			return strBinary(left, right, std::greater_equal<>{});
		case scanner::EQUAL_EQUAL:
			return strBinary(left, right, std::equal_to<>{});
		case scanner::BANG_EQUAL:
			return strBinary(left, right, std::not_equal_to<>{});
		default:
			break;
		}
	} else {
		switch (op.type) {
		case scanner::PLUS:
			return numBinary(left, right, std::plus<>{});
		case scanner::MINUS:
			return numBinary(left, right, std::minus<>{});
		case scanner::STAR:
			return numBinary(left, right, std::multiplies<>{});
		case scanner::SLASH:
			return numBinary(left, right, std::divides<>{});
		case scanner::LESS:
			return numBinary(left, right, std::less<>{});
		case scanner::LESS_EQUAL:
			return numBinary(left, right, std::less_equal<>{});
		case scanner::GREATER:
			return numBinary(left, right, std::greater<>{});
		case scanner::GREATER_EQUAL:
			return numBinary(left, right, std::greater_equal<>{});
		case scanner::EQUAL_EQUAL:
			return numBinary(left, right, std::equal_to<>{});
		case scanner::BANG_EQUAL:
			return numBinary(left, right, std::not_equal_to<>{});
		default:
			break;
		}
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
	case scanner::CHAR:
		return scanner::Char(
		    scanner::UnicodeUtil::getU16String(lit.lexeme).char32At(0));
	case scanner::STRING:
		return lit.lexeme;
	case scanner::TRUE:
		return true;
	case scanner::FALSE:
		return false;
	case scanner::NIL:
		return std::monostate{};
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
		if (std::holds_alternative<std::string>(opd))
			break;
		opd = ifNullToInt(opd);
		opd = ifCharToInt(opd);
		opd = ifBoolToInt(opd);
		if (std::holds_alternative<int>(opd)) {
			return -std::get<int>(opd);
		} else {
			return -std::get<double>(ifIntToDouble(opd));
		}
	case scanner::BANG:
		return !toBool(opd);
	default:
		break;
	}
	return scanner::Variant();
}

std::string AST_printer::visit(Expr &expr) {
	layers = 0;
	std::string head = "<AST>:{";
	std::string child = visitExpr(expr);
	std::string rear = "}";
	return head + "\n" + child + "\n" + rear;
}

std::string AST_printer::visitExpr(Expr &expr) {
	std::string expr_str;
	if (std::holds_alternative<Binary>(expr)) {
		expr_str = visitBinary(std::get<Binary>(expr));
	} else if (std::holds_alternative<Literal>(expr)) {
		expr_str = visitLiteral(std::get<Literal>(expr));
	} else if (std::holds_alternative<Grouping>(expr)) {
		expr_str = visitGrouping(std::get<Grouping>(expr));
	} else if (std::holds_alternative<Unary>(expr)) {
		expr_str = visitUnary(std::get<Unary>(expr));
	}
	return expr_str;
}

std::string AST_printer::visitBinary(Binary &binary) {
	layers++;
	auto op = binary.op;
	std::string head =
	    std::string(layers, '\t') + "<BINARY:" + op.toString() + ">:{";
	auto left = visitExpr(*binary.left);
	auto right = visitExpr(*binary.right);
	std::string child = left + ",\n" + right;
	std::string rear = std::string(layers, '\t') + "}";
	layers--;
	return head + "\n" + child + "\n" + rear;
}

std::string AST_printer::visitLiteral(Literal &literal) {
	auto lit = literal.literal;

	layers++;
	std::string lit_str =
	    std::string(layers, '\t') + "<LITERAL:" + lit.toString() + ">";
	layers--;
	return lit_str;
}

std::string AST_printer::visitGrouping(Grouping &grouping) {
	layers++;
	auto expr = visitExpr(*grouping.expr);
	std::string head = std::string(layers, '\t') + "<Grouping>:{";
	std::string rear = std::string(layers, '\t') + "}";
	layers--;
	return head + "\n" + expr + "\n" + rear;
}

std::string AST_printer::visitUnary(Unary &unary) {
	auto op = unary.op;

	layers++;
	std::string head =
	    std::string(layers, '\t') + "<UNARY:" + op.toString() + ">:{";
	auto opd = visitExpr(*unary.opd);
	std::string rear = std::string(layers, '\t') + "}";
	layers--;
	return head + "\n" + opd + "\n" + rear;
}

/*
  <+
      12
      23
  >

  <+ <+
        1
        2
     >
     <grouping 10>
  >

  [PLUS "+" line:0]:{
      [INT "100" line:0],
      [INT "200" line:0]
  }

  [MINUS "-" line:1]:{
      [FLOAT "1.2" line:1]
  }

  [Grouping]:{
      [MINUS "-" line:1]:{
          [FLOAT "1.2" line:1]
      }
  }

  [ + -- 1
      |- 2
  ]
  (+ 1,2)
  <1 + 2>
 */

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

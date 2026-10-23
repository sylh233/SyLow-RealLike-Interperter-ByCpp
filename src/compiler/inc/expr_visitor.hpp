#pragma once

#include <cstddef>
#include <cstdint>
#include <expr.hpp>
#include <functional>
#include <variant.hpp>
#include <variant>

namespace cbycpp {
namespace parser {
class Evalator {
  public:
	scanner::Variant visit(Expr &expr); // 对外接口

  private:
	scanner::Variant visitExpr(Expr &expr); // 对内类型转发
	scanner::Variant visitBinary(Binary &binary);
	scanner::Variant visitLiteral(Literal &literal);
	scanner::Variant visitGrouping(Grouping &grouping);
	scanner::Variant visitUnary(Unary &unary);

	inline scanner::Variant ifCharToInt(scanner::Variant &var) const {
		if (std::holds_alternative<scanner::Char>(var)) {
			return std::get<scanner::Char>(var).getInt();
		} else {
			return var;
		}
	}

	inline scanner::Variant ifBoolToInt(scanner::Variant &var) const {
		if (std::holds_alternative<bool>(var)) {
			return static_cast<int>(std::get<bool>(var));
		} else {
			return var;
		}
	}

	inline scanner::Variant ifIntToDouble(scanner::Variant &var) const {
		if (std::holds_alternative<int>(var)) {
			return static_cast<double>(std::get<int>(var));
		} else {
			return var;
		}
	}

	inline scanner::Variant ifNullToInt(scanner::Variant &var) const {
		if (std::holds_alternative<std::monostate>(var)) {
			return 0;
		} else {
			return var;
		}
	}

	inline bool toBool(scanner::Variant &var) const {
		if (std::holds_alternative<std::string>(var)) {
			if (std::get<std::string>(var).length())
				return true;
			else
				return false;
		} else if (!std::holds_alternative<bool>(var)) {
			auto t = ifCharToInt(var);
			if (std::get<double>(ifIntToDouble(t)))
				return true;
			else
				return false;
		} else if (std::holds_alternative<std::monostate>(var)) {
			return false;
		} else {
			return std::get<bool>(var);
		}
	}

	template <typename Op>
	inline scanner::Variant numBinary(scanner::Variant left,
	                                  scanner::Variant right, Op op) const {
		left = ifNullToInt(left);
		right = ifNullToInt(right);
		left = ifCharToInt(left);
		right = ifCharToInt(right);
		left = ifBoolToInt(left);
		right = ifBoolToInt(right);
		if (std::holds_alternative<int>(left) &&
		    std::holds_alternative<int>(right)) {
			return op(std::get<int>(left), std::get<int>(right));
		} else {
			return op(std::get<double>(ifIntToDouble(left)),
			          std::get<double>(ifIntToDouble(right)));
		}
	}

	template <typename Op>
	inline std::string strBinary(scanner::Variant &left,
	                             scanner::Variant &right, Op op) const {
		return scanner::var_to_string(left) + scanner::var_to_string(right);
	}
};

class AST_printer {
  private:
	std::size_t layers{};

	std::string visitExpr(Expr &expr);
	std::string visitBinary(Binary &binary);
	std::string visitLiteral(Literal &literal);
	std::string visitGrouping(Grouping &grouping);
	std::string visitUnary(Unary &unary);

  public:
	std::string visit(Expr &expr);
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

#pragma once

#include <expr.hpp>

namespace cbycpp {
namespace parser {
	template<typename T>
	class ExprVisitor{
		public:
		virtual T visitBinary(Binary& binary) = 0;
		
		virtual ~ExprVisitor() = default;
	};

	
}
} // namespace cbycpp

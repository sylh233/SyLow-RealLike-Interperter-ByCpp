#pragma once

#include <cstddef>
#include <memory>
#include <scanner.hpp>
#include <string>

namespace cbycpp {
namespace exception {
class ScanException {
  private:
	const std::string msg;
	const std::size_t lines;
	const char c;

  public:
	ScanException(char c, std::size_t lines, std::string msg)
	    : c(c), lines(lines), msg(msg) {}
	std::string ExceptionMsg() const;
};

class ParseException {
  private:
	const scanner::Token token;
	const std::string msg;

  public:
	ParseException(scanner::Token token, std::string msg)
	    : token(token), msg(msg) {}

	std::string ExceptionMsg() const;
};
} // namespace exception
} // namespace cbycpp

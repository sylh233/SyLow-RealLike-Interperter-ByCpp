#include <exception.hpp>

namespace cbycpp {
namespace exception {
std::string ScanException::ExceptionMsg() const {
	std::string str =
	    "Scan Exception: " + msg + " [in line " + std::to_string(lines);
	if (c == '\0')
		return str + " at End]";
	return str + " at Char '" + c + "']";
}

std::string ParseException::ExceptionMsg() const {
	std::string str =
	    "Parse Exception: " + msg + " [in line " + std::to_string(token.lines);
	if (token.type == scanner::END)
		return str + " at End]";
	return str + " at '" + token.lexeme + "']";
}
} // namespace exception

} // namespace cbycpp

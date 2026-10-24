#include <exception.hpp>

namespace cbycpp {
namespace exception {
std::string ScanException::ExceptionMsg() const {
	std::string str =
	    "Scan Exception: " + msg + " [in line " + std::to_string(lines);
	if (c == '\0')
		return str + " at End]";
	return str + " at Char '" + scanner::UnicodeUtil::getU8String(c) + "']";
}

std::string ParseException::ExceptionMsg() const {
	std::string str =
	    "Parse Exception: " + msg;
	return str + " at " + token.toString();
}
} // namespace exception

} // namespace cbycpp

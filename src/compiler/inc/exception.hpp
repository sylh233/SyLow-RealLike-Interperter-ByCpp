#include <cstddef>
#include <string>

namespace cbycpp {
namespace exception {
class ScanException {
  private:
	std::string msg{};
	std::size_t lines{};
	char c{};

  public:
	ScanException(char c, std::size_t lines, std::string msg)
	    : c(c), lines(lines), msg(msg) {}
	std::string ExceptionMsg() {
		std::string str =
		    "Scan Exception:" + msg + " in line " + std::to_string(lines);
		if (c == '\0')
			return str + " End";
		return str + " Char " + c;
	}
};
} // namespace exception
} // namespace cbycpp

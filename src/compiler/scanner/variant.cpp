#include <cstring>
#include <variant.hpp>

namespace cbycpp {
namespace scanner {
std::string var_to_string(Variant &var) {
	if (std::holds_alternative<int>(var)) {
		return std::to_string(std::get<int>(var));
	} else if (std::holds_alternative<double>(var)) {
		return std::to_string(std::get<double>(var));
	} else if (std::holds_alternative<char>(var)) {
		return std::string(1, std::get<char>(var));
	} else if (std::holds_alternative<std::string>(var)) {
		return std::get<std::string>(var);
	} else if (std::holds_alternative<bool>(var)) {
		if (std::get<bool>(var))
			return "true";
		else
			return "false";
	} else {
		return "null";
	}
}

std::string VTStr_with_quote(Variant &var) {
	if (std::holds_alternative<char>(var)) {
		return "'" + std::string(1, std::get<char>(var)) + "'";
	} else if (std::holds_alternative<std::string>(var)) {
		return "\"" + std::get<std::string>(var) + "\"";
	} else {
		return var_to_string(var);
	}
}
} // namespace scanner
} // namespace cbycpp

// Variant::Variant(void *value, Type type) : type(type) {
// 	memcpy(&(this->value), value, get_size());
// }

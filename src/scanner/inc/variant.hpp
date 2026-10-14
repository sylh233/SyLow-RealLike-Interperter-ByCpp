#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

namespace scanner {
class Variant {
  public:
	enum Type { I32, D64, C8 };
	const std::unordered_map<Type, std::size_t> type_size_map = {
	    {I32, 4}, {D64, 8}, {C8, 1}};

	inline std::size_t get_size() const { return type_size_map.at(type); }
	inline Type get_type() const { return type; }
	inline std::uint64_t get_value() const { return value; }
	inline std::string to_string() const;

	Variant(void *value, Type type);

  private:
	const Type type;
	std::uint64_t value{};
};

inline std::string Variant::to_string() const {
	switch (this->type) {
	case I32: {
		int value{};
		memcpy(&value, &(this->value), get_size());
		return std::to_string(value);
	} break;
	case D64:
	case C8:
	default:
		return "";
		break;
	}
}
} // namespace scanner

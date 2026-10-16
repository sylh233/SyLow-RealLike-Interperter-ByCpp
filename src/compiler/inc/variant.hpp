#pragma once

#include <cstdint>
#include <cstring>
#include <string>
#include <unordered_map>

namespace cbycpp {
namespace scanner {
class Variant {
  public:
	enum Type { I32, F64, C8 };
	// const std::unordered_map<Type, std::size_t> type_size_map = {
	//     {I32, 4}, {F64, 8}, {C8, 1}};

	inline std::size_t get_size() const {
		switch (type) {
		case I32:
			return 4;
		case F64:
			return 8;
		case C8:
			return 1;
		default:
			return 0;
		}
	}
	inline Type get_type() const { return type; }
	inline void get_value(void *dst) const;
	inline std::string to_string() const;

	Variant(void *value, Type type);

  private:
	const Type type;
	std::uint64_t value{};
};

inline void Variant::get_value(void *dst) const {
	switch (type) {
	case I32:
	case F64:
		std::memcpy(dst, &value, get_size());
		break;
	default:
		break;
	}
}

inline std::string Variant::to_string() const {
	switch (this->type) {
	case I32: {
		int value{};
		get_value(&value);
		return std::to_string(value);
	} break;
	case F64:
	case C8:
	default:
		return "";
		break;
	}
}
} // namespace scanner
} // namespace cbycpp

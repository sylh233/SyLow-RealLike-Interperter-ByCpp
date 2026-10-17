#pragma once

#include <cstdint>
#include <cstring>
#include <string>
#include <unordered_map>
#include <variant>

namespace cbycpp {
namespace scanner {
using Variant = std::variant<std::monostate, int, double, char, std::string>;

} // namespace scanner
} // namespace cbycpp

/*class Variant {
public:
enum class Type { INT, DOUBLE, CHAR, STRING };
// const std::unordered_map<Type, std::size_t> type_size_map = {
//     {I32, 4}, {F64, 8}, {C8, 1}};
template <Type T> struct TypeMap;
template <> struct TypeMap<Type::INT> {
    using type = int;
};
template <> struct TypeMap<Type::DOUBLE> {
    using type = double;
};
template <> struct TypeMap<Type::CHAR> {
    using type = char;
};
template <> struct TypeMap<Type::STRING> {
    using type = std::string;
};

inline std::size_t get_size() const {
    switch (type) {
    case Type::INT:
        return sizeof(TypeMap<Type::INT>::type);
    case Type::DOUBLE:
        return sizeof(TypeMap<Type::DOUBLE>::type);
    case Type::CHAR:
        return sizeof(TypeMap<Type::CHAR>::type);
    case Type::STRING:
        return sizeof(TypeMap<Type::STRING>::type);
    default:
        return 0;
    }
}
inline Type get_type() const { return type; }
template <Type T> inline typename TypeMap<T>::type get_value() const;
inline std::string to_string() const;

Variant(void *value, Type type);

private:
const Type type;
std::uint64_t value{};
};

template <Variant::Type T>
inline typename Variant::TypeMap<T>::type Variant::get_value() const {
// switch (type) {
// case Type::INT:
// case Type::DOUBLE:
// case Type::CHAR:
// 	std::memcpy(dst, &value, get_size());
// 	break;
// default:
// 	break;
// }
return;
}

inline std::string Variant::to_string() const {
switch (this->type) {
case Type::INT: {
    int value{};
    get_value(&value);
    return std::to_string(value);
} break;
case Type::DOUBLE:
case Type::CHAR:
default:
    return "";
    break;
}
}
*/

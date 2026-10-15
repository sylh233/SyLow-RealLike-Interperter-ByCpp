#include <cstring>
#include <variant.hpp>

namespace cbycpp {
namespace scanner {
Variant::Variant(void *value, Type type) : type(type) {
	memcpy(&(this->value), value, get_size());
}
} // namespace scanner
} // namespace cbycpp

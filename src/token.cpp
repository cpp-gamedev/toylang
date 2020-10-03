#include <token.hpp>

namespace tl {
std::string_view TokenLocation::view(const std::string& source) const {
	return std::string_view(source.c_str() + location.start, length);
}

std::string_view Token::raw(const std::string& source) const {
	return location.view(source);
}
} // namespace tl

std::ostream& operator<<(std::ostream& os, const tl::Token& token) {
	return os << kt::to_string(token.type);
}

#ifndef TL_TOKEN
#define TL_TOKEN
#include <token.hpp>

namespace tl {

std::string_view TokenLocation::view(const std::string& source) const {
	return std::string_view(source.c_str() + start, length);
}

std::string_view Token::raw(const std::string& source) const {
	return location.view(source);
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
	return os << utils::toStr(token.type);
}

} // namespace tl
#endif
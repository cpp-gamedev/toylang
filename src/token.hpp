#pragma once
#include <utils/str_enum.hpp>
#include <string>
#include <string_view>

namespace tl {
using number_t = double;

STR_ENUM(TokenType,
		 plus,	 // +
		 minus,	 // -
		 mult,	 // *
		 div,	 // /
		 mod,	 // %
		 num,	 // [0-9]+(.[0-9]+)?
		 string, // '[^']' | "[^"]"
		 true_,	 // true
		 false_, // false
		 let,	 // let
		 nil,	 // nil
		 eof_,	 // '/0'
		 error,	 // unknown
		 eCOUNT_);

struct TokenLocation {
	std::size_t start = 0;
	std::size_t length = 0;
	std::size_t line = 0;
	std::uint32_t column = 0;

	std::string_view view(const std::string& source) const;
};

struct Token {
	TokenType type = TokenType::error;
	TokenLocation location = {};

	std::string_view raw(const std::string& source) const;
};

std::ostream& operator<<(std::ostream& os, const Token& token);

} // namespace tl
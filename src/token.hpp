#pragma once
#include <string>
#include <string_view>
#include <std_types.hpp>
#include <str_enum/str_enum.hpp>

namespace tl {
using number_t = f64;

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

struct Location final {
	std::size_t start = 0;
	std::size_t line = 1;
	u32 column = 0;
};

struct TokenLocation {
	Location location;
	std::size_t length = 0;

	std::string_view view(const std::string& source) const;
};

struct Token {
	TokenType type = TokenType::error;
	TokenLocation location = {};

	std::string_view raw(const std::string& source) const;
};
} // namespace tl

std::ostream& operator<<(std::ostream& os, const tl::Token& token);

#include <utils/str_enum.hpp>
#include <utils/str_format.hpp>

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

using TT = TokenType;

struct TokenLocation {
	std::size_t start = 0;
	std::size_t length = 0;
	std::size_t line = 0;
	std::uint32_t column = 0;

	std::string_view view(const std::string& source) const;
};

struct Token {
	TT type = TT::error;
	TokenLocation location = {};

	std::string_view raw(const std::string& source) const;
};

std::ostream& operator<<(std::ostream& os, const Token& token);

} // namespace tl
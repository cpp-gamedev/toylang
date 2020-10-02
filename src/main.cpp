#include <array>
#include <cassert>
#include <cctype>
#include <cstring>
#include <iostream>
#include <stdint.h>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utils/str_enum.hpp>
#include <utils/str_format.hpp>
#include <token.hpp>

namespace tl {
using number_t = double;

const static std::unordered_map<std::string_view, TokenType> keywords = {
	// clang-format off
	{"nil"  , TT::nil   },
	{"let"  , TT::let   },
	{"true" , TT::true_ },
	{"false", TT::false_},
	// clang-format on
};

class Lexer {
  private:
	struct SourcePosition {
		std::size_t current = 0;
		std::size_t start = 0;
		std::size_t line = 1;
		std::uint32_t column = 0;
	} pos;

	std::string_view fileName;
	std::string source;

  public:
	Lexer(std::string_view& fileName, std::string& src) : fileName(fileName), source(src) {
	}

	Token nextToken() {
		skipWhiteSpace();
		pos.start = pos.current;

		if (eof()) {
			return makeToken(TT::eof_);
		}

		char c = advance();

		// clang-format off
		switch (c) {
			case '+': return makeToken(TT::plus );
			case '-': return makeToken(TT::minus);
			case '*': return makeToken(TT::mult );
			case '/': return makeToken(TT::div  );
			case '%': return makeToken(TT::mod  );
			default:  
			if (isdigit(c)) return number();
		}
		// clang-format on
		return makeToken(TT::error);
	}

  private:
	Token number() {
		while (isdigit(peek())) {
			advance();
		}

		if (peek() == '.' && isdigit(peekAhead())) {
			advance(); // consume the '.'
			while (isdigit(peek())) {
				advance();
			}
		}

		return makeToken(TT::num);
	}

	// helper functions:

	bool isKeyword(const Token& token) const {
		return (keywords.find(token.raw(source)) != keywords.end());
	}

	char advance() {
		if (pos.current >= source.length())
			return '\0';
		return source[pos.current++];
	}

	char peek() const {
		if (eof())
			return '\0';
		return source[pos.current];
	}

	char peekAhead() const {
		if (eof() || pos.current + 1 >= source.length())
			return '\0';
		return source[pos.current + 1];
	}

	bool checkChar(char ch) const {
		return peek() == ch;
	}

	bool matchChar(char ch) {
		if (peek() == ch) {
			advance();
			return true;
		}
		return false;
	}

	bool eof() const {
		return pos.current >= source.length();
	}

	void skipWhiteSpace() {
		while (!eof()) {
			switch (peek()) {
			case ' ':
			case '\r':
			case '\t': advance(); break;
			case '\n':
				++pos.line;
				advance();
				break;
			default: return;
			}
		}
	}

	Token makeToken(TokenType type) const {
		return Token{type, TokenLocation{pos.start, pos.current, pos.line, pos.column}};
	}
};

void lexerTest() {
	std::string test = "11 + 2 - 3.5 * 4.62";
	std::string_view name = "<test_script>";
	Lexer lexer = {name, test};

	const int expectedCount = 8;

	TokenType expected[expectedCount] = {TT::num, TT::plus, TT::num, TT::minus, TT::num, TT::mult, TT::num, TT::eof_};

	for (int i = 0; i < expectedCount; i++) {
		Token token = lexer.nextToken();
		print("Token: {}\n", token);
		assert(token.type == expected[i]);
	}
}
} // namespace tl

int main([[maybe_unused]] int argc, [[maybe_unused]] char const* argv[]) {
	using namespace tl;
	print("--Toylang--\n\n");
	assert(utils::toStr(TokenType::div) == "div");
	assert(utils::toEnum("plus", TokenType::error) == TokenType::plus);
	lexerTest();
	return 0;
}

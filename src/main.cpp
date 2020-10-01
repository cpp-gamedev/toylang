#include <cassert>
#include <cctype>
#include <cstring>
#include <ctype.h>
#include <iostream>
#include <stdint.h>
#include <string>
#include <string_view>
#include <unordered_map>

#define DEBUG_MODE		   /* empty */

using tl_Number = double;

enum TokenType {
	PLUS,	// +
	MINUS,	// -
	MULT,	// *
	DIV,	// /
	MOD,	// %
	NUM,	// [0-9]+(.[0-9]+)?
	STRING, // '[^']' | "[^"]"
	TRUE,	// true
	FALSE,	// false
	LET,	// let
	NIL,	// nil
	_EOF,	// '/0'
	ERROR,	// unknown
	NUM_TOKENS
};

// clang-format off
const std::string TokenNames[TokenType::NUM_TOKENS] = {
	"PLUS",	
	"MINUS",	
	"MULT",	
	"DIV",	
	"MOD",	
	"NUM",	
	"STRING", 
	"TRUE",	
	"FALSE",	
	"LET",	
	"NIL",
	"END_OF_FILE",	
	"<ERROR>"
};
// clang-format on

const static std::unordered_map<std::string, TokenType> keywords = {
#define KW(str, tokentype) {#str, TokenType::tokentype},

	// clang-format off
    KW(true , TRUE )
    KW(false, FALSE)
    KW(let  , LET  )
    KW(nil  , NIL  )
	// clang-format on
#undef KW
};

struct Location {
	std::size_t	  start	 = 0;
	std::size_t	  length = 0;
	std::size_t	  line	 = 0;
	std::uint32_t column = 0;

	std::string_view view(std::string& source) {
		return std::string_view(source.c_str() + start, length);
	}
};

struct Token {
	TokenType type	   = TokenType::ERROR;
	Location  location = {};

	std::string_view raw(std::string& source) {
		return location.view(source);
	}
};

std::ostream& operator<<(std::ostream& os, const Token& token) {
	return os << TokenNames[(int)(token.type)];
}

class Lexer {
  private:
	std::string fileName;
	std::string	  source;
	std::size_t	  currentPos = 0;
	std::size_t	  startPos	 = 0;
	std::size_t	  line		 = 1;
	std::uint32_t column	 = 0;

  public:
	Lexer(std::string& fileName, std::string& src) {
		fileName = fileName;
		source	 = src;
	}

	Token nextToken() {
		skipWhiteSpace();
		startPos = currentPos;

		if (eof())
			return makeToken(TokenType::_EOF);
		char c = advance();

		// clang-format off
		switch (c) {
			case '+': return makeToken(TokenType::PLUS );
			case '-': return makeToken(TokenType::MINUS);
			case '*': return makeToken(TokenType::MULT );
			case '/': return makeToken(TokenType::DIV  );
			case '%': return makeToken(TokenType::MOD  );
			default:  
			if (isdigit(c)) return number();
		}
		// clang-format on
		return makeToken(TokenType::ERROR);
	}

  private:
	Token number() {
		while (isdigit(peek())) {
			advance();
		}

		if (peek() == '.' && isdigit(peekNext())) {
			advance(); // consume the '.'
			while (isdigit(peek())) {
				advance();
			}
		}

		return makeToken(TokenType::NUM);
	}

	// helper functions:

	// somewhat expensive, optimize later.
	bool isKeyword(Token& token) {
		auto raw = std::string(token.raw(source));
		return (keywords.find(raw) != keywords.end());
	}

	char advance() {
		if (currentPos >= source.length())
			return '\0';
		return source[currentPos++];
	}

	char peek() {
		if (eof())
			return '\0';
		return source[currentPos];
	}

	char peekNext() {
		if (eof() || currentPos + 1 >= source.length())
			return '\0';
		return source[currentPos + 1];
	}

	bool checkChar(char ch) {
		return peek() == ch;
	}

	bool matchChar(char ch) {
		if (peek() == ch) {
			advance();
			return true;
		}
		return false;
	}

	bool eof() {
		return currentPos >= source.length();
	}

	void skipWhiteSpace() {
		while (!eof()) {
			switch (peek()) {
			case ' ':
			case '\r':
			case '\t': advance(); break;
			case '\n':
				line++;
				advance();
				break;
			default: return;
			}
		}
	}

	Token makeToken(TokenType type) {
		return Token{type, Location{startPos, currentPos, line, column}};
	}
};

void LexerTest() {
	std::string test = "11 + 2 - 3.5 * 4.6";
	std::string name = "<test_script>";

	const int expectedCount = 8;

	TokenType expected[expectedCount] = {
		TokenType::NUM, TokenType::PLUS, TokenType::NUM, TokenType::MINUS,
		TokenType::NUM, TokenType::MULT, TokenType::NUM, TokenType::_EOF};

	Lexer lexer = {name, test};

	for (int i = 0; i < expectedCount; i++) {
		Token token = lexer.nextToken();
		std::cout << "TOKEN_" << token << std::endl;
		assert(token.type == expected[i]);
	}
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char const* argv[]) {
	std::cout << "--Toylang--\n" << std::endl;
	LexerTest();
	return 0;
}

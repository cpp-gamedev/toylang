#include <cctype>
#include <unordered_map>
#include <lexer.hpp>

namespace tl {

using TT = TokenType;

const std::unordered_map<std::string_view, TokenType> Lexer::keywords = {
	// clang-format off
	{"nil"  , TokenType::nil   },
	{"let"  , TokenType::let   },
	{"true" , TokenType::true_ },
	{"false", TokenType::false_},
	// clang-format on
};

Token Lexer::nextToken() {
	skipWhiteSpace();
	pos.location.start = pos.current;

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

Token Lexer::number() {
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

bool Lexer::isKeyword(const Token& token) const {
	return (keywords.find(token.raw(source)) != keywords.end());
}

char Lexer::advance() {
	if (pos.current >= source.length())
		return '\0';
	return source[pos.current++];
}

char Lexer::peek() const {
	if (eof())
		return '\0';
	return source[pos.current];
}

char Lexer::peekAhead() const {
	if (eof() || pos.current + 1 >= source.length())
		return '\0';
	return source[pos.current + 1];
}

bool Lexer::checkChar(char ch) const {
	return peek() == ch;
}

bool Lexer::matchChar(char ch) {
	if (peek() == ch) {
		advance();
		return true;
	}
	return false;
}

bool Lexer::eof() const {
	return pos.current >= source.length();
}

void Lexer::skipWhiteSpace() {
	while (!eof()) {
		switch (peek()) {
		case ' ':
		case '\r':
		case '\t': advance(); break;
		case '\n':
			++pos.location.line;
			advance();
			break;
		default: return;
		}
	}
}

Token Lexer::makeToken(TT type) const {
	return Token{type, TokenLocation{pos.location, pos.current}};
}

} // namespace tl

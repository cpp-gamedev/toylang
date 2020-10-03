#pragma once
#include <std_types.hpp>
#include <token.hpp>

namespace tl {

class Lexer {
  private:
	struct SourcePosition {
		Location location;
		std::size_t current = 0;
	} pos;

	std::string_view fileName;
	std::string source;
	const static std::unordered_map<std::string_view, TokenType> keywords;

  public:
	Lexer(std::string_view& fileName, std::string& src) : fileName(fileName), source(src) {
	}

	Token nextToken();

  private:
	Token number();

	// helper functions:

	bool isKeyword(const Token& token) const;
	char advance();
	char peek() const;
	char peekAhead() const;
	bool checkChar(char ch) const;
	bool matchChar(char ch);
	bool eof() const;
	void skipWhiteSpace();
	Token makeToken(TokenType type) const;
};

} // namespace tl

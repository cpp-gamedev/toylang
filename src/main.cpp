#include "token.hpp"
#include <cassert>
#include <iostream>
#include <lexer.hpp>
#include <utils/str_format.hpp>

using namespace tl;

void lexerTest() {
	std::string test = "11 + 2 - 3.5 * 4.62";
	std::string_view name = "<test_script>";
	Lexer lexer = {name, test};

	const int expectedCount = 8;
	using TT = TokenType;

	TokenType expected[expectedCount] = {TT::num, TT::plus, TT::num, TT::minus, TT::num, TT::mult, TT::num, TT::eof_};

	for (int i = 0; i < expectedCount; i++) {
		Token token = lexer.nextToken();
		print("Token: {}\n", token);
		assert(token.type == expected[i]);
	}
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char const* argv[]) {
	print("--Toylang--\n\n");
	assert(utils::toStr(TokenType::div) == "div");
	assert(utils::toEnum("plus", TokenType::error) == TokenType::plus);
	lexerTest();
	return 0;
}
#include "token.hpp"
#include <cassert>
#include <iostream>
#include <lexer.hpp>
#include <utils/str_format.hpp>

void lexerTest() {
	std::string test = "11 + 2 - 3.5 * 4.62";
	std::string_view name = "<test_script>";
	tl::Lexer lexer = {name, test};

	const int expectedCount = 8;
	using TT = tl::TokenType;

	tl::TokenType expected[expectedCount] = {TT::num, TT::plus, TT::num, TT::minus, TT::num, TT::mult, TT::num, TT::eof_};

	for (int i = 0; i < expectedCount; i++) {
		tl::Token token = lexer.nextToken();
		print("Token: {}\n", token);
		assert(token.type == expected[i]);
	}
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char const* argv[]) {
	using namespace tl;
	print("--Toylang--\n\n");
	assert(utils::toStr(tl::TokenType::div) == "div");
	assert(utils::toEnum("plus", tl::TokenType::error) == tl::TokenType::plus);
	lexerTest();
	return 0;
}
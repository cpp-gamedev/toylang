#include <cassert>
#include <iostream>
#include <lexer.hpp>
#include <utils/utils.hpp>
#include "token.hpp"

using namespace tl;

void lexerTest() {
	std::string test = "11 + 2 - 3.5 * 4.62";
	std::string_view name = "<test_script>";
	Lexer lexer = {name, test};

	constexpr s32 expectedCount = 8;
	using TT = TokenType;

	TokenType expected[expectedCount] = {TT::num, TT::plus, TT::num, TT::minus, TT::num, TT::mult, TT::num, TT::eof_};

	for (s32 i = 0; i < expectedCount; i++) {
		Token token = lexer.nextToken();
		print("Token: {}\n", token);
		assert(token.type == expected[i]);
	}
}

s32 main([[maybe_unused]] s32 argc, [[maybe_unused]] char const* argv[]) {
	print("--Toylang--\n\n");
	assert(kt::to_string(TokenType::div) == "div");
	assert(kt::to_enum("plus", TokenType::error) == TokenType::plus);
	lexerTest();
	return 0;
}

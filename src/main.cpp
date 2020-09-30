#include <cstring>
#include <ctype.h>
#include <iostream>
#include <string>
#include <unordered_map>

#define KW(str, tokentype) {#str, TokenType::tokentype},

typedef double tl_Number;

enum TokenType {
    PLUS,   // +
    MINUS,  // -
    MULT,   // *
    EXP,    // **
    NUM,    // [0-9]+(.[0-9]+)?
    STRING, // '[^']' | "[^"]"
    TRUE,   // true
    FALSE,  // false
    LET,    // let
    NIL,    // nil
};

const static std::unordered_map<const char*, TokenType> keywords = {
    // clang-format off
    KW(true , TRUE )
    KW(false, FALSE)
    KW(let  , LET  )
    KW(nil  , NIL  )
    // clang-format on
};

struct Token {
    TokenType   type;
    const char* firstChar;
    size_t      length;

    Token(TokenType type, const char* firstChar, size_t length) {
        type      = type;
        firstChar = firstChar;
        length    = length;
    }
};

class Lexer {
  private:
    std::string fileName;
    std::string source;
    size_t      current;
    size_t      start;

  public:
    Lexer(std::string& fileName, std::string& src) {
        fileName = fileName;
        source   = src;
    }

  private:
    // somewhat expensive, optimize later.
    static bool isKeyword(Token& token) {
        char* buf;
        std::memcpy(buf, token.firstChar, token.length);
        return keywords.find(buf) != keywords.end();
    }
};

int main(int argc, char const* argv[]) {
    std::cout << "hello\n" << std::endl;
    return 0;
}

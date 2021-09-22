#include <vector>
#include <string>
#include <iostream>

enum Token {
    LITERAL,
    OPERATOR,
    KEYWORD,
    IDENTIFIER,
    ENDLINE,
    UNKNOWN,
    END_OF_FILE,
    ERROR
};

enum CharType {
    WHITESPACE,
    ALPHA,
    NUMERIC,
    OPCOMP,
    BRACKET,
    COMMENT,
    OTHER
};

CharType getType(std::string str, int index);
Token getToken(std::string code, int &start, int &end);
std::vector<std::pair<std::string,Token>> getTokens(std::string code);
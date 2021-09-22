#ifndef LEXER_H
#define LEXER_H
#include <vector>
#include <string>
#include <iostream>
#include <stack>

enum Token {
    LITERAL,
    OPERATOR,
    KEYWORD,
    IDENTIFIER,
    ENDLINE,
    BRACKET,
    WHITESPACE,
    ALPHA,
    NUMERIC,
    END_OF_FILE,
    UNKNOWN
};

Token getType(std::string str, int index);
Token getToken(std::string code, int &start, int &end);
std::vector<std::pair<std::string,Token>> getTokens(std::string code);
char getClosingBracket(char openingBracket);

#endif
#include<iostream>
#include<fstream>
#include<sstream>
#include "Lexer.h"

inline const std::string tokenString(Token token) {
	switch(token) {
		case Token::LITERAL:		return "LITERAL";
		case Token::OPERATOR:		return "OPERATOR";
		case Token::KEYWORD:		return "KEYWORD";
		case Token::IDENTIFIER:		return "IDENTIFIER";
		case Token::ENDLINE:		return "ENDLINE";
		case Token::UNKNOWN:		return "UNKNOWN";
		case Token::END_OF_FILE:	return "END_OF_FILE";
		case Token::ERROR:			return "ERROR";
		default:					return "INVALID";
	}
}

int main() {
	
	std::string code;
	std::ifstream codeFile("code.nya");

	code = std::string((std::istreambuf_iterator<char>(codeFile)), std::istreambuf_iterator<char>());

	auto tokens = getTokens(code);

	for (auto tokenPair : tokens) {
		std::cout << tokenPair.first << " => " << tokenString(tokenPair.second) << std::endl;
	}

	return 0;
}

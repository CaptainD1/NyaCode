#include<iostream>
#include<fstream>
#include<sstream>
#include "Lexer.h"

const std::string tokenString(Token token) {
	switch(token) {
		case Token::LITERAL:		return "LITERAL";
		case Token::OPERATOR:		return "OPERATOR";
		case Token::KEYWORD:		return "KEYWORD";
		case Token::IDENTIFIER:		return "IDENTIFIER";
		case Token::ENDLINE:		return "ENDLINE";
		case Token::BRACKET:		return "BRACKET";
		case Token::END_OF_FILE:	return "END_OF_FILE";
		case Token::UNKNOWN:		return "UNKNOWN";
		default:					return "UNREGISTERED";
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

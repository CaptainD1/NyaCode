#include "Lexer.h"

const std::string KEYWORDS[] = {
    "nyif",
    "nyelse",
    "nyew",
    "mewot",
    "myaybe",
    "sowwy",
    "finyally",
    "okidoki",
    "nyathing"
};

const int NUM_KEYWORDS = 9;// sizeof(KEYWORDS) * sizeof(std::string);

CharType getType(char ch) {

    if (std::isspace(ch)) return CharType::WHITESPACE;
    if (std::isalpha(ch) || ch == '_') return CharType::ALPHA;
    if (std::isdigit(ch)) return CharType::NUMERIC;
    switch (ch) {
        case '!':
        case '%':
        case '^':
        case '&':
        case '*':
        case '=':
        case '+':
        case '-':
        case '/':
        case '\\':
            return CharType::OPCOMP;
        case '{':
        case '[':
        case '(':
        case '"':
        case '\'':
        case '}':
        case ']':
        case ')':
            return CharType::BRACKET;
    }
    return CharType::OTHER;
}

Token getToken(std::string code, int &start, int &end) {
    int index = start;
    CharType startType;
    std::string literal;

    // Bypass all whitespace and comments
    while (true) {

        // If comment never ends or at end of file
        if (index == -1 || index >= code.size()) {
            start, end = code.size();
            return Token::END_OF_FILE;
        // Go to next character if space
        } else if (isspace(code[index])) {
            ++index;
        // Could be a comment
        } else if (code.size() - index >= 2 && code[index] == '/') {
            // Is a multi-line comment
            if (code[index + 1] == '*') {
                // Go to end of comment
                index = code.find("*/", index);
                if(index != -1) {
                    index += 2;
                }
            // Is a single-line comment
            } else if (code[index + 1] == '/') {
                // Go to end of line
                index = code.find('\n', index);
            }
        // Otherwise, it is a non-comment non-whitespace character
        } else {
            break;
        }
    }

    start = index;

    startType = getType(code[index]);

    if (startType == CharType::ALPHA) {
        literal = code[index];
        ++index;

        while (index < code.size()) {
            CharType curType = getType(code[index]);
            if (curType == CharType::ALPHA || curType == CharType::NUMERIC) {
                literal += code[index];
            } else if (literal.compare("nya") == 0 && code[index] == '~') {
                end = index + 1;
                return Token::ENDLINE;
            } else {
                break;
            }
            ++index;
        }
        end = index;
        for (int i = 0; i < NUM_KEYWORDS; ++i) {
            if (literal.compare(KEYWORDS[i]) == 0) return Token::KEYWORD;
        }
        return Token::IDENTIFIER;

    } else if (startType == CharType::OPCOMP || startType == CharType::OTHER) {
        for (; index < code.size() && getType(code[index]) == startType; ++index);
        end = index;
        switch (startType) {
            case CharType::OPCOMP:  return Token::OPERATOR;
            case CharType::OTHER:   return Token::UNKNOWN;
        }
    } else if (startType == CharType::NUMERIC) {
        ++index;

        for (CharType curType = getType(code[index]); index < code.size() &&
            curType == CharType::ALPHA || curType == CharType::NUMERIC; ++index, curType = getType(code[index]));

        end = index;
        return Token::LITERAL;
    } else if (startType == CharType::BRACKET) {
        if (code[index] == '"' || code[index] == '\'') {
            char startCh = code[index];
            while (true) {
                index = code.find(startCh, index + 1);
                if (index == -1) {
                    end = code.size();
                    return Token::ERROR;
                } else if (code[index - 1] != '\\') {
                    break;
                }
            }
            end = index + 1;
            return Token::LITERAL;
        } else {
            // HANDLE NON-STRING BRACKETS
        }
    }

    return Token::UNKNOWN;
}

std::vector<std::pair<std::string,Token>> getTokens(std::string code) {

    std::vector<std::pair<std::string,Token>> tokens;

    int start = 0;
    int end;
    Token token;

    token = getToken(code, start, end);

    while (token != Token::END_OF_FILE) {
        tokens.push_back({code.substr(start, end - start), token});

        start = end;
        token = getToken(code, start, end);
    }

    return tokens;
}
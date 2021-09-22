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

char getClosingBracket(char openingBracket) {
    switch(openingBracket) {
        case '{': return '}';
        case '(': return ')';
        case '[': return ']';
        case '<': return '>';
        default:  return 0;
    }
}

Token getType(char ch) {

    if (std::isspace(ch)) return Token::WHITESPACE;
    if (std::isalpha(ch) || ch == '_') return Token::ALPHA;
    if (std::isdigit(ch)) return Token::NUMERIC;
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
        case '.':
        case ',':
        case '|':
        case '<':
        case '>':
        case '#':
        case '~':
            return Token::OPERATOR;
        case '{':
        case '[':
        case '(':
        case '"':
        case '\'':
        case '`':
        case '}':
        case ']':
        case ')':
            return Token::BRACKET;
    }
    return Token::UNKNOWN;
}

Token getToken(std::string code, int &start, int &end) {
    int index = start;
    Token startType;
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

    if (startType == Token::ALPHA) {
        literal = code[index];
        ++index;

        while (index < code.size()) {
            Token curType = getType(code[index]);
            if (curType == Token::ALPHA || curType == Token::NUMERIC) {
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

    } else if (startType == Token::OPERATOR || startType == Token::UNKNOWN) {
        for (; index < code.size() && getType(code[index]) == startType; ++index);
        end = index;
        return startType;
    } else if (startType == Token::NUMERIC) {
        ++index;

        for (Token curType = getType(code[index]); index < code.size() &&
            curType == Token::ALPHA || curType == Token::NUMERIC; ++index, curType = getType(code[index]));

        end = index;
        return Token::LITERAL;
    } else if (startType == Token::BRACKET) {
        if (code[index] == '"' || code[index] == '\'' || code[index] == '`') {
            // String brackets
            char startCh = code[index];
            while (true) {
                index = code.find(startCh, index + 1);
                if (index == -1) {
                    end = code.size();
                    return Token::UNKNOWN;
                } else if (code[index - 1] != '\\') {
                    break;
                }
            }
            end = index + 1;
            return Token::LITERAL;
        } else {
            // Non-string brackets
            end = start + 1;
            return Token::BRACKET;
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
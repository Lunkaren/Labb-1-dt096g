//
// Created by willi on 01/02/2024.
//

#ifndef Labb1_dt096g_LEXER_H
#define Labb1_dt096g_LEXER_H


#include <string>

using it = std::string::iterator;

class lex{
public:
    lex(){};
    enum{
        LPAREN, RPAREN, LETTER, DIGIT, LCURL, RCURL, SLASH, MULT, OR, ANY, END, OUT, CAPITAL
    };
    int type;
    char check(it&first, it last){
        if (first == last){
            type = END;
            return ' ';
        }
        switch (*first) {
            case '(':
                type = LPAREN;
                return *first;
            case ')':
                type = RPAREN;
                return *first;
            case '{':
                type = LCURL;
                return *first;
            case '}':
                type = RCURL;
                return *first;
            case '\\':
                type = SLASH;
                return *first;
            case '*':
                type = MULT;
                return *first;
            case '+':
                type = OR;
                return *first;
            case '.':
                type = ANY;
                return *first;
            case 'O':
                type = OUT;
                return *first;
            case 'I':
                type = CAPITAL;
                return *first;
            }
        if (isdigit(*first)){
            type = DIGIT;
            return *first;
        }
        type = LETTER;
        return *first;
    }
    int getType(){
        return type;
    }
};

#endif //Labb1_dt096g_LEXER_H

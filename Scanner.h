#ifndef SCANNER_H   
#define SCANNER_H   

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;


#define TRUE 1
#define FALSE 0

#define MAXRESERVED 8
#define MAXTOKENLEN 40
#define BUFLEN 256


enum class TokenType {
    ENDFILE,
    ERROR,
    IF,
    THEN,
    ELSE,
    END,
    REPEAT,
    UNTIL,
    READ,
    WRITE,
    ID,
    NUM,
    ASSIGN,
    EQ,
    LT,
    PLUS,
    MINUS,
    TIMES,
    OVER,
    LPAREN,
    RPAREN,
    SEMI
};

enum class StateType {
    START,
    INASSIGN,
    INCOMMENT,
    INNUM,
    INID,
    DONE
};

struct Token {
    TokenType type;
    string lexeme;
};

const std::vector<std::pair<std::string, TokenType>> reservedWords = {
    {"if", TokenType::IF},
    {"then", TokenType::THEN},
    {"else", TokenType::ELSE},
    {"end", TokenType::END},
    {"repeat", TokenType::REPEAT},
    {"until", TokenType::UNTIL},
    {"read", TokenType::READ},
    {"write", TokenType::WRITE}
};



// functions 

Token getToken(vector <string>& tokens_vector, ifstream &source);
char getNextChar(ifstream &source);
void ungetNextChar(void);
TokenType reservedLookup(const std::string& s);
void printToken(TokenType token, const std::string& tokenString, vector <string>& tokens_vector);

#endif 


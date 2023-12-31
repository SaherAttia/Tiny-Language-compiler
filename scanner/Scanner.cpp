#include"Scanner.h"


int lineno = 0;
bool EchoSource = true;
static int EOF_flag = FALSE;
int TraceScan = TRUE;


static char tokenString[MAXTOKENLEN + 1];
static char lineBuf[BUFLEN];
static int linepos = 0;
static int bufsize = 0;




char getNextChar(ifstream &source) {
    if (!(linepos < bufsize)) {
        lineno++;
        if (source.getline(lineBuf, BUFLEN - 1)) {
            if (EchoSource) {
                // std::cout << std::setw(4) << lineno << ": " << lineBuf;
                // Replace the above line with your desired output mechanism.
            }
            bufsize = source.gcount();
            linepos = 0;
            return lineBuf[linepos++];
        }
        else {
            EOF_flag = TRUE;
            return EOF;
        }
    }
    else {
        return lineBuf[linepos++];
    }
}

void ungetNextChar(void) {
    linepos--;
}

TokenType reservedLookup(const std::string& s) {
    for (const auto& pair : reservedWords) {
        if (s == pair.first) {
            return pair.second;
        }
    }
    return TokenType::ID;
}

void printToken(TokenType token, const std::string& tokenString, vector <string> &tokens_vector) {
    switch (token) {
    case TokenType::IF:
        //listing << tokenString << ",IF\n";
        tokens_vector.push_back(tokenString + ",IF");
        break;
    case TokenType::THEN:
        //listing << tokenString << ",THEN\n";
        tokens_vector.push_back(tokenString + ",THEN");
        break;
    case TokenType::ELSE:
        //listing << tokenString << ",ELSE\n";
        tokens_vector.push_back(tokenString + ",ELSE");
        break;
    case TokenType::END:
        //listing << tokenString << ",END\n";
        tokens_vector.push_back(tokenString + ",END");
        break;
    case TokenType::REPEAT:
        //listing << tokenString << ",REPEAT\n";
        tokens_vector.push_back(tokenString + ",REPEAT");
        break;
    case TokenType::UNTIL:
        //listing << tokenString << ",UNTIL\n";
        tokens_vector.push_back(tokenString + ",UNTIL");
        break;
    case TokenType::READ:
        //listing << tokenString << ",READ\n";
        tokens_vector.push_back(tokenString + ",READ");
        break;
    case TokenType::WRITE:
        //listing << tokenString << ",WRITE\n";
        tokens_vector.push_back(tokenString + ",WRITE");
        break;
    case TokenType::ASSIGN:
        //listing << ":=,ASSIGN\n";
        tokens_vector.push_back(":=,ASSIGN");
        break;
    case TokenType::LT:
        //listing << "<,LESSTHAN\n";
        tokens_vector.push_back("<,LESSTHAN");
        break;
    case TokenType::EQ:
        //listing << "=,EQUAL\n";
        tokens_vector.push_back("=,EQUAL");
        break;
    case TokenType::LPAREN:
        //listing << "(,OPENBRACKET\n";
        tokens_vector.push_back("(,OPENBRACKET");
        break;
    case TokenType::RPAREN:
        //listing << "),CLOSEDBRACKET\n";
        tokens_vector.push_back("),CLOSEDBRACKET");
        break;
    case TokenType::SEMI:
        //listing << ";,SEMICOLON\n";
        tokens_vector.push_back(";,SEMICOLON");
        break;
    case TokenType::PLUS:
        //listing << "+,PLUS\n";
        tokens_vector.push_back("+,PLUS");
        break;
    case TokenType::MINUS:
        //listing << "-,MINUS\n";
        tokens_vector.push_back("-,MINUS");
        break;
    case TokenType::TIMES:
        //listing << "*,MULT\n";
        tokens_vector.push_back("*,MULT");
        break;
    case TokenType::OVER:
        //listing << "/,DIV\n";
        tokens_vector.push_back("/,DIV");
        break;
    case TokenType::ENDFILE:
        //listing << "EOF\n";
        tokens_vector.push_back("EOF");
        break;
    case TokenType::NUM:
        //listing << tokenString << ",NUMBER\n";
        tokens_vector.push_back(tokenString + ",NUMBER");
        break;
    case TokenType::ID:
        //listing << tokenString << ",IDENTIFIER\n";
        tokens_vector.push_back(tokenString + ",IDENTIFIER");
        break;
    case TokenType::ERROR:
        //listing << "ERROR: " << tokenString << "\n";
        tokens_vector.push_back("ERROR: " + tokenString);
        break;
    default:
        //listing << "Unknown token: " << static_cast<int>(token) << "\n";
        tokens_vector.push_back("Unknown token: " + static_cast<int>(token));
    }
}



Token getToken(vector <string>& tokens_vector, ifstream &source) {
    int tokenStringIndex = 0;
    Token currentToken;
    currentToken.lexeme = "";
    StateType state = StateType::START;
    int save;
    while (state != StateType::DONE) {
        char c;
        if (EOF_flag == FALSE) {
            c = getNextChar(source);
        }
        else {
            c = EOF;
        }
        save = TRUE;
        switch (state) {
        case StateType::START:
            if (std::isdigit(c)) {
                state = StateType::INNUM;
            }
            else if (std::isalpha(c)) {
                state = StateType::INID;
            }
            else if (c == ':') {
                state = StateType::INASSIGN;
            }
            else if ((c == ' ') || (c == '\t') || (c == 0)) {
                save = FALSE;
            }
            else if (c == '{') {
                save = FALSE;
                state = StateType::INCOMMENT;
            }
            else {
                state = StateType::DONE;
                switch (c) {
                case EOF:
                    save = FALSE;
                    currentToken.type = TokenType::ENDFILE;
                    break;
                case '=':
                    currentToken.type = TokenType::EQ;
                    currentToken.lexeme += c;
                    break;
                case '<':
                    currentToken.type = TokenType::LT;
                    currentToken.lexeme += c;
                    break;
                case '+':
                    currentToken.type = TokenType::PLUS;
                    currentToken.lexeme += c;
                    break;
                case '-':
                    currentToken.type = TokenType::MINUS;
                    currentToken.lexeme += c;
                    break;
                case '*':
                    currentToken.type = TokenType::TIMES;
                    currentToken.lexeme += c;
                    break;
                case '/':
                    currentToken.type = TokenType::OVER;
                    currentToken.lexeme += c;
                    break;
                case '(':
                    currentToken.type = TokenType::LPAREN;
                    currentToken.lexeme += c;
                    break;
                case ')':
                    currentToken.type = TokenType::RPAREN;
                    currentToken.lexeme += c;
                    break;
                case ';':
                    currentToken.type = TokenType::SEMI;
                    currentToken.lexeme += c;
                    break;
                default:
                    currentToken.type = TokenType::ERROR;
                    currentToken.lexeme += c;
                    break;
                }
            }
            break;
        case StateType::INCOMMENT:
            save = FALSE;
            if (c == EOF) {
                state = StateType::DONE;
                currentToken.type = TokenType::ENDFILE;
            }
            else if (c == '}') {
                state = StateType::START;
            }
            break;
        case StateType::INASSIGN:
            state = StateType::DONE;
            if (c == '=') {
                currentToken.type = TokenType::ASSIGN;
                currentToken.lexeme += c;
            }
            else {
                ungetNextChar();
                save = FALSE;
                currentToken.type = TokenType::ERROR;
                currentToken.lexeme += c;
            }
            break;
        case StateType::INNUM:
            if (!std::isdigit(c)) {
                ungetNextChar();
                save = FALSE;
                state = StateType::DONE;
                currentToken.type = TokenType::NUM;
            }
            else {
                currentToken.lexeme += c;
            }
            break;
        case StateType::INID:
            if (!std::isalpha(c)) {
                ungetNextChar();
                save = FALSE;
                state = StateType::DONE;
                currentToken.type = TokenType::ID;
            }
            else {
                currentToken.lexeme += c;
            }
            break;
        case StateType::DONE:
        default:
            std::cerr << "Scanner Bug: state= " << static_cast<int>(state) << "\n";
            state = StateType::DONE;
            currentToken.type = TokenType::ERROR;
            break;
        }

        if (save && tokenStringIndex <= MAXTOKENLEN) {
            tokenString[tokenStringIndex++] = static_cast<char>(c);
        }

        if (state == StateType::DONE) {
            tokenString[tokenStringIndex] = '\0';
            currentToken.lexeme = tokenString;
            if (currentToken.type == TokenType::ID) {
                currentToken.type = reservedLookup(currentToken.lexeme);
            }

            if (currentToken.type == TokenType::END) {
                EOF_flag = TRUE;
            }
        }
    }

    if (TraceScan) {
            printToken(currentToken.type, currentToken.lexeme,  tokens_vector);
    }
    return currentToken;
}



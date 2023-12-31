// Parser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <utility>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
using namespace std;


struct Token 
{
    string value;
    string type;
};

struct SyntaxTreeNode 
{
    Token token;
    SyntaxTreeNode* next;
    vector<SyntaxTreeNode*> children;
    string shape;
    SyntaxTreeNode() : token({ "", "" }), next(nullptr) {}
    SyntaxTreeNode(Token t) : token(t), next(nullptr) {}
};


// Define TinyParser class with a queue
class TinyParser {
public:
    // Constructor
    TinyParser(std::queue<Token>& tokenQueue) : tokenQueue(tokenQueue) {}

    // Parse program
    SyntaxTreeNode* parseProgram() {
        return statementList();
    }

private:
    // Member variables
    queue<Token>& tokenQueue;

    // Helper functions for parsing
    SyntaxTreeNode* statementList() {
        SyntaxTreeNode* head = nullptr;
        SyntaxTreeNode* current = nullptr;

        while (tokenQueue.size() != 0) {
            if (head == nullptr) {
                head = current = statement();
            }
            else {
                current->next = statement();
                current = current->next;
            }

            if (tokenQueue.size() != 0 && tokenQueue.front().type == "SEMICOLON") {
                // Consume SEMICOLON
                match("SEMICOLON");
            }
            if (tokenQueue.size() != 0 && ( (tokenQueue.front().type == "UNTIL") || (tokenQueue.front().type == "END")) )
            {
                return head;
            }

        }
        return head;
    }

    SyntaxTreeNode* statement() {

        if (tokenQueue.front().type == "READ") {
            return readStatement();
        }
        else if (tokenQueue.front().type == "WRITE") {
            return writeStatement();
        }
        else if (tokenQueue.front().type == "IF") {
            return ifStatement();
        }
        else if (tokenQueue.front().type == "REPEAT") {
            return repeatStatement();
        }// if there's another input not only "IDENTIFIER", put this case in else not else if 
        else if (tokenQueue.front().type == "IDENTIFIER") { 
            return assignmentStatement();
        }
        else {
            throw runtime_error("Token Statement Mismatch");
        }
        
    }

    SyntaxTreeNode* ifStatement() {
        match("IF");
        SyntaxTreeNode* conditionNode = condition();
        match("THEN");
        SyntaxTreeNode* statementListNode = statementList();
        match("END");
        SyntaxTreeNode* result = new SyntaxTreeNode();
        result->token.type = "IF";
        result->token.value = "if";
        result->shape = "rectangle";
        result->children.push_back(conditionNode);
        result->children.push_back(statementListNode);


        return result;
    }

    SyntaxTreeNode* condition() {
        SyntaxTreeNode* result = new SyntaxTreeNode();
        SyntaxTreeNode* leftOperandNode = expression();
        string op = relationalOperator();
        SyntaxTreeNode* rightOperandNode = expression();
        result->token.type = "op";
        result->token.value = op;
        result->shape = "ellipse";
        result->children.push_back(leftOperandNode);
        result->children.push_back(rightOperandNode);
        return result;
    }

    string relationalOperator() {
        string op;
        if (tokenQueue.front().type == "LESSTHAN" || tokenQueue.front().type == "EQUAL") {
            op = "(" + tokenQueue.front().value + ")";
            match(tokenQueue.front().type);
        
        }
        else {
            throw runtime_error("InvalidRelationalOperator");
        }
        return op;
    }

    SyntaxTreeNode* expression() {
        SyntaxTreeNode* expNode = term();
        SyntaxTreeNode* temp = new SyntaxTreeNode();

        while (tokenQueue.front().type == "PLUS" || tokenQueue.front().type == "MINUS") {
            temp->token.value = "(" + tokenQueue.front().value + ")";
            match(tokenQueue.front().type);
            temp->token.type = "op";
            temp->shape = "ellipse";
            temp->children.push_back(expNode);
            temp->children.push_back(term());
            expNode = temp;
        }
        return expNode;
    }

    SyntaxTreeNode* term() {
        SyntaxTreeNode* factorNode = factor();
        SyntaxTreeNode* temp = new SyntaxTreeNode();

        while (tokenQueue.front().type == "MULT" || tokenQueue.front().type == "DIV") {
            temp->token.value = "(" + tokenQueue.front().value + ")";
            match(tokenQueue.front().type);
            temp->token.type = "op";
            temp->shape = "ellipse";
            temp->children.push_back(factorNode);
            temp->children.push_back(factor());
            factorNode = temp;
        }
        return factorNode;
    }

    SyntaxTreeNode* factor() {
        SyntaxTreeNode* result = new SyntaxTreeNode();

        if (tokenQueue.front().type == "IDENTIFIER") 
        {
            string s = tokenQueue.front().value;
            match(tokenQueue.front().type);
            result->token.value = "(" + s + ")";
            result->token.type = "id";
            result->shape = "ellipse";
        }
        else if (tokenQueue.front().type == "NUMBER")
        {
            string s = tokenQueue.front().value;
            match(tokenQueue.front().type);
            result->token.value = "(" + s + ")";
            result->token.type = "const";
            result->shape = "ellipse";
        }
        else if (tokenQueue.front().type == "OPENBRACKET") {
            match("OPENBRACKET");
            result = expression();
            match("CLOSEDBRACKET");
        }
        else {
            throw runtime_error("InvalidFactor");
        }

        return result;
    }

    SyntaxTreeNode* repeatStatement() {
        SyntaxTreeNode* result = new SyntaxTreeNode();
        match("REPEAT");
        SyntaxTreeNode* statementListNode = statementList();
        match("UNTIL");
        SyntaxTreeNode* conditionNode = condition();
        result->shape = "rectangle";
        result->token.type = "REPEAT";
        result->token.value = "repeat";
        result->children.push_back(statementListNode);
        result->children.push_back(conditionNode);
        return result;
    }

    SyntaxTreeNode* readStatement() {
        SyntaxTreeNode* result = new SyntaxTreeNode();
        match("READ");
        string s = tokenQueue.front().value;
        match("IDENTIFIER");

        
        result->token.type = "read";
        result->token.value = "(" + s + ")";
        result->shape = "rectangle";
        return result;
    }

    SyntaxTreeNode* writeStatement() {
        SyntaxTreeNode* result = new SyntaxTreeNode();
        match("WRITE");
        result->token.type = "WRITE";
        result->token.value = "write";
        result->shape = "rectangle";
        result->children.push_back(expression());

        return result;
    }

    SyntaxTreeNode* assignmentStatement() {
        SyntaxTreeNode* result = new SyntaxTreeNode();
        string s = tokenQueue.front().value;
        match("IDENTIFIER");
        match("ASSIGN");
        SyntaxTreeNode* expressionNode = expression();
        result->shape = "rectangle";
        result->token.type = "assign";
        result->token.value = "(" + s +")";
        result->children.push_back(expressionNode);
        return result;
    }

    // Match function to check if the current token matches the expected token
    void match(string expectedType) {

        if (tokenQueue.front().type == expectedType) {
            tokenQueue.pop();
        }
        else {
            throw runtime_error("Token Mismatch");
        }
    }

};


Token tokenizeLine(const string& line) {

    istringstream iss(line);
    string temp;
    Token token;
    while (getline(iss, temp, ',')) {
        // Assuming the format is always "value, type"
        getline(iss, token.type, '\n');
        token.value = temp;
    }

    return token;
}

void printSyntaxTree(SyntaxTreeNode* node, int depth = 0) {
    if (node == nullptr) {
        return;
    }

    // Print the information of the current node
    for (int i = 0; i < depth; ++i) {
        std::cout << "  ";  // Adjust the indentation for better readability
    }
    std::cout << "Value: " << node->token.value << ", Type: " << node->token.type << ", Shape: " << node->shape << std::endl;

    // Recursively print the children
    for (SyntaxTreeNode* child : node->children) {
        printSyntaxTree(child, depth + 1);
    }

    // Print the next node
    if (node->next != nullptr) {
        std::cout << "Next node:" << std::endl;
        printSyntaxTree(node->next, depth);
    }
}


int main() {

    std::cout << "Hello World!\n";
    ifstream infile("tokens.txt");
    string line;

    if (!infile.is_open()) {
        cerr << "Error opening file!" << endl;
        return 1;
    }
    queue<Token> tokenQueue;

    // Read the file line by line
    while (getline(infile, line)) {
        // Tokenize each line and add tokens to the queue
        Token lineTokens = tokenizeLine(line);
        tokenQueue.push(lineTokens);
    }

    infile.close();
    /*
    // Print the tokens from the queue
    while (!tokenQueue.empty()) {
        Token t = tokenQueue.front();
        cout <<  t.value << "," << t.type << endl;
        tokenQueue.pop();
    }
    */

    TinyParser parser(tokenQueue);
    SyntaxTreeNode* root = parser.parseProgram();

    printSyntaxTree(root);
    return 0;
}


#include"Scanner.h"

vector <string> tokens_vector;
ifstream source;

int main() {
    std::string pgm = "tiny_sample_code.txt";
    source.open(pgm);
    if (!source) {
        std::cerr << "File " << pgm << " not found\n";
        return 1;
    }
    std::cout << "\nTINY COMPILATION: " << pgm << "\n";
    while (getToken(tokens_vector, source).type != TokenType::ENDFILE);

    source.close();

    for (int i = 0; i < tokens_vector.size(); i++)
    {
        cout << tokens_vector[i] << "\n";
    }

    return 0;
}
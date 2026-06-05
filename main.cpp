#include "LexerP.hpp"
#include <sstream>
#include <iostream>
#include <fstream>

int main(int argv, char* argc[]){

    if(argv <2 ){
        std::cerr << "Usage: " << argc[0] << " <source_file>" << std::endl;
        return 1;
    }

    std::fstream file(argc[1], std::ios::in);
    if(!file.is_open()){
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    std::stringstream input;
    input << file.rdbuf();
    file.close();

    std::istringstream input_stream(input.str());
    Lexer lexer(input_stream);

    Token token;
    
    do {
        token = lexer.NextToken();
        std::cout << token.ToString()  << std::endl;
    } while (token.Id != TokenId::END_OF_FILE);

    return 0;
}
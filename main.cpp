#include "ParserP.hpp"
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

    try{std::cout<<"Initialazing Parsing Gramar\n"; Parser parser(lexer);}catch(std::runtime_error error){
        std::cerr << "\nError Sintactico en el Parser, por favor cheque\n" << error.what() << std::endl;
        return 1;
    }
    return 0;
}
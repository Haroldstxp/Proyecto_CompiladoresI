#include "ParserP.hpp"
#include <stdexcept>

void Parser::program() {
    while (TokenId::END_OF_FILE != CurrentToken.Id) {
        topDecl();
    }
}

void Parser::topDecl() {
    if (CurrentToken.Id == TokenId::ID_IDENT) { // 'VAR'
        varDecl();
    } else if (CurrentToken.Id == TokenId::ID_FUNC) {
        funcDecl();
    } else {
        throw std::runtime_error("Expected 'var' or 'func' declaration at top level: " + CurrentToken.ToString() + "/n");
    }

}

void Parser::varDecl() {
    consume(); // consume 'var'
    if (CurrentToken.Id != TokenId::IDENTIFIER) {
        throw std::runtime_error("Expected identifier after 'var'");
    }
    consume(); // consume identifier
    type(); 
    if (CurrentToken.Id == TokenId::OP_ASSING) {
        consume(); // consume '='
        expr();
    }

    if (CurrentToken.Id != TokenId::SEMICOLON) {
        throw std::runtime_error("Expected ';' after variable declaration");
    }
    consume(); // consume ';'
}

void Parser::type() {
    if (CurrentToken.Id == TokenId::ID_INT || CurrentToken.Id == TokenId::ID_BOOL) {
        consume(); // consume 'int' o 'bool'
    } else {
        throw std::runtime_error("Expected type 'int' or 'bool'");
    }
}

void Parser::funcDecl() {
    consume(); // consume 'func'
    if (CurrentToken.Id != TokenId::IDENTIFIER) {
        throw std::runtime_error("Expected identifier after 'func'");
    }
    consume(); // consume identifier

    if (CurrentToken.Id != TokenId::OPEN_PAR) {
        throw std::runtime_error("Expected '(' in function declaration");
    }
    consume(); // consume '('

    //checar gramar ID_IDENT
    if (CurrentToken.Id == TokenId::ID_IDENT || CurrentToken.Id == TokenId::IDENTIFIER) {
        paramList();
    }

    if (CurrentToken.Id == TokenId::ADDRESS || CurrentToken.Id == TokenId::ADDRESSK){
        param();
    }

    if (CurrentToken.Id != TokenId::CLOSE_PAR) {
        throw std::runtime_error("Expected ')' after parameter list" + CurrentToken.ToString()  );
    }
    consume(); // consume ')'

    // El tipo de retorno es opcional
    if (CurrentToken.Id == TokenId::ID_INT || CurrentToken.Id == TokenId::ID_BOOL) {
        type();
    }
    block();
}

void Parser::paramList() {
    param();
    while (CurrentToken.Id == TokenId::COMA) {
        consume(); // consume ','
        param();
    }
}

void Parser::param() {
    if (CurrentToken.Id == TokenId::ADDRESSK) {
        consume(); // consume 'ref'
    }
    if (CurrentToken.Id != TokenId::IDENTIFIER) {
        throw std::runtime_error("Expected identifier in parameter declaration");
    }
    consume(); // consume identifier
    type();
}

void Parser::block() {
    if (CurrentToken.Id != TokenId::OPEN_BREAK) { // '{'
        throw std::runtime_error("Expected '{' to start a block");
    }
    consume(); // consume '{'

    while (CurrentToken.Id != TokenId::CLOSE_BREAK && CurrentToken.Id != TokenId::END_OF_FILE) {
        stmt();
    }

    if (CurrentToken.Id != TokenId::CLOSE_BREAK) {  //'}'
        throw std::runtime_error("Expected '}' to close a block");
    }
    consume(); // consume '}'
}

void Parser::stmt() {
    if (CurrentToken.Id == TokenId::ID_IDENT) {
        varDecl();
    } else if (CurrentToken.Id == TokenId::ID_IF) {
        ifStmt();
    } else if (CurrentToken.Id == TokenId::ID_FOR) {
        forStmt();
    } else if (CurrentToken.Id == TokenId::ID_RETURN) {
        returnStmt();
    } else if (CurrentToken.Id == TokenId::ID_PRINT || CurrentToken.Id == TokenId::ID_PRINTLN) {
        printStmt();
    } else if (CurrentToken.Id == TokenId::IDENTIFIER) {
        // Al empezar con IDENTIFIER, puede ser: callFixstmt, shortDecl o assignStmt
        consume(); // consume IDENTIFIER
        
        if (CurrentToken.Id == TokenId::OPEN_PAR) {
            callFixstmt();
        } else if (CurrentToken.Id == TokenId::OP_SHDECLA) { // ':='
            consume(); // consume ':='
            expr();
            if (CurrentToken.Id != TokenId::SEMICOLON) throw std::runtime_error("Expected ';' after short declaration");
            consume();
        } else if (CurrentToken.Id == TokenId::OP_ASSING) { // '='
            consume(); // consume '='
            expr();
            if (CurrentToken.Id != TokenId::SEMICOLON) throw std::runtime_error("Expected ';' after assignment");
            consume();
        } else {
            throw std::runtime_error("Invalid statement after identifier");
        }
    } else {
        throw std::runtime_error("Unknown or invalid statement "  + CurrentToken.ToString() + "\n");
    }
}

void Parser::callFixstmt() {
    consume(); // consume '('
    if (CurrentToken.Id != TokenId::CLOSE_PAR) {
        argList();
    }
    if (CurrentToken.Id != TokenId::CLOSE_PAR) {
        throw std::runtime_error("Expected ')' after arguments");
    }
    consume(); // consume ')'
    if (CurrentToken.Id != TokenId::SEMICOLON) {
        throw std::runtime_error("Expected ';' after function call statement");
    }
    consume(); // consume ';'
}

void Parser::ifStmt() {
    consume(); // consume 'if'
    expr();
    block();

    while (CurrentToken.Id == TokenId::ID_ELSE) {
        consume(); // consume 'else'
        if (CurrentToken.Id == TokenId::ID_IF) {
            consume(); // consume 'if'
            expr();
            block();
        } else {
            block();
            break; // 'else' plano termina la cadena del if
        }
    }
}

void Parser::forStmt() {
    consume(); // consume 'for'
    expr();
    block();
}

void Parser::returnStmt() {
    consume(); // consume 'return'
    if (CurrentToken.Id != TokenId::SEMICOLON) {
        expr();
    }
    if (CurrentToken.Id != TokenId::SEMICOLON) {
        throw std::runtime_error("Expected ';' after return statement");
    }
    consume(); // consume ';'
}

void Parser::printStmt() {
    consume(); // consume 'print' o 'println'
    if (CurrentToken.Id != TokenId::OPEN_PAR) {
        throw std::runtime_error("Expected '(' after print/println");
    }
    consume(); // consume '('

    printArg();
    while (CurrentToken.Id == TokenId::COMA) {
        consume(); // consume ','
        printArg();
    }

    if (CurrentToken.Id != TokenId::CLOSE_PAR) {
        throw std::runtime_error("Expected ')' after print arguments");
    }
    consume(); // consume ')'
    if (CurrentToken.Id != TokenId::SEMICOLON) {
        throw std::runtime_error("Expected ';' after print statement");
    }
    consume(); // consume ';'
}

void Parser::printArg() {
    if (CurrentToken.Id == TokenId::STRING_PRINT) {
        consume(); // consume string literal
    } else {
        expr(); // de lo contrario es una expresión
    }
}

//---------- EXPRESIONES ----------//

void Parser::expr() {
    orExpr();
}

void Parser::orExpr() {
    andExpr();
    while (CurrentToken.Id == TokenId::OP_BOOLEAN_OR) {
        consume(); 
        andExpr();
    }
}

void Parser::andExpr() {
    notExpr();
    while (CurrentToken.Id == TokenId::OP_BOOLEAN_AND) {
        consume(); 
        notExpr();
    }
}

 void Parser::notExpr(){
    if (CurrentToken.Id == TokenId::OP_BOOLEAN_SI){
        consume();
        notExpr();
    }else{
        relExpr();
    }
} 

void Parser::relExpr() {
    addExpr();
    // Verificamos si el token actual es un operador relacional
    if (CurrentToken.Id == TokenId::OP_RELATION_EQUAL || CurrentToken.Id == TokenId::OP_RELATION_DISNT ||
        CurrentToken.Id == TokenId::OP_RELATION_LESS_EQUAL || CurrentToken.Id == TokenId::OP_RELATION_MORE_EQUAL ||
        CurrentToken.Id == TokenId::OP_RELATION_LESS || CurrentToken.Id == TokenId::OP_RELATION_MORE) {
        relOp(); 
        addExpr();
    }
}

void Parser::relOp() {
    if (CurrentToken.Id == TokenId::OP_RELATION_EQUAL || CurrentToken.Id == TokenId::OP_RELATION_DISNT ||
        CurrentToken.Id == TokenId::OP_RELATION_LESS_EQUAL || CurrentToken.Id == TokenId::OP_RELATION_MORE_EQUAL ||
        CurrentToken.Id == TokenId::OP_RELATION_LESS || CurrentToken.Id == TokenId::OP_RELATION_MORE) {
        consume(); 
    } else {
        throw std::runtime_error("Expected relational operator");
    }
}

void Parser::addExpr() {
    mulExpr();
    while (CurrentToken.Id == TokenId::OP_PLUS || CurrentToken.Id == TokenId::OP_MINUS) {
        consume(); // consume '+' o '-'
        mulExpr();
    }
}

void Parser::mulExpr() {
    unaryExpr();
    while (CurrentToken.Id == TokenId::OP_MULT || CurrentToken.Id == TokenId::OP_DIV || CurrentToken.Id == TokenId::OP_MOD) {
        consume(); // consume '*', '/' o '%'
        unaryExpr();
    }
}

void Parser::unaryExpr() {
    if (CurrentToken.Id == TokenId::OP_MINUS) {
        consume();
        unaryExpr();
    } else {
        primary();
    }
}

void Parser::primary() {
    if (CurrentToken.Id == TokenId::IDENTIFIER) {
        consume(); // consume IDENTIFIER
        // sufixstmt -> "(" [ argList ] ")" | epsilon
        if (CurrentToken.Id == TokenId::OPEN_PAR) {
            consume(); // consume '('
            if (CurrentToken.Id != TokenId::CLOSE_PAR) {
                argList();
            }
            if (CurrentToken.Id != TokenId::CLOSE_PAR) {
                throw std::runtime_error("Expected ')' in function call");
            }
            consume(); // consume ')'
        }
    } else if (CurrentToken.Id == TokenId::NUMBER) {
        consume();
    } else if (CurrentToken.Id == TokenId::ID_TRUE_BOOL || CurrentToken.Id == TokenId::ID_FALSE_BOOL) {
        consume();
    } else if (CurrentToken.Id == TokenId::OPEN_PAR) {
        consume(); // consume '('
        expr();
        if (CurrentToken.Id != TokenId::CLOSE_PAR) {
            throw std::runtime_error("Expected ')' after expression");
        }
        consume(); // consume ')'
    } else {
        throw std::runtime_error("Expected primary expression (identifier, literal or definition) " + CurrentToken.ToString());
    }
}

void Parser::argList() {
    arg();
    while (CurrentToken.Id == TokenId::COMA) {
        consume(); // consume ','
        arg();
    }
}

void Parser::arg() {
    if (CurrentToken.Id == TokenId::ADDRESS) { // Token para '&'
        consume(); // consume '&'
        expr();
    } else {
        expr();
    }
}
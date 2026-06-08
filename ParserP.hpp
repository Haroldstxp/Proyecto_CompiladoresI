#include "LexerP.hpp"
#include <iostream>

class Parser
{
private:
    Lexer& lexer;
    Token CurrentToken;
    void consume()
    {
        CurrentToken = lexer.NextToken();
    };
    void Parse(){
        consume();
        program();
        if (CurrentToken.Id != TokenId::END_OF_FILE)
        {
            throw std::runtime_error("Expect end of file");
        }
        std::cout<<"\n -_- PARSING SUCCESS WITHOUT ERRORS IN GRAMMAR -_- \n";
    }
    
public:
    Parser(Lexer& lex):lexer(lex){Parse();};
 //! program → topDeclar* EOF ;
    void program();


 //! varDecl | funcDecl
    void topDecl();


 //? outside we can include type = "int" | "bool" ; ?
//!varDecl = "var" IDENTIFIER type ["=" expr ] ";" ;
    void varDecl();

    void type();

//!funcDecl → "func" IDENTIFIER "(" [ paramList ] ")" [ type ] block ;
    void funcDecl();


//*paramList → param { "," param } ;
//! func holamundo(ref,ref,variable1 int, variable2 bool,param,"opcional") int bloque 
    void paramList();

//!param → [ "ref" ] IDENTIFIER type ;
    void param();


//! block → "{" { stmt } "}"
    void block();


//! stmt →  varDecl
        //! | shortDecl
        //! | assignStmt
        //! | ifStmt
        //! | forStmt
        //! | returnStmt
        //! | callStmt
        //! | printStmt ;
void stmt();

void callFixstmt();

//! shortDecl → IDENTIFIER ":=" expr ";" ; (* local scope only *)
void shortDecl();

//! assignStmt → IDENTIFIER "=" expr ";" ;
void assignStmt();

//! ifStmt → "if" expr block
        //!{ "else" "if" expr block }
        //![ "else" block ]
void ifStmt();

//! forStmt →  "for" expr block ; 
void forStmt();

//! returnStmt → "return" [ expr ] ";" ;
void returnStmt();

//! callStmt → IDENTIFIER "(" [ argList ] ")" ";" ;
void callStmt();

//! argList → arg { "," arg } ;
void argList();

//! arg → [ "&" ] expr ;
//* Cadena de resultado: `Variable1(optional_argList,arg,arg,&exp);`
void arg();

//! printStmt → ( "print" | "println" ) "(" printArg { "," printArg } ")" ";" ;
void printStmt();

//! printArg → expr | STRING_LIT ;
void printArg();

//! expr → orExpr ;
void expr();

//! orExpr → andExpr { "||" andExpr } ;
void orExpr();

//! andExpr → notExpr { "&&" notExpr } ;
void andExpr();

//? notExpr → "!" notExpr | relExpr ; ? es recursivo por la izquierda?
void notExpr();

//! relExpr → addExpr [ relOp addExpr ] ;
void relExpr();

//! relOp → "==" | "!=" | "<=" | ">=" | "<" | ">" ;
void relOp();

//! addExpr → mulExpr { ("+" | "-") mulExpr } ;
void addExpr();

//! mulExpr → unaryExpr { ("*" | "/" | "%") unaryExpr } ;
void mulExpr();

//! unaryExpr → "-" unaryExpr | primary ;
void unaryExpr();


//! primary → INT_LIT
            //! | "true"
            //! | "false"
            //! | IDENTIFIER
            //! | callExpr
            //! | "(" expr ")" ;
void primary();



//*Funciones que no se ha agregado porque esta en duda
//! callExpr → IDENTIFIER "(" [ argList ] ")" ; [hace misma funcion que callStmt]
//! type → "int" | "bool" ; [se puede incluir en varDecl y param]
};


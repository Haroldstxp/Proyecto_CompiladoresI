#include "LexerP.hpp"
#include "format"
#include <iostream>

Token Lexer::NextToken() {
    do {
        // 1. Omitir espacios y caracteres de control
        if (ch == '\t' || ch == '\n' || ch == '\r') {
            consume();
            while (ch == '\t' || ch == '\n' || ch == '\r') {
                consume();
            }
            //return {TokenId::CSCAPE}; Ignoro los caracteres de esape, porque la gramatica no se que hacerlo :v
        }

        if(ch == ' '){
            consume();
            while(ch == ' '){
                consume();
            }
        }

        // 2. Fin de archivo
        if (ch == EOF) {
            return {TokenId::END_OF_FILE, "<<EOF>>"};
        }

        // 3. Comentarios y Operadores con '/'
        if (ch == '/') {
            std::string l(1, ch);
            consume();
            if (ch == '*') { // Bloque
                l += ch; consume();
                while (true) {
                    if (ch == EOF) break;
                    if (ch == '*') {
                        l += ch; consume();
                        if (ch == '/') { l += ch; consume(); break; }
                    } else { l += ch; consume(); }
                }
                return {TokenId::BLOCK_COMNT, l};
            } else if (ch == '/') { // Línea
                while (ch != '\n' && ch != EOF && ch != '\r') {
                    l += ch; consume();
                }
                return {TokenId::COMMENT_LINE, l};
            }
            return {TokenId::OP_DIV, l};
        }

        // 4. Identificadores y Palabras Reservadas
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_') {
            std::string l(1, ch);
            consume();
            while ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_' || (ch >= '0' && ch <= '9')) {
                l += ch;
                consume();
            }
            // Keywards
            if (l == "var") return {TokenId::ID_IDENT, l};
            if (l == "func") return {TokenId::ID_FUNC, l};
            if (l == "return") return {TokenId::ID_RETURN, l};
            if (l == "if") return {TokenId::ID_IF, l};
            if (l == "else") return {TokenId::ID_ELSE, l};
            if (l == "for") return {TokenId::ID_FOR, l};
            if (l == "true") return {TokenId::ID_TRUE_BOOL, l};
            if (l == "false") return {TokenId::ID_FALSE_BOOL, l};
            if (l == "int") return {TokenId::ID_INT, l};
            if (l == "bool") return {TokenId::ID_BOOL, l};
            if (l == "print") return {TokenId::ID_PRINT, l};
            if (l == "println") return {TokenId::ID_PRINTLN, l};
            if (l == "ref") return {TokenId::ADDRESSK, l};

            //identificador
            return {TokenId::IDENTIFIER, l};
        }

        // 5. Números
        if ((ch >= '0' && ch <= '9') || ch == '-') {
            std::string l(1, ch);
            consume();
            while (ch >= '0' && ch <= '9') {
                l += ch;
                consume();
            }
            return {TokenId::NUMBER, l};
        }

        // 6. Operadores Compuestos y Puntuación
        if (ch == '=') {
            consume();
            if (ch == '=') { consume(); return {TokenId::OP_RELATION_EQUAL, "=="}; }
            return {TokenId::OP_ASSING, "="};
        }
        if (ch == ':') {
            consume();
            if (ch == '=') { consume(); return {TokenId::OP_SHDECLA, ":="}; }
            return {TokenId::UNK, ":"};
        }
        if (ch == '<') {
            consume();
            if (ch == '=') { consume(); return {TokenId::OP_RELATION_LESS_EQUAL, "<="}; }
            return {TokenId::OP_RELATION_LESS, "<"};
        }
        if(ch == '>') {
            consume();
            if (ch == '=') { consume(); return {TokenId::OP_RELATION_MORE_EQUAL, ">="}; }
            return {TokenId::OP_RELATION_MORE, ">"};
        }
        if (ch == '!') {
            consume();
            if (ch == '=') { consume(); return {TokenId::OP_RELATION_DISNT, "!="}; }
            return {TokenId::OP_BOOLEAN_SI, "!"};
        }
        if (ch == '&') {
            consume();
            if (ch == '&') { consume(); return {TokenId::OP_BOOLEAN_AND, "&&"}; }
            return {TokenId::ADDRESS, "&"};
        }
        if (ch == '|') {
            consume();
            if (ch == '|') { consume(); return {TokenId::OP_BOOLEAN_OR, "||"}; }
            return {TokenId::UNK, "|"};
        }

        if (ch == '+') { consume(); return {TokenId::OP_PLUS, "+"}; }
        if (ch == '-') { consume(); return {TokenId::OP_MINUS, "-"}; }
        if (ch == '*') { consume(); return {TokenId::OP_MULT, "*"}; }
        if (ch == '%') { consume(); return {TokenId::OP_MOD, "%"}; }
        
        // Puntuación simple
        if (ch == '(') { consume(); return {TokenId::OPEN_PAR, "("}; }
        if (ch == ')') { consume(); return {TokenId::CLOSE_PAR, ")"}; }
        if (ch == '{') { consume(); return {TokenId::OPEN_BREAK, "{"}; }
        if (ch == '}') { consume(); return {TokenId::CLOSE_BREAK, "}"}; }
        if (ch == ';') { consume(); return {TokenId::SEMICOLON, ";"}; }
        if (ch == ',') { consume(); return {TokenId::COMA, ","}; }

        // 6.5 ContentPrint
        if (ch == '"') {
            std::string l(1, ch);
            consume();
            
            while (ch != '"' && ch != EOF) {

                if (ch == '\\') { 
                    l += ch; //se tiene "/
                    consume();
                    if (ch == EOF) break;

                    if (ch != '\\' && ch != 'n' && ch != 't' && ch != '"' && ch != 'r') { 
                        throw std::runtime_error("Invalid escape sequence in string literal, wait: \\n, \\t, \\\\, \\, \\r");
                        l += ch; // Almacenamos el caracter // "\nn
                        consume();
                        return {TokenId::UNK, l};
                    }else{
                        l += ch;
                        consume();
                    }
                }else{
                    l += ch;
                    consume();
                    if (ch == '"')
                    {
                        l += ch;
                        consume();
                        return {TokenId::STRING_PRINT, l};
                    }  
                }
            }
            if (ch == '"') {
                l += ch;
                consume();
                return {TokenId::STRING_PRINT, l};
            } else {
                throw std::runtime_error("Unterminated string literal");
            }
        }

        // 7. Carácter no reconocido
        std::string l(1, ch);
        consume();
        return {TokenId::UNK, l};

    } while (true);
}

std::string Token::ToString(){

    std::string str{};

    switch (Id)
    {
    //!KEYWORDS
    case TokenId::ID_IDENT: str += "K_var"; break;
    case TokenId::ID_FUNC: str += "K_function"; break;
    case TokenId::ID_RETURN: str += "K_return"; break;
    case TokenId::ID_IF: str += "K_if"; break;
    case TokenId::ID_ELSE: str += "K_else"; break;
    case TokenId::ID_FOR: str += "K_for"; break;
    case TokenId::ID_TRUE_BOOL: str += "K_true"; break;
    case TokenId::ID_FALSE_BOOL: str += "K_false"; break;
    case TokenId::ID_INT: str += "K_int"; break;
    case TokenId::ID_BOOL: str += "K_bool"; break;
    case TokenId::ADDRESSK: str += "K_Ref"; break;
    case TokenId::ID_PRINT: str += "K_print"; break;
    case TokenId::ID_PRINTLN: str += "K_println"; break;

    //!COMMENTS
    case TokenId::COMMENT_LINE: str += "Comment_line"; break;
    case TokenId::BLOCK_COMNT: str += "Block_comment"; break;

    //!DELIMITERS OR PUNCTUATION
    case TokenId::OPEN_PAR: str += "Open_par"; break;
    case TokenId::CLOSE_PAR: str += "Close_par"; break;
    case TokenId::SEMICOLON: str += "S_colon"; break;
    case TokenId::OPEN_BREAK: str += "Open_break"; break;
    case TokenId::CLOSE_BREAK: str += "Close_break"; break;
    case TokenId::COMA: str += "Coma"; break;
    //!OPERATORS
    case TokenId::OP_PLUS: str += "Plus"; break;
    case TokenId::OP_MINUS: str += "Minus"; break;
    case TokenId::OP_MULT: str += "Mult"; break;
    case TokenId::OP_DIV: str += "Div"; break;
    case TokenId::OP_MOD: str += "Mod"; break;
    case TokenId::OP_RELATION_EQUAL: str += "Igual-igual"; break;
    case TokenId::OP_RELATION_DISNT: str += "Distinto"; break;
    case TokenId::OP_RELATION_LESS_EQUAL: str += "Menor-igual"; break;
    case TokenId::OP_RELATION_MORE_EQUAL: str += "Mayor-igual"; break;
    case TokenId::OP_RELATION_LESS: str += "Menor"; break;
    case TokenId::OP_RELATION_MORE: str += "Mayor"; break;
    case TokenId::OP_BOOLEAN_SI: str += "SI"; break;
    case TokenId::OP_BOOLEAN_OR: str += "OR"; break;
    case TokenId::OP_BOOLEAN_AND: str += "AND"; break;
    case TokenId::OP_BOOLEAN_DIST: str += "Disinto Bool"; break;
    case TokenId::OP_ASSING: str += "Assing"; break;
    case TokenId::OP_SHDECLA: str += "Declaracion_corta"; break;
    //!DATA TYPES
    case TokenId::IDENTIFIER: str+= "Ident"; break;
    case TokenId::NUMBER: str+= "Number"; break;
    case TokenId::CSCAPE: str+= "Caracter_Escape"; break;
    case TokenId::STRING_PRINT: str+= "String_Print"; break;
    case TokenId::ADDRESS: str+= "Aspersan"; break;
    //!END
    case TokenId::END_OF_FILE: str += "EOF"; break;

    default: str += "Unk"; break;
    }
    return std::format("{:<12} : {}", str, lexema); 
    
}
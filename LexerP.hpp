#include <string>
#include <sstream>

enum class TokenId{
ID_IDENT,
ID_FUNC,
ID_RETURN,
ID_IF,
ID_ELSE,
ID_FOR,
ID_TRUE_BOOL,
ID_FALSE_BOOL,
ID_INT,
ID_BOOL,
ADDRESSK,
ID_PRINT,
ID_PRINTLN,
BLOCK_COMNT,
COMMENT_LINE,
OPEN_PAR,
CLOSE_PAR,
SEMICOLON,
OPEN_BREAK,
CLOSE_BREAK,
COMA,
OP_PLUS,
OP_MINUS,
OP_MULT,
OP_DIV,
OP_MOD,
OP_RELATION_EQUAL,
OP_RELATION_DISNT,
OP_RELATION_LESS_EQUAL,
OP_RELATION_MORE_EQUAL,
OP_RELATION_LESS,
OP_RELATION_MORE,
OP_BOOLEAN_SI,
OP_BOOLEAN_OR,
OP_BOOLEAN_AND,
OP_BOOLEAN_DIST,
OP_ASSING,
OP_SHDECLA,
IDENTIFIER,
NUMBER,
CSCAPE,
STRING_PRINT,
ADDRESS,
END_OF_FILE,
UNK
};

struct Token {
    TokenId Id;
    std::string ToString();
    std::string lexema;
};

class Lexer
{
private:
    std::istream&_input;
    char ch;
    void consume()
    {ch = _input.get();};
public:
    Lexer(std::istream&input):_input(input){consume();};
    Token NextToken();
    Lexer(const Lexer&) = delete;
};



%%
//KEYWORDS
"var"                                   { return ID_IDENT; }
"func"                                  { return ID_FUNC; }
"return"                                { return ID_RETURN; }
"if"                                    { return ID_IF; }
"else"                                  { return ID_ELSE; }
"for"                                   { return ID_FOR; }
"true"                                  { return ID_TRUE_BOOL; }
"false"                                 { return ID_FALSE_BOOL; }
"int"                                   { return ID_INT; }
"bool"                                  { return ID_BOOL; }
"ref"                                   { return ADDRESS; }
"print"                                 { return ID_PRINT; }
"println"                               { return ID_PRINTLN; }


//COMMENTS
"/*"([^*]|\*+[^*/])*\*+"/"              { return BLOCK_COMNT; }
\/\/.*                                  { return COMMENT_LINE; }


//DELIMITERS OR PUNCTUATION
"("                                     { return OPEN_PAR; }
")"                                     { return CLOSE_PAR; }
";"                                     { return SEMICOLON; }
"{"                                     { return OPEN_BREAK; }
"}"                                     { return CLOSE_BREAK; }
","                                     { return COMA; }


//OPERATORS
"+"                                     { return OP_PLUS; }
"-"                                     { return OP_MINUS; }
"*"                                     { return OP_MULT; }
"/"                                     { return OP_DIV; }
"%"                                     { return OP_MOD; }
"=="                                    { return OP_RELATION; }
"!="                                    { return OP_RELATION; }
"<="                                    { return OP_RELATION; }
">="                                    { return OP_RELATION; }
"<"                                     { return OP_RELATION; }
">"                                     { return OP_RELATION; }
"&&"                                    { return OP_BOOLEAN_SI; }
"||"                                    { return OP_BOOLEAN_OR; }
"!"                                     { return OP_BOOLEAN_DIST; }
"="                                     { return OP_ASSING; }
":="                                    { return OP_SHDECLA; }


//DATA TYPES
[a-zA-Z_][a-zA-Z0-9_]*                  { return IDENTIFIER; }
-?[0-9]+                                { return NUMBER; }
[ \t\n]+                                { return CSCAPE; }
\"([^\"\\]|\\.)*\"                      { return STRING_PRINT; }
"&"                                     { return ADDRESS; }


//END
<<EOF>>                                 { return END_OF_FILE; }
.                                       { return UNK; }
%%
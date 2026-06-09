`Follow de rules of origen file`

`{ X } = zero or more X`
`[ X ] = optional X`
`X | Y = X or Y`
`Terminals are in "quotes" or ALL_CAPS`

program → {topDeclar} EOF ;
topDeclar → varDecl | funcDecl ;

`//----------- Declaraciones --------------------//`

varDecl → "var" IDENTIFIER type ["=" expr ] ";" ;

type → "int" | "bool" ;

funcDecl → "func" IDENTIFIER "(" [ paramList ] ")" [ type ] block ;
`func holamundo(ref,ref,variable1 int, variable2 bool,param,"opcional") int { stmt }`

paramList → param { "," param } ;
param → [ "ref" ] IDENTIFIER type ;

//una o mas veces el stmt
block → "{" { stmt } "}"

stmt -> IDENTIFIER callFixstmt | varDecl
| shortDecl
| assignStmt
| ifStmt
| forStmt
| returnStmt
| printStmt ;

callFixstmt -> "(" [ argList ] ")" ";" ;

shortDecl → IDENTIFIER ":=" expr ";" ; (_ local scope only _)

assignStmt → IDENTIFIER "=" expr ";" ;

ifStmt → "if" expr block
{ "else" "if" expr block }
[ "else" block ]

forStmt → "for" expr block ;

returnStmt → "return" [ expr ] ";" ;

c//FACTORIZADA EN callFixstmt allStmt → IDENTIFIER "(" [ argList ] ")" ";" ;
argList → arg { "," arg } ;
arg → [ "&" ] expr ;

`Variable1(optional_argList,arg,arg,&exp);`

printStmt → ( "print" | "println" ) "(" printArg { "," printArg } ")" ";" ;
printArg → expr | STRING_LIT ;
`print(printArg,expr,STRING_LIT);`

`//---------- EXPRESIONES ----------//`

expr → orExpr ;
orExpr → andExpr { "||" andExpr } ;
andExpr → notExpr { "&&" notExpr } ;
notExpr → "!" notExpr | relExpr ; ? es recursivo por la izquierda?
`print(andExpr || andExpr || notExpr && "!cuidado!" , orExpr);`
`print(andExpr || andExpr || notExpr ! notExpr| relExpr" , orExpr);`
relExpr → addExpr [ relOp addExpr ] ;

relOp → "==" | "!=" | "<" | ">" | "<=" | ">=" ;
addExpr → mulExpr { ( "+" | "-" ) mulExpr } ;

mulExpr → unaryExpr { ( "\*" | "/" | "%" ) unaryExpr } ;

unaryExpr → "-" unaryExpr | primary ;

primary -> IDENTIFIER sufixstmt | INT_LIT
| "true"
| "false"
| "(" expr ")" ;
sufixstmt -> "(" [ argList ] ")" ";" | epsilon

//factorizada callStmt → IDENTIFIER "(" [ argList ] ")" ";" ;

variable1(arg1,arg2,arg3);

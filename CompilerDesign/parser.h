#ifndef PARSER_H
#define PARSER_H
#include "header.h"
#include "lexer.h"
int constructBlocks();
LInfo Prog(LInfo info);
LInfo Main(LInfo info);
LInfo Block(LInfo info);
LInfo Decls(LInfo info);
LInfo Decl(LInfo info);
LInfo NameList(LInfo info);
LInfo NameList1(LInfo info);
LInfo STMTS(LInfo info);
LInfo STMT(LInfo info);
LInfo STMT1(LInfo info);
LInfo BOOLL(LInfo info);
LInfo BOOL1(LInfo info);
LInfo BOOL2(LInfo info);
LInfo Expr(LInfo info);
LInfo Expr1(LInfo info);
LInfo Term(LInfo info);
LInfo Term1(LInfo info);
LInfo Factor(LInfo info);
LInfo ForRep(LInfo info);
LInfo Rep(LInfo info);
LInfo Rep1(LInfo info);
LInfo Rep2(LInfo info);
LInfo SwitchBlock(LInfo info);
LInfo Cases(LInfo info);
LInfo Case(LInfo info);
LInfo Default(LInfo info);


bool Type1();
bool RelOp2();
int Num();
bool AddOp();
bool MulOp();
int findId(char ch[20]);
bool RelOp();
bool RelOp1();
bool Type();
bool Name();
void Grammer();
void GraAnalyer();

#endif // PARSER_H

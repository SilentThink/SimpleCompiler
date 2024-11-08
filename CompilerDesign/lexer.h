#ifndef LEXER_H
#define LEXER_H
#include "header.h"
#include "parser.h"
char hexdigit(char buffer);
int digit3(char num[]);
char Digit(char buffer);
char cut1(char buffer);
char charprocess(char buffer);
char  Alpha(char buffer);
char stringprocess(char buffer);
int search(char searchchar[ ]);
char Other(char buffer);
void analy();
#endif // LEXER_H

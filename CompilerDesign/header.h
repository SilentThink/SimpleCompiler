#ifndef HEADER_H
#define HEADER_H
#define MAXSIZE 500
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QObject>
#include <QShortcut>
#include <QKeySequence>
#include <vector>
#include <stdio.h>
#include <ctype.h>
#include <malloc.h>
#include <stdlib.h>
#include<iostream>
#include<cstring>
#include<string.h>
#include <fstream>
#include <cstdio>
#include <string>
#include <unordered_set>
#include <unordered_map>

extern FILE *fp;
extern FILE *fp0;
extern FILE *fp1;
extern FILE *fp2;
extern char cbuffer;
extern int error;
extern int warning;
extern char* buf;
extern char key[35][20];
extern int atype,id;
extern char* str;
extern int line;

using namespace std;

// token符号表
typedef struct
{
    int typenum;    // 种别码
    char type[20];  // 种类（宏、关键字、界符、标识符、字符常量）
    char word[20];  // 名称
    int wline;
}WordChart;
extern WordChart* w;
extern int len;//记录程序中所有的单词个数

// 标识符表
typedef struct
{
    char word[20];
    char type[10];
    float numValue;
    char rel[20];   // 语义信息
}ID;
extern ID* IDs;
extern int nlen;//记录定义的变量个数（位置）

// 四元式
typedef struct
{
    char op[10];
    char arg1[20];
    char arg2[20];
    char result[20];
    int block;
    bool active1=false,active2=false,active3=false;
}Quaternion;
extern Quaternion* Q;
extern int qlen;
extern int nowQamount;
extern int nowNum;

// 左值信息
struct LInfo
{
    int addr;
    ID Id;
};
extern LInfo info;
extern char tree[MAXSIZE][MAXSIZE];
extern int tlen;
extern int idx;
extern bool flag;

extern int e_len;
extern char* curr_idtype;

extern int regNum;
extern int lbNum;
#endif // HEADER_H

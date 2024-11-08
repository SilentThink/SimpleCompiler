#include "header.h"

int error=0;
FILE *fp0=NULL;
FILE *fp1=NULL;
FILE *fp2=NULL;
FILE *fp=NULL;
int idx = 0;
int warning=0;
char* buf = new char[200];
char key[35][20]= {"main","void","char","short","int","string","bool","long","float","double",
                    "sizeof","signed","unsigner","strcut","union","enum","typedef","auto","static",
                    "extern","const","if","else","for","while","do","break",
                    "goto","continue","return","switch","case","default"};
int id=4;
char* str = new char[100];
int line=1;

// token串（符号表）
WordChart* w = new WordChart[MAXSIZE];
int len=-1;
int nlen=-1;
ID* IDs = new ID[MAXSIZE];

Quaternion* Q= new Quaternion[MAXSIZE];
int qlen=-1;
int nowQamount=0;
int nowNum=0;

int tlen=-1;
int e_len=-1;
char* curr_idtype = new char[10];

// 递推式生成树
char tree[MAXSIZE][MAXSIZE];
bool flag;
char cbuffer;

int regNum = 0;
int lbNum = 0;

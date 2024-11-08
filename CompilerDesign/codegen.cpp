#include "codegen.h"

void huibian() {
    int i;
    FILE* fp2 = fopen("D://d6.txt", "w");
    if (!fp2) {
        cerr << "Failed to open file." << endl;
        return;
    }

    for (i = 0; i <= qlen; i++) {
        if (strcmp(Q[i].op, "=") == 0) {
            fprintf(fp2, "MOV %s, %s\r\n---------------------------------------\r\n", Q[i].result, Q[i].arg1);
        } else if (strcmp(Q[i].op, "+") == 0) {
            fprintf(fp2, "MOV AX, %s\r\nADD AX, %s\r\nMOV %s, AX\r\n---------------------------------------\r\n", Q[i].arg1, Q[i].arg2, Q[i].result);
        } else if (strcmp(Q[i].op, "-") == 0) {
            fprintf(fp2, "MOV AX, %s\r\nSUB AX, %s\r\nMOV %s, AX\r\n---------------------------------------\r\n", Q[i].arg1, Q[i].arg2, Q[i].result);
        } else if (strcmp(Q[i].op, "*") == 0) {
            fprintf(fp2, "MOV AX, %s\r\nMOV BX, %s\r\nMUL BX\r\nMOV %s, AX\r\n---------------------------------------\r\n", Q[i].arg1, Q[i].arg2, Q[i].result);
        } else if (strcmp(Q[i].op, "/") == 0) {
            fprintf(fp2, "MOV AX, %s\r\nMOV DX, 0\r\nMOV BX, %s\r\nDIV BX\r\nMOV %s, AX\r\n---------------------------------------\r\n", Q[i].arg1, Q[i].arg2, Q[i].result);
        } else if (strcmp(Q[i].op, "%") == 0) {
            fprintf(fp2, "MOV AX, %s\r\nMOV DX, 0\r\nMOV BX, %s\r\nDIV BX\r\nMOV %s, DX\r\n---------------------------------------\r\n", Q[i].arg1, Q[i].arg2, Q[i].result);
        } else if (strcmp(Q[i].op, "<") == 0) {
            fprintf(fp2, "MOV DX, 1\r\nMOV AX, %s\r\nCMP AX, %s\r\nJB _LT_%d\r\nMOV DX, 0\r\n_LT_%d: MOV %s, DX\r\n---------------------------------------\r\n", Q[i].arg1, Q[i].arg2, i, i, Q[i].result);
        } else if (strcmp(Q[i].op, ">=") == 0) {
            fprintf(fp2, "MOV DX, 1\r\nMOV AX, %s\r\nCMP AX, %s\r\nJNB _GE_%d\r\nMOV DX, 0\r\n_GE_%d: MOV %s, DX\r\n---------------------------------------\r\n", Q[i].arg1, Q[i].arg2, i, i, Q[i].result);
        } else if (strcmp(Q[i].op, ">") == 0) {
            fprintf(fp2, "MOV DX, 1\r\nMOV AX, %s\r\nCMP AX, %s\r\nJA _GT_%d\r\nMOV DX, 0\r\n_GT_%d: MOV %s, DX\r\n---------------------------------------\r\n", Q[i].arg1, Q[i].arg2, i, i, Q[i].result);
        } else if (strcmp(Q[i].op, "<=") == 0) {
            fprintf(fp2, "MOV DX, 1\r\nMOV AX, %s\r\nCMP AX, %s\r\nJNA _LE_%d\r\nMOV DX, 0\r\n_LE_%d: MOV %s, DX\r\n---------------------------------------\r\n", Q[i].arg1, Q[i].arg2, i, i, Q[i].result);
        } else if (strcmp(Q[i].op, "==") == 0) {
            fprintf(fp2, "MOV DX, 1\r\nMOV AX, %s\r\nCMP AX, %s\r\nJE _EQ_%d\r\nMOV DX, 0\r\n_EQ_%d: MOV %s, DX\r\n---------------------------------------\r\n", Q[i].arg1, Q[i].arg2, i, i, Q[i].result);
        } else if (strcmp(Q[i].op, "!=") == 0) {
            fprintf(fp2, "MOV DX, 1\r\nMOV AX, %s\r\nCMP AX, %s\r\nJNE _NE_%d\r\nMOV DX, 0\r\n_NE_%d: MOV %s, DX\r\n---------------------------------------\r\n", Q[i].arg1, Q[i].arg2, i, i, Q[i].result);
        } else if (strcmp(Q[i].op, "&&") == 0) {
            fprintf(fp2, "MOV DX, 0\r\nMOV AX, %s\r\nCMP AX, 0\r\nJE _AND_%d\r\nMOV AX, %s\r\nCMP AX, 0\r\nJE _AND_%d\r\nMOV DX, 1\r\n_AND_%d: MOV %s, DX\r\n---------------------------------------\r\n", Q[i].arg1, i, Q[i].arg2, i, i, Q[i].result);
        } else if (strcmp(Q[i].op, "||") == 0) {
            fprintf(fp2, "MOV DX, 1\r\nMOV AX, %s\r\nCMP AX, 0\r\nJNE _OR_%d\r\nMOV AX, %s\r\nCMP AX, 0\r\nJNE _OR_%d\r\nMOV DX, 0\r\n_OR_%d: MOV %s, DX\r\n---------------------------------------\r\n", Q[i].arg1, i, Q[i].arg2, i, i, Q[i].result);
        } else if (strcmp(Q[i].op, "if") == 0) {
            fprintf(fp2, "MOV AX, %s\r\nCMP AX, 0\r\nJE %s\r\n---------------------------------------\r\n", Q[i].arg1, Q[i].result);
        } else if (strcmp(Q[i].op, "el") == 0) {
            fprintf(fp2, "NOP\r\n---------------------------------------\r\n");
        } else if (strcmp(Q[i].op, "wh") == 0) {
            fprintf(fp2, "WHILE:\r\n---------------------------------------\r\n");
        } else if (strcmp(Q[i].op, "dow") == 0) {
            fprintf(fp2, "MOV AX, %s\r\nCMP AX, 0\r\nJE END_WHILE\r\n---------------------------------------\r\n", Q[i].arg1);
        } else if (strcmp(Q[i].op, "we") == 0) {
            fprintf(fp2, "JMP WHILE\r\nEND_WHILE:\r\n---------------------------------------\r\n");
        } else if (strncmp(Q[i].op, "lb", 2) == 0) {
            fprintf(fp2, "%s:\r\n---------------------------------------\r\n", Q[i].op);
        } else if (strcmp(Q[i].op, "ie") == 0) {
            fprintf(fp2, "NOP\r\n---------------------------------------\r\n");
        } else if (strcmp(Q[i].op, "for") == 0) {
            fprintf(fp2, "FOR:\r\n---------------------------------------\r\n");
        } else if (strcmp(Q[i].op, "dof") == 0) {
            fprintf(fp2, "MOV AX, %s\r\nCMP AX, 0\r\nJE END_FOR\r\n---------------------------------------\r\n", Q[i].arg1);
        } else if (strcmp(Q[i].op, "fe") == 0) {
            fprintf(fp2, "JMP FOR\r\nEND_FOR:\r\n---------------------------------------\r\n");
        } else if (strcmp(Q[i].op, "--") == 0) {
            fprintf(fp2, "DEC %s\r\n---------------------------------------\r\n", Q[i].arg1);
        } else if (strcmp(Q[i].op, "++") == 0) {
            fprintf(fp2, "INC %s\r\n---------------------------------------\r\n", Q[i].arg1);
        } else if (strcmp(Q[i].op, "sw") == 0) {
            fprintf(fp2, "SWITCH %s\r\n---------------------------------------\r\n", Q[i].arg1);
        } else if (strcmp(Q[i].op, "case") == 0) {
            fprintf(fp2, "CASE_%d:\r\n---------------------------------------\r\n", i/3);
            fprintf(fp2, "CMP %s, %s\r\nJNE CASE_%d\r\n---------------------------------------\r\n", Q[i].arg1, Q[i].arg2, i/3+1);
        } else if (strcmp(Q[i].op, "default") == 0) {
            fprintf(fp2, "CASE_%d:\r\n---------------------------------------\r\n", i/3);
        } else if (strcmp(Q[i].op, "se") == 0) {
            fprintf(fp2, "END_SWITCH:\r\n---------------------------------------\r\n");
        } else if (strcmp(Q[i].op,"break") == 0) {
            fprintf(fp2, "JMP END_%s\r\n",Q[i].arg1);
        } else if (strcmp(Q[i].op,"ret") == 0) {
            fprintf(fp2, "RET %s",Q[i].arg1);
        }
    }

    fclose(fp2);
}



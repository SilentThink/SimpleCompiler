#include "parser.h"

int constructBlocks() {
    int i;
    int num = 0;
    int isEntry[qlen + 1];  // 标记入口语句
    memset(isEntry, 0, sizeof(isEntry));
    char target[100][20];
    int target_num=0;
    // 第一步：找出所有的基本块入口语句
    isEntry[0] = 1;  // 程序的第一个语句是入口语句
    for (i = 0; i <= qlen; i++) {
        if (strcmp(Q[i].op, "if") == 0 || strcmp(Q[i].op, "el") == 0|| strcmp(Q[i].op, "we") == 0 || strcmp(Q[i].op, "wh") == 0 ||
            strcmp(Q[i].op, "dow") == 0 || strcmp(Q[i].op, "fe") == 0 || strcmp(Q[i].op, "sw") == 0 ||
            strcmp(Q[i].op, "dof") == 0||strcmp(Q[i].op, "for") == 0 ) {
            isEntry[i + 1] = 1;  // 紧跟在转向语句后面的语句
        }
        if(strcmp(Q[i].op, "case") == 0 || strcmp(Q[i].op, "default") == 0||strcmp(Q[i].op, "ie") == 0||
            strcmp(Q[i].op, "we") == 0||strcmp(Q[i].op, "fe") == 0){
            isEntry[i] = 1;
        }
        if (strcmp(Q[i].op, "if") == 0 || strcmp(Q[i].op, "el") == 0) {
            // 转向语句转移到的语句
            // 需要将字符串转换为整数以获取目标语句的索引
            if (target_num < 100) {  // 确保不会越界
                strcpy(target[target_num++], Q[i].result);  // 复制字符串到 target 数组中
            }
        }
        else{
            for(int j=0;j<target_num;j++){
                if(strcmp(Q[i].op, target[j]) == 0)
                    isEntry[i] = 1;
            }
        }
    }
    // 第二步：构造基本块
    for (i = 0; i <= qlen; i++) {
        if (isEntry[i] == 1) {
            num++;  // 新的基本块编号
        }
        Q[i].block = num;
    }
    return num;  // 返回基本块的总数
}

// 查找标识符位置
int findId(char ch[20])
{
    int i;
    for (i = 0; i <= nlen; i++)
    {
        if (strcmp(IDs[i].word, ch) == 0) return i;
    }
    return -1;
}

LInfo Prog(LInfo info) {
    if (flag)
    {
        strncpy(tree[++tlen], "<Prog>→<Main>", sizeof(tree[tlen]) - 1);
        LInfo info1;
        info1 = Main(info);
        if (!flag) return info;
        return info;
    }
    return info;
}

LInfo Main(LInfo info) {
    if (flag) {
        strncpy(tree[++tlen], "<Main>→<Type1>main()<Block>", sizeof(tree[tlen]) - 1);
        if (Type1()) {
            idx++;
            if (strcmp(w[idx].word, "main") == 0) {
                idx++;
                if (strcmp(w[idx].word, "(") == 0) {
                    idx++;
                    if (strcmp(w[idx].word, ")") == 0) {
                        idx++;
                        LInfo info1;
                        info1 = Block(info);
                        if (!flag) return info;
                        return info;
                    }
                    else {
                        fprintf(fp2, "%s之前缺少 )\r\n", w[idx].word);
                        error++;
                        flag = false;
                        return info;
                    }
                }
                else {
                    fprintf(fp2, "%s之前缺少 (\n", w[idx].word);
                    error++;
                    flag = false;
                    return info;
                }
            }
            else {
                fprintf(fp2, "没有主函数\r\n"); error++;
                flag = false;
                return info;
            }
        }
        else {
            fprintf(fp2, "没有函数类型\r\n"); error++;
            flag = false;
            return info;
        }
    }
    return info;
}

// <Block>→{<Decls><STMTS>}
LInfo Block(LInfo info) {
    if (flag) {
        strncpy(tree[++tlen], "<Block>→{<Decls><STMTS>}", sizeof(tree[tlen]) - 1);
        if (strcmp(w[idx].word, "{") == 0) {
            idx++;
            LInfo info1;
            info1 = Decls(info);
            if (!flag) return info;
            LInfo info2;
            info2 = STMTS(info);
            if (!flag) return info;
            if (strcmp(w[idx].word, "}") == 0) {
                idx++;
                return info;
            }
            else {
                fprintf(fp2, "%s之前缺少 }\r\n", w[idx].word); error++;
                flag = false;
                return info;
            }
        }
        else {
            fprintf(fp2, "%s之前缺少 {\r\n", w[idx].word); error++;
            flag = false;
            return info;
        }
    }
    return info;
}

// <DECLS>-><DECL><DECLS>
LInfo Decls(LInfo info) {
    if(flag) {
        strncpy(tree[++tlen],"<DECLS>->",sizeof(tree[tlen]-1));
        if(Type()) {
            strcat(tree[tlen], "<DECL><DECLS>");
            LInfo info1;info1 = Decl(info);
            if(!flag) return info;
            LInfo info2;info2 = Decls(info);
            if(!flag) return info;
            return info2;
        }
        else
        {
            strcat(tree[tlen],"empty");return info;
        }
    }
    return info;
}

LInfo Decl(LInfo info) {
    if (flag) {
        strncpy(tree[++tlen], "<Decl>→", sizeof(tree[tlen]) - 1);
        if (Type()) {
            strcat(tree[tlen], "<Type><NameList>");
            strcat(tree[tlen], ";<Decl>");
            strncpy(tree[++tlen], "<Type>→", sizeof(tree[tlen]) - 1);
            strcat(tree[tlen], w[idx].word);
            strncpy(info.Id.type, w[idx].word, sizeof(info.Id.type) - 1);
            idx++;
            LInfo info1;
            info1 = NameList(info);
            if (!flag) return info;
            if (strcmp(w[idx].word, ";") == 0) {
                idx++;
                LInfo info2;
                info2 = Decl(info);
                if (!flag) return info;
                return info;
            }
            else {
                fprintf(fp2, "%s之前缺少 ;\r\n", w[idx].word); error++;
                flag = false;
                return info;
            }
        }
        else {
            strcat(tree[tlen], "empty");
            return info;
        }
    }
    return info;
}



LInfo NameList(LInfo info)
{
    if (flag) {
        strncpy(tree[++tlen], "<NameList>→<Name><NameList1>", sizeof(tree[tlen]) - 1);
        if (Name()) {
            strncpy(tree[++tlen], "<Name>→", sizeof(tree[tlen]) - 1);
            strcat(tree[tlen], w[idx].word);
            if (findId(w[idx].word) != -1) {
                fprintf(fp2, "%s标识符重复！\r\n", w[idx].word); error++;
                flag = false; return info;
            }
            else
            {
                strncpy(IDs[++nlen].word, w[idx].word, sizeof(IDs[nlen].word) - 1);
                strncpy(IDs[nlen].type, info.Id.type, sizeof(IDs[nlen].type) - 1);
                idx++;
            }
            info.Id = IDs[nlen];
            LInfo info1;
            info1 = NameList1(info);
            if (!flag) return info;
            return info;
        }
        else {
            fprintf(fp2, "%s后缺少变量\r\n", w[idx].word); error++;
            flag = false;
            return info;
        }
    }
    return info;
}

LInfo NameList1(LInfo info) {
    if (flag) {
        strncpy(tree[++tlen], "<NameList1>→", sizeof(tree[tlen]) - 1);
        if (strcmp(w[idx].word, ",") == 0) {
            strcat(tree[tlen], ",<Name><NameList1>");
            idx++;
            if (Name()) {
                strncpy(tree[++tlen], "<Name>→", sizeof(tree[tlen]) - 1);
                strcat(tree[tlen], w[idx].word);
                if (findId(w[idx].word) != -1) {
                    fprintf(fp2, "%s标识符重复！\r\n", w[idx].word); error++;
                    flag = false;
                    return info;
                }
                else {
                    strncpy(IDs[++nlen].word, w[idx].word, sizeof(IDs[nlen].word) - 1);
                    strncpy(IDs[nlen].type, info.Id.type, sizeof(IDs[nlen].type) - 1);
                    idx++;
                }
                LInfo info1;
                info1 = NameList1(info);
                if (!flag) return info;
                return info;
            }
            else {
                fprintf(fp2, "%s后缺少变量\r\n", w[idx].word); error++;
                flag = false;
                return info;
            }
        }
        if (strcmp(w[idx].word, "=") == 0)
        {
            strcat(tree[tlen], "=<Expr><NameList1>");
            idx++;
            if (Name() || w[idx].typenum == 34) {
                if (w[idx].typenum == 33) {

                    if (findId(w[idx].word) != -1) {
                        fprintf(fp2, "%s标识符重复！\r\n", w[idx].word); error++;
                        flag = false;
                        return info;
                    }
                    else {
                        strncpy(IDs[++nlen].word, w[idx].word, sizeof(IDs[nlen].word) - 1);
                        strncpy(IDs[nlen].type, info.Id.type, sizeof(IDs[nlen].type) - 1);
                        idx++;
                    }
                }
                LInfo info1 = Expr(info);
                info.Id.numValue = info1.Id.numValue;
                //printf("(=,%d,_,%s)",info.Id.numValue,info.Id.word);
                LInfo info2;
                info2 = NameList1(info);
                if (!flag) return info;
                return info;
            }
            else {
                fprintf(fp2, "%s前缺少变量\r\n", w[idx].word); error++;
                flag = false;
                return info;
            }
        }
        else {
            strcat(tree[tlen], "empty");
            return info;
        }

    }
    return info;
}

bool Type1() {

    if (strcmp(w[idx].word, "int") == 0 || strcmp(w[idx].word, "void") == 0)
    {
        strncpy(tree[++tlen], "<Type1>→", sizeof(tree[tlen]) - 1); strcat(tree[tlen], w[idx].word); return true;
    }
    else return false;
}

bool Type() {

    if (strcmp(w[idx].word, "int") == 0 || strcmp(w[idx].word, "char") == 0 || strcmp(w[idx].word, "float") == 0 || strcmp(w[idx].word, "string") == 0 || strcmp(w[idx].word, "double") == 0 || strcmp(w[idx].word, "bool") == 0)
    {
        return true;
    }
    else return false;
}

// 判断类别是不是标识符
bool Name() {
    if (w[idx].typenum == 33) {
        return true;
    }
    else return false;
}

// <STMTS>→<STMT><STMTS>
LInfo STMTS(LInfo info) {
    if (flag) {
        strncpy(tree[++tlen], "<STMTS>→", sizeof(tree[tlen]) - 1);
        if (w[idx].typenum == 33 || strcmp(w[idx].word, "for") == 0 || strcmp(w[idx].word, "if") == 0 || strcmp(w[idx].word, "while") == 0 || strcmp(w[idx].word,"switch")==0 || strcmp(w[idx].word,"break")==0 || strcmp(w[idx].word, "return") == 0 || strcmp(w[idx].word, "{") == 0) {
            strcat(tree[tlen], "<STMT><STMTS>");
            LInfo info1; info1 = STMT(info);
            if (!flag) return info;
            LInfo info2; info2 = STMTS(info);
            if (!flag) return info;
            return info;
        }
        else {
            strcat(tree[tlen], "empty"); return info;
        }
    }
    return info;
}

LInfo STMT(LInfo info) {
    if (flag) {
        strncpy(tree[++tlen], "<STMT>→", sizeof(tree[tlen]) - 1);

        if (Name()) {//赋值语句 或 自增自减
            int nowIdAddr = findId(w[idx].word);
            if (nowIdAddr == -1) {
                flag = false;
                fprintf(fp2, "%s该标识符未定义\r\n", w[idx].word); error++;
                return info;
            }
            idx++;
            if (strcmp(w[idx].word, "=") == 0) {
                strcat(tree[tlen], "<Name>=<Expr>;");
                strncpy(tree[++tlen], "<Name>→", sizeof(tree[tlen]) - 1);
                strcat(tree[tlen], w[idx-1].word);
                idx++;
                LInfo info1 = Expr(info);
                if (!flag) return info;
                IDs[nowIdAddr].numValue = info1.Id.numValue;

                strncpy(Q[++qlen].op, "=", sizeof(Q[qlen].op) - 1);
                strncpy(Q[qlen].arg1, info1.Id.word, sizeof(Q[qlen].arg1) - 1);
                strncpy(Q[qlen].arg2, "_", sizeof(Q[qlen].arg2) - 1);
                strncpy(Q[qlen].result, IDs[nowIdAddr].word, sizeof(Q[qlen].result) - 1);

                info.addr = qlen;
                if (strcmp(w[idx].word, ";") == 0) {
                    idx++;
                    return info;
                }
                else {
                    fprintf(fp2, "%s前缺少 ;\n", w[idx].word); error++;
                    flag = false;
                    return info;
                }
            }
            else if(strcmp(w[idx].word, "++") == 0 || strcmp(w[idx].word, "--")==0)
            {
                strcat(tree[tlen], "<Name><RelOp2>;");
                strncpy(tree[++tlen], "<Name>→", sizeof(tree[tlen]) - 1);
                strcat(tree[tlen], w[idx].word);
                if(strcmp(w[idx].word, "++") == 0)
                {
                    strncpy(tree[++tlen], "<RelOp2>→++", sizeof(tree[tlen]) - 1);
                    IDs[nowIdAddr].numValue++;
                    strncpy(Q[++qlen].op, "++", sizeof(Q[qlen].op) - 1);
                    strncpy(Q[qlen].arg1, w[idx-1].word, sizeof(Q[qlen].arg1) - 1);
                    strncpy(Q[qlen].arg2, "_", sizeof(Q[qlen].arg2) - 1);
                    strncpy(Q[qlen].result, w[idx-1].word, sizeof(Q[qlen].result) - 1);
                }
                else if(strcmp(w[idx].word, "--") == 0)
                {
                    strncpy(tree[++tlen], "<RelOp2>→--", sizeof(tree[tlen]) - 1);
                    IDs[nowIdAddr].numValue--;
                    strncpy(Q[++qlen].op, "--", sizeof(Q[qlen].op) - 1);
                    strncpy(Q[qlen].arg1, w[idx-1].word, sizeof(Q[qlen].arg1) - 1);
                    strncpy(Q[qlen].arg2, "_", sizeof(Q[qlen].arg2) - 1);
                    strncpy(Q[qlen].result, w[idx-1].word, sizeof(Q[qlen].result) - 1);
                }
                idx++;
                if (!flag) return info;
                info.addr = qlen;
                if (strcmp(w[idx].word, ";") == 0) {
                    idx++;
                    return info;
                }
                else {
                    fprintf(fp2, "%s前缺少 ;\n", w[idx].word); error++;
                    flag = false;
                    return info;
                }
            }
            else {
                fprintf(fp2, "%s前缺少 =\n", w[idx].word); error++;
                flag = false;
                return info;
            }
        }
        else if (strcmp(w[idx].word, "if") == 0) {//if-else语句
            strcat(tree[tlen], "if(<BOOL>)<STMT><STMTS>");
            idx++;
            if (strcmp(w[idx].word, "(") == 0) {
                idx++;
                LInfo info1;
                info1 = BOOLL(info);
                if (!flag) return info;
                strncpy(Q[++qlen].op, "if", sizeof(Q[qlen].op) - 1);
                strncpy(Q[qlen].arg1, info1.Id.rel, sizeof(Q[qlen].arg1) - 1);
                strncpy(Q[qlen].arg2, "_", sizeof(Q[qlen].arg2) - 1);
                strncpy(Q[qlen].result, ("lb"+to_string(lbNum)).c_str(), sizeof(Q[qlen].result) - 1);
                info.addr = qlen;
                if (strcmp(w[idx].word, ")") == 0) {
                    idx++;
                    LInfo info2;
                    info2 = STMT(info);
                    if (!flag) return info;

                    LInfo info3;
                    info3 = STMT1(info);
                    if (!flag) return info;

                    return info;
                }
                else {
                    fprintf(fp2, "%s前缺少 )\n", w[idx].word); error++;
                    flag = false;
                    return info;
                }
            }
            else {
                fprintf(fp2, "10:%s前缺少 (\n", w[idx].word); error++;
                flag = false;
                return info;
            }
        }
        else if (strcmp(w[idx].word, "while") == 0) {//while循环
            strcat(tree[tlen], "while(<BOOL>)<STMT>");
            idx++;
            strncpy(info.Id.rel,"WHILE",sizeof(info.Id.rel)-1);
            if (strcmp(w[idx].word, "(") == 0) {
                strncpy(Q[++qlen].op, "wh", sizeof(Q[qlen].op) - 1);
                strncpy(Q[qlen].arg1, "_", sizeof(Q[qlen].arg1) - 1);
                strncpy(Q[qlen].arg2, "_", sizeof(Q[qlen].arg2) - 1);
                strncpy(Q[qlen].result,"_",sizeof(Q[qlen].result)-1);
                idx++;
                LInfo info1;
                info1 = BOOLL(info);
                if (!flag) return info;
                info.addr = qlen;
                if (strcmp(w[idx].word, ")") == 0) {
                    idx++;
                    LInfo info2;
                    strncpy(Q[++qlen].op, "dow", sizeof(Q[qlen].op) - 1);
                    strncpy(Q[qlen].arg1, ("t"+to_string(regNum)).c_str(), sizeof(Q[qlen].arg1) - 1);
                    strncpy(Q[qlen].arg2, "_", sizeof(Q[qlen].arg2) - 1);
                    strncpy(Q[qlen].result,"_",sizeof(Q[qlen].result)-1);
                    info2 = STMT(info);
                    if (!flag) return info;

                    strncpy(Q[++qlen].op, "we", sizeof(Q[qlen].op) - 1);
                    strncpy(Q[qlen].arg1, "_", sizeof(Q[qlen].arg1) - 1);
                    strncpy(Q[qlen].arg2, "_", sizeof(Q[qlen].arg2) - 1);
                    strncpy(Q[qlen].result,"_",sizeof(Q[qlen].result)-1);
                    return info;
                }
                else {
                    fprintf(fp2, "%s前缺少 )\n", w[idx].word); error++;
                    flag = false;
                    return info;
                }
            }
            else {
                fprintf(fp2, "%s前缺少 (\n", w[idx].word); error++;
                flag = false;
                return info;
            }
        }
        else if (strcmp(w[idx].word, "for") == 0) { //for循环
            strcat(tree[tlen], "for(<ForRep>)<STMT>");
            idx++;
            strncpy(info.Id.rel,"FOR",sizeof(info.Id.rel)-1);
            if (strcmp(w[idx].word, "(") == 0) {
                idx++;
                LInfo info1;

                info1 = ForRep(info);
                if (!flag) return info;
                if (strcmp(w[idx].word, ")") == 0) {
                    idx++;
                    LInfo info2;
                    strncpy(Q[++qlen].op, "dof", sizeof(Q[qlen].op) - 1);
                    strncpy(Q[qlen].arg1, ("t"+to_string(regNum)).c_str(), sizeof(Q[qlen].arg1) - 1);
                    strncpy(Q[qlen].arg2, "_", sizeof(Q[qlen].arg2) - 1);
                    strncpy(Q[qlen].result,"_",sizeof(Q[qlen].result)-1);
                    info2 = STMT(info);
                    strncpy(Q[++qlen].op, info1.Id.rel, sizeof(Q[qlen].op) - 1);
                    strncpy(Q[qlen].arg1, info1.Id.word, sizeof(Q[qlen].arg1) - 1);
                    strncpy(Q[qlen].arg2, "_", sizeof(Q[qlen].arg2) - 1);
                    strncpy(Q[qlen].result,info1.Id.word,sizeof(Q[qlen].result)-1);

                    strncpy(Q[++qlen].op, "fe", sizeof(Q[qlen].op) - 1);
                    strncpy(Q[qlen].arg1, "_", sizeof(Q[qlen].arg1) - 1);
                    strncpy(Q[qlen].arg2, "_", sizeof(Q[qlen].arg2) - 1);
                    strncpy(Q[qlen].result,"_",sizeof(Q[qlen].result)-1);
                    if (!flag) return info;
                    return info;
                }
                else {
                    fprintf(fp2, "%s前缺少 )\n", w[idx].word); error++;
                    flag = false;
                    return info;
                }
            }
            else {
                fprintf(fp2, "%s前缺少 (\n", w[idx].word); error++;
                flag = false;
                return info;
            }
        }
        // <STMT> → switch ( <Expr> ) <SwitchBlock>
        else if (strcmp(w[idx].word, "switch") == 0) { // switch语句
            strcat(tree[tlen], "switch ( <Expr> ) <SwitchBlock>");
            idx++;
            strncpy(info.Id.rel,"SWITCH",sizeof(info.Id.rel)-1);
            if (strcmp(w[idx].word, "(") == 0) {
                idx++;
                LInfo info1;
                info1 = Expr(info);
                strncpy(Q[++qlen].op, "sw", sizeof(Q[qlen].op) - 1);
                strncpy(Q[qlen].arg1, info1.Id.word, sizeof(Q[qlen].arg1) - 1);
                strncpy(Q[qlen].arg2, "_", sizeof(Q[qlen].arg2) - 1);
                strncpy(Q[qlen].result,"_",sizeof(Q[qlen].result)-1);
                if (!flag) return info;
                if (strcmp(w[idx].word, ")") == 0) {
                    idx++;
                    LInfo info2;
                    info2 = SwitchBlock(info1);
                    strncpy(Q[++qlen].op, "se", sizeof(Q[qlen].op) - 1);
                    strncpy(Q[qlen].arg1, "_", sizeof(Q[qlen].arg1) - 1);
                    strncpy(Q[qlen].arg2, "_", sizeof(Q[qlen].arg2) - 1);
                    strncpy(Q[qlen].result,"_",sizeof(Q[qlen].result)-1);
                    return info2;
                }
                else {
                    fprintf(fp2, "%s前缺少 )\n", w[idx].word); error++;
                    flag = false;
                    return info;
                }
            }
            else {
                fprintf(fp2, "%s前缺少 (\n", w[idx].word); error++;
                flag = false;
                return info;
            }
        }
        // <STMT> → break;
        else if(strcmp(w[idx].word, "break") == 0){
            strcat(tree[tlen], "break");
            idx++;
            if(strcmp(w[idx].word,";")==0) {
                strcat(tree[tlen], ";");
                strncpy(Q[++qlen].op, "break", sizeof(Q[qlen].op) - 1);
                strncpy(Q[qlen].arg1, info.Id.rel, sizeof(Q[qlen].arg1) - 1);
                strncpy(Q[qlen].arg2, "_", sizeof(Q[qlen].arg2) - 1);
                strncpy(Q[qlen].result,"_",sizeof(Q[qlen].result)-1);
                idx++;
            }
            else
            {
                fprintf(fp2, "%s前缺少 ;\n", w[idx].word); error++;
            }
            return info;
        }
        else if (strcmp(w[idx].word, "{") == 0) {//域
            strcat(tree[tlen], "<Block>");
            //idx++;
            LInfo info1;
            info1 = Block(info);
            if (!flag) return info;
            return info;
        }
        // <STMT> → return 数字;|return <Name>;
        else if (strcmp(w[idx].word, "return") == 0) {//return语句
            strcat(tree[tlen], "return");
            idx++;
            int tag;
            tag = Num();
            if (tag != -1)
            {
                strcat(tree[tlen], "<Num>;");
                strncpy(tree[++tlen], "<Num>→", sizeof(tree[tlen]) - 1);
                strcat(tree[tlen], w[idx].word);
                idx++;
                if (strcmp(w[idx].word, ";") == 0) {
                    strncpy(Q[++qlen].op, "ret", sizeof(Q[qlen].op) - 1);
                    strncpy(Q[qlen].arg1, w[idx-1].word, sizeof(Q[qlen].arg1) - 1);
                    strncpy(Q[qlen].arg2, "_", sizeof(Q[qlen].arg2) - 1);
                    strncpy(Q[qlen].result,"_",sizeof(Q[qlen].result)-1);
                    idx++;
                    return info;
                }
                else {
                    fprintf(fp2, "%s前缺少 ;\n", w[idx].word); error++;
                    flag = false;
                    return info;
                }
            }
            else if (Name())
            {
                strcat(tree[tlen], "<Name>;");
                strncpy(tree[++tlen], "<Name>→", sizeof(tree[tlen]) - 1);
                strcat(tree[tlen], w[idx].word);
                int nowIdAddr = findId(w[idx].word);
                if (nowIdAddr == -1)
                {
                    fprintf(fp2, "%s该标识符未定义", w[idx].word); error++;
                    flag = false; return info;
                }
                else
                {
                    idx++;
                    if (strcmp(w[idx].word, ";") == 0) {
                        strncpy(Q[++qlen].op, "ret", sizeof(Q[qlen].op) - 1);
                        strncpy(Q[qlen].arg1, w[idx-1].word, sizeof(Q[qlen].arg1) - 1);
                        strncpy(Q[qlen].arg2, "_", sizeof(Q[qlen].arg2) - 1);
                        strncpy(Q[qlen].result,"_",sizeof(Q[qlen].result)-1);
                        idx++;
                        return info;
                    }
                    else {
                        fprintf(fp2, "%s前缺少 ;\n", w[idx].word); error++;
                        flag = false;
                        return info;
                    }
                }
            }
            else {
                fprintf(fp2, "%s后缺少返回值\n", w[idx].word); error++;
                flag = false;
                return info;
            }
        }
        else {
            fprintf(fp2, "%s后缺少变量\n", w[idx].word); error++;
            flag = false;
            return info;
        }
    }
    return info;
}

LInfo STMT1(LInfo info) {
    if (flag) {
        strncpy(tree[++tlen], "<STMT1>→", sizeof(tree[tlen]) - 1);
        if (strcmp(w[idx].word, "else") == 0) {
            strcat(tree[tlen], "else<STMT>");
            idx++;
            strncpy(Q[++qlen].op, "el", sizeof(Q[qlen].op) - 1);
            strncpy(Q[qlen].arg1, "_", sizeof(Q[qlen].arg1) - 1);
            strncpy(Q[qlen].arg2, "_", sizeof(Q[qlen].arg2) - 1);
            strncpy(Q[qlen].result, "_", sizeof(Q[info.addr].result) - 1);
            strncpy(Q[++qlen].op, ("lb"+to_string(lbNum)).c_str(), sizeof(Q[qlen].op) - 1);
            strncpy(Q[qlen].arg1, "_", sizeof(Q[qlen].arg1) - 1);
            strncpy(Q[qlen].arg2, "_", sizeof(Q[qlen].arg2) - 1);
            strncpy(Q[qlen].result, "_", sizeof(Q[info.addr].result) - 1);
            LInfo info1;
            info1 = STMT(info);
            strncpy(Q[++qlen].op, "ie", sizeof(Q[qlen].op) - 1);
            strncpy(Q[qlen].arg1, "_", sizeof(Q[qlen].arg1) - 1);
            strncpy(Q[qlen].arg2, "_", sizeof(Q[qlen].arg2) - 1);
            strncpy(Q[qlen].result, "_", sizeof(Q[info.addr].result) - 1);
            return info;
        }
        else {
            strncpy(Q[++qlen].op, ("lb"+to_string(lbNum)).c_str(), sizeof(Q[qlen].op) - 1);
            strncpy(Q[qlen].arg1, "_", sizeof(Q[qlen].arg1) - 1);
            strncpy(Q[qlen].arg2, "_", sizeof(Q[qlen].arg2) - 1);
            strncpy(Q[qlen].result, "_", sizeof(Q[info.addr].result) - 1);
            strcat(tree[tlen], "empty");
            lbNum++;
            return info;
        }

    }
    return info;
}

// <ForRep>→<Rep>;<Rep2>;<Rep1>
LInfo ForRep(LInfo info)
{
    if (flag) {
        strncpy(tree[++tlen], "<ForRep>→<Rep>;<Rep2>;<Rep1>", sizeof(tree[tlen]) - 1);
        LInfo info3;
        info3 = Rep(info);
        if (!flag) return info;
        strncpy(Q[++qlen].op, "for", sizeof(Q[qlen].op) - 1);
        strncpy(Q[qlen].arg1, "_", sizeof(Q[qlen].arg1) - 1);
        strncpy(Q[qlen].arg2, "_", sizeof(Q[qlen].arg2) - 1);
        strncpy(Q[qlen].result,"_",sizeof(Q[qlen].result)-1);
        if (strcmp(w[idx].word, ";") == 0) {
            idx++;
            LInfo info1;
            info1 = Rep2(info);
            if (!flag)
                return info;
            if (strcmp(w[idx].word, ";") == 0) {
                idx++;
                LInfo info2;
                info2 = Rep1(info);
                info = info2;
                if (!flag)
                    return info;
            }
            else {
                fprintf(fp2, "%s后缺少 ;", w[idx].word); error++; flag = false;
                return info;
            }
        }
        else {
            fprintf(fp2, "%s后缺少 ;", w[idx].word); error++; flag = false;
            return info;
        }
    }
    return info;
}

// <Rep>→<Name>=<Expr>
LInfo Rep(LInfo info)
{
    if (flag)
    {
        strncpy(tree[++tlen], "<Rep>→", sizeof(tree[tlen]) - 1);
        if (Name())
        {
            strcat(tree[tlen], "<Name>=<Expr>");
            strncpy(tree[++tlen], "<Name>→", sizeof(tree[tlen]) - 1);
            strcat(tree[tlen], w[idx].word);
            if (findId(w[idx].word) == -1) {
                flag = false;
                fprintf(fp2, "%s该标识符未定义\r\n", w[idx].word);
                error++;
                return info;
            }
            else
                idx++;
            if (strcmp(w[idx].word, "=") == 0) {
                idx++;
                LInfo info1;
                info1 = Expr(info);
                strncpy(Q[++qlen].op, "=", sizeof(Q[qlen].op) - 1);
                strncpy(Q[qlen].arg1, info1.Id.word, sizeof(Q[qlen].arg1) - 1);
                strncpy(Q[qlen].arg2, "_", sizeof(Q[qlen].arg2) - 1);
                strncpy(Q[qlen].result, w[idx-3].word, sizeof(Q[qlen].result) - 1);
                if (!flag) return info;
            }
        }
        else {
            strcat(tree[tlen], "empty"); return info;
        }
    }
    return info;
}

LInfo Rep1(LInfo info)
{
    if (flag)
    {
        strncpy(tree[++tlen], "<Rep1>→", sizeof(tree[tlen]) - 1);
        if (Name())
        {
            strcat(tree[tlen], "<Name><RepOp2>");
            strncpy(tree[++tlen], "<Name>→", sizeof(tree[tlen]) - 1);
            strcat(tree[tlen], w[idx].word);
            if (findId(w[idx].word) == -1) {
                flag = false;
                fprintf(fp2, "%s该标识符未定义\r\n", w[idx].word); error++;
                return info;
            }
            else
                idx++;
            // ++ 或 --
            if (RelOp2()) {
                strncpy(info.Id.word, w[idx-1].word, sizeof(info.Id.word) - 1);
                strncpy(info.Id.rel, w[idx].word, sizeof(info.Id.rel) - 1);
                idx++;
                return info;
            }
            else {
                fprintf(fp2, "%s后缺少界符\r\n", w[idx].word); error++;
            }

        }
        else {
            strcat(tree[tlen], "empty"); return info;
        }
        return info;
    }
    return info;
}

// <Rep2>→<BOOL>
LInfo Rep2(LInfo info)
{
    if (flag)
    {
        strncpy(tree[++tlen], "<Rep2>→", sizeof(tree[tlen]) - 1);
        // 当前token类型是标识符、(、常量整数、常量小数
        if (w[idx].typenum == 33 || w[idx].typenum == 39 || w[idx].typenum == 34 || w[idx].typenum == 35)
        {
            strcat(tree[tlen], "<BOOL>");
            LInfo info1;
            info1 = BOOLL(info);
            return info;
        }
        else
        {
            strcat(tree[tlen], "empty");
            return info;
        }
    }
    return info;
}

bool RelOp2()
{
    if (strcmp(w[idx].word, "++") == 0 || strcmp(w[idx].word, "--") == 0)
    {
        strncpy(tree[++tlen], "<RepOp2>→", sizeof(tree[tlen]) - 1); strcat(tree[tlen], w[idx].word);
        return true;
    }
    return false;
}

// <BOOL1>→<Expr><RelOp><Expr>
LInfo BOOL1(LInfo info) {
    if (flag) {
        strncpy(tree[++tlen], "<BOOL1>→<Expr><RelOp><Expr>", sizeof(tree[tlen]) - 1);
        LInfo info1;
        info1 = Expr(info);
        if (!flag) return info;
        if (RelOp()) {  //若是关系界符
            char nowOp[20];
            strncpy(nowOp, w[idx].word, sizeof(nowOp) - 1);
            idx++;
            LInfo info2;
            info2 = Expr(info);
            if (!flag) return info;
            bool result;
            strncpy(Q[++qlen].op, nowOp, sizeof(Q[qlen].op) - 1);
            strncpy(Q[qlen].arg1, info1.Id.word, sizeof(Q[qlen].arg1) - 1);
            strncpy(Q[qlen].arg2, info2.Id.word, sizeof(Q[qlen].arg2) - 1);
            if (strcmp(nowOp, "<") == 0) {
                result = info1.Id.numValue < info2.Id.numValue;
                if (result == false)
                {
                    strncpy(Q[qlen].result, ("t"+to_string(regNum)).c_str(), sizeof(Q[qlen].result) - 1);
                    strncpy(info.Id.rel, "<", sizeof(info.Id.rel) - 1);
                }
                else
                {
                    strncpy(Q[qlen].result, ("t"+to_string(regNum)).c_str(), sizeof(Q[qlen].result) - 1);
                    strncpy(info.Id.rel, "<", sizeof(info.Id.rel) - 1);
                }
            }
            else if (strcmp(nowOp, "<=") == 0) {
                result = info1.Id.numValue <= info2.Id.numValue;
                if (result == false)
                {
                    strncpy(Q[qlen].result, ("t"+to_string(regNum)).c_str(), sizeof(Q[qlen].result) - 1);
                    strncpy(info.Id.rel, ("t"+to_string(regNum)).c_str(), sizeof(info.Id.rel) - 1);
                }
                else
                {
                    strncpy(Q[qlen].result, ("t"+to_string(regNum)).c_str(), sizeof(Q[qlen].result) - 1);
                    strncpy(info.Id.rel, ("t"+to_string(regNum)).c_str(), sizeof(info.Id.rel) - 1);
                }
            }
            else if (strcmp(nowOp, ">") == 0) {
                result = info1.Id.numValue > info2.Id.numValue;
                if (result == false)
                {
                    strncpy(Q[qlen].result, ("t"+to_string(regNum)).c_str(), sizeof(Q[qlen].result) - 1);
                    strncpy(info.Id.rel, ("t"+to_string(regNum)).c_str(), sizeof(info.Id.rel) - 1);
                }
                else
                {
                    strncpy(Q[qlen].result, ("t"+to_string(regNum)).c_str(), sizeof(Q[qlen].result) - 1);
                    strncpy(info.Id.rel, ("t"+to_string(regNum)).c_str(), sizeof(info.Id.rel) - 1);
                }
            }
            else if (strcmp(nowOp, ">=") == 0) {
                result = info1.Id.numValue >= info2.Id.numValue;
                if (result == false)
                {
                    strncpy(Q[qlen].result, ("t"+to_string(regNum)).c_str(), sizeof(Q[qlen].result) - 1);
                    strncpy(info.Id.rel, ("t"+to_string(regNum)).c_str(), sizeof(info.Id.rel) - 1);
                }
                else
                {
                    strncpy(Q[qlen].result, ("t"+to_string(regNum)).c_str(), sizeof(Q[qlen].result) - 1);
                    strncpy(info.Id.rel, ("t"+to_string(regNum)).c_str(), sizeof(info.Id.rel) - 1);
                }
            }
            else if (strcmp(nowOp, "==") == 0) {
                result = info1.Id.numValue == info2.Id.numValue;
                if (result == false)
                {
                    strncpy(Q[qlen].result, ("t"+to_string(regNum)).c_str(), sizeof(Q[qlen].result) - 1);
                    strncpy(info.Id.rel, ("t"+to_string(regNum)).c_str(), sizeof(info.Id.rel) - 1);
                }
                else
                {
                    strncpy(Q[qlen].result, ("t"+to_string(regNum)).c_str(), sizeof(Q[qlen].result) - 1);
                    strncpy(info.Id.rel, ("t"+to_string(regNum)).c_str(), sizeof(info.Id.rel) - 1);
                }
            }
            else if (strcmp(nowOp, "!=") == 0) {
                result = info1.Id.numValue != info2.Id.numValue;
                if (result == false)
                {
                    strncpy(Q[qlen].result, ("t"+to_string(regNum)).c_str(), sizeof(Q[qlen].result) - 1);
                    strncpy(info.Id.rel, ("t"+to_string(regNum)).c_str(), sizeof(info.Id.rel) - 1);
                }
                else
                {
                    strncpy(Q[qlen].result, ("t"+to_string(regNum)).c_str(), sizeof(Q[qlen].result) - 1);
                    strncpy(info.Id.rel, ("t"+to_string(regNum)).c_str(), sizeof(info.Id.rel) - 1);
                }
            }
            return info;
        }
        else {
            fprintf(fp2, "%s后缺少界符\r\n", w[idx].word); error++;
            flag = false;
            return info;
        }
        return info;
    }
    return info;
}

//<BOOL2>→<RelOp1><BOOL1>
LInfo BOOL2(LInfo info)
{
    if (flag) {
        strncpy(tree[++tlen], "<BOOL2>→", sizeof(tree[tlen]) - 1);
        if (RelOp1()) {
            char nowOp[20];
            strncpy(nowOp, w[idx].word, sizeof(nowOp) - 1);
            strcat(tree[tlen], "<RelOp1>");
            strncpy(tree[++tlen], "<RelOp1>→", sizeof(tree[tlen]) - 1);
            strcat(tree[tlen], w[idx].word);
            idx++;
            LInfo info1;
            info1 = BOOL1(info);
            if (!flag) return info;
            bool result;
            strncpy(Q[++qlen].op, nowOp, sizeof(Q[qlen].op) - 1);
            strncpy(Q[qlen].arg1, info.Id.rel, sizeof(Q[qlen].arg1) - 1);
            strncpy(Q[qlen].arg2, info1.Id.rel, sizeof(Q[qlen].arg2) - 1);
            if (strcmp(nowOp, "||") == 0)
            {
                result = info.Id.rel == info1.Id.rel;
                if (result == true)
                {
                    strncpy(Q[qlen].result, info1.Id.rel, sizeof(Q[qlen].result) - 1);
                    strncpy(info.Id.rel, info1.Id.rel, sizeof(info.Id.rel) - 1);
                }
                else
                {
                    strncpy(Q[qlen].result, "1", sizeof(Q[qlen].result) - 1);
                    strncpy(info.Id.rel, "1", sizeof(info.Id.rel) - 1);
                }
            }
            else//&&
            {
                result = info.Id.rel == info1.Id.rel;
                if (result == false)
                {
                    strncpy(Q[qlen].result, info1.Id.rel, sizeof(Q[qlen].result) - 1);
                    strncpy(info.Id.rel, info1.Id.rel, sizeof(info.Id.rel) - 1);
                }
                else
                {
                    strncpy(Q[qlen].result, "0", sizeof(Q[qlen].result) - 1);
                    strncpy(info.Id.rel, "0", sizeof(info.Id.rel) - 1);
                }
            }
            return info;
        }
        else
        {
            strcat(tree[tlen], "empty"); return info;
        }
        return info;
    }
    return info;
}

bool RelOp1()
{
    if (strcmp(w[idx].word, "||") == 0 || strcmp(w[idx].word, "&&") == 0)
    {
        return true;
    }
    else return false;
}

LInfo BOOLL(LInfo info)
{
    if (flag) {
        strncpy(tree[++tlen], "<BOOL>→<BOOL1><BOOL2>", sizeof(tree[tlen]) - 1);
        LInfo info1;
        info1 = BOOL1(info);
        if (!flag) return info;
        info.Id = info1.Id;
        LInfo info2;
        info2 = BOOL2(info);
        if (!flag) return info;
        info.Id = info2.Id;
        return info;
    }
    return info;
}

bool RelOp() {
    if (strcmp(w[idx].word, "<") == 0 ||
        strcmp(w[idx].word, "<=") == 0 ||
        strcmp(w[idx].word, ">") == 0 ||
        strcmp(w[idx].word, ">=") == 0 ||
        strcmp(w[idx].word, "==") == 0 ||
        strcmp(w[idx].word, "!=") == 0)
    {
        strncpy(tree[++tlen], "<RelOp>→", sizeof(tree[tlen]) - 1);
        strcat(tree[tlen], w[idx].word);
        return true;
    }
    return false;
}

// <SwitchBlock> → { <Cases> }
LInfo SwitchBlock(LInfo info) {
    if (flag) {
        strncpy(tree[++tlen], "<SwitchBlock>→{<Cases>}", sizeof(tree[tlen]) - 1);
        if(strcmp(w[idx++].word,"{")==0){
            LInfo info1;
            info1 = Cases(info);
            if(!flag) return info;
            info.Id = info1.Id;
            if(strcmp(w[idx].word,"}")==0){
                idx++;
                return info;
            }
            else {
                fprintf(fp2,"%s之前缺少 }\r\n",w[idx].word); error++;
                flag = false;
                return info;
            }
        }
        else {
            fprintf(fp2,"%s之前缺少 {\r\n",w[idx].word); error++;
            flag = false;
            return info;
        }
    }
    return info;
}

// <Cases> → <Case> <Cases> | <Default> | empty
LInfo Cases(LInfo info) {
    if (flag) {
        strncpy(tree[++tlen], "<Cases>→", sizeof(tree[tlen]) - 1);
        if(strcmp(w[idx].word,"case")==0){
            strncat(tree[tlen], "<Case><Cases>", sizeof(tree[tlen]) - 1);
            LInfo info1;
            info1 = Case(info);
            if(!flag) return info;
            LInfo info2;
            info2 = Cases(info);
            return info2;
        }
        else if(strcmp(w[idx].word,"default")==0) {
            strncat(tree[tlen], "<Default>", sizeof(tree[tlen]) - 1);
            LInfo info1;
            info1 = Default(info);
            if(!flag) return info;
            info = info1;
        }
        else
        {
            strncat(tree[tlen], "empty", sizeof(tree[tlen]) - 1);
            strncpy(info.Id.word,"empty",sizeof(info.Id.word)-1);
            return info;
        }
    }
    return info;
}

// <Case> → case 数字 : <STMTS>
LInfo Case(LInfo info) {
    if (flag) {
        strncpy(tree[++tlen], "<Case>→case <Num> : <STMTS>", sizeof(tree[tlen]) - 1);
        if(strcmp(w[idx].word,"case")==0){
            idx++;
            if(Num()!=-1) {
                strncpy(tree[++tlen], "<Num>->", sizeof(tree[tlen]) - 1);
                strcat(tree[tlen],w[idx].word);
                strncpy(Q[++qlen].op, "case", sizeof(Q[qlen].op) - 1);
                strncpy(Q[qlen].arg1, info.Id.word, sizeof(Q[qlen].arg1) - 1);
                strncpy(Q[qlen].arg2, w[idx].word, sizeof(Q[qlen].arg2) - 1);
                strncpy(Q[qlen].result,"_",sizeof(Q[qlen].result)-1);
                idx++;
                if(strcmp(w[idx].word,":")==0){
                    idx++;
                    LInfo info1;
                    info1 = STMTS(info);
                    if(!flag) return info;
                    info = info1;
                }
                else{
                    fprintf(fp2,"%s之前缺少 :\r\n",w[idx].word); error++;
                    flag = false;
                    return info;
                }
            }
            else{
                fprintf(fp2,"%s之前缺少 数字\r\n",w[idx].word); error++;
                flag = false;
                return info;
            }
        }
        else
        {
            fprintf(fp2,"%s之前缺少 case\r\n",w[idx].word); error++;
            flag = false;
            return info;
        }
        return info;
    }
    return info;
}

// <Default> → default : <STMTS>
LInfo Default(LInfo info) {
    if (flag) {
        strncpy(tree[++tlen], "<Default>→default: <STMTS>", sizeof(tree[tlen]) - 1);
        if(strcmp(w[idx].word,"default")==0)
        {
            idx++;
            strncpy(Q[++qlen].op, "default", sizeof(Q[qlen].op) - 1);
            strncpy(Q[qlen].arg1, "_", sizeof(Q[qlen].arg1) - 1);
            strncpy(Q[qlen].arg2, "_", sizeof(Q[qlen].arg2) - 1);
            strncpy(Q[qlen].result,"_",sizeof(Q[qlen].result)-1);
            if(strcmp(w[idx].word,":")==0)
            {
                idx++;
                LInfo info1 = STMTS(info);
                if(!flag) return info;
                info.Id = info1.Id;
                return info;
            }
            else {
                fprintf(fp2,"%s之前缺少 :\r\n",w[idx].word); error++;
                flag = false;
                return info;
            }
        }
        else {
            fprintf(fp2,"%s之前缺少 default\r\n",w[idx].word); error++;
            flag = false;
            return info;
        }
    }
    return info;
}

// 表达式<Expr>→<Term><Expr1>
LInfo Expr(LInfo info) {
    if (flag) {
        strncpy(tree[++tlen], "<Expr>→<Term><Expr1>", sizeof(tree[tlen]) - 1);
        LInfo info1 = Term(info);
        if (!flag) return info;
        info.Id = info1.Id;
        LInfo info2 = Expr1(info);
        if (!flag) return info;
        info.Id = info2.Id;
        return info;
    }
    return info;
}

// <Expr1>→<AddOp><Term><Expr1>
LInfo Expr1(LInfo info) {
    if (flag) {

        strncpy(tree[++tlen], "<Expr1>→", sizeof(tree[tlen]) - 1);
        if (AddOp()) {
            strcat(tree[tlen], "<AddOp><Term><Expr1>");
            strncpy(tree[++tlen], "<AddOp>→", sizeof(tree[tlen]) - 1);
            strcat(tree[tlen], w[idx].word);
            char nowOp[20];
            strncpy(nowOp, w[idx].word, sizeof(nowOp) - 1);  //若字符为+或-
            idx++;
            LInfo info1 = Term(info);
            if (!flag) return info;
            float result;
            if (strcmp(nowOp, "-") == 0) {
                result = info.Id.numValue - info1.Id.numValue;
                strncpy(Q[++qlen].op, "-", sizeof(Q[qlen].op) - 1);
                strncpy(Q[qlen].arg1, info.Id.word, sizeof(Q[qlen].arg1) - 1);
                strncpy(Q[qlen].arg2, info1.Id.word, sizeof(Q[qlen].arg2) - 1);
                // strncpy(Q[qlen].arg1, to_string(info.Id.numValue).c_str(), sizeof(Q[qlen].arg1) - 1);
                // strncpy(Q[qlen].arg2, to_string(info1.Id.numValue).c_str(), sizeof(Q[qlen].arg2) - 1);
                strncpy(Q[qlen].result, ("t"+to_string(regNum)).c_str(), sizeof(Q[qlen].result) - 1);
                // strncpy(Q[++qlen].op, "=", sizeof(Q[qlen].op) - 1);
                // strncpy(Q[qlen].arg1, ("t"+to_string(regNum)).c_str(), sizeof(Q[qlen].arg1) - 1);
                // strncpy(Q[qlen].arg2, "_", sizeof(Q[qlen].arg2) - 1);
                // strncpy(Q[qlen].result, to_string(result).c_str(), sizeof(Q[qlen].result) - 1);
                info.Id.numValue = result;
                strncpy(info.Id.word, ("t"+to_string(regNum)).c_str(), sizeof(info.Id.word) - 1);
            }
            else if (strcmp(nowOp, "+") == 0) {
                result = info.Id.numValue + info1.Id.numValue;
                strncpy(Q[++qlen].op, "+", sizeof(Q[qlen].op) - 1);
                strncpy(Q[qlen].arg1, info.Id.word, sizeof(Q[qlen].arg1) - 1);
                strncpy(Q[qlen].arg2, info1.Id.word, sizeof(Q[qlen].arg2) - 1);
                // strncpy(Q[qlen].arg1, to_string(info.Id.numValue).c_str(), sizeof(Q[qlen].arg1) - 1);
                //strncpy(Q[qlen].arg2, to_string(info1.Id.numValue).c_str(), sizeof(Q[qlen].arg2) - 1);
                strncpy(Q[qlen].result, ("t"+to_string(regNum)).c_str(), sizeof(Q[qlen].result) - 1);
                info.Id.numValue = result;
                strncpy(info.Id.word, Q[qlen].result, sizeof(info.Id.word) - 1);
            }
            LInfo info2 = Expr1(info);
            if (!flag) return info;
            info.Id = info2.Id;

            return info;
        }
        else
        {
            strcat(tree[tlen], "empty"); return info;
        }

        return info;
    }
    return info;
}

LInfo Term(LInfo info) {
    if (flag) {
        regNum++;
        strncpy(tree[++tlen], "<Term>→<Factor><Term1>", sizeof(tree[tlen]) - 1);
        LInfo info1 = Factor(info);
        if (!flag) return info;
        info.Id = info1.Id;
        LInfo info2 = Term1(info);
        if (!flag || strcmp(info2.Id.word,"empty")==0 )
        {
            regNum--;
            return info;
        }

        info.Id = info2.Id;
        return info;
    }
    return info;
}

LInfo Term1(LInfo info) {
    if (flag) {
        strncpy(tree[++tlen], "<Term1>→", sizeof(tree[tlen]) - 1);
        if (MulOp()) {  //若字符为*或/
            strcat(tree[tlen], "<MulOp><Factor><Term1>");
            strncpy(tree[++tlen], "<MulOp>→", sizeof(tree[tlen]) - 1);
            strcat(tree[tlen], w[idx].word);
            char nowOp[20];
            strncpy(nowOp, w[idx].word, sizeof(nowOp) - 1);
            idx++;
            LInfo info1 = Factor(info);
            if (!flag) return info;
            float result;
            if (strcmp(nowOp, "*") == 0) {
                result = info.Id.numValue * info1.Id.numValue;
                strncpy(Q[++qlen].op, "*", sizeof(Q[qlen].op) - 1);
                strncpy(Q[qlen].arg1, info.Id.word, sizeof(Q[qlen].arg1) - 1);
                strncpy(Q[qlen].arg2, info1.Id.word, sizeof(Q[qlen].arg2) - 1);
                // strncpy(Q[qlen].arg1, to_string(info.Id.numValue).c_str(), sizeof(Q[qlen].arg1) - 1);
                // strncpy(Q[qlen].arg2, to_string(info1.Id.numValue).c_str(), sizeof(Q[qlen].arg2) - 1);
                strncpy(Q[qlen].result, ("t"+to_string(regNum)).c_str(), sizeof(Q[qlen].result) - 1);
                info.Id.numValue = result;
                strncpy(info.Id.word, ("t"+to_string(regNum)).c_str(), sizeof(info.Id.word) - 1);
            }
            else if (strcmp(nowOp, "/") == 0) {
                result = info.Id.numValue / info1.Id.numValue;
                strncpy(Q[++qlen].op, "/", sizeof(Q[qlen].op) - 1);
                strncpy(Q[qlen].arg1, info.Id.word, sizeof(Q[qlen].arg1) - 1);
                strncpy(Q[qlen].arg2, info1.Id.word, sizeof(Q[qlen].arg2) - 1);

                // strncpy(Q[qlen].arg1, to_string(info.Id.numValue).c_str(), sizeof(Q[qlen].arg1) - 1);
                // strncpy(Q[qlen].arg2, to_string(info1.Id.numValue).c_str(), sizeof(Q[qlen].arg2) - 1);
                strncpy(Q[qlen].result, ("t"+to_string(regNum)).c_str(), sizeof(Q[qlen].result) - 1);
                info.Id.numValue = result;
                strncpy(info.Id.word, ("t"+to_string(regNum)).c_str(), sizeof(info.Id.word) - 1);
            }
            else if (strcmp(nowOp, "%") == 0) {
                result = (int)info.Id.numValue % (int)info1.Id.numValue;
                strncpy(Q[++qlen].op, "%", sizeof(Q[qlen].op) - 1);
                strncpy(Q[qlen].arg1, info.Id.word, sizeof(Q[qlen].arg1) - 1);
                strncpy(Q[qlen].arg2, info1.Id.word, sizeof(Q[qlen].arg2) - 1);
                // strncpy(Q[qlen].arg1, to_string(info.Id.numValue).c_str(), sizeof(Q[qlen].arg1) - 1);
                // strncpy(Q[qlen].arg2, to_string(info1.Id.numValue).c_str(), sizeof(Q[qlen].arg2) - 1);
                strncpy(Q[qlen].result, ("t"+to_string(regNum)).c_str(), sizeof(Q[qlen].result) - 1);
                info.Id.numValue = result;
                strncpy(info.Id.word, ("t"+to_string(regNum)).c_str(), sizeof(info.Id.word) - 1);
            }
            LInfo info2 = Term1(info);
            if (!flag) return info;
            info.Id = info2.Id;
            strncpy(info.Id.word,Q[qlen].result, sizeof(info.Id.word) - 1);
            return info;
        }
        else
        {
            strcat(tree[tlen], "empty");
            strncpy(info.Id.word,"empty",sizeof(info.Id.word));
            return info;
        }
    }
    return info;
}

// <Factor> → <Name> | number | ( <Expr> )
LInfo Factor(LInfo info) {
    if (flag) {
        strncpy(tree[++tlen], "<Factor>→", sizeof(tree[tlen]) - 1);
        if (Name()) {
            strcat(tree[tlen], "<Name>");
            strncpy(tree[++tlen], "<Name>→", sizeof(tree[tlen]) - 1);
            strcat(tree[tlen], w[idx].word);
            int nowIdAddr = findId(w[idx].word);
            if (nowIdAddr == -1) {
                flag = false;
                fprintf(fp2, "%s该标识符未定义\r\n", w[idx].word); error++;
                return info;
            }
            info.Id.numValue = IDs[nowIdAddr].numValue;
            strncpy(info.Id.word, IDs[nowIdAddr].word, sizeof(info.Id.word) - 1);
            strncpy(info.Id.type, IDs[nowIdAddr].type, sizeof(info.Id.type) - 1);
            idx++;
            return info;
        }
        else if (Num() != -1)
        {
            strcat(tree[tlen], "<Num>");
            strncpy(tree[++tlen], "<Num>→", sizeof(tree[tlen]) - 1);
            strcat(tree[tlen], w[idx].word);
            LInfo info1;
            info1.Id.numValue = stof(w[idx].word);
            strncpy(info1.Id.word,w[idx].word,sizeof(info1.Id.word)-1);
            info.Id = info1.Id;
            idx++;
            return info;
        }
        else if (strcmp(w[idx].word, "(") == 0) {
            strcat(tree[tlen], "(<Expr>)");
            idx++;
            LInfo info1 = Expr(info);
            if (!flag) return info;
            if (strcmp(w[idx].word, ")") == 0) {
                info.Id = info1.Id;
                idx++;
                return info;
            }
            else {  //若一直没有),则说明该算术表达式错误
                fprintf(fp2, "%s后缺少 )\r\n", w[idx].word);
                error++;
                flag = false;
                return info;
            }
        }
        else {
            fprintf(fp2, "%s后缺少变量\r\n", w[idx].word);
            error++;
            flag = false;
            return info;
        }
        return info;
    }
    return info;
}

// 判断当前token种别码是否是数，如果是，返回该种别码
int Num()
{
    // 34:整数 35:小数 36:字符常量ASCLL  67:科学计数法 68:16进制 69:8进制
    if ((w[idx].typenum >= 34 && w[idx].typenum <= 36) || (w[idx].typenum >= 67 && w[idx].typenum <= 69))
    {
        return w[idx].typenum;
    }
    return -1;
}

// 判断当前token种别码是否是+,-
bool AddOp() {
    if (strcmp(w[idx].word, "+") == 0 || strcmp(w[idx].word, "-") == 0)
    {
        return true;
    }
    return false;
}

// 判断当前token种别码是否是*,/
bool MulOp() {
    if (strcmp(w[idx].word, "*") == 0 || strcmp(w[idx].word, "/") == 0 || strcmp(w[idx].word, "%") == 0)
    {
        return true;
    }
    return false;
}

// 语法分析
void Grammer()
{
    idx = 0;
    error = 0;
    flag = true;
    LInfo info;
    fp2 = fopen("D://d3.txt", "w");
    info = Prog(info);
    fprintf(fp2, "\r\n语法分析完毕！\r\n");
    fprintf(fp2, "%d error(s), %d warning(s)\r\n", error, warning);
    fclose(fp2);
}

// 产生递归下降语法分析过程
void GraAnalyer()
{
    int i;
    tlen=-1;
    nlen=-1;
    qlen=-1;
    len=-1;
    idx=0;
    error=0;
    Grammer();

    fp2=fopen("D://d4.txt","w");
    for(i=0;i<=tlen;i++)
    {
        fprintf(fp2,"%d:\t\t%s\r\n",i+1,tree[i]);
    }
    fclose(fp2);
    FILE *f1;
    static char buf[200];
    f1=fopen("D://d3.txt","r");
    for(i=0;fgets(buf,sizeof(buf),f1)!=NULL;i++)
    {
        fprintf(fp2,"%s",buf);
    }
    fclose(f1);
    fclose(fp2);
}

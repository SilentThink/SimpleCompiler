#include "emurunning.h"


// 执行四元式并返回return表示符的值
string emuRunning()
{
    float* t = new float[100];
    memset(t, 0, 100 * sizeof(float)); // 初始化为 0
    for (int i = 0; i <= qlen; i++)
    {
        Quaternion q = Q[i];
        if (strcmp(q.op, "=") == 0)
        {
            int idx = findId(q.result);
            if (idx != -1)
            {
                int srcIdx = findId(q.arg1);
                if (srcIdx != -1)
                {
                    IDs[idx].numValue = IDs[srcIdx].numValue;
                }
                else if (strncmp(q.arg1, "t",1) == 0)
                {
                    int tIdx = q.arg1[1] - '0';
                    IDs[idx].numValue = t[tIdx];
                }
                else
                {
                    IDs[idx].numValue = atof(q.arg1); // 处理直接赋值数字的情况
                }
            }
        }
        else if (strcmp(q.op, ">") == 0)
        {
            int idx1 = findId(q.arg1);
            int idx2 = findId(q.arg2);
            float num1 = (idx1==-1?atof(q.arg1):IDs[idx1].numValue);
            float num2 = (idx2==-1?atof(q.arg2):IDs[idx2].numValue);
            int tIdx=-1;
            if (strncmp(q.result, "t",1) == 0)
            {
                tIdx = q.result[1] - '0';
            }
            t[tIdx] = num1>num2?1:0;
        }
        else if (strcmp(q.op, "<") == 0)
        {
            int idx1 = findId(q.arg1);
            int idx2 = findId(q.arg2);
            float num1 = (idx1==-1?atof(q.arg1):IDs[idx1].numValue);
            float num2 = (idx2==-1?atof(q.arg2):IDs[idx2].numValue);
            int tIdx=-1;
            if (strncmp(q.result, "t",1) == 0)
            {
                tIdx = q.result[1] - '0';
            }
            t[tIdx] = num1<num2?1:0;
        }
        else if (strcmp(q.op,"==")==0)
        {
            int idx1 = findId(q.arg1);
            int idx2 = findId(q.arg2);
            float num1 = (idx1==-1?atof(q.arg1):IDs[idx1].numValue);
            float num2 = (idx2==-1?atof(q.arg2):IDs[idx2].numValue);
            int tIdx=-1;
            if (strncmp(q.result, "t",1) == 0)
            {
                tIdx = q.result[1] - '0';
            }
            t[tIdx] = (num1==num2)?1:0;
        }
        else if (strcmp(q.op,">=")==0)
        {
            int idx1 = findId(q.arg1);
            int idx2 = findId(q.arg2);
            float num1 = (idx1==-1?atof(q.arg1):IDs[idx1].numValue);
            float num2 = (idx2==-1?atof(q.arg2):IDs[idx2].numValue);
            int tIdx=-1;
            if (strncmp(q.result, "t",1) == 0)
            {
                tIdx = q.result[1] - '0';
            }
            t[tIdx] = (num1>=num2)?1:0;
        }
        else if (strcmp(q.op,"<=")==0)
        {
            int idx1 = findId(q.arg1);
            int idx2 = findId(q.arg2);
            float num1 = (idx1==-1?atof(q.arg1):IDs[idx1].numValue);
            float num2 = (idx2==-1?atof(q.arg2):IDs[idx2].numValue);
            int tIdx=-1;
            if (strncmp(q.result, "t",1) == 0)
            {
                tIdx = q.result[1] - '0';
            }
            t[tIdx] = (num1<=num2)?1:0;
        }
        else if (strcmp(q.op, "dow") == 0)
        {
            int tIdx=-1;
            if (strncmp(q.arg1, "t",1) == 0)
            {
                tIdx = q.arg1[1];
            }
            if (t[tIdx] == 0)
            {
                while (strcmp(Q[i].op, "we") != 0)
                {
                    i++;
                }
            }
        }
        else if (strcmp(q.op, "if") == 0)
        {
            int tIdx=-1;
            if (strncmp(q.arg1, "t",1) == 0)
            {
                tIdx = q.arg1[1] - '0';
            }
            if (t[tIdx] == 0)
            {
                while (strncmp(Q[i].op, "lb",2) != 0)
                {
                    i++;
                }
            }
        }
        else if (strcmp(q.op, "el") == 0)
        {
            while (strcmp(Q[i].op, "ie") != 0)
            {
                i++;
            }
        }
        else if (strcmp(q.op, "--") == 0)
        {
            int idx = findId(q.arg1);
            if (idx != -1)
            {
                IDs[idx].numValue -= 1;
            }
        }
        else if (strcmp(q.op, "++") == 0)
        {
            int idx = findId(q.arg1);
            if (idx != -1)
            {
                IDs[idx].numValue += 1;
            }
        }
        else if (strcmp(q.op, "we") == 0)
        {
            int j = i;
            while (j >= 0 && strcmp(Q[j].op, "wh") != 0)
            {
                j--;
            }
            if (j >= 0)
            {
                i = j - 1;
            }
        }
        else if (strcmp(q.op, "*") == 0)
        {
            int tIdx=0;
            float num1,num2;
            int idx1 = findId(q.arg1);
            if(idx1!=-1)
            {
                num1 = IDs[idx1].numValue;
            }
            else if(strncmp(q.arg1, "t",1) == 0)
            {
                tIdx = q.arg1[1]-'0';
                num1 = t[tIdx];
            }
            else
            {
                num1 = atof(q.arg1);
            }
            int idx2 = findId(q.arg2);
            if(idx2!=-1)
            {
                num2 = IDs[idx2].numValue;
            }
            else if(strncmp(q.arg2, "t",1) == 0)
            {
                tIdx = q.arg2[1]-'0';
                num2= t[tIdx];
            }
            else
            {
                num2 = atof(q.arg2);
            }
            if (strncmp(q.result, "t",1) == 0)
            {
                tIdx = q.result[1]-'0';
            }
            t[tIdx] = num1*num2;
        }
        else if (strcmp(q.op, "+") == 0)
        {
            int tIdx=0;
            float num1,num2;
            int idx1 = findId(q.arg1);
            if(idx1!=-1)
            {
                num1 = IDs[idx1].numValue;
            }
            else if(strncmp(q.arg1, "t",1) == 0)
            {
                tIdx = q.arg1[1] - '0';
                num1 = t[tIdx];
            }
            else
            {
                num1 = atof(q.arg1);
            }
            int idx2 = findId(q.arg2);
            if(idx2!=-1)
            {
                num2 = IDs[idx2].numValue;
            }
            else if(strncmp(q.arg2, "t",1) == 0)
            {
                tIdx = q.arg2[1] - '0';
                num2= t[tIdx];
            }
            else
            {
                num2 = atof(q.arg2);
            }

            if (strncmp(q.result, "t",1) == 0)
            {
                tIdx = q.result[1] - '0';
            }
            t[tIdx] = num1+num2;
        }
        else if (strcmp(q.op, "ret") == 0)
        {
            int idx = findId(q.arg1);
            if (idx != -1) {
                delete[] t;
                return std::to_string(IDs[idx].numValue);
            } else {
                delete[] t;
                return q.arg1;
            }
        }
        else if (strcmp(q.op, "for") == 0) {
            continue;
        }
        else if (strcmp(q.op, "dof") == 0) {
            int tIdx = -1;
            if (strncmp(q.arg1, "t", 1) == 0) {
                tIdx = q.arg1[1] - '0';
            }
            if (t[tIdx] == 0) {
                while (strcmp(Q[i].op, "fe") != 0) {
                    i++;
                }
            }
        }
        else if (strcmp(q.op, "fe") == 0) {
            int j = i;
            while (j >= 0 && strcmp(Q[j].op, "for") != 0) {
                j--;
            }
            if (j >= 0) {
                i = j - 1;
            }
        }
        else if (strcmp(q.op, "sw") == 0) {
            int switchValueIdx = findId(q.arg1);
            float switchValue = (switchValueIdx == -1 ? atof(q.arg1) : IDs[switchValueIdx].numValue);
            bool caseMatched = false;
            for (int j = i + 1; j <= qlen; j++) {
                if (strcmp(Q[j].op, "case") == 0) {
                    int caseValueIdx = findId(Q[j].arg2);
                    float caseValue = (caseValueIdx == -1 ? atof(Q[j].arg2) : IDs[caseValueIdx].numValue);
                    if (switchValue == caseValue) {
                        i = j;
                        caseMatched = true;
                        break;
                    }
                } else if (strcmp(Q[j].op, "default") == 0) {
                    if (!caseMatched) {
                        i = j;
                    }
                    break;
                }
            }
        }
        else if(strcmp(q.op,"break") == 0) {
            if(strcmp(q.arg1,"WHILE")==0)
            {
                while (strcmp(Q[i].op, "we") != 0) {
                    i++;
                }
            }
            else if(strcmp(q.arg1,"FOR")==0)
            {
                while (strcmp(Q[i].op, "fe") != 0) {
                    i++;
                }
            }
            else if(strcmp(q.arg1,"SWITCH")==0)
            {
                while (strcmp(Q[i].op, "se") != 0) {
                    i++;
                }
            }
        }

    }
    return "";
}


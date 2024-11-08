#include "optimize.h"

bool AllisNum(const char* str)
{
    for (int i = 0; i < strlen(str); i++)
    {
        int tmp = (int)str[i];
        if (tmp >= 48 && tmp <= 57)
        {
            continue;
        }
        else
        {
            return false;
        }
    }
    return true;
}

// 常值表达式节省
void constantExpressionSavings() {
    vector<Quaternion> preQ;//创建四元式容器，便于进行查找删除等操作

    bool whetherchangeflag=false;
    int res_num;
    int res_num2;
    char temp_result[20];
    bool temp_flag=false;
    for (int i = 0; i <= qlen; ++i) {  //常值表达式节省
        Quaternion& q = Q[i];  //初始化
        int arg1=atoi(q.arg1);
        int arg2=atoi(q.arg2);
        char pre_result[20];
        if(i>0){
            strcpy(pre_result,Q[i-1].result);
        }

        if(whetherchangeflag){  //由于上一个四元式，需要对当前四元式进行改变
            if(temp_flag){
                if(strcmp(q.arg1,temp_result)==0){
                    arg1=res_num2;
                    _itoa(arg1,q.arg1,10);
                    cout<<q.arg1;
                }
                else if(strcmp(q.arg2,temp_result)==0){
                    arg2=res_num2;
                    _itoa(arg2,q.arg2,10);
                    cout<<q.arg2;
                }
                temp_flag=false;
            }


            if(strcmp(q.arg1,pre_result)==0){
                _itoa(res_num,q.arg1,10);
                arg1=res_num;
            }
            else if(strcmp(q.arg2,pre_result)==0){
                _itoa(res_num,q.arg2,10);
                arg2=res_num;
            }
            else {
                strcpy(temp_result,pre_result);
                res_num2=res_num;
                cout<<temp_result;
                temp_flag=true;
            }
        }

        if(AllisNum(q.arg1)&&AllisNum(q.arg2)){
            whetherchangeflag=true;
            if(strcmp(q.op, "+") == 0){
                res_num=arg1+arg2;
            }
            else if(strcmp(q.op, "-") == 0){
                res_num=arg1-arg2;
            }
            else if(strcmp(q.op, "/") == 0){
                res_num=arg1/arg2;
            }
            else if(strcmp(q.op, "*") == 0){
                res_num=arg1*arg2;
            }
        }
        else{
            whetherchangeflag=false;
            preQ.push_back(Q[i]);
        }
    }
    qlen = preQ.size()-1;
    for (int i = 0; i <= qlen+1; ++i) {
        Q[i] = preQ[i];
    }

}

// 去除无用赋值
void removeUselessAssign()
{
    unordered_map<string, int> lastAssignmentIndex;
    unordered_set<int> toRemove;
    vector<Quaternion> optimizedQ;
    bool inConditionalBlock = false;

    for (int i = 0; i <= qlen; ++i) {
        Quaternion& q = Q[i];

        if (strcmp(q.op, "if") == 0 || strcmp(q.op, "el") == 0 || strcmp(q.op, "ie") == 0) {
            inConditionalBlock = strcmp(q.op, "ie") != 0; // if 或 el 设置为 true，ie 设置为 false
            optimizedQ.push_back(q);
            continue;
        }

        if (strcmp(q.op, "wh") == 0 || strcmp(q.op, "dow") == 0 || strcmp(q.op, "we") == 0) {
            inConditionalBlock = strcmp(q.op, "we") != 0;
            optimizedQ.push_back(q);
            continue;
        }

        if (strcmp(q.op, "for") == 0 || strcmp(q.op, "dof") == 0 || strcmp(q.op, "fe") == 0) {
            inConditionalBlock = strcmp(q.op, "fe") != 0;
            optimizedQ.push_back(q);
            continue;
        }

        if (strcmp(q.op, "sw") == 0 || strcmp(q.op, "case") == 0 || strcmp(q.op, "default") == 0 || strcmp(q.op, "se") == 0) {
            inConditionalBlock = strcmp(q.op, "se") != 0;
            optimizedQ.push_back(q);
            continue;
        }

        if (strcmp(q.op, "=") == 0) {
            string result = q.result;

            if (!inConditionalBlock && lastAssignmentIndex.find(result) != lastAssignmentIndex.end()) {
                toRemove.insert(lastAssignmentIndex[result]);
            }
            lastAssignmentIndex[result] = i;
        }
        optimizedQ.push_back(q);
    }

    // 去除多余的赋值语句
    vector<Quaternion> finalQ;
    for (int i = 0; i < optimizedQ.size(); ++i) {
        if (toRemove.find(i) == toRemove.end()) {
            finalQ.push_back(optimizedQ[i]);
        }
    }

    qlen = finalQ.size()-1;
    for (int i = 0; i <= qlen+1; ++i) {
        Q[i] = finalQ[i];
    }
}

void optimizeQuaternion() {
    removeUselessAssign();
    constantExpressionSavings();
}

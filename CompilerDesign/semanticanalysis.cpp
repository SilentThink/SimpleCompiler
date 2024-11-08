#include "semanticanalysis.h"

// 将四元式写入文件d5
void yuyiAnalyer()
{
    constructBlocks();
    int i;
    fp2=fopen("D://d5.txt","w");
    for(i=0;i<=qlen;i++)
    {
        fprintf(fp2,"%d:\t(%s,%s,%s,%s)  %d\r\n",i,Q[i].op,Q[i].arg1,Q[i].arg2,Q[i].result,Q[i].block);
    }
    fclose(fp2);
}



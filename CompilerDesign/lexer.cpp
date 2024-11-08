#include "lexer.h"

//十六进制数
char hexdigit(char buffer)
{
    char ch[20];int i=2;
    ch[0]='0';
    ch[1]='x';
    buffer=fgetc(fp);
    if((buffer>='0'&&buffer<='9')||(buffer>='a'&&buffer<='f')||(buffer>='A'&&buffer<='F'))
    {
        ch[i]=buffer;
        i++;
        ch[i]='\0';
        buffer=fgetc(fp);
        if((buffer>='0'&&buffer<='9')||(buffer>='a'&&buffer<='f')||(buffer>='A'&&buffer<='F'))
        {
            ch[i]=buffer;
            i++;
            ch[i]='\0';
            w[++len].typenum=68;
            strcpy(w[len].word,ch);
            strcpy(w[len].type,"常量");
            w[len].wline=line;
            fprintf(fp1,"(%s,68)\n",ch);
        }
        else
        {
            fprintf(fp0,"%d:%s%c:[Error]invalid suffix '%c' on integer constant\n",line,ch,buffer,buffer);
            error++;
        }
    }
    else
    {
        fprintf(fp0,"%d:%s%c:[Error]invalid suffix '%c' on integer constant\n",line,ch,buffer,buffer);
        error++;
    }
    buffer=fgetc(fp);
    return(buffer);
}
//八进制数
int digit3(char num[])
{
    int i=0;
    while(num[i]!='\0')
    {
        if(num[i]>'7'||num[i]<'0')
        {
            return 0;
        }
        i++;
    }
    return 1;
}

//关键字
int search(char searchchar[ ])//判断关键字
{
    int i=0;
    int p;
    for (i=0;i<=34;i++)
    {
        if (strcmp(key[i],searchchar)==0)
        {
            p=i+1;
            break;
        }
        else p=0;
    }
    return(p);
}
//字母
char  Alpha(char buffer)//判断标识符
{
    int   atype;
    int   i=-1;
    char  alphatp[20];
    while (isdigit(buffer)||isalpha(buffer)||buffer=='_')
    {

        alphatp[++i]=buffer;
        buffer=fgetc(fp);
    }
    alphatp[i+1]='\0';
    atype=search(alphatp);
    if(atype!=0)
    {
        w[++len].typenum=atype-1;
        strcpy(w[len].word,alphatp);
        strcpy(w[len].type,"关键字");
        fprintf(fp1,"(%s,1,%d)\n",alphatp,atype-1);
        id=1;
    }
    else if(strcmp(alphatp,"true")==0||strcmp(alphatp,"false")==0)//true false常量
    {
        w[++len].typenum=34;
        strcpy(w[len].word,alphatp);
        strcpy(w[len].type,"常量");
        fprintf(fp1,"(%s,34)\n",alphatp);
        id=3;
    }
    else
    {
        w[++len].typenum=33;
        strcpy(w[len].word,alphatp);
        strcpy(w[len].type,"标识符");
        fprintf(fp1,"(%s,33)\n",alphatp);
        id=2;
    }
    return(buffer);
}
//其他数字常量
char Digit(char buffer)//Ne+N
{
    char ch[20];
    int i;
    int tag;
    int count1=0;
    ch[0]=buffer;
    ch[1]='\0';
    if(buffer=='0')
    {
        buffer=fgetc(fp);
        if(buffer=='X'||buffer=='x') return(hexdigit(buffer));
        else if(isalpha(buffer)||buffer=='_')
        {
            ch[1]=buffer;
            buffer=fgetc(fp);
            i=2;
            while(isalpha(buffer)||isdigit(buffer)||buffer=='_')
            {
                ch[i]=buffer;
                i++;
                buffer=fgetc(fp);
            }
            ch[i]='\0';
            strncpy(str,ch+1,i);
            fprintf(fp0,"%d:%s:[Error]invalid suffix '%s' on integer constant\n",line,ch,str);
            error++;
            return(fgetc(fp));
        }
        else if((buffer>=32&&buffer<=34)||(buffer>=37&&buffer<=45)||(buffer==47)||(buffer>=58&&buffer<=63)||(buffer==93)||(buffer>=124&&buffer<=125)||buffer=='\n')
        {    if(buffer=='\n')line++;
            w[++len].typenum=34;
            strncpy(w[len].word,ch,sizeof(w[len].word)-1);
            strncpy(w[len].type,"常量",sizeof(w[len].type)-1);
            fprintf(fp1,"(%s,34)\n",ch);
            return(buffer);
        }
        else if(buffer=='.')
        {
            ch[1]=buffer;
            buffer=fgetc(fp);
            i=2;
            while(isdigit(buffer))
            {
                ch[i]=buffer;
                i++;
                buffer=fgetc(fp);
            }
            ch[i]='\0';
            w[++len].typenum=35;
            strncpy(w[len].word,ch,sizeof(w[len].word)-1);
            strncpy(w[len].type,"常量",sizeof(w[len].type)-1);
            fprintf(fp1,"(%s,35)\n",ch);
            return(buffer);
        }
        else
        {
            ch[1]=buffer;i=2;buffer=fgetc(fp);
        }
    }
    else
    {
        buffer=fgetc(fp);
        i=1;
    }
    while(isdigit(buffer)||buffer=='.')
    {
        if(buffer=='.')count1++;
        ch[i]=buffer;
        i++;
        buffer=fgetc(fp);
    }
    ch[i]='\0';
    if(count1>1)
    {
        fprintf(fp0,"%d:%s:[Error] too many decimal points in number\n",line,ch);
        error++;return(buffer);
    }
    if(buffer=='e')
    {
        ch[i]=buffer;
        i++;
        ch[i]='\0';
        buffer=fgetc(fp);
        if(buffer=='+'||buffer=='-')
        {
            ch[i]=buffer;
            i++;
            ch[i]='\0';
            buffer=fgetc(fp);
            if(isdigit(buffer))
            {
                while(isdigit(buffer))
                {
                    ch[i]=buffer;i++;

                    buffer=fgetc(fp);
                }
                ch[i]='\0';
                w[++len].typenum=67;
                strncpy(w[len].word,ch,sizeof(w[len].word)-1);
                strncpy(w[len].type,"常量",sizeof(w[len].type)-1);
                fprintf(fp1,"(%s,67)\n",ch);
                id=3;
            }
            else
            {
                fprintf(fp0,"%d:%s%c:[Error]\n",line,ch,buffer);
                error++;
            }
        }
        else if(isdigit(buffer))
        {
            while(isdigit(buffer))
            {
                ch[i]=buffer;i++;
                ch[i]='\0';
                buffer=fgetc(fp);
            }
            id=3;
            w[++len].typenum=67;
            strncpy(w[len].word,ch,sizeof(w[len].word)-1);
            strncpy(w[len].type,"常量",sizeof(w[len].type)-1);
            fprintf(fp1,"(%s,67)\n",ch);
        }
        else
        {
            fprintf(fp0,"%d:%s:[Error] invalid suffix '%c' on integer constant\n",line,ch,ch[i-1]);
            error++;
            buffer=fgetc(fp);
        }
    }
    else if(isalpha(buffer)||buffer=='_')
    {
        ch[i]=buffer;
        buffer=fgetc(fp);
        i++;
        while(isalpha(buffer)||isdigit(buffer)||buffer=='_')
        {
            ch[i]=buffer;
            i++;
            buffer=fgetc(fp);
        }
        ch[i]='\0';
        strncpy(str,ch+1,i);
        error++;
        fprintf(fp0,"%d:%s:[Error]invalid suffix '%s' on integer constant\n",line,ch,str);
        return(fgetc(fp));
    }
    else if(ch[0]!='0'&&((buffer>=32&&buffer<=34)||(buffer>=37&&buffer<=45)||(buffer==47)||(buffer>=58&&buffer<=63)||(buffer==93)||(buffer>=124&&buffer<=125)||buffer=='\n'))
    {   if(buffer=='\n')line++;
        id=3;
        if(count1==1)
        {
            w[++len].typenum=35;
            strncpy(w[len].word,ch,sizeof(w[len].word)-1);
            strncpy(w[len].type,"常量",sizeof(w[len].type)-1);
            fprintf(fp1,"(%s,35)\n",ch);
        }
        else
        {
            w[++len].typenum=34;
            strncpy(w[len].word,ch,sizeof(w[len].word)-1);
            strncpy(w[len].type,"常量",sizeof(w[len].type)-1);
            fprintf(fp1,"(%s,34)\n",ch);
        }
    }
    else if((buffer>='a'&&buffer<='z')||(buffer>='A'&&buffer<='Z')||buffer=='_')
    {
        fprintf(fp0,"%d:%s%c:[Error] invalid suffix '%c' on integer constant\n",line,ch,buffer,buffer);
        error++;
        buffer=fgetc(fp);
    }
    else if(ch[0]=='0'||count1==0)
    {
        tag=digit3(ch);
        if(tag==1)
        {
            w[++len].typenum=69;
            strncpy(w[len].word,ch,sizeof(w[len].word)-1);
            strncpy(w[len].type,"常量",sizeof(w[len].type)-1);
            fprintf(fp1,"(%s,69)\n",ch);
        }
        else
        {error++;fprintf(fp0,"%d:%s:[Error]八进制格式错误\n",line,ch);}
    }
    else
    {
        fprintf(fp0,"%d:%s%c:[Error] invalid suffix '%c' on integer constant\n",line,ch,buffer,buffer);
        error++;
    }
    return buffer;
}

char Other(char buffer)
{
    char ch[20];
    ch[0]=buffer;
    ch[1]='\0';
    if(ch[0]=='|')
    {
        buffer=fgetc(fp);
        ch[1]=buffer;
        ch[2]='\0';
        id=4;
        if(buffer=='|')
        {
            w[++len].typenum=58;
            strcpy(w[len].word,ch);
            strcpy(w[len].type,"界符");
            fprintf(fp1,"(%s,58)\n",ch);
        }
        else if(buffer=='=')
        {
            w[++len].typenum=58;
            strcpy(w[len].word,ch);
            strcpy(w[len].type,"界符");
            fprintf(fp1,"(%s,58)\n",ch);
        }
        else
        {   ch[1]='\0';
            w[++len].typenum=76;
            strcpy(w[len].word,ch);
            strcpy(w[len].type,"界符");
            fprintf(fp1,"(%s,76)\n",ch);
            return(buffer);
        }
        buffer=fgetc(fp);
        return(buffer);
    }
    else if(ch[0]=='&')
    {
        buffer=fgetc(fp);
        ch[1]=buffer;
        ch[2]='\0';
        id=4;
        if(ch[1]=='&')
        {   w[++len].typenum=57;
            strcpy(w[len].word,ch);
            strcpy(w[len].type,"界符");
            fprintf(fp1,"(%s,57)\n",ch);}
        else if(ch[1]=='=')
        {
            w[++len].typenum=57;
            strcpy(w[len].word,ch);
            strcpy(w[len].type,"界符");
            fprintf(fp1,"(%s,57)\n",ch);
        }
        else
        {   ch[1]='\0';
            w[++len].typenum=75;
            strcpy(w[len].word,ch);
            strcpy(w[len].type,"界符");
            fprintf(fp1,"(%s,75)\n",ch);return(buffer);
        }
        buffer=fgetc(fp);
        return(buffer);
    }
    else if(ch[0]==',')
    {
        w[++len].typenum=42;
        strcpy(w[len].word,ch);
        strcpy(w[len].type,"界符");
        fprintf(fp1,"(%s,42)\n",ch);
        buffer=fgetc(fp);
        id=4;
        return(buffer);
    }
    else if(ch[0]==';')
    {
        w[++len].typenum=41;
        strcpy(w[len].word,ch);
        strcpy(w[len].type,"界符");
        fprintf(fp1,"(%s,41)\n",ch);
        buffer=fgetc(fp);
        id=4;
        return(buffer);
    }
    else if(ch[0]==':')
    {
        w[++len].typenum=58;
        strcpy(w[len].word,ch);
        strcpy(w[len].type,"界符");
        fprintf(fp1,"(%s,58)\n",ch);
        buffer=fgetc(fp);
        id=4;
        return(buffer);
    }
    else if(ch[0]==')')
    {
        w[++len].typenum=40;
        strcpy(w[len].word,ch);
        strcpy(w[len].type,"界符");
        fprintf(fp1,"(%s,40)\n",ch);
        buffer=fgetc(fp);
        id=4;
        return(buffer);
    }
    else if(ch[0]=='(')
    {
        w[++len].typenum=39;
        strcpy(w[len].word,ch);
        strcpy(w[len].type,"界符");
        fprintf(fp1,"(%s,39)\n",ch);
        buffer=fgetc(fp);
        id=4;
        return(buffer);
    }
    else if(ch[0]=='}')
    {
        w[++len].typenum=38;
        strcpy(w[len].word,ch);
        strcpy(w[len].type,"界符");
        fprintf(fp1,"(%s,38)\n",ch);
        buffer=fgetc(fp);
        id=4;
        return(buffer);
    }
    else if(ch[0]=='{')
    {
        w[++len].typenum=37;
        strcpy(w[len].word,ch);
        strcpy(w[len].type,"界符");
        fprintf(fp1,"(%s,37)\n",ch);
        buffer=fgetc(fp);
        id=4;
        return(buffer);
    }
    else if(ch[0]=='[')
    {
        w[++len].typenum=70;
        strcpy(w[len].word,ch);
        strcpy(w[len].type,"界符");
        fprintf(fp1,"(%s,70)\n",ch);
        buffer=fgetc(fp);
        id=4;
        return(buffer);
    }
    else if(ch[0]==']')
    {
        w[++len].typenum=71;
        strcpy(w[len].word,ch);
        strcpy(w[len].type,"界符");
        fprintf(fp1,"(%s,71)\n",ch);
        buffer=fgetc(fp);
        id=4;
        return(buffer);
    }
    else if(ch[0]=='*')
    {
        buffer=fgetc(fp);
        ch[1]=buffer;
        if(ch[1]=='=')
        {
            ch[2]='\0';
            buffer=fgetc(fp);
            if(buffer==39||isdigit(buffer)||buffer==32||isalpha(buffer)||buffer=='('||buffer=='_')
            {
                w[++len].typenum=64;
                strcpy(w[len].word,ch);
                strcpy(w[len].type,"界符");
                fprintf(fp1,"(%s,64)\n",ch);return(buffer);
            }
            else
            {
                fprintf(fp0,"%d:%s%c:[Error]invalid suffix '%c' on integer constant\n",line,ch,buffer,buffer);
                error++;}
        }
        else
        {
            ch[1]='\0';
            w[++len].typenum=45;
            strcpy(w[len].word,ch);
            strcpy(w[len].type,"界符");
            fprintf(fp1,"(%s,45)\n",ch);
            return(buffer);
        }
        buffer=fgetc(fp);
        id=4;
        return(buffer);
    }
    else if(ch[0]=='/')
    {
        buffer=fgetc(fp);
        ch[1]=buffer;
        if(ch[1]=='=')
        {
            ch[2]='\0';
            w[++len].typenum=65;
            strcpy(w[len].word,ch);
            strcpy(w[len].type,"界符");
            fprintf(fp1,"(%s,65)\n",ch);
            buffer=fgetc(fp);
            id=4;
            return(buffer);
        }
        else
        {
            ch[1]='\0';
            w[++len].typenum=46;
            strcpy(w[len].word,ch);
            strcpy(w[len].type,"界符");
            fprintf(fp1,"(%s,46)\n",ch);return(buffer);
        }
        id=4;
        return(buffer);
    }

    else if(ch[0]=='%')
    {
        buffer=fgetc(fp);
        ch[1]=buffer;
        if(ch[1]=='=')
        {
            ch[2]='\0';
            w[++len].typenum=66;
            strcpy(w[len].word,ch);
            strcpy(w[len].type,"界符");
            fprintf(fp1,"(%s,66)\n",ch);
            buffer=fgetc(fp);
            id=4;
            return(buffer);
        }
        else
        {
            ch[1]='\0';
            w[++len].typenum=47;
            strcpy(w[len].word,ch);
            strcpy(w[len].type,"界符");
            fprintf(fp1,"(%s,47)\n",ch);
        }
        id=4;
        return(buffer);
    }
    else if(ch[0]=='=')
    {
        buffer=fgetc(fp);
        if(buffer=='=')
        {
            ch[1]=buffer;
            ch[2]='\0';
            buffer=fgetc(fp);
            id=4;
            if(buffer==39||isdigit(buffer)||buffer==32||isalpha(buffer)||buffer=='('||buffer=='_')
            {
                w[++len].typenum=49;
                strcpy(w[len].word,ch);
                strcpy(w[len].type,"界符");
                fprintf(fp1,"(%s,49)\n",ch);return(buffer);
            }
            else
            {error++;
                fprintf(fp0,"%d:%s%c:[Error]invalid suffix '%c' on integer constant\n",line,ch,buffer,buffer);}
        }
        else
        {
            w[++len].typenum=48;
            strcpy(w[len].word,ch);
            strcpy(w[len].type,"界符");
            fprintf(fp1,"(%s,48)\n",ch);
            id=4;
            return(buffer);
        }
        buffer=fgetc(fp);
        id=4;
        return(buffer);

    }
    else if(ch[0]=='!')
    {
        buffer=fgetc(fp);
        if(buffer=='=')
        {
            ch[1]=buffer;
            ch[2]='\0';
            buffer=fgetc(fp);
            if(buffer==39||isdigit(buffer)||buffer==32||isalpha(buffer)||buffer=='('||buffer=='_')
            {
                w[++len].typenum=54;
                strcpy(w[len].word,ch);
                strcpy(w[len].type,"界符");
                fprintf(fp1,"(%s,54)\n",ch);return(buffer);
            }
            else
            {
                error++;
                fprintf(fp0,"%d:%s%c:[Error]invalid suffix '%c' on integer constant\n",line,ch,buffer,buffer);
            }


        }
        else if(buffer =='>')
        {
            ch[1]=buffer;
            ch[2]='\0';
            buffer=fgetc(fp);
            if(buffer==39||isdigit(buffer)||buffer==32||isalpha(buffer)||buffer=='('||buffer=='_')
            {
                w[++len].typenum=55;
                strcpy(w[len].word,ch);
                strcpy(w[len].type,"界符");
                fprintf(fp1,"(%s,55)\n",ch);return(buffer);
            }
            else
            {error++;
                fprintf(fp0,"%d:%s%c:[Error]invalid suffix '%c' on integer constant\n",line,ch,buffer,buffer);}

        }
        else if(buffer =='<')
        {
            ch[1]=buffer;
            ch[2]='\0';
            buffer=fgetc(fp);
            if(buffer==39||isdigit(buffer)||buffer==32||isalpha(buffer)||buffer=='('||buffer=='_')
            {   w[++len].typenum=56;
                strcpy(w[len].word,ch);
                strcpy(w[len].type,"界符");
                fprintf(fp1,"(%s,56)\n",ch);return(buffer);
            }
            else
            {error++;
                fprintf(fp0,"%d:%s%c:[Error]invalid suffix '%c' on integer constant\n",line,ch,buffer,buffer);}
        }
        else
        {   w[++len].typenum=59;
            strcpy(w[len].word,ch);
            strcpy(w[len].type,"界符");
            fprintf(fp1,"(%s,59)\n",ch);
            id=4;
            return(buffer);
        }
        buffer=fgetc(fp);
        id=4;
        return(buffer);
    }
    else if(ch[0]=='<')
    {
        buffer=fgetc(fp);
        if(buffer=='=')
        {
            ch[1]=buffer;
            ch[2]='\0';
            buffer=fgetc(fp);
            if(buffer==39||isdigit(buffer)||buffer==32||isalpha(buffer)||buffer=='('||buffer=='_')
            {
                w[++len].typenum=53;
                strcpy(w[len].word,ch);
                strcpy(w[len].type,"界符");
                fprintf(fp1,"(%s,53)\n",ch);return(buffer);
            }
            else
            {error++;
                fprintf(fp0,"%d:%s%c:[Error]invalid suffix '%c' on integer constant\n",line,ch,buffer,buffer);}
        }
        else if(buffer=='<')
        {
            ch[1]=buffer;
            ch[2]='\0';
            buffer=fgetc(fp);
            if(buffer=='=')
            {
                ch[2]=buffer;
                ch[3]='\0';
                buffer=fgetc(fp);
                if(buffer==39||isdigit(buffer)||buffer==32||isalpha(buffer)||buffer=='('||buffer=='_')
                {
                    w[++len].typenum=74;
                    strcpy(w[len].word,ch);
                    strcpy(w[len].type,"界符");
                    fprintf(fp1,"(%s,74)\n",ch);return(buffer);
                }
                else
                {error++;
                    fprintf(fp0,"%d:%s%c:[Error]invalid suffix '%c' on integer constant\n",line,ch,buffer,buffer);}
            }
        }
        else
        {
            w[++len].typenum=50;
            strcpy(w[len].word,ch);
            strcpy(w[len].type,"界符");
            fprintf(fp1,"(%s,50)\n",ch);
            id=4;
            return(buffer);
        }
        buffer=fgetc(fp);
        id=4;
        return(buffer);
    }
    else if(ch[0]=='>')
    {
        buffer=fgetc(fp);
        if(buffer=='=')
        {
            ch[1]=buffer;
            ch[2]='\0';
            buffer=fgetc(fp);
            if(buffer==39||isdigit(buffer)||buffer==32||isalpha(buffer)||buffer=='('||buffer=='_')
            {
                w[++len].typenum=52;
                strcpy(w[len].word,ch);
                strcpy(w[len].type,"界符");
                fprintf(fp1,"(%s,52)\n",ch);return(buffer);
            }
            else
            {error++;
                fprintf(fp0,"%d:%s%c:[Error]invalid suffix '%c' on integer constant\n",line,ch,buffer,buffer);}
        }
        else if(buffer=='>')
        {
            ch[1]=buffer;
            ch[2]='\0';
            buffer=fgetc(fp);
            if(buffer=='=')
            {
                ch[2]=buffer;
                ch[3]='\0';
                buffer=fgetc(fp);
                if(buffer==39||isdigit(buffer)||buffer==32||isalpha(buffer)||buffer=='('||buffer=='_')
                {
                    w[++len].typenum=73;
                    strcpy(w[len].word,ch);
                    strcpy(w[len].type,"界符");
                    fprintf(fp1,"(%s,73)\n",ch);return(buffer);
                }
                else
                {error++;
                    fprintf(fp0,"%d:%s%c:[Error]invalid suffix '%c' on integer constant\n",line,ch,buffer,buffer);}
            }
        }
        else
        {
            w[++len].typenum=51;
            strcpy(w[len].word,ch);
            strcpy(w[len].type,"界符");
            fprintf(fp1,"(%s,51)\n",ch);
            id=4;
            return(buffer);
        }
        buffer=fgetc(fp);
        id=4;
        return(buffer);
    }
    else if(ch[0]=='+')
    {
        if(id==4)
        {
            buffer=fgetc(fp);
            ch[1]=buffer;
            ch[2]='\0';
            if(isdigit(buffer))
            {
                w[++len].typenum=34;
                strcpy(w[len].word,ch);
                strcpy(w[len].type,"常量");
                fprintf(fp1,"(%s,常量)\n",ch);
                id=3;
                buffer=fgetc(fp);
                return(buffer);
            }
        }
        else
        {
            buffer=fgetc(fp);
            ch[1]=buffer;
        }
        if(ch[1] =='=')
        {
            ch[2]='\0';
            w[++len].typenum=62;
            strcpy(w[len].word,ch);
            strcpy(w[len].type,"界符");
            fprintf(fp1,"(%s,62)\n",ch);
        }
        else if(ch[1] =='+')
        {
            ch[2]='\0';
            w[++len].typenum=60;
            strcpy(w[len].word,ch);
            strcpy(w[len].type,"界符");
            fprintf(fp1,"(%s,60)\n",ch);
        }
        else
        {
            ch[1]='\0';
            w[++len].typenum=43;
            strcpy(w[len].word,ch);
            strcpy(w[len].type,"界符");
            fprintf(fp1,"(%s,43)\n",ch);
            id=4;
            return(buffer);
        }
        id=4;
        buffer=fgetc(fp);
        return(buffer);
    }
    else if(ch[0]=='-')
    {
        if(id==4)
        {
            buffer=fgetc(fp);
            ch[1]=buffer;
            ch[2]='\0';
            if(isdigit(buffer))
            {
                w[++len].typenum=34;
                strcpy(w[len].word,ch);
                strcpy(w[len].type,"常量");
                fprintf(fp1,"(%s,负数)\n",ch);
                id=3;
                buffer=fgetc(fp);
                return(buffer);
            }
        }
        else
        {
            buffer=fgetc(fp);
            ch[1]=buffer;
        }
        if(ch[1] =='=')
        {
            ch[2]='\0';
            w[++len].typenum=63;
            strcpy(w[len].word,ch);
            strcpy(w[len].type,"界符");
            fprintf(fp1,"(%s,63)\n",ch);
        }
        else if(ch[1] =='-')
        {
            ch[2]='\0';
            w[++len].typenum=61;
            strcpy(w[len].word,ch);
            strcpy(w[len].type,"界符");
            fprintf(fp1,"(%s,61)\n",ch);
        }
        else
        {
            ch[1]='\0';
            w[++len].typenum=44;
            strcpy(w[len].word,ch);
            strcpy(w[len].type,"界符");
            fprintf(fp1,"(%s,44)\n",ch);
            id=4;
            return(buffer);
        }
        id=4;
        buffer=fgetc(fp);
        return(buffer);
    }
    else if(ch[0]=='#')
    {
        buffer=fgetc(fp);
        int i=1;
        while(isalpha(buffer)||isdigit(buffer)||buffer=='_')
        {
            ch[i]=buffer;i++;
            buffer=fgetc(fp);
        }ch[i]='\0';
        w[++len].typenum=0;
        strcpy(w[len].word,ch);
        strcpy(w[len].type,"宏");
        return(buffer);
    }
    else if(buffer=='\n'||buffer==' '||buffer=='\t') return(buffer);
    else
    {
        buffer=fgetc(fp);
        int i=1;
        while(isalpha(buffer)||isdigit(buffer)||buffer=='_')
        {
            ch[i]=buffer;i++;
            buffer=fgetc(fp);
        }
        ch[i]='\0';
        //fprintf(fp0,"%d:%s:[Error]未知符号\n",line,ch);
        return(buffer);
    }
    return(buffer);
}

char charprocess(char buffer)//'A'
{   int i=1;
    char ch[100];
    int flag=0;
    ch[0]=buffer;
    buffer=fgetc(fp);
    while(buffer!='\n')
    {
        ch[i]=buffer;i++;
        if(buffer==39)
        {
            flag=1;break;
        }
        buffer=fgetc(fp);
    }
    ch[i]='\0';
    if(flag==1)
    {
        if(strlen(ch)==3)
        {w[++len].typenum=36;
            strncpy(w[len].word,ch,sizeof(w[len].word)-1);
            strncpy(w[len].type,"字符常量",sizeof(w[len].type)-1);
            fprintf(fp1,"(%s,36)\n",ch);
        }
        else
        {fprintf(fp0,"%d:'%s':[warning]字符长度超出\n",line,ch);warning++;}
        buffer=fgetc(fp);
    }
    else
    {
        fprintf(fp0,"%d:%s:missing terminating ' character\n",line,ch);
        error++;
    }
    id=3;
    return(buffer);
}

char stringprocess(char buffer)//"ABC"
{
    char ch[100];
    int i=1;
    int flag=0;
    ch[0]=buffer;
    buffer=fgetc(fp);
    while(buffer!='\n')
    {
        ch[i]=buffer;i++;
        if(buffer==34)
        {
            flag=1;break;
        }
        buffer=fgetc(fp);
    }
    ch[i]='\0';
    if(flag==1)
    {
        w[++len].typenum=36;
        strncpy(w[len].word,ch,sizeof(w[len].word)-1);
        strncpy(w[len].type,"字符常量",sizeof(w[len].type)-1);
        fprintf(fp1,"(%s,36)\n",ch);
        buffer=fgetc(fp);
    }
    else
    {
        fprintf(fp0,"%d:%s:missing terminating %c character\n",line,ch,34);
        error++;
    }
    id=3;
    return(buffer);
}

char cut1(char buffer)
{
    buffer=fgetc(fp);// '/*ABC...\nDEFG...*/\n'
    if(buffer=='*')
    {
        buffer=fgetc(fp);
        while(buffer!=EOF)
        {
            while(buffer!='*')
            {
                if(buffer=='\n')line++;
                buffer=fgetc(fp);
            }
            buffer=fgetc(fp);
            if(buffer=='/')
            {
                buffer=fgetc(fp);
                break;
            }
            else continue;
        }
    }
    else if(buffer=='/')// '//ABCD....'
    {
        fgets(buf,sizeof buf,fp);
        //line++;//跳到下一行
    }
    else
    {
        fseek(fp,-2L,1);
        buffer=fgetc(fp);
        return(Other(buffer));
    }
    return(buffer);
}

/* 词法分析
 * 读取源程序code.txt
 * 将警告和错误信息写入d.txt
 * 将token表写入d2.txt
 */
void analy()
{
    int i;
    if ((fp=fopen("D://code.txt","r"))==NULL)
        printf("error\n");
    if ((fp0=fopen("D://d.txt","w"))==NULL)
        printf("error\n");
    if ((fp1=fopen("D://d2.txt","w"))==NULL)
        printf("error\n");
    else
    {
        cbuffer = fgetc(fp);
        while (cbuffer!=EOF)
        {
            // 跳过空格、回车、换行、制表符
            if(cbuffer==32||cbuffer==13||cbuffer=='\n'||cbuffer=='\t')
            {   if(cbuffer=='\n'||cbuffer==13) line++;
                cbuffer=fgetc(fp);}

            // 读到单引号处理字符
            else if(cbuffer==39)
            {
                cbuffer=charprocess(cbuffer);
            }
            // 读到双引号处理字符串
            else if(cbuffer==34)
            {
                cbuffer=stringprocess(cbuffer);
            }
            // 处理数字
            else if(isdigit(cbuffer))
            {
                cbuffer=Digit(cbuffer);
            }
            // 处理字母或下划线（关键字或标识符）
            else if(isalpha(cbuffer)||cbuffer=='_')
            {
                cbuffer=Alpha(cbuffer);
            }
            // 处理注释
            else if(cbuffer=='/')
            {
                cbuffer=cut1(cbuffer);
            }
            // 处理其他界符
            else{cbuffer=Other(cbuffer);}
        }
        for(i=0;i<=len;i++)
        {
            fprintf(fp1,"%s\t%s\t%d\n",w[i].word,w[i].type,w[i].typenum);
        }
        fprintf(fp0,"%d error(s), %d warning(s)\r\n",error,warning);
    }
    fclose(fp0);
    fclose(fp1);
    fclose(fp);
}

#include <bits/stdc++.h>

using namespace std;

void E();  //先声明
char s[50]; //s[50]用于存放输入的赋值表达式
char n='1';   //T的下标
char STACK[50][3];  //产生中间代码所需的符号表
int flag=0; //SIGN用于指示赋值表达式是否出现错误
int i=0;   //i为s[50]中当前读入符号的下标
int j=0;   //符号表的下标

void V()  // V -> a|b|c|d|e...|z 判断是不是小写字母
{
	if(islower(s[i]))
	{
		STACK[j][0] = s[i];              //把读取的小写字母存入符号表，便于分析是生成中间代码
		STACK[j][1] = '\0';
		i++;
		j++;
	}
	else
	{
		cout<<"运算对象错误，比如输入了大写字母、数字或者符号"<<endl;      //运算对象错误
		flag=1;
		exit(0);
	}
}

void F()    //F -> V|(E) 判断括号
{
	if(flag==0)
	{
		if(s[i]=='(')
		{
			i++;
			E();
			if(s[i]==')')
                i++;
			else
			{
                cout<<"缺乏完整括号，错误"<<endl;
				flag=1;
                exit(0);
			}
		}
        else if(islower(s[i]))  //判断s[sym]是否是小写字母
			V();
		else
		{
			cout<<"式子错误"<<endl;
            flag=1;
			exit(0);
		}
	}
}

void T_COUNT()  //  T'-> *FT'|/FT'|null 计算乘除法
{
    int p;
	if(flag==0)
	{
		if(s[i] == '*'||s[i]=='/')
		{
			p=i;
			i++;
			F();
			char ch[3];
			ch[0] = 'T';
            ch[1] = n;
			ch[2] = '\0';
			if(s[p] == '*')
            {
				printf("%s:=%s*%s",ch,STACK[j-2],STACK[j-1]);   //输出三地址代码
				printf("     (*,%s,%s,%s)\n", STACK[j-2],STACK[j-1],ch);//输出四元式
			}
			else
			{
				printf("%s:=%s/%s",ch,STACK[j-2],STACK[j-1]);   //输出三地址代码
				printf("     (/,%s,%s,%s)\n", STACK[j-2],STACK[j-1],ch);//输出四元式
			}
            strcpy(STACK[j-2],ch);      //将当前结果归结式放在符号表中
			j--;
			n++;
			T_COUNT();
		}
	}
}

void T()   // T -> FT' 乘除过度
{
	if(flag==0)
	{
		F();
		T_COUNT();
	}
}

void E_COUNT()          //E'-> +TE'|-TE'|null  计算加减法
{
	int p;
	if(flag==0)
	{
		if(s[i] == '+'||s[i]=='-')
		{
			p=i;                  //用p记录出现'+'或'-'时sym的值
			i++;
			T();
			char ch[3];
			ch[0] = 'T';
			ch[1] = n;
            ch[2] = '\0';
            if(s[p] == '+')
            {
                printf("%s:=%s+%s",ch,STACK[j-2],STACK[j-1]);                //输出三地址代码
				printf("     (+,%s,%s,%s)\n", STACK[j-2],STACK[j-1],ch);             //输出四元式
			}
			else
			{
				printf("%s:=%s-%s",ch,STACK[j-2],STACK[j-1]);               //输出三地址代码
				printf("     (-,%s,%s,%s)\n", STACK[j-2],STACK[j-1],ch);        //输出四元式
			}
			strcpy(STACK[j-2],ch);           //将当前结果归结式放在符号表中
			j--;
			n++;
			E_COUNT();
        }
	}
}

void E()     //E -> TE'  判断加减过度
{
	if(flag==0)
	{
		T();
		E_COUNT();
	}
}

void A()  //  A -> V:=E  起初的赋值号判断
{
	V();
	if(s[i]==':'&&s[i+1]=='=') //判断赋值号是否有拼写错误
	{
		i=i+2;
		E();
		printf("%s:=%s",STACK[j-2],STACK[j-1]);
		printf("     (:=,%s,-,%s)\n",STACK[j-1],STACK[j-2]);
	}
	else
	{
		cout<<"不是赋值语句或赋值号错误，赋值号为\":=\"而不是\"=\""<<endl; //赋值号拼写错误
		flag=1;
		exit(0);
	}
}

int main()
{
    cin>>s;
    A();
    return 0;
}

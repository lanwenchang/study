%{
/*本lex的生成文件是lex.yy.c
lex文件由3段组成，用2个%%行把这3段隔开。
第1段是声明段，包括：
1-C代码部分：include头文件、函数、类型等声明，这些声明会原样拷到生成的.c文件中。
2-状态声明，如%x COMMENT。
3-正则式定义，如digit ([0-9])。
第2段是规则段，是lex文件的主体，包括每个规则(如identifier)是如何匹配的，以及匹配后要执行的C代码动作。
第3段是C函数定义段，如yywrap()的定义，这些C代码会原样拷到生成的.c文件中。该段内容可以为空*/

//第1段：声明段
#include "StatCalculator.h"		//lex和yacc要共用的头文件，里面包含了一些头文件，重定义了YYSTYPE
#include "yacc.tab.h"	//用yacc编译yacc.y后生成的C头文件，内含%token、YYSTYPE、yylval等定义(都是C宏)，供lex.yy.c和yacc.tab.c使用
%}

/*非数字由大小写字母、下划线组成*/
nondigit	([_A-Za-z])

/*一位数字，可以是0到9*/
digit		([0-9])

/*整数由1至多位数字组成*/
double		({digit}*\.{digit}+)

/*整数由1至多位数字组成*/
integer		({digit}+)


/*标识符，以非数字开头，后跟0至多个数字或非数字*/
variable	({nondigit}({nondigit}|{digit})*)


/*下面%%后开始第2段：规则段*/
%%

{variable}	    {							//匹配标识符串，此时串值由yytext保存
					yylval.sValue=yytext;	//通过yylval向yacc传递识别出的记号的值，由于yylval已定义为struct Type，这里就可以把yytext赋给其m_sId成员，到了yacc里就可以用$n的方式来引用了
					return VARIABLE;		//向yacc返回: 识别出的记号类型是VARIABLE
				}

{integer} 		{							//匹配整数串
					yylval.dValue=atoi(yytext);
					return VALUE;			
				}
{double} 		{							//匹配double串
					yylval.dValue=atof(yytext);
					return VALUE;			
				}
"?" return QUE;			
":" return COL;			
"||" return OR;			
"&&" return AND;			
">" return GRE;			
"<" return LES;			
">=" return GE;			
"<=" return LE;			
"==" return EQU;			
"!=" return NE;			
"+" return ADD;			
"-" return SUB;			
"*" return MUL;			
"/" return DIV;			
"!" return NON;
"(" return LBK;			
")" return RBK;			

[ \t\n] 		{
				}
.				{
				}
%%

//第3段：C函数定义段
int yywrap(void)
{
	return 1;								//返回1表示读取全部结束。如果要接着读其它文件，可以这里fopen该文件，文件指针赋给yyin，并返回0
}




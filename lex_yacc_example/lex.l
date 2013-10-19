%{
/*��lex�������ļ���lex.yy.c
lex�ļ���3����ɣ���2��%%�а���3�θ�����
��1���������Σ�������
1-C���벿�֣�includeͷ�ļ������������͵���������Щ������ԭ���������ɵ�.c�ļ��С�
2-״̬��������%x COMMENT��
3-����ʽ���壬��digit ([0-9])��
��2���ǹ���Σ���lex�ļ������壬����ÿ������(��identifier)�����ƥ��ģ��Լ�ƥ���Ҫִ�е�C���붯����
��3����C��������Σ���yywrap()�Ķ��壬��ЩC�����ԭ���������ɵ�.c�ļ��С��ö����ݿ���Ϊ��*/

//��1�Σ�������
#include "StatCalculator.h"		//lex��yaccҪ���õ�ͷ�ļ������������һЩͷ�ļ����ض�����YYSTYPE
#include "yacc.tab.h"	//��yacc����yacc.y�����ɵ�Cͷ�ļ����ں�%token��YYSTYPE��yylval�ȶ���(����C��)����lex.yy.c��yacc.tab.cʹ��
%}

/*�������ɴ�Сд��ĸ���»������*/
nondigit	([_A-Za-z])

/*һλ���֣�������0��9*/
digit		([0-9])

/*������1����λ�������*/
double		({digit}*\.{digit}+)

/*������1����λ�������*/
integer		({digit}+)


/*��ʶ�����Է����ֿ�ͷ�����0��������ֻ������*/
variable	({nondigit}({nondigit}|{digit})*)


/*����%%��ʼ��2�Σ������*/
%%

{variable}	    {							//ƥ���ʶ��������ʱ��ֵ��yytext����
					yylval.sValue=yytext;	//ͨ��yylval��yacc����ʶ����ļǺŵ�ֵ������yylval�Ѷ���Ϊstruct Type������Ϳ��԰�yytext������m_sId��Ա������yacc��Ϳ�����$n�ķ�ʽ��������
					return VARIABLE;		//��yacc����: ʶ����ļǺ�������VARIABLE
				}

{integer} 		{							//ƥ��������
					yylval.dValue=atoi(yytext);
					return VALUE;			
				}
{double} 		{							//ƥ��double��
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

//��3�Σ�C���������
int yywrap(void)
{
	return 1;								//����1��ʾ��ȡȫ�����������Ҫ���Ŷ������ļ�����������fopen���ļ����ļ�ָ�븳��yyin��������0
}



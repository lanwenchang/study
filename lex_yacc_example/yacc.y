%{
/*本yacc的生成文件是yacc.tab.c和yacc.tab.h
yacc文件由3段组成，用2个%%行把这3段隔开。
第1段是声明段，包括：
	1-C代码部分：include头文件、函数、类型等声明，这些声明会原样拷到生成的.c文件中。 
	2-记号声明，如%token 
	3-类型声明，如%type
第2段是规则段，是yacc文件的主体，包括每个产生式是如何匹配的，以及匹配后要执行的C代码动作。
第3段是C函数定义段，如yyerror()的定义，这些C代码会原样拷到生成的.c文件中。该段内容可以为空*/

//第1段：声明段
#include "StatCalculator.h"	//lex和yacc要共用的头文件，里面包含了一些头文件，重定义了YYSTYPE
#include "lex.yy.h" //采用 lex --header-file=lex.yy.h lex.l 生成， 默认不生成头文件，主要时引进lex中的一些函数声明

//语法树的头指针
ExpNode* g_pHead = NULL;

%}

/*lex里要return的记号的声明
用token后加一对<member>来定义记号，旨在用于简化书写方式。
假定某个产生式中第1个终结符是记号OPERATOR，则引用OPERATOR属性的方式：
1-如果记号OPERATOR是以普通方式定义的，如%token OPERATOR，则在动作中要写$1.m_cOp，以指明使用YYSTYPE的哪个成员
2-用%token<m_cOp>OPERATOR方式定义后，只需要写$1，yacc会自动替换为$1.m_cOp
另外用<>定义记号后，非终结符如file, tokenlist，必须用%type<member>来定义(否则会报错)，以指明它们的属性对应YYSTYPE中哪个成员，这时对该非终结符的引用，如$$，会自动替换为$$.member*/
%union {
	double dValue; /* 变量值 */
	char *sValue;  /* 变量字符串 */
	ExpNode *nPtr; /* 结点地址 */
};
%token<dValue>VALUE
%token<sValue>VARIABLE

%type<nPtr> exp
%type<nPtr> line

%right QUE COL
%left  OR
%left  AND
%left  GRE LES GE LE EQU NE 
%left  ADD SUB
%left  MUL DIV
%right NON
%token LBK RBK 

%%
line: exp { g_pHead = $1; };
exp:  exp QUE  exp COL  exp {$$=new QUENode($1,$3,$5);cout<<"exp?exp:exp"<<endl;}
	| exp OR  exp { $$=new ORNode($1,$3); cout<<"exp||exp"<<endl;}
	| exp AND exp { $$=new ANDNode($1,$3);cout<<"exp&&exp"<<endl;}
	| exp GRE exp { $$=new GRENode($1,$3);cout<<"exp>exp"<<endl;}
	| exp LES exp { $$=new LESNode($1,$3);cout<<"exp<exp"<<endl;}
	| exp GE  exp { $$=new GENode($1,$3);cout<<"exp>=exp"<<endl;}
	| exp LE  exp { $$=new LENode($1,$3);cout<<"exp<=exp"<<endl;}
	| exp EQU exp { $$=new EQUNode($1,$3);cout<<"exp==exp"<<endl;}
	| exp NE  exp { $$=new NENode($1,$3);cout<<"exp!=exp"<<endl;}
	| exp ADD exp { $$=new ADDNode($1,$3);cout<<"exp+exp"<<endl;}
	| exp SUB exp { $$=new SUBNode($1,$3);cout<<"exp-exp"<<endl;}
	| exp MUL exp { $$=new MULNode($1,$3);cout<<"exp*exp"<<endl;}
	| exp DIV exp { $$=new DIVNode($1,$3);cout<<"exp/exp"<<endl;}
	| NON exp     { $$=new NONNode($2);cout<<"!exp"<<endl;}
	| SUB exp     { $$=new NEGNode($2);cout<<"-exp"<<endl;}
	| LBK exp RBK { $$=$2;cout<<"(exp)"<<endl;}
	| VALUE     { $$ = new IntNode(($1)); }
	| VARIABLE    { $$ = new VarNode($1);}; 

%%


void yyerror(const char *s )
{
	string tmp = yytext;
	string err = s;
	StatCalculator::sErrInfo = err  +": check before '"+tmp+"'";
}

//错误信息
string StatCalculator::sErrInfo;
/**
* 获取对应exp的语法树
*/
ExpNode* StatCalculator::getExpTree(string exp)
{
	sErrInfo = "";
	g_pHead = NULL;
	try
	{
		const char *str = exp.c_str();
		int len = exp.size();
		YY_BUFFER_STATE state = yy_scan_bytes(str,len );
		yy_switch_to_buffer ( state  );
		yyparse();						//使yacc开始读取输入和解析，它会调用lex的yylex()读取记号
		yy_delete_buffer (state);
	}
	catch(exception &e )
	{
		cout<<e.what()<<endl;
		g_pHead = NULL;
		StatCalculator::sErrInfo = "syntax error: check express last";
	}
	return g_pHead;
}


/**
* 测试
*/
int main()							//程序主函数，这个函数也可以放到其它.c, .cpp文件里
{
	map<string,double> m;
	m["a0"] = 0;
	m["a1"] = 1;
	m["a2"] = 2;
	
	freopen("in.txt","r",stdin);
	char *tmp;
	size_t len = 1024;
	while( getline(&tmp,&len,stdin) != -1 )
	{
		string exp = tmp;
		cout<<"-------------------------------------------"<<endl;
		ExpNode* pExpNode = StatCalculator::getExpTree(exp);
		cout<<exp<<endl;
		if ( pExpNode == NULL )
		{
			cout<<StatCalculator::sErrInfo<<endl;
		}
		else
		{
			cout<<"exp="<<pExpNode->getNodeValue(m)<<endl;
		}
	}
	return 0;
}

%{
/*��yacc�������ļ���yacc.tab.c��yacc.tab.h
yacc�ļ���3����ɣ���2��%%�а���3�θ�����
��1���������Σ�������
	1-C���벿�֣�includeͷ�ļ������������͵���������Щ������ԭ���������ɵ�.c�ļ��С� 
	2-�Ǻ���������%token 
	3-������������%type
��2���ǹ���Σ���yacc�ļ������壬����ÿ������ʽ�����ƥ��ģ��Լ�ƥ���Ҫִ�е�C���붯����
��3����C��������Σ���yyerror()�Ķ��壬��ЩC�����ԭ���������ɵ�.c�ļ��С��ö����ݿ���Ϊ��*/

//��1�Σ�������
#include "StatCalculator.h"	//lex��yaccҪ���õ�ͷ�ļ������������һЩͷ�ļ����ض�����YYSTYPE
#include "lex.yy.h" //���� lex --header-file=lex.yy.h lex.l ���ɣ� Ĭ�ϲ�����ͷ�ļ�����Ҫʱ����lex�е�һЩ��������

//�﷨����ͷָ��
ExpNode* g_pHead = NULL;

%}

/*lex��Ҫreturn�ļǺŵ�����
��token���һ��<member>������Ǻţ�ּ�����ڼ���д��ʽ��
�ٶ�ĳ������ʽ�е�1���ս���ǼǺ�OPERATOR��������OPERATOR���Եķ�ʽ��
1-����Ǻ�OPERATOR������ͨ��ʽ����ģ���%token OPERATOR�����ڶ�����Ҫд$1.m_cOp����ָ��ʹ��YYSTYPE���ĸ���Ա
2-��%token<m_cOp>OPERATOR��ʽ�����ֻ��Ҫд$1��yacc���Զ��滻Ϊ$1.m_cOp
������<>����Ǻź󣬷��ս����file, tokenlist��������%type<member>������(����ᱨ��)����ָ�����ǵ����Զ�ӦYYSTYPE���ĸ���Ա����ʱ�Ը÷��ս�������ã���$$�����Զ��滻Ϊ$$.member*/
%union {
	double dValue; /* ����ֵ */
	char *sValue;  /* �����ַ��� */
	ExpNode *nPtr; /* ����ַ */
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

//������Ϣ
string StatCalculator::sErrInfo;
/**
* ��ȡ��Ӧexp���﷨��
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
		yyparse();						//ʹyacc��ʼ��ȡ����ͽ������������lex��yylex()��ȡ�Ǻ�
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
* ����
*/
int main()							//�������������������Ҳ���Էŵ�����.c, .cpp�ļ���
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

//lex.l和yacc.y共同使用的头文件
#ifndef MAIN_HPP
#define MAIN_HPP

#include <iostream>//使用C++库
#include <map>
#include <string>
#include <stdio.h>//prdoublef和FILE要用的
#include <stdlib.h>

using namespace std;

class ExpNode
{
public:
	ExpNode():_pChild1(NULL),_pChild2(NULL),_pChild3(NULL){}
	virtual double getNodeValue(map<string,double> &mVar) = 0;
	virtual ~ExpNode(){}
protected:
	ExpNode *_pChild1, *_pChild2, *_pChild3;
};

class VarNode:public ExpNode
{
public:
	VarNode(char* sValue):_sValue(sValue){}
	double getNodeValue(map<string,double> &mVar){ return mVar[_sValue];}
public:
	string _sValue;
};
class IntNode:public ExpNode
{
public:
	IntNode(double iValue):_iValue(iValue){}
	double getNodeValue(map<string,double> &mVar){ return _iValue;}
public:
	double _iValue;
};

class TwoOpNode:public ExpNode
{
public:
	TwoOpNode(ExpNode *pChild1, ExpNode *pChild2){_pChild1=pChild1; _pChild2=pChild2;}
	double getNodeValue(map<string,double> &mVar){ return _pChild1->getNodeValue(mVar)+_pChild2->getNodeValue(mVar);}
	~TwoOpNode(){ delete _pChild1; delete _pChild2;}
};
class ORNode:public TwoOpNode
{
public:
	ORNode(ExpNode *pChild1, ExpNode *pChild2):TwoOpNode(pChild1,pChild2){}
	double getNodeValue(map<string,double> &mVar){ return _pChild1->getNodeValue(mVar)||_pChild2->getNodeValue(mVar);}
};
class ANDNode:public TwoOpNode
{
public:
	ANDNode(ExpNode *pChild1, ExpNode *pChild2):TwoOpNode(pChild1,pChild2){}
	double getNodeValue(map<string,double> &mVar){ return _pChild1->getNodeValue(mVar)&&_pChild2->getNodeValue(mVar);}
};
class GRENode:public TwoOpNode
{
public:
	GRENode(ExpNode *pChild1, ExpNode *pChild2):TwoOpNode(pChild1,pChild2){}
	double getNodeValue(map<string,double> &mVar){ return _pChild1->getNodeValue(mVar)>_pChild2->getNodeValue(mVar);}
};
class LESNode:public TwoOpNode
{
public:
	LESNode(ExpNode *pChild1, ExpNode *pChild2):TwoOpNode(pChild1,pChild2){}
	double getNodeValue(map<string,double> &mVar){ return _pChild1->getNodeValue(mVar)<_pChild2->getNodeValue(mVar);}
};
class GENode:public TwoOpNode
{
public:
	GENode(ExpNode *pChild1, ExpNode *pChild2):TwoOpNode(pChild1,pChild2){}
	double getNodeValue(map<string,double> &mVar){ return _pChild1->getNodeValue(mVar)>=_pChild2->getNodeValue(mVar);}
};
class LENode:public TwoOpNode
{
public:
	LENode(ExpNode *pChild1, ExpNode *pChild2):TwoOpNode(pChild1,pChild2){}
	double getNodeValue(map<string,double> &mVar){ return _pChild1->getNodeValue(mVar)<=_pChild2->getNodeValue(mVar);}
};
class EQUNode:public TwoOpNode
{
public:
	EQUNode(ExpNode *pChild1, ExpNode *pChild2):TwoOpNode(pChild1,pChild2){}
	double getNodeValue(map<string,double> &mVar){ return _pChild1->getNodeValue(mVar)==_pChild2->getNodeValue(mVar);}
};
class NENode:public TwoOpNode
{
public:
	NENode(ExpNode *pChild1, ExpNode *pChild2):TwoOpNode(pChild1,pChild2){}
	double getNodeValue(map<string,double> &mVar){ return _pChild1->getNodeValue(mVar)!=_pChild2->getNodeValue(mVar);}
};
class ADDNode:public TwoOpNode
{
public:
	ADDNode(ExpNode *pChild1, ExpNode *pChild2):TwoOpNode(pChild1,pChild2){}
	double getNodeValue(map<string,double> &mVar){ return _pChild1->getNodeValue(mVar)+_pChild2->getNodeValue(mVar);}
};
class SUBNode:public TwoOpNode
{
public:
	SUBNode(ExpNode *pChild1, ExpNode *pChild2):TwoOpNode(pChild1,pChild2){}
	double getNodeValue(map<string,double> &mVar){ return _pChild1->getNodeValue(mVar)-_pChild2->getNodeValue(mVar);}
};
class MULNode:public TwoOpNode
{
public:
	MULNode(ExpNode *pChild1, ExpNode *pChild2):TwoOpNode(pChild1,pChild2){}
	double getNodeValue(map<string,double> &mVar){ return _pChild1->getNodeValue(mVar)*_pChild2->getNodeValue(mVar);}
};
class DIVNode:public TwoOpNode
{
public:
	DIVNode(ExpNode *pChild1, ExpNode *pChild2):TwoOpNode(pChild1,pChild2){}
	double getNodeValue(map<string,double> &mVar){ return _pChild1->getNodeValue(mVar)/_pChild2->getNodeValue(mVar);}
};
class NEGNode:public ExpNode
{
public:
	NEGNode(ExpNode* pChild1){ _pChild1=pChild1;}
	double getNodeValue(map<string,double> &mVar){  return -_pChild1->getNodeValue(mVar);}
public:
	~NEGNode(){ delete _pChild1;}
};
class NONNode:public ExpNode
{
public:
	NONNode(ExpNode* pChild1){ _pChild1=pChild1;}
	double getNodeValue(map<string,double> &mVar){  return !_pChild1->getNodeValue(mVar);}
public:
	~NONNode(){ delete _pChild1;}
};
class QUENode:public ExpNode
{
public:
	QUENode(ExpNode* pChild1,ExpNode* pChild2,ExpNode* pChild3){ _pChild1=pChild1; _pChild2=pChild2; _pChild3=pChild3;}
	double getNodeValue(map<string,double> &mVar){  return _pChild1->getNodeValue(mVar)?_pChild2->getNodeValue(mVar):_pChild3->getNodeValue(mVar);}
public:
	~QUENode(){ delete _pChild1; delete _pChild2; delete _pChild3;}
};

class StatCalculator
{
public:
	// 若有错误,保存错误提示 
	static string sErrInfo;
	// 获取对应exp的语法树, 有错误，返回NULL
	static ExpNode* getExpTree(string exp);
};

extern char* yytext;
//为了能够在C++程序里面调用C函数，必须把每一个需要使用的C函数，其声明都包括在extern "C"{}块里面，这样C++链接时才能成功链接它们。extern "C"用来在C++环境下设置C链接类型。
extern "C"
{						
	int yywrap(void);
	int yylex(void);	//这个是lex生成的词法分析函数，yacc的yyparse()里会调用它，如果这里不声明，生成的yacc.tab.c在编译时会找不到该函数
	void yyerror(const char *s);
}

#endif

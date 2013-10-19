//lex.l��yacc.y��ͬʹ�õ�ͷ�ļ�
#ifndef MAIN_HPP
#define MAIN_HPP

#include <iostream>//ʹ��C++��
#include <map>
#include <string>
#include <stdio.h>//prdoublef��FILEҪ�õ�
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
	// ���д���,���������ʾ 
	static string sErrInfo;
	// ��ȡ��Ӧexp���﷨��, �д��󣬷���NULL
	static ExpNode* getExpTree(string exp);
};

extern char* yytext;
//Ϊ���ܹ���C++�����������C�����������ÿһ����Ҫʹ�õ�C��������������������extern "C"{}�����棬����C++����ʱ���ܳɹ��������ǡ�extern "C"������C++����������C�������͡�
extern "C"
{						
	int yywrap(void);
	int yylex(void);	//�����lex���ɵĴʷ�����������yacc��yyparse()����������������ﲻ���������ɵ�yacc.tab.c�ڱ���ʱ���Ҳ����ú���
	void yyerror(const char *s);
}

#endif

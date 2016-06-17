#ifndef __CALCULATOR_H__
#define __CALCULATOR_H__

using namespace std;

template<class Data_element>
class Calculator{
public:
	void Run();                              //执行表达式
private:
	Stack<Data_element> OPND;                //操作数栈 
	Stack<char> OPTR;                        //操作符栈          
	int isp(char op);                        //栈内优先数,返回的类型为优先级大小
	int icp(char op);                        //栈外优先数,返回的类型为优先级大小
	bool Get2Operands(double &x,double &y);  //从栈中取出两个操作数
	bool DoOperator(char op);                //形成运算指令，进行运算
	void GetChar(char &ch);                  //从输入流获取一字符ch,并跳过空格及回车
	bool IsOperator(char ch);                //判断ch是否为操作符 
};

template<class Data_element>
void Calculator<Data_element>::Run()
{
	OPND.clear();OPTR.clear();
	char ch,OPTR_top,prior_char,op;double operand;
	OPTR.push('=');
	prior_char='=';  //prior_char表示输入的前一个字符，如为数，则令其值为'0'
	GetChar(ch);
	OPTR.top(OPTR_top);
	if(OPTR.top(OPTR_top)==underflow)
	{
		cout<<"表达式有错!"<<endl;
		return;	
	}; 
	while(OPTR_top!='='||ch!='=')
	{
		if(isdigit(ch)||ch=='.')
		{
			cin.putback(ch);
			cin>>operand;
			OPND.push(operand);
			prior_char='0';
			GetChar(ch);
		}
		else if(!IsOperator(ch))
		{
			cout<<"表达式中出现非法字符!"<<endl;		
			while(cin>>ch,ch!='=');
			return;
		}
		else
		{
			if((prior_char=='='||prior_char=='(')&&(ch=='+'||ch=='-'))OPND.push(0);
			if(isp(OPTR_top)<icp(ch))
			{
				OPTR.push(ch);
			    prior_char=ch;
				GetChar(ch);    
			}
			else if(isp(OPTR_top)>icp(ch))
			{
				OPTR.top(op);OPTR.pop();
				if(!DoOperator(op))return;
			}
			else if(isp(OPTR_top)==icp(ch)&&ch==')')
			{
				OPTR.pop();
			    prior_char=')';
			    GetChar(ch);
			};
		};
		if(OPTR.top(OPTR_top)==underflow)
		{
			cout<<"表达式有错!"<<endl;
			return;	
		}; 
	}
	if(OPND.top(operand) == underflow || OPTR.pop() == underflow)
	{
		cout<<"表达式有错!"<<endl;
		return;	
	} 
	else
	{	// 栈OPND与OPTR.中只能有一个元素
		OPND.pop();
		if (OPND.pop() == success || OPTR.pop() == success)
		{
			cout<<"表达式有错!"<<endl;
			return;	
		}
		cout<<operand<<endl;
		return;	
	}
		
};

template<class Data_element>
int Calculator<Data_element>::isp(char op)
{
	int result;
	switch(op)
	{
		case '=':
			result=0;
			break;
		case '(':
			result=1;
			break;
		case '^':
			result=7;
			break;
		case '*':
		case '/':
		case '%':
			result=5;
			break;
        case '+':
		case '-':
			result=3;
			break;
		case ')':
			result=8;
	}
	return result;
};

template<class Data_element>
int Calculator<Data_element>::icp(char op)
{
	int result;
	switch(op)
	{
		case '=':
			result=0;
			break;
		case '(':
			result=8;
			break;
		case '^':
			result=6;
			break;
		case '*':
		case '/':
		case '%':
			result=4;
			break;
		case '+':
		case '-':
			result=2;
			break;
		case ')':
			result=1; 
	}
	return result;
};

template<class Data_element>
bool Calculator<Data_element>::Get2Operands(double &x, double &y)
{
	if(OPND.empty())
	{
		cout<<"表达式有错!"<<endl;
		return false;
	}
	OPND.top(y);OPND.pop();

	if(OPND.empty())
	{
		cout<<"表达式有错!"<<endl;
		return false;
	}
	OPND.top(x);OPND.pop();

	return true;
};

template<class Data_element>
bool Calculator<Data_element>::DoOperator(char op)
{
	Data_element x,y;
	bool result=Get2Operands(x,y);
    if(result==true)
	{
		switch(op)
		{
			case '+':
				OPND.push(x+y);
				break;
			case '-':
				OPND.push(x-y);
				break;
			case '*':
				OPND.push(x*y);
				break;
			case '/':
				if(y==0)
				{
					cout<<"除数为零!"<<endl;
					return false;
				}
				OPND.push(x/y);
				break;
			case '%':
				if((long)y==0)
				{
					cout<<"除数为零!"<<endl;
					return false;
				}
				OPND.push((long)x % (long)y);
				break;
			case '^':
				OPND.push(pow(x,y));
		}
		return true;
	}
	else return false;
};

template<class Data_element>
void Calculator<Data_element>::GetChar(char &ch)
{
	cin>>ch;
	while(ch==' '||ch=='\n')
		cin>>ch;
};

template<class Data_element>
bool Calculator<Data_element>::IsOperator(char ch)
{
	if(ch=='='||ch=='('||ch=='^'||ch=='*'||
		ch=='/'||ch=='%'||ch=='+'||ch=='-'||ch==')')
		return true;
	else 
		return false;
};

#endif

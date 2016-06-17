#include "Utility.h"
#include "Lkstack.h"
#include "Calculator.h"

int main()
{
  Calculator<double> s;
  char iscontinue='Y';
  while(iscontinue=='Y')
  {
    cout<<"输入表达式(以等号“=”结束):"<<endl;
    s.Run();
    cout<<"是否继续(Y/N)?";
    cin>>iscontinue;
    iscontinue=toupper(iscontinue);
  }
  return 0;
}

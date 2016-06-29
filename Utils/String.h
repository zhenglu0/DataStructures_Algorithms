#ifndef __STRING_H__
#define __STRING_H__

#include "Utility.h"
#include "List.h"
#include "Dlist.h"

// 串类
class String 
{
public:                           
//  抽象数据类型方法声明及重载编译系统默认方法声明:
   
    String();                            // 构造函数 
    ~String();                           // 析构函数
    String(const String &copy);          // 复制构造函数
    String(const char * copy);           // 从C风格串转换构造函数
    String(LList<char> &copy);           // 从线性表转换构造函数

    void operator =(const String &copy);// 赋值语句重载
    const char *c_str() const;           // 将串转换成C风格串

protected:
    char *entries;                       // 串值
    int length;                          // 串长    
};
// 串相关函数
String read_in(istream &input);                        // 从输入流读入串
String read_in(istream &input,char &terminal_char);    // 从输入流读入串，并用terminal_char返回串结束字符
void write(String &s);                                 // 输出串
void strcat(String &add_to, const String &add_on);     // 将串add_on连接到add_to串的后面
void strcpy(String &copy, const String &original);     // 将串original复制到串copy
void strncpy(String &copy, const String &original, int n); // 将串original复制n个字符到串copy 
int strstr(const String &text, const String &target);      // 查找模式串target第一次在目标串text中出现的位置
ostream &operator <<(ostream &outStream, const String &outString); // 重载运算符<<
bool operator ==(const String &first, const String &second); // 重载关系运算符==
bool operator <(const String &first, const String &second);  // 重载关系运算符<
bool operator >(const String &first, const String &second);  // 重载关系运算符>
bool operator <=(const String &first, const String &second); // 重载关系运算符<=
bool operator >=(const String &first, const String &second); // 重载关系运算符>=
bool operator !=(const String &first, const String &second); // 重载关系运算符!=


String::String()
// 构造函数 
{
    length = 0;                // 串长度为0
    entries = NULL;            // 空串
}


String::~String()
// 析构函数
{
    delete []entries;        // 释放串entries
}


String::String(const String &copy)
// 复制构造函数
{
    length = strlen(copy.c_str());    // 串长
    entries = new char[length + 1];    // 分配存储空间
    strcpy(entries, copy.c_str());    // 复制串值
}


String::String(const char *in_string)
// 从C风格串转换构造函数
{
    length = strlen(in_string);        // 串长
    entries = new char[length + 1];    // 分配存储空间 
    strcpy(entries, in_string);        // 复制串值
}


String::String(LList<char> &copy)
// 从线性表转换构造函数
{
    length = copy.leftLength() + copy.rightLength();// 串长
    entries = new char[length + 1];    // 分配存储空间 
    for (int pos = 0; pos < length; pos++) 
    {    // 复制串值
        copy.setPos(pos);
        copy.getValue(entries[pos]);
    }
    entries[length] = '\0';            // 串值以'\0'结束    
}


void String::operator =(const String &copy)
// 赋值语句重载
{
    delete []entries;                // 释放原串存储空间
    length = strlen(copy.c_str());    // 串长
    entries = new char[length + 1];    // 分配存储空间 
    strcpy(entries, copy.c_str());    // 复制串值
}

const char*String::c_str() const
// 将串转换成C风格串
{
    return (const char *)entries;    // 串值类型转换
}


void strcat(String &add_to, const String &add_on)
// 将串add_on连接到add_to串的后面
{
    const char *cfirst = add_to.c_str();    // 指向第一个串
    const char *csecond = add_on.c_str();    // 指向第二个串
    char *copy = new char[strlen(cfirst) + strlen(csecond) + 1];    
        // 分配存储空间
    strcpy(copy, cfirst);                    // 复制第一个串
    strcat(copy, csecond);                    // 连接第二个串
    add_to = copy;                            // 串赋值
    delete []copy;                            // 释放copy
}


String read_in(istream &input)
// 从输入流读入串，并用terminal_char返回串结束字符
{
    LList<char> temp;                    // 临时线性表
    char c;                                    // 临时字符
    while ((c = input.peek()) != EOF &&        // peek()从输入流中取一个字符
                                            // 输入流指针不变
        (c = input.get()) != '\n')            // get()从输入流中取一个字符
                                            // 输入流指针指向下一个字符
    {    // 将输入的字符追加线性表中
        temp.append(c);
    }
    String answer(temp);                    // 构造串
    return answer;                            // 返回串
}


String read_in(istream &input,char &terminal_char)
// 从输入流读入串，并用terminal_char返回串结束字符
{
    LList<char> temp;                    // 临时线性表
    char c;                                    // 临时字符
    while ((c = input.peek()) != EOF && 
        (c = input.get()) != '\n')
    {    // 将输入的字符追加线性表中
        temp.append(c);
    }
    terminal_char=c;    // 用terminal_char返回串结束字符
    String answer(temp);                    // 构造串
    return answer;                            // 返回串
}


void write(String &s)
// 输出串
{
    cout << s.c_str() << endl;                // 输出串值
}


void strcpy(String &copy, const String &original)
// 将串original复制到串copy
{
    const char *coriginal = original.c_str();// 初始串
    char *ccopy = new char[strlen(coriginal) + 1];// 分配存储空间
    strcpy(ccopy, coriginal);                // 复制串
    copy = ccopy;                            // 串赋值
    delete []ccopy;                            // 释放串ccopy
}


void strncpy(String &copy, const String &original, int n)
// 将串original复制n个字符到串copy 
{
    const char *coriginal = original.c_str();// 初始串
    int len = (int)strlen(coriginal) < n ? strlen(coriginal) : n;//目标串长
    char *ccopy = new char[len + 1];        // 分配存储空间
    strncpy(ccopy, coriginal, n);            // 复制串
    ccopy[len] = '\0';                        // 串值以'\0'结束
    copy = ccopy;                            // 串赋值
    delete []ccopy;                            // 释放串ccopy
}


int strstr(const String &text, const String &target)
// 查找模式串target第一次在目标串text中出现的位置
{
    const char *ctext = text.c_str();        // 目标串
    const char *ctarget = target.c_str();    // 模式串
    const char *ptr=strstr(ctext, ctarget);        // 模式匹配
    if (ptr == NULL)
    {    // 匹配失败    
        return -1; 
    }
    else
    {    // 匹配成功    
        return ptr - ctext;
    }
}

ostream &operator <<(ostream &outStream, const String &outString)
// 重载运算符<<
{
    outStream << outString.c_str();
    return outStream;
}

bool operator ==(const String &first, const String &second)
// 重载关系运算符==
{
    return strcmp(first.c_str(), second.c_str()) == 0;
}


bool operator <(const String &first, const String &second)
// 重载关系运算符<
{
    return strcmp(first.c_str(), second.c_str()) < 0;
}


bool operator >(const String &first, const String &second)
// 重载关系运算符>
{
    return strcmp(first.c_str(), second.c_str()) > 0;
}


bool operator <=(const String &first, const String &second)
// 重载关系运算符<=
{
    return strcmp(first.c_str(), second.c_str()) <= 0;
}


bool operator >=(const String &first, const String &second)
// 重载关系运算符>=
{
    return strcmp(first.c_str(), second.c_str()) >= 0;
}

bool operator !=(const String &first, const String &second)
// 重载关系运算符!=
{
    return strcmp(first.c_str(), second.c_str()) != 0;
}

#endif

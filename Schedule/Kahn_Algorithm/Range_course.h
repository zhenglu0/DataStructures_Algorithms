#ifndef __RANGE_COURSE_H__
#define __RANGE_COURSE_H__

#include "Sq_queue.h"
#include "Adjacency_lists.h"

using namespace std;

struct CourseType {
    char course_no[5];        //课程编号
    char course_name[100];    //课程名
    int period;               //学时数
    int term;                  //开课学期
    int indegree;             //入度
};

bool operator ==(const CourseType &first, const CourseType &second)
{
   return strcmp(first.course_no, second.course_no)==0;
}


bool operator !=(const CourseType &first, const CourseType &second)
{
   return strcmp(first.course_no, second.course_no)!=0;
}


template<int terms_num>                   //terms_num表示要排课的学期数
class Range_Course:Digraph<CourseType,1000>{
public:
    Range_Course(ifstream *file_in, ofstream *file_out);
    void read();  //从输入流中输入有关信息，以建立一个有向图
    //用拓扑排序方式进行排课
    void topological_order();
    void write(); //输出课表信息
private:
    ifstream *infile;                    //输入流 
    ofstream *outfile;                   //输出流
    Extended_queue<int> q[terms_num+1];     //对入度为0的结点缓冲队列
    //课表,下标分别表示：学期、节次、星期、课程名
    char course_table[terms_num+1][11][6][100];
    int course_num[terms_num+1];         //每学期的课程数
    int locate_vex(char course_no[]);    //重载定位顶点
    void readln();                       //跳行  
    char get_char();                     //从输入流中输入一有意义的字符  
    bool range(int num,int term,const char course_name[]);//排课
    bool range_one(int &week_day,int term,const char course_name[]);//排一天的1节课
    bool range_two(int &week_day,int term,const char course_name[]);//排一天的2节课
    bool range_three(int &week_day,int term,const char course_name[]);//排一天的3节课
    int find_one(int week_day,int term);//找查一天1节空课
    int find_two(int week_day,int term);//找查一天连续2节空课
    int find_three(int week_day,int term);//找查一天连续3节空课
    void write(const char s[],int l);           //输出指定长度的字符串 
    char *decimal_to_chinese_char(int n,char *s); //用中文表示十进制数 
};


template<int terms_num> 
Range_Course<terms_num>::Range_Course(ifstream *file_in, ofstream *file_out)
{
   infile = file_in;
   outfile = file_out;

   int i,j,k;
   for(i=1;i<=terms_num;i++)                     //学期数
       for(j=1;j<=10;j++)                        //节次
           for(k=1;k<=5;k++)                     //星期   
               strcpy(course_table[i][j][k],""); //清空课表 

}


template<int terms_num> 
void Range_Course<terms_num>::read()
{   //从输入流中输入有关信息，以建立一个有向图
    char c;
    int i,n,v,w,course_total_num=0;
    CourseType course;
    char prio_course_no[5];  //先决课程号

    //下面为从输入流中输入各学期所开的课程数
    (*infile).seekg(0);
    c = get_char();
    while(c<'0'||c>'9'){  //查找数字开始的行
        if(c!='\n')readln();
        c = get_char();
    }
    (*infile).putback(c);
    for(i=1;i<=terms_num;i++){
        (*infile)>>n;
        course_num[i]=n;
        course_total_num+=n;
    }

    //下面为从输入流中输入各课程信息(建立图的顶点信息)
    (*infile).seekg(0);
    c = get_char();
    while(c!=EOF){
        while(c!=EOF&&c!='c'){  //查找以'c'开始的行
            if(c!='\n')readln();
            c = get_char();
        }
        if(c!=EOF){             //处理以'c'开始的行
            (*infile).putback(c);
            (*infile)>>course.course_no;
            (*infile)>>course.course_name;
            (*infile)>>course.period;
            (*infile)>>course.term;
            course.indegree=0;
            readln();
            if(insert_vex(course)!=success){
                cout<<"线性表溢出!"<<endl;
                exit(1);
            }
        }
        c = get_char();
    }

    //下面为从输入流中输入各课程先决条件信息(建立图的弧信息)
    (*infile).seekg(0);
    c = get_char();
    while(c!=EOF){
        while(c!=EOF&&c!='c'){  //查找以'c'开始的行
            if(c!='\n')readln();
            c = get_char();
        }
        if(c!=EOF){             //处理以'c'开始的行
            (*infile).putback(c);
            (*infile)>>course.course_no;
            (*infile)>>course.course_name;
            (*infile)>>course.period;
            (*infile)>>course.term;
            w=locate_vex(course.course_no);
            if(w==-1){
                cout<<"非法顶点!"<<endl;
                exit(1);
            }
  
            c=get_char();
            while(c=='c'){
                (*infile).putback(c);
                (*infile)>>prio_course_no;
                v=locate_vex(prio_course_no);  //表示有一条弧<v,w>
                if(v==-1){
                    cout<<"非法先决条件!"<<endl;
                    exit(1);
                }
                insert_arc(v,w);              //插入弧
                vertices[w].data.indegree++;  //顶点w入度加1  
                c=get_char();
            }
        }
        if(c!='\n')readln();
        c=get_char();
    }

    if(course_total_num!=vex_num()){
        cout<<"各学期所开课程总数与实际课程数不等!"<<endl;
        exit(1);
    }

}


template<int terms_num> 
void Range_Course<terms_num>::topological_order()
{    //用拓扑排序方式进行排课
    int term;   
    int i,j,size0,size_term,v,w;
    
    //建立入度为0顶点的队列q[0..terms_num]
    for(v=0;v<count;v++){  
        if(vertices[v].data.indegree==0)  
            if(q[vertices[v].data.term].append(v)!=success){
                cout<<"队列溢出!"<<endl;
                exit(1);
            }  //入度为0者进入相应学期的队列
    }


    //生成课表
    for(term=1;term<=terms_num;term++){
        size0=q[0].size();
        size_term=q[term].size();
        if(size0+size_term<course_num[term]){
            cout<<"排课时出现冲突!"<<endl;
            exit(1);
        }
        if(size_term>course_num[term]){
            cout<<"排课时出现冲突!"<<endl;
            exit(1);
        } 
        for(i=1;i<=size_term;i++){
            q[term].retrieve(v);
            q[term].serve();
            range(vertices[v].data.period,term,vertices[v].data.course_name);
            j=0;
            while(adj_vex(v,j++,w)==success){
                vertices[w].data.indegree--;
                if(vertices[w].data.indegree==0&&(vertices[w].data.term==0||vertices[w].data.term>term))
                    q[vertices[w].data.term].append(w);
                else if(vertices[w].data.indegree==0&&(vertices[w].data.term>0||vertices[w].data.term<=term)){
                    cout<<"排课时出现冲突!"<<endl;
                    exit(1);
                }
            }
        }
        for(i=1;i<=course_num[term]-size_term;i++){
            q[0].retrieve(v);
            q[0].serve();
            range(vertices[v].data.period,term,vertices[v].data.course_name);
            j=0;
            while(adj_vex(v,j++,w)==success){
                vertices[w].data.indegree--;
                if(vertices[w].data.indegree==0&&(vertices[w].data.term==0||vertices[w].data.term>term))
                    q[vertices[w].data.term].append(w);
                else if(vertices[w].data.indegree==0&&(vertices[w].data.term>0||vertices[w].data.term<=term)){
                    cout<<"排课时出现冲突!"<<endl;
                    exit(1);
                }
            }
        }
    }

}


template<int terms_num> 
void Range_Course<terms_num>::write()
{    //输出课表信息
    int term,i,week_day;
    char s[20],s_tem[20];
    for(term=1;term<=terms_num;term++){
        
        //显示表头
        (*outfile)<<"第"<<decimal_to_chinese_char(term,s_tem)<<"学期课表"<<endl;
        (*outfile)<<endl;
        
        //显示标题
        write("节次",8);
        for(i=1;i<=5;i++)
            write(strcat(strcpy(s,"星期"),decimal_to_chinese_char(i,s_tem)),18);
        (*outfile)<<endl<<endl;
        
        //显示课表内容
        for(i=1;i<=10;i++){
            write(strcat(strcat(strcpy(s,"第"),decimal_to_chinese_char(i,s_tem)),"节"),18);
            for(week_day=1;week_day<=5;week_day++)
                write(course_table[term][i][week_day],25);
            (*outfile)<<endl;

            if(i==2||i==7)(*outfile)<<endl<<"课间休息"<<endl<<endl;
            if(i==5)(*outfile)<<endl<<endl<<"午间休息"<<endl<<endl<<endl;
            if(i==10)(*outfile)<<endl<<"晚自习"<<endl<<endl;
        }

        //各课表之间以较多空行相隔
        (*outfile)<<endl<<endl<<endl<<endl<<endl<<endl;
    }
}


template<int terms_num> 
int Range_Course<terms_num>::locate_vex(char course_no[])
{    //重载定位顶点
    int position=0;
    CourseType course;
    strcpy(course.course_no,course_no);
    while(position<count && vertices[position].data!=course)position++;
    if(position<count)return position;   //定位成功
    else return -1;                      //定位失败
}


template<int terms_num> 
void Range_Course<terms_num>::readln()
{    //跳行   
    char c;
    while ((c = (*infile).peek()) != EOF && (c = (*infile).get()) != '\n');
}


template<int terms_num> 
char Range_Course<terms_num>::get_char()
{    //从输入流中输入一有意义的字符
    char c;
    while ((c = (*infile).peek()) != EOF && ((c = (*infile).get()) == ' '||c=='\t'));
    return c;
}


template<int terms_num> 
bool Range_Course<terms_num>::range(int num,int term,const char course_name[])
{    //排课
    static int week_day=1; //要排课的星期
    int i;
    if(num==1){     //对1节课进行排课
        if(range_one(week_day,term,course_name)==false)return false;
        else{
            week_day=(week_day<4)?(week_day+2):(week_day+2-5);
            return true;
        };
    }
    else if(num%3==0){
        for(i=0;i<num/3;i++)
            if(range_three(week_day,term,course_name)==false)return false;
            else week_day=(week_day<4)?(week_day+2):(week_day+2-5);
        return true;
    }
    else if(num%3==1){
        for(i=0;i<num/3-1;i++)
            if(range_three(week_day,term,course_name)==false)return false;
            else week_day=(week_day<4)?(week_day+2):(week_day+2-5);
        for(i=0;i<2;i++)
            if(range_two(week_day,term,course_name)==false)return false;
            else week_day=(week_day<4)?(week_day+2):(week_day+2-5);
        return true;
    }
    else{
        for(i=0;i<num/3;i++)
            if(range_three(week_day,term,course_name)==false)return false;
            else week_day=(week_day<4)?(week_day+2):(week_day+2-5);
        if(range_two(week_day,term,course_name)==false)    return false;
        else{
            week_day=(week_day<4)?(week_day+2):(week_day+2-5);
            return true;
        }
    }
}


template<int terms_num> 
bool Range_Course<terms_num>::range_one(int &week_day,int term, const char course_name[])
{    //排一天的1节课
    int i,k;
    for(k=0;k<5;k++){
        i=find_one(week_day,term);
        if(i==-1)
            week_day=(week_day<4)?(week_day+2):(week_day+2-5);
        else{
            strcpy(course_table[term][i][week_day],course_name);
            return true; 
        }
    }
    return false;
}


template<int terms_num> 
bool Range_Course<terms_num>::range_two(int &week_day,int term, const char course_name[])
{    //排一天的2节课
    int i,k,week_day_tem;
    for(k=0;k<5;k++){
        i=find_two(week_day,term);
        if(i==-1)
            week_day=(week_day<4)?(week_day+2):(week_day+2-5);
        else{
            strcpy(course_table[term][i][week_day],course_name);
            strcpy(course_table[term][i+1][week_day],course_name);
            return true; 
        }
    }
    //将2节连续上的课分解为两个单独上的1节课
    week_day_tem=week_day;
    if(range_one(week_day_tem,term,"")==true&&range_one(week_day_tem,term,"")==true){
        range_one(week_day,term,course_name);
        range_one(week_day,term,course_name);
        return true;
    }
    return false;
}


template<int terms_num> 
bool Range_Course<terms_num>::range_three(int &week_day,int term, const char course_name[])
{    //排一天的3节课
    int i,k,week_day_tem;
    for(k=0;k<5;k++){
        i=find_three(week_day,term);
        if(i==-1)
            week_day=(week_day<4)?(week_day+2):(week_day+2-5);
        else{
            strcpy(course_table[term][i][week_day],course_name);
            strcpy(course_table[term][i+1][week_day],course_name);
            strcpy(course_table[term][i+2][week_day],course_name);
            return true; 
        }
    }
    //将3节连续上的课分解为2节连续上的课与单独上的1节课
    week_day_tem=week_day;
    if(range_one(week_day_tem,term,"")==true&&range_two(week_day_tem,term,"")==true){
        range_one(week_day,term,course_name);
        range_two(week_day,term,course_name);
        return true;
    }
    return false;
}


template<int terms_num> 
int Range_Course<terms_num>::find_one(int week_day,int term)
{    //找查一天1节空课,返回有空的节次，返回-1表示课已排满
    int i;
    for(i=1;i<=8;i++)   //检查第1、3大节是否有空
        if((i<=2||(i>=6&&i<=7))&&strlen(course_table[term][i][week_day])==0)
            return i;
    for(i=1;i<=8;i++)   //检查第2、4大节是否有空
        if(((i>=3&&i<=5)||(i>=8))&&strlen(course_table[term][i][week_day])==0)
            return i;
    return -1;
}


template<int terms_num> 
int Range_Course<terms_num>::find_two(int week_day,int term)
{    //找查一天连续2节空课,返回连续2节课有空的第1节课的节次，返回-1表示课已排满
    if(strlen(course_table[term][1][week_day])==0&&strlen(course_table[term][2][week_day])==0)
        return 1;   //检查第1大节是否有空
    if(strlen(course_table[term][6][week_day])==0&&strlen(course_table[term][7][week_day])==0)
        return 6;   //检查第3大节是否有空
    if(strlen(course_table[term][3][week_day])==0&&strlen(course_table[term][4][week_day])==0)
        return 3;   //检查第2大节是否有空
    if(strlen(course_table[term][4][week_day])==0&&strlen(course_table[term][5][week_day])==0)
        return 4;   //检查第2大节是否有空
    if(strlen(course_table[term][8][week_day])==0&&strlen(course_table[term][9][week_day])==0)
        return 8;   //检查第4大节是否有空
    if(strlen(course_table[term][9][week_day])==0&&strlen(course_table[term][10][week_day])==0)
        return 9;   //检查第4大节是否有空
    return -1;
}


template<int terms_num> 
int Range_Course<terms_num>::find_three(int week_day,int term)
{    //找查一天连续3节空课,返回连续3节课有空的第1节课的节次，返回-1表示课已排满
    if(strlen(course_table[term][3][week_day])==0&&strlen(course_table[term][4][week_day])==0&&strlen(course_table[term][5][week_day])==0)
        return 3;   //检查第2大节是否有空
    if(strlen(course_table[term][8][week_day])==0&&strlen(course_table[term][9][week_day])==0&&strlen(course_table[term][10][week_day])==0)
        return 8;   //检查第3大节是否有空
    return -1;
}


template<int terms_num> 
void Range_Course<terms_num>::write(const char s[],int l)
{    //输出指定长度的字符串 
    int i;
    (*outfile)<<s;
    for(i=0;i<l-strlen(s);i++)(*outfile)<<" ";
}


template<int terms_num> 
char *Range_Course<terms_num>::decimal_to_chinese_char(int n,char *s)
{    //用中文表示十进制数 
    char digital[11][4]={"〇","一","二","三","四","五","六","七","八","九","十"};
    s[0]='\0';

    if(n<0){
        strcat(s,"负");
        n=-n;
    }
    n=n%100;      //假设n最多是一个两位数
    if(n/10>1){
        strcat(s,digital[n/10]);
        strcat(s,digital[10]);
        if(n%10>0)
            strcat(s,digital[n%10]);
    }
    else if(n/10==1){
        strcat(s,digital[10]);
        if(n%10>0)
            strcat(s,digital[n%10]);
    }
    else strcpy(s,digital[n%10]);
    return s;
}

#endif

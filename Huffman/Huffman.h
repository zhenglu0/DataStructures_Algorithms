#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

#include "../Utils/Utility.h"
#include "../Utils/Lkstack.h"

using namespace std;

const unsigned int n=256;             //字符数
const unsigned int m=256*2-1;         //结点总数 


struct HTNode{                        //压缩用Huffman树结点
    unsigned long weight;             //字符频度（权值）
    unsigned int parent,lchild,rchild;
};


struct Buffer{                        //字节缓冲压缩用Huffman树  
    char ch;                          //字节
    unsigned int bits;                //实际比特数     
};


class HuffmanTree{                    //Huffman树  
public:
  void EnCode();                      //编码
  void DeCode();                      //译码
private:  
  HTNode HT[m+1];                     //树结点表(HT[1]到HT[m])
  char Leaf[n+1];                     //叶结点对应字符(leaf[1]到leaf[n])
  char *HuffmanCode[n+1];             //叶结点对应编码(*HuffmanCode[1]到*HuffmanCode[n])
  unsigned int count;                 //频度大于零的字符数     
  unsigned int char_index[n];         //字符对应在树结点表的下标(char_index[0]到char_index[n-1])
  unsigned long size;                 //被压缩文件长度
  FILE *infp,*outfp;                  //输入/出文件
  Buffer buf;                         //字符缓冲
  void Statistics();                  //统计字符出现频度并过滤掉频度为零的字符
  void Select(unsigned int k, unsigned int &s1, unsigned int &s2); //在HT[0]~HT[k]中选择parent为-1，树值最小的两个结点s1,s2
  void Write(unsigned int bit);       //向outfp中写入一个比特
  void Write(unsigned int num,unsigned int k);//向outfp中写入k个比特
  void WriteToOutfp();                //强行写入outfp
  void Read(unsigned int &bit);       //从infp中读出一个比特
  void Read(unsigned int &num,unsigned int k);//从infp中读出k个比特
  int  NToBits(unsigned int num);     //0~num之间的整数用二进位表示所需的最少位数
  void CreateFromCodeFile();          //由编码文件中存储的树结构建立Huffman树 
  void CreateFromSourceFile();        //由被压缩文件建立Huffman树,将树结构存入编码文件的文件头部中,并求每个字符的Huffman编码
};


void HuffmanTree::EnCode()            //编码
{
    char infName[256],outfName[256];
    
    cout<<"Please input source file name(size less than 4GB):";  //被压缩文件最多4GB
    cin>>infName;                  
    if((infp=fopen(infName,"rb"))==NULL){
        cout<<"Can not open file:"<<infName<<endl;
        system("PAUSE");
        exit(1);
    }

    fgetc(infp);
    if(feof(infp)){
        cout<<"Empty source file:"<<infName<<endl;
        system("PAUSE");
        exit(1);
    }

    cout<<"Please input code file name:";
    cin>>outfName;                  
    if((outfp=fopen(outfName,"wb"))==NULL){
        cout<<"Can not open file:"<<outfName<<endl;
        system("PAUSE");
        exit(1);
    }
    
    cout<<"Pocessing..."<<endl;

    unsigned char ch;
    unsigned int i,c;
    for(i=0;i<=n;i++)HuffmanCode[i]=NULL;
    CreateFromSourceFile();  

    rewind(infp);
    ch=fgetc(infp);
    while(!feof(infp)){
        c=char_index[ch];
        for(i=0;i<strlen(HuffmanCode[c]);i++){
            if(HuffmanCode[c][i]=='0')Write(0);
            else Write(1);
        } 
        ch=fgetc(infp);
    }
    WriteToOutfp();
    
    fclose(infp);fclose(outfp);

    cout<<"Process end."<<endl<<endl;

}


void HuffmanTree::DeCode()
{
    char infName[256],outfName[256];
    
    cout<<"Please input code file name:";
    cin>>infName;                  
    if((infp=fopen(infName,"rb"))==NULL){
        cout<<"Can not open file:"<<infName<<endl;
        system("PAUSE");
        exit(1);
    }
    fgetc(infp);
    if(feof(infp)){
        cout<<"Empty code file:"<<infName<<endl;
        system("PAUSE");
        exit(1);
    }

    cout<<"Please input target file name:";
    cin>>outfName;                  
    if((outfp=fopen(outfName,"wb"))==NULL){
        cout<<"Can not open file:"<<outfName<<endl;
        system("PAUSE");
        exit(1);
    }
    
    cout<<"Pocessing..."<<endl;

    unsigned int bit,c,i;
    CreateFromCodeFile();                    //建立Huffman树
    Read(bit);
    for(i=0;i<size;i++){   
        c=2*count-1;                         //2*count-1为根结点的下标
        while((HT[c].lchild!=0||HT[c].rchild!=0)&&(feof(infp)==0)){
            if(bit==0)c=HT[c].lchild;
            else c=HT[c].rchild;
            Read(bit);  
        }
        fputc(Leaf[c],outfp);                //将字符写入outfp中
    }

    fclose(infp);fclose(outfp);

    cout<<"Process end."<<endl<<endl;
}


//统计字符出现频度并过滤掉频度为零的字符
void HuffmanTree::Statistics()
{
    unsigned int i,cha;
    for(i=1;i<=n;i++)HT[i].weight=0;
    size=0;
    rewind(infp);
    cha=fgetc(infp);
    while(!feof(infp))    //统计字符出现频度
    {
        HT[cha+1].weight++;
        size++;
        cha=fgetc(infp);
    }

    count=0;
    for(cha=0;cha<n;cha++){    //过滤掉频度为零的字符
        if(HT[cha+1].weight>0){
            count++;
            Leaf[count]=cha;
            HT[count].weight=HT[cha+1].weight;
            char_index[cha]=count;
        }
    }
}


//s1,s2为权值最小的根,且s1的权值小于s2的权值
void HuffmanTree::Select(unsigned int k, unsigned int &s1, unsigned int &s2)
{
    unsigned int root_count=0;            //根结点数;
    unsigned int root_index[n];           //根结点下标;
    unsigned int tem,i,j;   
    for(i=1;i<=k;i++)
        if(HT[i].parent==0)
            root_index[root_count++]=i;
    s1=root_index[0];s2=root_index[1];
    if(HT[s1].weight>HT[s2].weight){
        tem=s1;s1=s2;s2=tem;
    }

    for(i=2;i<root_count;i++){
        j=root_index[i];
        if(HT[j].weight<HT[s2].weight){
            s2=j;
            if(HT[s1].weight>HT[s2].weight){
                tem=s1;s1=s2;s2=tem;
            }
        }
    }
}


void HuffmanTree::Write(unsigned int bit)                 //向outfp中写入一个比特
{
    buf.bits++;
    buf.ch=(buf.ch<<1)+bit;
    if(buf.bits==8){  //缓冲区已满,写入outfp
        fputc(buf.ch,outfp);
        buf.bits=0;
        buf.ch=0;
    }
}


void HuffmanTree::Write(unsigned int num,unsigned int k)   //向outfp中写入k个比特
{
    Stack<unsigned int> s;
    unsigned int i,bit;
    for(i=1;i<=k;i++){
        s.push(num & 1);
        num=(num>>1);
    }

    for(i=1;i<=k;i++){
        s.top(bit);
        Write(bit);
        s.pop();
    }

}


void HuffmanTree::WriteToOutfp()            //强行写入outfp
{
    unsigned int l=buf.bits;
    if(l>0)
        for(unsigned int i=0;i<8-l;i++)Write(0);
}


void HuffmanTree::Read(unsigned int &bit)                //从infp中读出一个比特
{
    if(buf.bits==0){
        buf.ch=fgetc(infp);
        buf.bits=8;
    }
    bit=(buf.ch & 128)>>7;    
    buf.ch=buf.ch<<1;
    buf.bits--;
}


void HuffmanTree::Read(unsigned int &num,unsigned int k)   //从infp中读出k个比特
{
    unsigned int bit;
    num=0;
    for(unsigned int i=0;i<k;i++){
        Read(bit);
        num=(num<<1)+bit;
    }
}


int HuffmanTree::NToBits(unsigned int num)    //0~num之间的整数用二进位表示所需的位数
{
    unsigned int l=0,power=1;
    while(power<=num){
        l++;power=power*2;
    }
    return l;
}


void HuffmanTree::CreateFromCodeFile()        //由编码文件中存储的树结构建立Huffman树 
{
    buf.bits=0;                               //清空缓冲区
    buf.ch=0;
    unsigned int num,l,i;
    rewind(infp);
    fread(&size,sizeof(unsigned long),1,infp);
    Read(count,8);
    count=count+1; 
    for(i=1;i<=count;i++)
        fread(&Leaf[i],sizeof(char),1,infp);
    l=NToBits(2*count-1);
    for(i=1;i<=count;i++){
        HT[i].lchild=0;
        HT[i].rchild=0;
    }
    for(i=count+1;i<=2*count-1;i++){
        HT[i].lchild=(Read(num,l),num);
        HT[i].rchild=(Read(num,l),num);
    }
}


//由被压缩文件建立Huffman树,将树结构存入编码文件的文件头部中,并求每个字符的Huffman编码
void HuffmanTree::CreateFromSourceFile()
{
    Statistics();//统计字符出现频度并过滤掉频度为零的字符

    //由被压缩文件建立Huffman树
    unsigned int i,s1,s2;
    for(i=1;i<=count;i++)HT[i].parent=HT[i].lchild=HT[i].rchild=0;
    for(i=count+1;i<=2*count-1;i++){//建立Huffman树
        Select(i-1,s1,s2);          //选择parent为0，权值最小的两个结点s1,s2
        HT[s1].parent=HT[s2].parent=i; 
        HT[i].parent=0;HT[i].lchild=s1;HT[i].rchild=s2; 
        HT[i].weight=HT[s1].weight+HT[s2].weight;
    }

    //将树结构存入编码文件的文件头部中
    unsigned int l;
    buf.bits=0;                 //清空缓冲区
    buf.ch=0;
    rewind(outfp);
    fwrite(&size,sizeof(unsigned int),1,outfp);
    Write(count-1,8);
    for(i=1;i<=count;i++)
        fwrite(&Leaf[i],sizeof(char),1,outfp);
    l=NToBits(2*count-1);
    for(i=count+1;i<=2*count-1;i++){
        Write(HT[i].lchild,l);
        Write(HT[i].rchild,l);
    }

    //求每个字符的Huffman编码
    unsigned int start,c,f;
    char *cd;                 //编码临时变量
    for(i=1;i<=n;i++) {
        if(HuffmanCode[i]!=NULL){
            delete []HuffmanCode[i];  //释放存储空间
            HuffmanCode[i]=NULL;
        }
    }
    cd=new char[count];       //分配求编码的工作空间
    cd[count-1]='\0';         //编码结束符
    for(i=1;i<=count;i++){    //逐位求Huffman编码
        start=count-1;        //编码结束符位置
        for(c=i,f=HT[i].parent;f!=0;c=f,f=HT[c].parent) //从叶到根求编码
            if(HT[f].lchild==c)cd[--start]='0';
            else cd[--start]='1';
        HuffmanCode[i]=new char[count-start];    //为第i个字符编码分配空间
        strcpy(HuffmanCode[i],&cd[start]);       //从cd复制编码到HuffmanCode
    }
    delete []cd;
 
}

#endif

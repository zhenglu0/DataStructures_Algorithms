#include "Utility.h"
#include "List.h"
#include "Dlist.h"
#include "String.h"
#include "Editor.h"


int main(int argc, char *argv[])    // 命令行参数
// 前提条件: 在命令行中给出输入文件和输出文件
// 操作结果: 读输入文件各行到文本缓存中，执行简单的行编辑,并写文本缓存到输出文件中
{
    char infName[256],outfName[256];

    if (argc < 2) 
    {    // argc < 2表无输入文件名
        cout << "请输入文件名(缺省: file_in.txt):";
        strcpy(infName, read_in(cin).c_str());
        if (strlen(infName) == 0)
        {    // infName为空
            strcpy(infName, "file_in.txt");
        }
    }
    else
    {    // 将argv[1]复制给infName
        strcpy(infName, argv[1]);
    }

    ifstream file_in(infName);   // 定义并打开输入文件流
    if (!file_in.is_open()) {
        cout << "打开输入文件" << infName << "失败" << endl;
        exit(1);
    }
    
    if (argc < 3) 
    {        // argc < 3表无输出文件名
        cout << "请输出文件名(缺省: file_out.txt):";
        strcpy(outfName, read_in(cin).c_str());
        if (strlen(outfName) == 0)
        {    // outfName为空
            strcpy(outfName, "file_out.txt");
        }
    }
    else
    {    // 将argv[2]复制给outfName
        strcpy(outfName,argv[2]);
    }

    ofstream file_out(outfName);   // 定义并打开输出文件流.
    if (!file_out.is_open()) {
        cout << "打开输入文件" << outfName << "失败" << endl;
        exit(2);
    }

    Editor buffer(&file_in, &file_out);    // 定义文本缓存
    while (buffer.get_command())
    {    // 接收并执行用户操作命令
        buffer.run_command();
    }

    return 0;
}

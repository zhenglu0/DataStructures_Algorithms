#include "Huffman.h"

int main(int argc, char *argv[]) 
{
    HuffmanTree hf;
    char c=0;
    while(c!='3')  
    {
        cout<<endl<<"1.Huffman compress.";
        cout<<endl<<"2.Huffman decompress.";
        cout<<endl<<"3.Exit.";
        cout<<endl<<"Please select:";
        cin>>c;
        switch(c)
        {
            case '1':   
                hf.EnCode();
                break;
            case '2':
                hf.DeCode();
        }
    }
    return 0;
}

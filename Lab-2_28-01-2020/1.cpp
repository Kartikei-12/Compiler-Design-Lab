// Author: Kartikei Mittal
// Find and print comment

#include<iostream>
#include<fstream>
#include<exception>

using namespace std;


string read_file(string file_name)
{
    char temp; string data = "";
    ifstream file(file_name.c_str(), ios::binary);
    if(!file.is_open()) throw "Input file not found!";
    while(!file.eof())
    {
        file.get(temp);
        data += temp;
    }file.close();
    return data;
}

string get_comment(string source_code)
{
    int start = 0, end = source_code.length() - 1;

    for(int i=0; i<source_code.length(); ++i)
        if(source_code[i] == '/')
        {
            start = i + 2;
            if(source_code[i+1] == '/')
                i = source_code.length();
            else if(source_code[i+1] == '*')
            {
                i += 2;
                while(!(source_code[i] == '*' && source_code[i+1] == '/'))
                    ++i;
                end = i - 1;
                i = source_code.length();
            }
            else;
        }
    return string(source_code.begin()+start, source_code.begin()+end);
}

int main() try
{
    string file, source_code;

    cout<<"Enter source code file name: ";
    cin>>file;

    source_code = read_file(file);

    cout<<get_comment(source_code);

    return 0;
}
catch(string &e) { cerr<<"Error: "<<e<<endl; }
catch(exception &e) { cerr<<e.what()<<endl; }
catch(...) { cerr<<"\n\nSomething Wrong\n\n"<<endl; }

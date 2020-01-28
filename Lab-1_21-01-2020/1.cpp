// Author: Kartikei Mittal
// Lexical analyzer 

#include<iostream>
#include<fstream>
#include<exception>
#include<vector>
#include<algorithm>

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

string get_clean_code(string org_code)
{
    for(int64_t i = 0; i<org_code.length(); ++i)
    {
        if(org_code[i] == '/' && org_code[i+1] == '/')
            while(org_code[i] != '\n')
                org_code.erase(org_code.begin() + i);

        if(org_code[i] == '/' && org_code[i+1] == '*')
        {
            while(org_code[i] != '*' || org_code[i+1] != '/')
                org_code.erase(org_code.begin() + i);
            org_code.erase(org_code.begin() + i);
            org_code.erase(org_code.begin() + i);
        }
    }
    return org_code;
}

vector<string> get_token_list(string code)
{
    string temp = "", temp2 = "";
    vector<string> token_list;

    code.erase(std::remove(code.begin(), code.end(), '\n'), code.end());
    code.erase(std::remove(code.begin(), code.end(), '\r'), code.end());

    for(int64_t i = 0; i<code.length(); ++i)
    {
        if(code[i] == '"')
        {
            if(temp.length() > 0)
                token_list.push_back(temp);
    
            temp = '"';
            ++i;
            while(code[i] != '"')
                temp += code[i++];
            temp += '"';
            token_list.push_back(temp); // String in ""
            temp = "";
            ++i;
        }

        if(!isalnum(code[i]))
        {
            if(temp.length() > 0)
                token_list.push_back(temp);
            temp = "";

            if(code[i] != ' ')
            {    
                while(!isalnum(code[i]) && code[i] != '"' && code[i] != ' ')
                    temp += code[i++];
                token_list.push_back(temp);
                temp = "";
                --i;
            }    
        }
        else
            temp += code[i];
    }
    
    return token_list;
}

/*


operators = ['<<', '>>', '!=', '==', '>=', '::',
             '<=', '||', '&&', '-=', '/=',
             '*=', '%=', '+=', '--', '++',
             '+', '-', '*', '/', '%', 
             '=','>', '<', '!', '&', '|', '~']


*/


int64_t count_keywords(vector<string> token_list)
{
    int ans = 0;
    vector<string> keywords = {
        "auto", "const", "double", "float", "int",
        "short", "struct", "unsigned", "break","continue",
        "else","for", "long", "signed","switch", 
        "void", "case", "default", "enum", "goto", 
        "register","sizeof","typedef","volatile",
        "char", "do", "extern", "if", "return",
        "static", "union", "while", "asm", "dynamic_cast",
        "namespace", "reinterpret_cast", "try", "bool", "explicit",
        "new", "static_cast", "typeid", "catch", "false",
        "operator", "template", "typename", "class", "friend",
        "private", "this", "using", "const_cast", "inline",
        "public", "throw", "virtual", "delete", "mutable", 
        "protected", "true", "wchar_t", "include"
    };
    for(string token : token_list)
    {
        if(find(keywords.begin(), keywords.end(), token) != keywords.end())
            ++ans;
    }
    return ans;
}

int main() try
{
    int num_keywords;
    string file, source_code, clean_code;
    vector<string> token_list;

    cout<<"Enter source code file name: ";
    cin>>file;

    source_code = read_file(file);

    clean_code = get_clean_code(source_code);

    token_list = get_token_list(clean_code);
    cout<<"Number of tokens: "<<token_list.size()<<endl;

    cout<<"**************************"<<endl;
    for(string str : token_list)
        cout<<str<<endl;
    cout<<"**************************"<<endl;

    cout<<"Number of keywords: "<<count_keywords(token_list);


    return 0;
}
catch(string &e) { cerr<<"Error: "<<e<<endl; }
catch(exception &e) { cerr<<e.what()<<endl; }
catch(...) { cerr<<"\n\nSomething Wrong\n\n"<<endl; }

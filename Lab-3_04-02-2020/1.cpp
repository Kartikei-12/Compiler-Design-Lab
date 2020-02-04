// Author: Kartikei Mittal
// Recursive Desent Parser for
// $ => EPSILON
// Grammer:
// E -> TS
// S -> +TS|$
// T -> FP
// P -> *FP|$
// F -> id|(E)

// T dash = P
// E dash = S


#include<iostream>
#include<fstream>
#include<exception>
#include<vector>
#include<algorithm>

using namespace std;

#define EPSILON "$"


void vector_print(vector<string> vec)
{
    cout<<" [ ";
    for(string ele:vec)
        cout<<ele<<", ";
    cout<<" ]";
}

bool is_epsilon(string ch)
{
    if(ch != EPSILON)
        return false;
    return true;
}

bool is_id(string ident)
{
    if(!isalpha(ident[0]) && ident[0] != '_')
        return false;
    for(int i=1; i<ident.length(); ++i)
        if(ident[i] != '_' && (!isalnum(ident[i])))
            return false;
    return true;
}

// bool is_id(const string& s)
// {
//     return !s.empty() && find_if(s.begin(), 
//         s.end(), [](unsigned char c) { return !isdigit(c); }) == s.end();
// }

bool is_id_part(char ch) { return isdigit(ch); }

vector<string> split(string s, string delimiter)
{
    vector<string> list;
    size_t pos = 0;
    string token;
    while ((pos = s.find(delimiter)) != string::npos) {
        token = s.substr(0, pos);
        list.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    list.push_back(s);
    return list;
}


vector<string> distinguish_input(string in)
{
    string temp = "", temp2 = "";
    vector<string> ans;
    for(char ch : in)
        if(is_id_part(ch))
            temp += ch;
        else
        {
            ans.push_back(temp);
            temp2 = ch;
            ans.push_back(temp2);
            temp = "";
        }
    ans.push_back(temp);
    ans.push_back(EPSILON);
    return ans;
}

void F();
void E();
void T();
void S();
void P();
int lookahead;
vector<string> input;

void match(string ch)
{
    cout<<"call match for "<<ch<<endl;
    string ch_ = input[lookahead];
    if(ch_ == ch || (is_id(ch) && is_id(ch_)))
        ++lookahead;
    else
        throw string("Parse Error at in match:"+ch);
}

void E()
{
    cout<<"call E."<<endl;
    string ch = input[lookahead];
    if(ch == "(" || is_id(ch))
    {
        T(); S();
    }
    else
        throw string("Parse Error at in E:"+ch);
    cout<<"end E."<<endl;
}

void T()
{
    cout<<"call T."<<endl;
    string ch = input[lookahead];
    if(ch == "(" || is_id(ch))
    {
        F(); P();
    }
    else
        throw string("Parse Error at in T:"+ch);
    cout<<"end T."<<endl;
}

void S()
{
    cout<<"call S."<<endl;
    string ch = input[lookahead];
    if(ch == "+")
    {
        match("+");
        T(); S();
    }
    else if(is_epsilon(ch));
    cout<<"end S."<<endl;
}

void F()
{
    cout<<"call F."<<endl;
    string ch = input[lookahead];
    if(is_id(ch))
        match(ch);
    else if(ch == "(")
    {
        match("(");
        E();
        match(")");
    }
    else
        throw string("Parse Error at in F:"+ch);
    cout<<"end F."<<endl;
}

void P()
{
    cout<<"call P."<<endl;
    string ch = input[lookahead];
    if(ch == "*")
    {
        match("*");
        F();
        P();
    }
    else if(is_epsilon(ch));
    cout<<"end P."<<endl;
}

int main() try
{
    string str;
    vector<string> in_string;
    cout<<"Enter string: ";
    cin>>str;
    in_string = distinguish_input(str);
    vector_print(in_string);
    cout<<endl;
    
    lookahead = 0;
    input = in_string;
    E();
    cout<<"String accepted."<<endl;

    return 0;
}
catch(string &e) { cerr<<"Error: "<<e<<endl; }
catch(exception &e) { cerr<<e.what()<<endl; }
catch(...) { cerr<<"\n\nSomething Wrong\n\n"<<endl; }

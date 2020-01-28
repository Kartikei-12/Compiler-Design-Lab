// Author: Kartikei Mittal
// Regular expression a*|a*b+

#include<iostream>
#include<fstream>
#include<exception>

using namespace std;

bool is_regular_exp(string str)
{
    bool is_b_reached = false;
    for(int i=0; i<str.length(); ++i)
    {
        if(str[i] != 'a' &&  str[i] != 'b')
            return false;
        if(str[i] == 'b')
            is_b_reached = true;
        if(is_b_reached && str[i] == 'a')
            return false;
    }
    return true;
}

int main() try
{
    string exp;

    cout<<"Enter string: ";
    cin>>exp;

    cout<<is_regular_exp(exp);

    return 0;
}
catch(string &e) { cerr<<"Error: "<<e<<endl; }
catch(exception &e) { cerr<<e.what()<<endl; }
catch(...) { cerr<<"\n\nSomething Wrong\n\n"<<endl; }

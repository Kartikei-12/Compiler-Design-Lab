// Author: Kartikei Mittal
// is valid identifier

#include<iostream>
#include<fstream>
#include<exception>

using namespace std;

bool is_valid_identifier(string ident)
{
    if(!isalpha(ident[0]) && ident[0] != '_')
        return false;
    for(int i=1; i<ident.length(); ++i)
        if(ident[i] != '_' && (!isalnum(ident[i])))
            return false;
    return true;
}

int main() try
{
    string code;

    cout<<"Enter identifier: ";
    cin>>code;

    cout<<is_valid_identifier(code);

    return 0;
}
catch(string &e) { cerr<<"Error: "<<e<<endl; }
catch(exception &e) { cerr<<e.what()<<endl; }
catch(...) { cerr<<"\n\nSomething Wrong\n\n"<<endl; }

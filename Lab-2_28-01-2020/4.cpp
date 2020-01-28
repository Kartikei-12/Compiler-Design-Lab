// Author: Kartikei Mittal
// First of the grammer
// Assumption:
//      $ Equivalent to Epsilon/Empty
//      small chracters terminal
//      capital chracter non terminal


#include<iostream>
#include<fstream>
#include<exception>
#include<vector>
#include<unordered_map>
#include<algorithm>

using namespace std;

#define EPSILON '$'

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

bool is_epsilon(string ch)
{
    if(ch[0] != EPSILON)
        return false;
    return true;
}

bool is_terminal(char ch)
{
    if(!islower(ch))
        return false;
    return true;
}

void vector_print(vector<auto> vec)
{
    cout<<"[";
    for(auto ele:vec)
        cout<<ele<<", ";
    cout<<"]";
}

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

class Grammer
{
    vector<char> non_terminal_list;
    unordered_map<char, vector<string>> grammer;
public:
    Grammer(string gramm_str)
    {
        vector<string> production_list = split(gramm_str, "\n");
        
        non_terminal_list.clear();
        
        for(string production : production_list)
        {
            non_terminal_list.push_back(production[0]);
            grammer[production[0]] = split(
                string(
                    production.begin() + 2,
                    production.end() - 1
                ), "|"
            );
        }
    }

    vector<char> get_first_of_non_terminal(char nonT)
    {
        string expression;
        vector<char> ans, first_ch;
        vector<string> pro_list = grammer[nonT];
        for(int i=0; i<pro_list.size(); ++i)
        {
            expression = pro_list[i];
            if(is_terminal(expression[0]))
                ans.push_back(expression[0]);
            else if(is_epsilon(expression))
                ans.push_back(EPSILON);
            else
            {
                int j = 1;
                first_ch = get_first_of_non_terminal(expression[0]);
                ans.insert(ans.end(), first_ch.begin(), first_ch.end());
                while(
                    find(
                        first_ch.begin(),
                        first_ch.end(),
                        EPSILON
                    ) != first_ch.end() &&
                    j<expression.length()
                )
                {
                    if(is_terminal(expression[j]))
                    {
                        ans.push_back(expression[j]);
                        break;
                    }
                    first_ch = get_first_of_non_terminal(expression[j]);
                    ++j;
                    ans.insert(ans.end(), first_ch.begin(), first_ch.end());
                }
            }
        }
        return ans;
    }

    void print_first()
    {
        cout<<"Grammer first:-"<<endl;
        for(char nonT : non_terminal_list)
        {
            cout<<"\t"<<nonT<<" -> ";
            vector_print(get_first_of_non_terminal(nonT));
            cout<<endl;
        }
        cout<<"-----------------------"<<endl;
    }

    void print()
    {
        cout<<"Grammer:-"<<endl;
        for(char nonT : non_terminal_list)
        {
            cout<<"\t"<<nonT<<" -> ";
            vector_print(grammer[nonT]);
            cout<<endl;
        }
        cout<<"-----------------------"<<endl;
    }

    
};

int main() try
{
    string file, grammer;

    cout<<"Enter grammer file: ";
    cin>>file;

    grammer = read_file(file);

    Grammer g(grammer);

    g.print();
    g.print_first();
    return 0;
}
catch(string &e) { cerr<<"Error: "<<e<<endl; }
catch(exception &e) { cerr<<e.what()<<endl; }
catch(...) { cerr<<"\n\nSomething Wrong\n\n"<<endl; }

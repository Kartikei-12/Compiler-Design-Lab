// Author: Kartikei Mittal
// First of the grammer
// Assumption:
//      $ Equivalent to Epsilon/Empty
//      small chracters terminal
//      capital chracter non terminal


#include<iostream>
#include<fstream>
#include<exception>
#include<set>
#include<vector>
#include<unordered_map>
#include<algorithm>

using namespace std;

#define STRING_END '$'
#define EPSILON "!"
#define DEPTH_LIMIT 1000
#define REMOVE(vec, element) vec.erase(find(vec.begin(), vec.end(), element));
#define FIND(vec, element) find(vec.begin(), vec.end(), element) != vec.end()


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

bool is_epsilon(string ch) { return ch == EPSILON; }

bool is_terminal(char ch) { return !isupper(ch); }

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

    vector<char> get_first_of_expression(string expression, uint64_t depth = 0)
    {
        vector<char> ans, first_ch;
        // if(expression == "")
        // {
        //     ans.push_back(STRING_END);
        //     return ans;   
        // }
        if(is_terminal(expression[0]))
            ans.push_back(expression[0]);
        else if(is_epsilon(expression))
            ans.push_back(EPSILON[0]);
        else
        {
            int j = 1;
            first_ch = get_first_of_non_terminal(expression[0], depth + 1);
            ans.insert(ans.end(), first_ch.begin(), first_ch.end());
            while(FIND(first_ch, EPSILON[0]) && j < expression.length())
            {
                REMOVE(ans, EPSILON[0]);
                if(is_terminal(expression[j]))
                {
                    ans.push_back(expression[j]);
                    break;
                }
                first_ch = get_first_of_non_terminal(expression[j], depth + 1);
                ++j;
                ans.insert(ans.end(), first_ch.begin(), first_ch.end());
            }
        }
    }

    vector<char> get_first_of_non_terminal(char nonT, uint64_t depth = 0)
    {
        string expression;
        vector<char> ans, first_ch;
        vector<string> pro_list = grammer[nonT];

        if(depth == DEPTH_LIMIT) // Break in case Cyclic loop
        {
            cout<<"Recursive depth reached aborting.";
            ans.clear();
            return ans;
        }

        for(int i=0; i<pro_list.size(); ++i)
        {
            vector<char> new_ans = get_first_of_expression(pro_list[i], depth);
            ans.insert(ans.end(), new_ans.begin(), new_ans.end());
        }
        set<char> s(ans.begin(), ans.end());
        ans.assign(s.begin(), s.end());
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

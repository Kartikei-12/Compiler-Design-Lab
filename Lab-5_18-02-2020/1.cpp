// Author: Kartikei Mittal
// LL(1) Parser
// Assumption:
//      capital chracter non terminal others terminal

#include<iostream>
#include<iomanip>
#include<fstream>
#include<exception>
#include<set>
#include<vector>
#include<stack>
#include<unordered_map>
#include<algorithm>

using namespace std;

#define ID_SYM "i"
#define ARROW "->"
#define EPSILON "!"
#define SEPRATOR "|"
#define STRING_END "$"
#define EPSILON_SEPRATOR "!|"
#define DEPTH_LIMIT 1000

#define REVERSE(str) reverse(str.begin(), str.end())
#define COUNT(vec, element) count(vec.begin(), vec.end(), element) 
#define REMOVE(vec, element) vec.erase(find(vec.begin(), vec.end(), element));
#define FIND(vec, element) find(vec.begin(), vec.end(), element) != vec.end()
#define APPEND(original, new_) original.insert(original.end(), new_.begin(), new_.end())

bool is_epsilon(string ch) { return ch == EPSILON; }
bool is_terminal(char ch) { return !isupper(ch); }
template<typename T> void vector_print(vector<T> vec, int64_t up_to = -1)
{
    if(up_to == -1 || up_to > vec.size()) up_to = vec.size();
    cout<<"[";
    for(int64_t i = 0; i<up_to; ++i) cout<<vec.at(i)<<", ";
    cout<<"]";
}
template<typename T> void stack_print(stack<T> s)
{
    cout<<"Top->";
    while (!s.empty()) {
        cout<<s.top()<<" ";
        s.pop(); 
    } cout<<"] ";
}

template<typename T>
vector<T> remove_duplicates(vector<T> original)
{
    set<T> s(original.begin(), original.end());
    original.assign(s.begin(), s.end());
    return original;   
}

template<typename T1, typename T2>
vector<T1> get_key_list(unordered_map<T1, T2> mapp)
{  
    vector<T1> key_list;
    key_list.reserve((mapp.size()));
    for(auto const& imap: mapp)
        key_list.push_back(imap.first);
    return key_list;
}

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

bool is_id(string ident)
{
    if(!isalpha(ident[0]) && ident[0] != '_')
        return false;
    for(int i=1; i<ident.length(); ++i)
        if(ident[i] != '_' && (!isalnum(ident[i])))
            return false;
    return true;
}

bool is_id_part(char ch) { return isalnum(ch) || ch == '_'; }

vector<string> distinguish_input(string in)
{
    string temp = "", temp2 = "";
    vector<string> ans;
    for(char ch : in)
        if(is_id_part(ch))
            temp += ch;
        else
        {
            if(temp.length() > 0)
                ans.push_back(temp);
            temp2 = ch;
            ans.push_back(temp2);
            temp = "";
        }
    ans.push_back(temp);
    ans.push_back(STRING_END);
    return ans;
}

class LL1_Parser
{
    char primary_non_terminal;
    vector<char> non_terminal_list;
    unordered_map<char, vector<string>> grammer;
    unordered_map<char, unordered_map<char, string>> ll1_table;

    static bool is_valid_ll1_entry(string exp) { return COUNT(exp , SEPRATOR[0]) == 1; }

    void fill_ll1_table()
    {
        char non_terminal;
        vector<char> first_list, follow_list;
        for(char non_terminal : non_terminal_list)
            for(string expression : grammer[non_terminal])
            {
                first_list = get_first_of_expression(expression);
                if(FIND(first_list, EPSILON[0]))
                {
                    REMOVE(first_list, EPSILON[0]);
                    follow_list = get_follow_of(non_terminal);
                    for(char follow : follow_list)
                    {
                        ll1_table[non_terminal][follow] += EPSILON_SEPRATOR;
                        if(!is_valid_ll1_entry(ll1_table[non_terminal][follow]))
                            throw string("Invalid grammer for LL(1) parser");
                    }
                }
                for(char first : first_list)
                {
                    ll1_table[non_terminal][first] += expression + SEPRATOR;
                    if(!is_valid_ll1_entry(ll1_table[non_terminal][first]))
                        throw string("Invalid grammer for LL(1) parser");
                }
            }
    }
public:
    LL1_Parser(string gramm_str)
    {
        vector<string> production_list = split(gramm_str, "\n");
        unordered_map<char, string> dummy_map;
        non_terminal_list.clear();
        
        primary_non_terminal = production_list[0][0];
        for(string production : production_list)
        {
            non_terminal_list.push_back(production[0]);
            ll1_table[production[0]] = dummy_map;
            grammer[production[0]] = split(
                string(
                    production.begin() + 2,
                    production.end() - 1
                ), SEPRATOR
            );
        }
        fill_ll1_table();
    }

    vector<char> get_first_of_expression(string expression, uint64_t depth = 0)
    {
        vector<char> ans, first_ch;
        if(expression == "")
            ans.push_back(STRING_END[0]);
        else if(is_terminal(expression[0]))
            ans.push_back(expression[0]);
        else if(is_epsilon(expression))
            ans.push_back(EPSILON[0]);
        else
        {
            int j = 1;
            first_ch = get_first_of_non_terminal(expression[0], depth + 1);
            APPEND(ans, first_ch);
            while(FIND(first_ch, EPSILON[0]) && j < expression.length())
            {
                REMOVE(ans, EPSILON[0]);
                if(is_terminal(expression[j]))
                {
                    ans.push_back(expression[j]);
                    break;
                }
                first_ch = get_first_of_non_terminal(expression[j++], depth + 1);
                APPEND(ans, first_ch);
            }
        }
        return remove_duplicates(ans);
    }

    vector<char> get_follow_of(char chrac)
    {
        int pos = 0;
        string sub_expression;
        vector<char> ans, first_ch;
        for(char non_terminal : non_terminal_list)
            for(string expression : grammer[non_terminal])
            {
                pos = expression.find(chrac);
                if(pos != string::npos)
                    while(pos != string::npos)
                    {
                        sub_expression = expression.substr(pos+1);
                        first_ch = get_first_of_expression(sub_expression);
                        APPEND(ans, first_ch);
                        pos = expression.find(chrac, pos+1);
                    }
            }
        return remove_duplicates(ans);
    }

    vector<char> get_first_of_non_terminal(char nonT, uint64_t depth = 0)
    {
        vector<char> ans, new_ans;
        vector<string> expression_list = grammer[nonT];

        if(depth == DEPTH_LIMIT) // Break in case Cyclic loop
        {
            cout<<"Recursive depth reached aborting.";
            return ans;
        }

        for(string expression : expression_list)
        {
            new_ans = get_first_of_expression(expression, depth);
            APPEND(ans, new_ans);
        }
        return remove_duplicates(ans);
    }

    bool validate_string(string in_string)
    {
        int ptr = 0, iter = 0;
        char current = STRING_END[0];
        string expression = "";
        stack<char> opr_stack;
        vector<string> in_str_vec = distinguish_input(in_string);

        opr_stack.push(STRING_END[0]);
        opr_stack.push(primary_non_terminal);
        
        cout<<"Attempting string validating: "<<in_string<<endl;
        cout<<"Pre-processed String:"; vector_print(in_str_vec); cout<<endl;
        cout<<"Action\t\tMatched\t\tStack"<<endl
            <<"-------------------------------------------------------"<<endl;
        while(ptr < in_str_vec.size() || !opr_stack.empty())
        {
            current = opr_stack.top();
            if(is_terminal(current))
                if(
                    (current == ID_SYM[0] && is_id(in_str_vec[ptr]) ) ||
                    current == in_str_vec[ptr][0]
                )
                {
                    cout<<"MATCH & POP:"<<in_str_vec[ptr]<<endl; opr_stack.pop();
                    ++ptr;
                }
                else
                {
                    cout<<endl<<"Matched:: "; vector_print(in_str_vec, ptr+1);
                    cout<<endl<<"stack: "; stack_print(opr_stack);
                    cout<<endl<<"string pointer on: "<<in_str_vec[ptr]
                        <<endl<<"String not acceptable."<<endl;
                    return false;
                }
            else// if(!is_terminal(current))
            {
                cout<<"POP: "<<opr_stack.top()<<endl;
                opr_stack.pop();
                if(is_id(in_str_vec[ptr]))
                    expression = ll1_table[current][ID_SYM[0]];
                else
                    expression = ll1_table[current][in_str_vec[ptr][0]];
                REVERSE(expression);
                cout<<"PUSH"<<expression<<endl;
                for(int i=1; i<expression.length(); ++i)
                    if(expression[i] != EPSILON[0])
                        opr_stack.push(expression[i]);
            }
            cout<<setw(23); vector_print(in_str_vec, ptr+1);
            cout<<setw(20)<<" "; stack_print(opr_stack);
            cout<<endl;
            if(iter++ == DEPTH_LIMIT)
            {
                cout<<"\nDEPTH LIMIT REACHED\n";
                break;
            }
        }
        if(!opr_stack.empty())
        {
            cout<<endl<<"Matched: "; vector_print(in_str_vec, ptr+1);
            cout<<endl<<"stack: "; stack_print(opr_stack);
            cout<<endl<<"String not ACCEPTABLE."<<endl;
        }
        else
            cout<<endl<<"String SUCCESSFULLY matched."<<endl;
        return opr_stack.empty();
    }

    void print_first()
    {
        cout<<"Grammer first:-"<<endl;
        for(char nonT : non_terminal_list)
        {
            cout<<"\t"<<nonT<<" -> "; vector_print(get_first_of_non_terminal(nonT)); cout<<endl;
        }
        cout<<"----------------------------------"<<endl;
    }

    void print_follow()
    {
        cout<<"Grammer follow:-"<<endl;
        for(char nonT : non_terminal_list)
        {
            cout<<"\t"<<nonT<<" -> "; vector_print(get_follow_of(nonT)); cout<<endl;
        }
        cout<<"-----------------------"<<endl;
    }

    void print()
    {
        cout<<"Grammer:-"<<endl;
        for(char nonT : non_terminal_list)
        {
            cout<<"\t"<<nonT<<" -> "; vector_print(grammer[nonT]); cout<<endl;
        }
        cout<<"-----------------------"<<endl;
    }

    void print_ll1_table()
    {
        vector<char> key_list;
        for(char non_terminal : non_terminal_list)
        {
            cout<<"For non terminal: "<<non_terminal<<endl;
            key_list = get_key_list(ll1_table[non_terminal]);
            for(char key : key_list)
                cout<<"\tTerminal: "<<key<<" Expression: "<<ll1_table[non_terminal][key]<<endl;
            cout<<endl;
        }
        cout<<"----------------------------------"<<endl;
    }
};

int main() try
{
    string file, str;

    // cout<<"Enter grammer file: ";cin>>file;
    // LL1_Parser g(read_file(file));
    // cout<<"Enter string to validate: "; cin>>str;
    // g.print_first();
    // g.print_follow();
    // g.print_ll1_table();


    LL1_Parser g(read_file("g2.txt"));
    str = "a1+a2*a3";

    g.validate_string(str);

    return 0;
}
catch(string &e) { cerr<<"Error: "<<e<<endl; }
catch(exception &e) { cerr<<e.what()<<endl; }
catch(...) { cerr<<"\n\nSomething Wrong\n\n"<<endl; }

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <string.h>
#include <stack>
#include <unordered_set>
#include <utility>
#include <cmath>
using namespace std;

string fire = "";
string input = "";
bool tm = false; // true if the input is a turing machine
bool verbose = false;

class Transition_pda{
    public:
        string cur_state, cur_symbol, pop_symbol, next_state, push_symbol;
        Transition_pda(string cur_state, string cur_symbol, string pop_symbol, string next_state, string push_symbol){
            this->cur_state = cur_state;
            this->cur_symbol = cur_symbol;
            this->pop_symbol = pop_symbol;
            this->next_state = next_state;
            this->push_symbol = push_symbol;
        }
        Transition_pda(){
            this->cur_state = "";
            this->cur_symbol = "";
            this->pop_symbol = "";
            this->next_state = "";
            this->push_symbol = "";
        }
};

class PDA{
public:
    unordered_set <string> Q, S, G, F;
    vector <Transition_pda> delta;
    string q0, z0;
    PDA(unordered_set <string> Q, unordered_set <string> S, unordered_set <string> G, unordered_set <string> F, vector<Transition_pda> delta, string q0, string z0){
        this->Q = Q;
        this->S = S;
        this->G = G;
        this->F = F;
        this->delta = delta;
        this->q0 = q0;
        this->z0 = z0;
    }
    PDA(){}
    bool judge_input(string input){
        for(int i = 0; i < input.size(); i++){
            if(S.find(input.substr(i,1)) == S.end()){
                if(verbose){
                    cerr << "Input  : " << input << endl;
                    cerr << "==================== ERR =====================\n";
                    cerr << "error  : " << input.substr(i,1) << "' was not declared in the set of input symbols \n";
                    cerr << "Input  : " << input <<endl;
                    cerr << "         ";
                    for(int j = 0; j < i; j++){
                        cerr << " ";
                    }
                    cerr << "^" << endl;
                    cerr << "==================== END ====================="<<endl;
                    exit(1);
                }
                cerr << "illegal input\n";
                exit(1);
            }
        }
        if(verbose){
            cout << "Input  : " << input << endl;
            cout << "==================== RUN ====================="<<endl;
        }
        return true;
    }
    bool run(string input){
        string cur_state = q0;
        string cur_symbol = "";
        stack <string> s;
        s.push(z0);
        string pop_symbol = "";
        int i = 0;
        int len = input.size();
        int step = 0;
        bool flag = true;
        while(flag){
            cur_symbol = input.substr(i,1);
            if(s.empty()){
                pop_symbol = "_";
            }
            else
                pop_symbol = s.top();
            if(verbose)
            {
                cout << "Step   : " << step << endl;
                cout << "State  : " << cur_state << endl;
                cout << "Input  : " << input << endl;
                cout << "         ";
                for(int j = 0; j < i; j++){
                    cout << " ";
                }
                cout << "^" << endl;
                cout << "Stack  : ";
                stack <string> temp = s;
                while(!temp.empty()){
                    cout << temp.top();
                    temp.pop();
                }
                cout << endl;
                cout << "pop    : " << pop_symbol << endl;
                cout << "----------------------------------------------"<<endl;
            }
            if(s.empty() && i < len){
                return false;
            }
            else if(s.empty() && i >= len)
            {
                break;
            }
            s.pop();
            bool flag1 = false;
            for(int j = 0; j < delta.size(); j++){
                if(i < len && delta[j].cur_state == cur_state && delta[j].cur_symbol == cur_symbol && delta[j].pop_symbol == pop_symbol){
                    flag1 = true;
                    cur_state = delta[j].next_state;
                    if(delta[j].push_symbol != "_"){
                        for(int k = delta[j].push_symbol.size()-1; k >= 0; k--){
                            s.push(delta[j].push_symbol.substr(k,1));
                        }
                    }
                    i++;
                    break;
                }
                else if(delta[j].cur_state == cur_state && delta[j].cur_symbol == "_" && delta[j].pop_symbol == pop_symbol ){
                    flag1 = true;
                    cur_state = delta[j].next_state;
                    if(delta[j].push_symbol != "_"){
                        for(int k = delta[j].push_symbol.size()-1; k >= 0; k--){
                            s.push(delta[j].push_symbol.substr(k,1));
                        }
                    }
                    break;
                }
            }
            flag = flag1;
            if(!flag1 && i <= len - 1){
                return false;
            }
            step++;
        }
        if(F.find(cur_state) != F.end()){
            return true;
        }
        else{
            return false;
        }

    }
};
class Transition_tm{
public:
    int tape_num;
    string cur_state;
    vector<string> old_symbol;
    vector<string> new_symbol;
    vector<string> direction;
    string next_state;
    Transition_tm(int tape_num, string cur_state, vector<string> old_symbol, vector<string> new_symbol, vector<string> direction, string next_state){
        this->tape_num = tape_num;
        this->cur_state = cur_state;
        this->old_symbol = old_symbol;
        this->new_symbol = new_symbol;
        this->direction = direction;
        this->next_state = next_state;
    }
    Transition_tm(){}
};

class TM{
public:
    unordered_set <string> Q, S, G, F;
    int N;
    string q0, B;   
    vector <Transition_tm> delta;
    TM(unordered_set <string> Q, unordered_set <string> S, unordered_set <string> G, unordered_set <string> F, int N, string q0, string B, vector<Transition_tm> delta){
        this->Q = Q;
        this->S = S;
        this->G = G;
        this->F = F;
        this->N = N;
        this->q0 = q0;
        this->B = B;
        this->delta = delta;
    }
    bool judge(string input){
        for(int i = 0; i < input.size(); i++){
            if(S.find(input.substr(i,1)) == S.end()){
                if(verbose){
                    cerr << "Input: " << input << endl;
                    cerr << "==================== ERR =====================\n";
                    cerr << "error: '" << input.substr(i,1) << "' was not declared in the set of input symbols \n";
                    cerr << "Input: " << input <<endl;
                    cerr << "       ";
                    for(int j = 0; j < i; j++){
                        cerr << " ";
                    }
                    cerr << "^" << endl;
                    cerr << "==================== END ====================="<<endl;
                    exit(1);
                }
                else{
                    cerr << "illegal input"<<endl;
                    exit(1);
                }
            }
        }
        if(verbose){
            cout << "Input: " << input << endl;
            cout<< "==================== RUN ====================="<<endl;
        }
        return true;
    }
    void run(string input){
        //模拟纸带
        vector<vector<string>> tapes;
        int len = input.size();
        vector<string> temp;
        if(len > 0){
            for(int j = 0; j < len; j++){
                temp.push_back(input.substr(j,1));
            }
        }
        else
            temp.push_back(B);
        tapes.push_back(temp);
        for(int i = 1; i < N; i++){
            vector<string> temp_;
            temp_.push_back(B);
            tapes.push_back(temp_);
        }
        //模拟tm运行
        bool flag = true;
        vector<int> cur_pos = vector<int>(N,0);
        string cur_state = q0;
        vector<string> old_symble;
        vector<string> new_symble;
        vector<string> directions;
        vector<int> pox_0 = vector<int>(N,0); // 记录各个纸带的索引0位置
        vector<int> print_end = vector<int>(N-1,0); // 记录各个纸带的打印结束位置
        int step = 0;
        //统一：前为8个字符位置
        while(flag)
        {
            old_symble.clear();
            new_symble.clear();
            directions.clear();
            if(verbose){
                cout << "Step    : " << step << endl;
                cout << "State   : " << cur_state << endl;
                for(int i = 0; i < N; i++){
                    if(i < 10){
                        cout << "Index" << i << "  : ";
                    }
                    else{
                        cout << "Index" << i << " : ";
                    }
                    for(int j = 0; j < tapes[i].size(); j++){
                        int pos = abs(j - pox_0[i]);
                        if(j == tapes[i].size() - 1){
                            cout << pos <<endl;
                        }
                        else{
                            cout<< pos << " ";
                        }
                    }
                    if(i < 10){
                        cout << "Tape" << i << "   : ";
                    }
                    else{
                        cout << "Tape" << i << "  : ";
                    }
                    for(int j = 0; j < tapes[i].size(); j++){
                        if(j == tapes[i].size() - 1){
                            cout << tapes[i][j] << endl;
                        }
                        else{
                            cout << tapes[i][j] << " ";
                        }
                    }
                    if(i < 10){
                        cout << "Head" << i << "   : ";
                    }
                    else{
                        cout << "Head" << i << "  : ";
                    }
                    for(int j = 0; j < cur_pos[i]; j++){
                        cout << "  ";
                    }
                    cout << "^" << endl;
                }
                cout << "----------------------------------------------"<<endl;
            }
            // 进行下一步
            step++;
            for(int i = 0; i < N; i++){
                old_symble.push_back(tapes[i][cur_pos[i]]);
            }
            vector<string> temp1;
            bool flag1 = false;
            for(int i = 0; i < delta.size(); i++){
                temp1 = delta[i].old_symbol;
                if(delta[i].cur_state == cur_state){
                    flag1 = true;
                    for(int j = 0; j < N ; j++){
                        if((temp1[j] == "*" && old_symble[j] != "_") || temp1[j] == old_symble[j]){
                            continue;
                        }
                        else{
                            flag1 = false;
                            break;
                        }
                    }
                    if(flag1){
                        flag = true;
                        cur_state = delta[i].next_state;
                        new_symble = delta[i].new_symbol;
                        directions = delta[i].direction;
                        break;
                    }
                }
                else{
                    continue;
                }
            }
            //更新tm的瞬时描述
            flag = flag1;
            if(flag1){
                for(int i = 0; i < N; i++){
                    int len = tapes[i].size();
                    if(new_symble[i] != "*"){
                        tapes[i][cur_pos[i]] = new_symble[i];
                    }
                    if(directions[i] == "r"){
                        cur_pos[i]++;   
                        if(cur_pos[i] == len ){
                            tapes[i].push_back(B);
                        }
                        if(cur_pos[i] == 1 && tapes[i][0] == B){
                            tapes[i].erase(tapes[i].begin(),tapes[i].begin()+1);
                            pox_0[i] -= 1;
                            cur_pos[i]--;
                        }
                    }
                    else if(directions[i] == "l"){
                        cur_pos[i]--;
                        if(cur_pos[i] < 0 && tapes[i][0] != B){
                            tapes[i].insert(tapes[i].begin(),B);
                            pox_0[i]++;
                            cur_pos[i]++;
                        }
                        else if(cur_pos[i] < 0 && tapes[i][0] == B && tapes[i].size() == 1){
                            pox_0[i]++;
                            cur_pos[i]++;
                        }
                        else if(cur_pos[i] < 0 && tapes[i][0] == B && tapes[i].size() > 1){
                            tapes[i].insert(tapes[i].begin(),B);
                            pox_0[i]++;
                            cur_pos[i]++;
                        }
                        else if(cur_pos[i] == len - 2 && tapes[i][len-1] == B){
                            tapes[i].pop_back();
                        }
                    }
                    else if(directions[i] == "*"){
                        continue;
                    }
                }
            }
            else{
                break;
            }
        }
        //输出第一条纸带结果
        int begin = 0;
        int end = tapes[0].size() - 1;
        for(;begin < tapes[0].size(); begin++){
            if(tapes[0][begin] != B)
                break;
        }
        for(; end >=0 ; end--){
            if(tapes[0][end] != B)
                break;
        }
        if(verbose){
            cout<<"Result  : ";
            for(int i = begin; i <= end; i++){
                cout << tapes[0][i];
            }
            cout <<endl;
            cout << "==================== END ====================="<<endl;
        }
        else{
            for(int i = begin; i <= end; i++){
                cout << tapes[0][i];
            }
            cout << endl;
        }
    }
};
//分割字符串，提取集合
vector<string> split(string s, char c){
    vector<string> res;
    if(s.size() == 0){
        return res;
    }
    for(int i = 0; i < s.size(); i++){
        string temp = "";
        while(s[i] != c && i < s.size()){
            temp += s[i];
            i++;
        }
        res.push_back(temp);
    }
    return res;
}

PDA get_pda(string file){
    ifstream f(file);
    if(!f){
        cerr << "file not found\n";
        exit(1);
    }
    bool flag_q = false, flag_s = false, flag_g = false, flag_f = false, flag_q0 = false, flag_z0 = false;
    bool flag = false;
    string line = "";
    unordered_set <string> Q, S, G, F;
    vector <Transition_pda> delta;
    string q0, z0;
    //  判断输入格式是否有误。
    while(getline(f,line)){ 
        if(line == ""){
            continue;
        }
        if(line[0] == ';'){
            continue;
        }
        int i = 0;
        for(; i < line.size(); i++){
            if(line[i] == ';'){
                break;
            }
            else{
                continue;
            }
        }
        line = line.substr(0,i);
        if(line.size() <= 6){   //判断是否完整有所需的输入
            cerr << "syntax error\n";
            exit(1);
        }
        int len = line.size();
        i = len -1;
        for(; i >= 0; i--){
            if(line[i] == ' '){
                continue;
            }
            else{
                break;
            }
        }
        line = line.substr(0,i+1);//去处末尾连续的空格
        if(line[0] == '#' && line[1] == 'Q' && line[2] == ' ' && line[3] == '=' && line[4] == ' ' && line[5] == '{' && line[line.size()-1] == '}' && flag_q == false){
            flag_q = true;
            vector<string> temp = split(line.substr(6,line.size()-7),',');
            if(temp.size() == 0){
                cerr << "syntax error\n";
                exit(1);
            }
            for(int i = 0; i < temp.size(); i++){
                string s = temp[i];
                if(s == ""){
                    cerr << "syntax error\n";
                    exit(1);
                }
                for(int j = 0; j < s.size(); j++){
                    if((s[j] >= 'a' && s[j] <= 'z') || (s[j] >= 'A' && s[j] <= 'Z') || (s[j] >= '0' && s[j] <= '9') || s[j] == '_'){
                        continue;
                    }
                    else{
                        cerr << "syntax error\n";
                        exit(1);
                    }
                }
                Q.insert(s);
            }
        }
        else if(line[0] == '#' && line[1] == 'S' && line[2] == ' ' && line[3] == '=' && line[4] == ' ' && line[5] == '{' && line[line.size()-1] == '}' && flag_s == false){
            flag_s = true;
            vector<string> temp = split(line.substr(6,line.size()-7),',');
            if(temp.size() == 0){
                cerr << "syntax error\n";
                exit(1);
            }
            for(int i = 0; i < temp.size(); i++){
                string s = temp[i];
                if(s == ""){
                    cerr << "syntax error\n";
                    exit(1);
                }
                if(s.size() != 1){
                    cerr << "syntax error\n";
                    cerr << "invalid S_input\n";
                    exit(1);
                }
                if(s == " "|| s == "," || s == ";" || s == "{" || s == "}" || s == "*" || s == "_"){
                    cerr << "syntax error\n";
                    exit(1);
                }
                S.insert(s);
            }
        }
        else if(line[0] == '#' && line[1] == 'G' && line[2] == ' ' && line[3] == '=' && line[4] == ' ' && line[5] == '{' && line[line.size()-1] == '}' && flag_g == false){
            flag_g = true;
            vector<string> temp = split(line.substr(6,line.size()-7),',');
            if(temp.size() == 0){
                cerr << "syntax error\n";
                exit(1);
            }
            for(int i = 0; i < temp.size(); i++){
                string s = temp[i];
                if(s == ""){
                    cerr << "syntax error\n";
                    exit(1);
                }
                if(s.size() != 1){
                    cerr << "syntax error\n";
                    cerr << "invalid S_input\n";
                    exit(1);
                }
                if(s == " "|| s == "," || s == ";" || s == "{" || s == "}" || s == "*" || s == "_"){
                    cerr << "syntax error\n";
                    exit(1);
                }
                G.insert(s);
            }
        }
        else if(line[0] == '#' && line[1] == 'F' && line[2] == ' ' && line[3] == '=' && line[4] == ' ' && line[5] == '{' && line[line.size()-1] == '}' && flag_f == false){
            flag_f = true;
            vector<string> temp = split(line.substr(6,line.size()-7),',');
            if(temp.size() == 0){
                cerr << "syntax error\n";
                exit(1);
            }
            for(int i = 0; i < temp.size(); i++){
                string s = temp[i];
                if(s == ""){
                    cerr << "syntax error\n";
                    exit(1);
                }
                if(Q.find(s) == Q.end()){
                    cerr << "syntax error\n";
                    exit(1);
                }
                F.insert(s);
            }
        }
        else if(line[0] == '#' && line[1] == 'q' && line[2] == '0' && line[3] == ' ' && line[4] == '=' && line[5] == ' ' && flag_q0 == false){
            flag_q0 = true;
            string temp = line.substr(6,line.size()-6);
            if(Q.find(temp) == Q.end()){
                cerr << "syntax error\n";
                exit(1);
            }
            else{
                q0 = temp;
            }
        }
        else if(line[0] == '#' && line[1] == 'z' && line[2] == '0' && line[3] == ' ' && line[4] == '=' && line[5] == ' ' && flag_z0 == false){
            flag_z0 = true;
            string temp = line.substr(6,line.size()-6);
            if(G.find(temp) == G.end()){
                cerr << "syntax error\n";
                exit(1);
            }
            else{
                z0 = temp;
            }
        }
        else{
            vector<string> temp = split(line,' ');
            if(temp.size() != 5){
                cerr << "syntax error\n";
                exit(1);
            }
            if(Q.find(temp[0]) == Q.end() || G.find(temp[2]) == G.end() || Q.find(temp[3]) == Q.end()){
                cerr << "syntax error\n";
                exit(1);
            }
            if(S.find(temp[1]) == S.end() && temp[1] != "_"){

                cerr << "syntax error\n";
                exit(1);
            }
            bool flag = false;
            if(temp[4] == "_"){
                flag = true;
            }
            else{
                for(int i = 0; i < temp[4].size(); i++){
                    if(G.find(temp[4].substr(i,1)) == G.end()){
                        cerr << "syntax error\n";
                        exit(1);
                    }
                }
                flag = true;
            }
            if(!flag){
                cerr << "syntax error\n";
                exit(1);
            }
            Transition_pda t(temp[0],temp[1],temp[2],temp[3],temp[4]);
            delta.push_back(t);
        }
    }
    if(flag_q == false || flag_s == false || flag_g == false || flag_f == false || flag_q0 == false || flag_z0 == false){
        cerr << "syntax error\n";
        exit(1);
    }
    PDA pda(Q,S,G,F,delta,q0,z0);
    return pda;
}

TM get_tm(string file){
    ifstream f(file);
    if(!f){
        cerr << "file not found\n";
        exit(1);
    }
    bool flag_q = false, flag_s = false, flag_g = false, flag_f = false, flag_q0 = false, flag_B = false, flag_N = false;
    unordered_set <string> Q, S, G, F;
    int N;
    string q0, B;
    vector <Transition_tm> delta;
    string line = "";
    while (getline(f,line))
    {
        if(line == ""){
            continue;
        }
        if(line[0] == ';'){
            continue;
        }
        int i = 0;
        for(; i < line.size(); i++){
            if(line[i] == ';'){
                break;
            }
            else{
                continue;
            }
        }
        line = line.substr(0,i);
        i = line.size() - 1;
        for(; i >= 0; i--){
            if(line[i] == ' '){
                continue;
            }
            else{
                break;
            }
        }
        line = line.substr(0,i+1);
        if(line.size() < 6){
            cerr << "syntax error\n";
            exit(1);
        }
        if(line[0] == '#' && line[1] == 'Q' && line[2] == ' ' && line[3] == '=' && line[4] == ' ' && line[5] == '{' && line[line.size()-1] == '}' && flag_q == false){
            flag_q = true;
            vector<string> temp = split(line.substr(6,line.size()-7),',');
            if(temp.size() == 0){
                cerr << "syntax error\n";
                exit(1);
            }
            for(int i = 0; i < temp.size(); i++){
                string s = temp[i];
                if(s == ""){
                    cerr << "syntax error\n";
                    exit(1);
                }
                for(int j = 0; j < s.size(); j++){
                    if((s[j] >= 'a' && s[j] <= 'z') || (s[j] >= 'A' && s[j] <= 'Z') || (s[j] >= '0' && s[j] <= '9') || s[j] == '_'){
                        continue;
                    }
                    else{
                        cerr << "syntax error\n";
                        exit(1);
                    }
                }
                Q.insert(s);
            }
        }
        else if(line[0] == '#' && line[1] == 'S' && line[2] == ' ' && line[3] == '=' && line[4] == ' ' && line[5] == '{' && line[line.size()-1] == '}' && flag_s == false){
            flag_s = true;
            vector<string> temp = split(line.substr(6,line.size()-7),',');
            if(temp.size() == 0){
                cerr << "syntax error\n";
                exit(1);
            }
            for(int i = 0; i < temp.size(); i++){
                string s = temp[i];
                if(s == ""){
                    cerr << "syntax error\n";
                    exit(1);
                }
                if(s.size() != 1){
                    cerr << "syntax error\n";
                    cerr << "invalid S_input\n";
                    exit(1);
                }
                if(s == " "|| s == "," || s == ";" || s == "{" || s == "}" || s == "*" || s == "_"){
                    cerr << "syntax error\n";
                    exit(1);
                }
                S.insert(s);
            }
        }
        else if(line[0] == '#' && line[1] == 'G' && line[2] == ' ' && line[3] == '=' && line[4] == ' ' && line[5] == '{' && line[line.size()-1] == '}' && flag_g == false){
            flag_g = true;
            vector<string> temp = split(line.substr(6,line.size()-7),',');
            if(temp.size() == 0){
                cerr << "syntax error\n";
                exit(1);
            }
            for(int i = 0; i < temp.size(); i++){
                string s = temp[i];
                if(s == ""){
                    cerr << "syntax error\n";
                    exit(1);
                }
                if(s.size() != 1){
                    cerr << "syntax error\n";
                    cerr << "invalid S_input\n";
                    exit(1);
                }
                if(s == " "|| s == "," || s == ";" || s == "{" || s == "}" || s == "*" ){
                    cerr << "syntax error\n";
                    exit(1);
                }
                G.insert(s);
            }
        }
        else if(line.substr(0,6) == "#q0 = " && flag_q0 == false){
            flag_q0 = true;
            string temp = line.substr(6,line.size()-6);
            if(Q.find(temp) == Q.end()){
                cerr << "syntax error\n";
                exit(1);
            }
            else{
                q0 = temp;
            }
        }
        else if(line.substr(0,5) == "#B = " && flag_B == false){
            flag_B = true;
            string temp = line.substr(5,line.size()-5);
            if(G.find(temp) == G.end()){
                cerr << "syntax error\n";
                exit(1);
            }
            else{
                B = temp;
            }
        }
        else if(line.substr(0,6) == "#F = {" && line[line.size()-1] == '}' && flag_f == false){
            flag_f = true;
            vector<string> temp = split(line.substr(6,line.size()-7),',');
            if(temp.size() == 0){
                cerr << "syntax error\n";
                exit(1);
            }
            for(int i = 0; i < temp.size(); i++){
                string s = temp[i];
                if(s == ""){
                    cerr << "syntax error\n";
                    exit(1);
                }
                if(Q.find(s) == Q.end()){
                    cerr << "syntax error\n";
                    exit(1);
                }
                F.insert(s);
            }
        }
        else if(line.substr(0,5) == "#N = " && flag_N == false){
            flag_N = true;
            string temp = line.substr(5,line.size()-5);
            for(int i = 0; i < temp.size(); i++){
                if(temp[i] < '0' || temp[i] > '9'){
                    cerr << "syntax error\n";
                    exit(1);
                }
            }
            N = stoi(temp);
        }
        else{
            vector<string> temp = split(line,' ');
            if(temp.size() != 5){
                cerr << "syntax error\n";
                exit(1);
            }
            if(Q.find(temp[0]) == Q.end() || Q.find(temp[4]) == Q.end()){
                cerr << "syntax error\n";
                exit(1);
            }
            int len1 = temp[1].size();
            int len2 = temp[2].size();
            int len3 = temp[3].size();
            if(len1 != N || len1 != len2 || len1 != len3){
                cerr << "syntax error\n";
                exit(1);
            }
            vector<string> old_symbol;
            vector<string> new_symbol;
            vector<string> direction;
            for(int i = 0; i < len1; i++){
                if(G.find(temp[1].substr(i,1)) == S.end() && temp[1].substr(i,1) != "*"){
                    cerr << "syntax error\n";
                    exit(1);
                }
                if(G.find(temp[2].substr(i,1)) == G.end() && temp[2].substr(i,1) != "*" ){
                    cerr << "syntax error\n";
                    exit(1);
                }
                if(temp[2].substr(i,1) == "*" && temp[1].substr(i,1) != "*"){
                    cerr << "syntax error\n";
                    exit(1);
                }
                if(temp[3].substr(i,1) != "r" && temp[3].substr(i,1) != "l" && temp[3].substr(i,1) != "*"){
                    cerr << "syntax error\n";
                    exit(1);
                }
                old_symbol.push_back(temp[1].substr(i,1));
                new_symbol.push_back(temp[2].substr(i,1));
                direction.push_back(temp[3].substr(i,1));
            }
            Transition_tm t(N,temp[0],old_symbol,new_symbol,direction,temp[4]);
            delta.push_back(t);
        }
    }
    if(flag_q == false || flag_s == false || flag_g == false || flag_f == false || flag_q0 == false || flag_B == false || flag_N == false){
        cerr << "syntax error\n";
        exit(1);
    }
    TM tm(Q,S,G,F,N,q0,B,delta);
    return tm;
    
}

bool judge(string file){ //判断文件名是否合理，以及判断是PDA还是TM
    int len = file.length();
    bool error = false;
    if(len < 3){
        error = true;
    }
    else if(len == 3){
        if(file[0] == '.' && file[1] == 't' && file[2] == 'm'){
            return true;
        }
        else{
            error = true;
        }
    }
    else{
        if(file[len-4] == '.' && file[len-3] == 'p' && file[len-2] == 'd' && file[len-1] == 'a'){
            return false;
        }
        else if(file[len-3] == '.' && file[len-2] == 't' && file[len-1] == 'm'){
            return true;
        }
        else{
            error = true;
        }
    }
    if(error){
        cerr << "syntax error\n";
        cerr << "usage: fla [-h|--help] <pda> <input>\n";
        cerr << "       fla [-v|--verbose] [-h|--help] <tm> <input>"<<endl;
        exit(-1);
    }
    return false;
}

int main(int argc, char* argv[]){
    bool error = false;
    bool help = false;
    vector <string> params;
    for(int i = 0; i < argc; i++){
        params.push_back(argv[i]);
    }
    if(params[0] != "fla" && params[0] != "./fla"){
        cerr << "no found command\n";
        exit(-1);
    }
    for(int i = 1 ; i < params.size(); i++){
        if(params[i] == "-v" || params[i] == "--verbose"){
            verbose = true;
        }
        else if(params[i] == "-h" || params[i] == "--help"){
            help = true;
        }
        else if(fire == ""){
            fire = params[i];
        }
        else if(input == ""){
            input = params[i];
        }
        else{
            error = true;
        }
    }
    
    if(help){
        cout<< "usage:   fla [-h|--help] <pda> <input>\n";
        cout<< "         fla [-v|--verbose] [-h|--help] <tm> <input>" <<endl;
        return 0;
    }
    if(error){
        cerr << "syntax error\n";
        cerr << "usage: fla [-h|--help] <pda> <input>\n";
        cerr << "       fla [-v|--verbose] [-h|--help] <tm> <input>"<<endl;
        exit(-1);
    }
    tm = judge(fire);
    if(!tm){
        PDA pda = get_pda(fire);
        bool flag = pda.judge_input(input);
        bool result = pda.run(input);
        if(verbose){
            if(result)
                cout<<"result : true"<<endl;
            else
                cout<<"result : false"<<endl; 
        }
        else{
            if(result){
                cout << "true\n";
            }
            else{
                cout << "false\n";
            }
        }
        return 0;
    }
    else{
        TM tm = get_tm(fire);
        bool flag = tm.judge(input);
        tm.run(input);
        return 0;
    }
}
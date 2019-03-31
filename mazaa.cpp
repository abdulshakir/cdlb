sssssslllllllllllrrrrrrrrrrrrrrrrr

#include<bits/stdc++.h>
using namespace std;
map< string, vector<string> > mp;
map< string, vector<string> > First;
map< string, vector<string> > Follow;
vector< vector< vector<string> > > dfa;
map< int,map<string,int> > dfamp;
class pti{
    public:
    bool shift,action;
    int ns;
    vector<string> rul;
    bool acc;

};
map<int, map<string,pti> > pt;
vector<string> term,nterm;
map<string,int> newmap(){
    map<string,int> mpp;
    return mpp;
}
vector< vector<string> > transit(vector< vector<string> > state,string sym){
    vector< vector<string> > nexts;
    for(vector<string> item:state){
        if(item[2]=="")
            continue;
        string fl(1,item[2][0]);
        if(sym==fl){
            vector<string> vs={item[0],item[1]+fl,item[2].substr(1)};
            if(find(nexts.begin(),nexts.end(),vs)==nexts.end())
                nexts.push_back(vs);
        }
    }
    return nexts;
}
vector< vector<string> > closure(vector< vector<string> > state){
    vector< vector<string> > cl=state;
    vector< vector<string> > temp;
    for(vector<string> item:cl){
        if(item[2]=="")
            continue;
        string fl(1,item[2][0]);
        if(find(nterm.begin(),nterm.end(),fl)!=nterm.end()){
            for(string rl:mp[fl]){
                vector<string> nextitem={fl,"",rl};
                if(find(cl.begin(),cl.end(),nextitem)==cl.end())
                    if(find(temp.begin(),temp.end(),nextitem)==temp.end())
                        temp.push_back(nextitem);
            }
        }    
    }
    if(temp.empty())
        return cl;
    for(vector<string> item:temp)
        cl.push_back(item);
    cl=closure(cl);
    return cl;    
}
int build(vector< vector<string> > state){
    state=closure(state);
    sort(state.begin(),state.end());
    auto it=find(dfa.begin(),dfa.end(),state);
    //return number of current state;
    if(it!=dfa.end()){
        return it-dfa.begin();
    }
    else{
        dfa.push_back(state);
        int curs=dfa.size()-1;
        dfamp[curs]=newmap();
        vector<string> symbols;
        for(auto vc:state){
            if(vc[2]!=""){
                string fl(1,vc[2][0]);
                if(find(symbols.begin(),symbols.end(),fl)==symbols.end())
                    symbols.push_back(fl);
            }
        }
        for(string sym:symbols){
            int ns=build(transit(state,sym));
            dfamp[curs][sym]=ns;
        }
        return curs;
    }
}

void buildFirstNT(string nt);
vector<string> buildFirst(string nt,string s){
    vector<string> ans;
    if(nt==s)
        return ans;
    if(s==""){
        ans.push_back("");
        return ans;
    }
    string fl(1,s[0]);
    if(mp.find(fl)==mp.end()){
        ans.push_back(fl);
        return ans;
    }
    if(First.find(fl)==First.end()){
        buildFirstNT(fl);
    }
    for(string s:First[fl])
            ans.push_back(s);
    auto it=find(ans.begin(),ans.end(),"");
    if(it!=ans.end()){
        ans.erase(it);
        vector<string> v=buildFirst(nt,s.substr(1));
        for(string s:v)
            if(find(ans.begin(),ans.end(),s)==ans.end())
                ans.push_back(s);
    }
    return ans;

}
void buildFirstNT(string nt){
        if(First.find(nt)!=First.end())
            return;
        set<string> curF;
        for(string s:mp[nt]){
            vector<string> v=buildFirst(nt,s);
            for(string x:v)
                curF.insert(x);
        }
        vector<string> curFv;
        for(string s:curF)
            curFv.push_back(s);
        First[nt]=curFv;

}
void buildFollow(string nt){
    vector<string> ans;
    if(nt=="X")
        ans.push_back("$");
    for(auto it:mp){
        for(string rl:it.second){
            int found;
            while((found=rl.find(nt))!=string::npos){
                rl=rl.substr(found+1);
                vector<string> fst=buildFirst(it.first,rl);
                bool empflag=false;
                if(find(fst.begin(),fst.end(),"")!=fst.end()){
                    empflag=true;
                    fst.erase(find(fst.begin(),fst.end(),""));
                }
                for(string s1:fst)
                    if(find(ans.begin(),ans.end(),s1)==ans.end())
                        ans.push_back(s1);
                if(empflag&&(it.first!=nt)){
                    if(Follow.find(it.first)==Follow.end()){
                        buildFollow(it.first);
                    }
                    for(string s1:Follow[it.first])
                        if(find(ans.begin(),ans.end(),s1)==ans.end())
                            ans.push_back(s1);

                }        
            }
        }
    }
    Follow[nt]=ans;
} 
void init(){
    term={"a","b"};
    nterm={"X","S","A"};
    vector<string> rulX={"S"};
    vector<string> rulS={"AA"};
    vector<string> rulA={"aA","b"};
    mp["X"]=rulX;
    mp["S"]=rulS;
    mp["A"]=rulA;
    vector< vector<string> > start={{"X","","S"}};
    build(start);
    bool flag=true;
    cout<<"--------ITEMS ARE--------"<<endl;
    for(int i=0;i<dfa.size();i++){
        cout<<"ITEMS IN I"<<i<<endl;
        for(vector<string> item:dfa[i])
            cout<<item[0]<<"->"<<item[1]<<"."<<item[2]<<endl;
    }
    cout<<"----PARSING TABLE----"<<endl;
    cout<<setw(15)<<"ACTION"<<setw(60)<<"GOTO"<<endl;
    nterm={"S","A"};
    term.push_back("$");
    for(string sy:term){
        cout<<setw(15)<<sy;
    }
    for(string sy:nterm)
        cout<<setw(15)<<sy;
    cout<<endl;
    
    bool valid=true;
    for(int i=0;i<dfa.size();i++){
        for(string sy:term){
            if(dfamp[i].find(sy)!=dfamp[i].end()){
                pti it;
                it.shift=true;
                it.action=true;
                it.ns=dfamp[i][sy];
                it.acc=false;
                pt[i][sy]=it;
            }
        }
        for(string sy:nterm){
            if(dfamp[i].find(sy)!=dfamp[i].end()){
                pti it;
                it.shift=false;
                it.action=false;
                it.ns=dfamp[i][sy];
                it.acc=false;
                pt[i][sy]=it;
            }
        }
    }
    vector<string> acr={"X","S",""};
    for(int i=0;i<dfa.size();i++){
        for(auto rule:dfa[i]){
            if(rule[2]!="")
                continue;
            if(rule==acr){
                pti it;
                it.acc=true;
                pt[i]["$"]=it;
            }
            buildFollow(rule[0]);
            for(string x:Follow[rule[0]]){                
                pti it;
                it.action=true;
                it.shift=false;
                it.rul=rule;
                pt[i][x]=it;
            }
        }
    }
    for(auto df:pt){
        string opp="I";
        opp=opp+to_string(df.first);
        cout<<left<<setw(15)<<opp;
        for(string s:term){
            if(df.second.find(s)!=df.second.end()){
                pti it=df.second[s];
                if(it.acc)
                    cout<<left<<setw(15)<<"ACC";
                else if(it.shift){
                    opp="S";
                    opp+=to_string(it.ns);
                    cout<<left<<setw(15)<<opp;
                }
                    
                else{
                    cout<<left<<setw(15);
                    string op="R(";
                    op+=it.rul[0];
                    op=op+"->";
                    op=op+it.rul[1];
                    op=op+")";
                    cout<<op;
                }
                    
                
                
            }
            else
                cout<<left<<setw(15)<<"";
        ;
        }
        for(string s:nterm){
            cout<<left<<setw(15);
            if(df.second.find(s)!=df.second.end())
                cout<<df.second[s].ns;
            else
                cout<<left<<setw(15)<<"";
           
        }
        cout<<endl;
        
    }
    if(flag)
        cout<<"Given Grammar is LR(1)"<<endl;
    else
        cout<<"Given Grammar is not LR(1)"<<endl;
}
int main(){
    init();
    cout<<"Enter string to parse"<<endl;
    string s1;
    cin>>s1;
    cout<<left<<setw(15);
    cout<<"STACK"<<left<<setw(15)<<"INPUT"<<left<<setw(15)<<"ACTION"<<endl;
    s1=s1+"$";
    string stk="$0";
    while(1){
        cout<<left<<setw(15)<<stk<<left<<setw(15)<<s1;
        string cs(1,stk[stk.length()-1]);
        string nip(1,s1[0]);
        if(pt[stoi(cs)].find(nip)==pt[stoi(cs)].end()){
            cout<<"ERROR"<<endl;
            break;
        }
        pti it=pt[stoi(cs)][nip];
        if(it.acc){
            cout<<"ACCEPTED"<<endl;
            break;
        }
        else if(it.action&&it.shift){
            cout<<"SHIFT"<<endl;
            stk+=nip;
            stk+=to_string(it.ns);
            s1=s1.substr(1);
        }
        else{
            cout<<"REDUCE "<<it.rul[0]<<"->"<<it.rul[1]<<endl;
            int len=it.rul[1].length();
            for(int i=0;i<2*len;i++){
                stk.erase(stk.begin()+stk.length()-1);
            }
            string ccs(1,stk[stk.length()-1]);
            cs=ccs;
            stk+=it.rul[0];
            stk+=to_string(pt[stoi(cs)][it.rul[0]].ns);
        }
    }

}

lllllllllllllllllllllllllllllllllllllllllllllllllll11111111111111111111111111111

#include<bits/stdc++.h>
using namespace std;
map< string, vector<string> > mp;
map< string, vector<string> > First;
map< string, vector<string> > Follow;
void buildFirstNT(string nt);
vector<string> buildFirst(string nt,string s){
    vector<string> ans;
    if(nt==s)
        return ans;
    if(s==""){
        ans.push_back("");
        return ans;
    }
    string fl(1,s[0]);
    if(mp.find(fl)==mp.end()){
        ans.push_back(fl);
        return ans;
    }
    if(First.find(fl)==First.end()){
        buildFirstNT(fl);
    }
    for(string s:First[fl])
            ans.push_back(s);
    auto it=find(ans.begin(),ans.end(),"");
    if(it!=ans.end()){
        ans.erase(it);
        vector<string> v=buildFirst(nt,s.substr(1));
        for(string s:v)
            if(find(ans.begin(),ans.end(),s)==ans.end())
                ans.push_back(s);
    }
    return ans;

}
void buildFirstNT(string nt){
        if(First.find(nt)!=First.end())
            return;
        set<string> curF;
        for(string s:mp[nt]){
            vector<string> v=buildFirst(nt,s);
            for(string x:v)
                curF.insert(x);
        }
        vector<string> curFv;
        for(string s:curF)
            curFv.push_back(s);
        First[nt]=curFv;

}
void buildFollow(string nt){
    vector<string> ans;
    if(nt=="E")
        ans.push_back("$");
    for(auto it:mp){
        for(string rl:it.second){
            int found;
            while((found=rl.find(nt))!=string::npos){
                rl=rl.substr(found+1);
                vector<string> fst=buildFirst(it.first,rl);
                bool empflag=false;
                if(find(fst.begin(),fst.end(),"")!=fst.end()){
                    empflag=true;
                    fst.erase(find(fst.begin(),fst.end(),""));
                }
                for(string s1:fst)
                    if(find(ans.begin(),ans.end(),s1)==ans.end())
                        ans.push_back(s1);
                if(empflag&&(it.first!=nt)){
                    if(Follow.find(it.first)==Follow.end()){
                        buildFollow(it.first);
                    }
                    for(string s1:Follow[it.first])
                        if(find(ans.begin(),ans.end(),s1)==ans.end())
                            ans.push_back(s1);

                }        
            }
        }
    }
    Follow[nt]=ans;
}
void buildFollow(){
    for(auto it:mp)
        buildFollow(it.first);
}
map<string, map<string, string> > pt;
void buildPT(){
    vector<string> term={"I","(",")","*","+","$"};
    vector<string> nterm={"E","K","T","L","F"};
    for(string s1:nterm){
        for(string s2:mp[s1]){
            vector<string> v1=buildFirst(s1,s2);
            bool flag=false;
            for(string s3:v1){
                if(s3!="")
                    pt[s1][s3]=s2;
                else
                    flag=true;
            }
            if(flag){
                for(string s3:Follow[s1])
                    pt[s1][s3]=s2;
            }
        }
    }
    cout<<"PARSING TABLE"<<endl;
    cout<<"\t";
    for(string s1:term)
        cout<<s1<<"\t";
    cout<<endl;
    for(string s1:nterm){
        cout<<s1<<"\t";
        for(string s2:term){
            if(pt[s1].find(s2)!=pt[s1].end())
                cout<<"\""<<pt[s1][s2]<<"\"\t";
        }
        cout<<endl;
    }


}
void printFirst(){
    cout<<"FIRST"<<endl;
    for(auto it:First){
        cout<<it.first<<"\t";
        for(string s:it.second){
            cout<<"\""<<s<<"\"\t";
        }
        cout<<endl;
    }

}
void printFollow(){
    cout<<"FOLLOW"<<endl;
    for(auto it:Follow){
        cout<<it.first<<"\t";
        for(string s:it.second){
            cout<<"\""<<s<<"\"\t";
        }
        cout<<endl;
    }

}
void init(){
    vector<string> rulE={"TK"};
    vector<string> rulK={"+TK",""};
    vector<string> rulT={"FL"};
    vector<string> rulL={"*FL",""};
    vector<string> rulF={"I","(E)"};
    mp["E"]=rulE;
    mp["K"]=rulK;
    mp["T"]=rulT;
    mp["L"]=rulL;
    mp["F"]=rulF;
    buildFirstNT("E");buildFirstNT("K");buildFirstNT("T");buildFirstNT("L");buildFirstNT("F");
    buildFollow();
    printFirst();
    printFollow();
    buildPT();
}

int main(){
    init();
    cout<<"Enter string to parse"<<endl;
    string s1;
    cin>>s1;
    cout<<left<<setw(15);
    cout<<"STACK"<<left<<setw(15)<<"INPUT"<<left<<setw(15)<<"ACTION"<<endl;
    s1=s1+"$";
    string stk="$E";
    bool flag=true;
    while(flag){
        cout<<left<<setw(15)<<stk<<left<<setw(15)<<s1;
        if(s1=="$"&&stk=="$"){
            cout<<"ACCEPTED"<<endl;
            break;
        }

        if(stk[stk.length()-1]==s1[0]){
            stk.erase(stk.begin()+stk.length()-1);
            s1.erase(s1.begin());
            cout<<endl;
            continue;
        }
        string fl(1,stk[stk.length()-1]);
        if(mp.find(fl)==mp.end()){
            cout<<"ERROR"<<endl;
            break;

        }
        string fll(1,s1[0]);
        if(pt[fl].find(fll)==pt[fl].end()){
            cout<<"ERROR"<<endl;
            break;
        }
        string rl=pt[fl][fll];
        cout<<fl<<"->"<<"\""<<rl<<"\""<<endl;
        reverse(rl.begin(),rl.end());
        stk.erase(stk.begin()+stk.length()-1);
        stk=stk+rl;

    }



    return 0;
}

cccccccccccccccccccccclllllllllllllllllllllllllllllllllrrrrrrrrrrrrrrrrrrrrrrr1111111111



#include<bits/stdc++.h>
using namespace std;
map< string, vector<string> > mp;
map< string, vector<string> > First;
map< string, vector<string> > Follow;
vector< vector< vector<string> > > dfa;
vector< vector< vector<string> > > dfaR;
vector<int> grp;
vector<int> par;
map< int,map<string,int> > dfamp;
map< int,map<string,int> > dfampR;
vector<string> buildFirst(string nt,string s);
class pti{
    public:
    bool shift,action;
    int ns;
    vector<string> rul;
    bool acc;

};
map<int, map<string,pti> > pt;
map<int, map<string,pti> > ptR;
vector<string> term,nterm;
map<string,int> newmap(){
    map<string,int> mpp;
    return mpp;
}
vector< vector<string> > transit(vector< vector<string> > state,string sym){
    vector< vector<string> > nexts;
    for(vector<string> item:state){
        if(item[2]=="")
            continue;
        string fl(1,item[2][0]);
        if(sym==fl){
            vector<string> vs={item[0],item[1]+fl,item[2].substr(1),item[3]};
            if(find(nexts.begin(),nexts.end(),vs)==nexts.end())
                nexts.push_back(vs);
        }
    }
    return nexts;
}
vector< vector<string> > closure(vector< vector<string> > state){
    vector< vector<string> > cl=state;
    vector< vector<string> > temp;
    for(vector<string> item:cl){
        if(item[2]=="")
            continue;
        string fl(1,item[2][0]);
        if(find(nterm.begin(),nterm.end(),fl)!=nterm.end()){
            string rems="";
            if(item[2].length()>1)
                rems=item[2].substr(1);
            for(string rl:mp[fl]){
                vector<string> nxi;
                for(char x:item[3]){
                    vector<string> nxx=buildFirst(item[0],rems+x);
                    for(string xxi:nxx)
                        if(xxi!=""&&find(nxi.begin(),nxi.end(),xxi)==nxi.end())
                            nxi.push_back(xxi); 
                }
                string ho="";
                for(string sss:nxi)
                    ho=ho+sss;

                vector<string> nextitem={fl,"",rl,ho};
                if(find(cl.begin(),cl.end(),nextitem)==cl.end())
                    if(find(temp.begin(),temp.end(),nextitem)==temp.end())
                        temp.push_back(nextitem);
            }
        }    
    }
    if(temp.empty())
        return cl;
    for(vector<string> item:temp)
        cl.push_back(item);
    cl=closure(cl);
    return cl;    
}
int build(vector< vector<string> > state){
    state=closure(state);
    sort(state.begin(),state.end());
    auto it=find(dfa.begin(),dfa.end(),state);
    //return number of current state;
    if(it!=dfa.end()){
        return it-dfa.begin();
    }
    else{
        dfa.push_back(state);
        int curs=dfa.size()-1;
        dfamp[curs]=newmap();
        vector<string> symbols;
        for(auto vc:state){
            if(vc[2]!=""){
                string fl(1,vc[2][0]);
                if(find(symbols.begin(),symbols.end(),fl)==symbols.end())
                    symbols.push_back(fl);
            }
        }
        for(string sym:symbols){
            int ns=build(transit(state,sym));
            dfamp[curs][sym]=ns;
        }
        return curs;
    }
}

void buildFirstNT(string nt);
vector<string> buildFirst(string nt,string s){
    vector<string> ans;
    if(nt==s)
        return ans;
    if(s==""){
        ans.push_back("");
        return ans;
    }
    string fl(1,s[0]);
    if(mp.find(fl)==mp.end()){
        ans.push_back(fl);
        return ans;
    }
    if(First.find(fl)==First.end()){
        buildFirstNT(fl);
    }
    for(string s:First[fl])
            ans.push_back(s);
    auto it=find(ans.begin(),ans.end(),"");
    if(it!=ans.end()){
        ans.erase(it);
        vector<string> v=buildFirst(nt,s.substr(1));
        for(string s:v)
            if(find(ans.begin(),ans.end(),s)==ans.end())
                ans.push_back(s);
    }
    return ans;

}
void buildFirstNT(string nt){
        if(First.find(nt)!=First.end())
            return;
        set<string> curF;
        for(string s:mp[nt]){
            vector<string> v=buildFirst(nt,s);
            for(string x:v)
                curF.insert(x);
        }
        vector<string> curFv;
        for(string s:curF)
            curFv.push_back(s);
        First[nt]=curFv;

}
void buildFollow(string nt){
    vector<string> ans;
    if(nt=="X")
        ans.push_back("$");
    for(auto it:mp){
        for(string rl:it.second){
            int found;
            while((found=rl.find(nt))!=string::npos){
                rl=rl.substr(found+1);
                vector<string> fst=buildFirst(it.first,rl);
                bool empflag=false;
                if(find(fst.begin(),fst.end(),"")!=fst.end()){
                    empflag=true;
                    fst.erase(find(fst.begin(),fst.end(),""));
                }
                for(string s1:fst)
                    if(find(ans.begin(),ans.end(),s1)==ans.end())
                        ans.push_back(s1);
                if(empflag&&(it.first!=nt)){
                    if(Follow.find(it.first)==Follow.end()){
                        buildFollow(it.first);
                    }
                    for(string s1:Follow[it.first])
                        if(find(ans.begin(),ans.end(),s1)==ans.end())
                            ans.push_back(s1);

                }        
            }
        }
    }
    Follow[nt]=ans;
} 
void init(){
    term={"a","b","$"};
    nterm={"X","S","A"};
    vector<string> rulX={"S"};
    vector<string> rulS={"AA"};
    vector<string> rulA={"aA","b"};
    mp["X"]=rulX;
    mp["S"]=rulS;
    mp["A"]=rulA;
    vector< vector<string> > start={{"X","","S","$"}};
    build(start);
    bool flag=true;
    cout<<"--------ITEMS ARE--------"<<endl;
    for(int i=0;i<dfa.size();i++){
        cout<<"ITEMS IN I"<<i<<endl;
        for(vector<string> item:dfa[i])
            cout<<item[0]<<"->"<<item[1]<<"."<<item[2]<<"   "<<item[3]<<endl;
    }
    cout<<"----PARSING TABLE----"<<endl;
    cout<<setw(21)<<"ACTION"<<setw(28)<<"GOTO"<<endl;
    cout<<" ";
    nterm={"S","A"};
    //term.push_back("$");
    for(string sy:term){
        cout<<setw(15)<<sy;
    }
    for(string sy:nterm)
        cout<<setw(15)<<sy;
    cout<<endl;
    
    bool valid=true;
    for(int i=0;i<dfa.size();i++){
        for(string sy:term){
            if(dfamp[i].find(sy)!=dfamp[i].end()){
                pti it;
                it.shift=true;
                it.action=true;
                it.ns=dfamp[i][sy];
                it.acc=false;
                pt[i][sy]=it;
            }
        }
        for(string sy:nterm){
            if(dfamp[i].find(sy)!=dfamp[i].end()){
                pti it;
                it.shift=false;
                it.action=false;
                it.ns=dfamp[i][sy];
                it.acc=false;
                pt[i][sy]=it;
            }
        }
    }
    vector<string> acr={"X","S","","$"};
    for(int i=0;i<dfa.size();i++){
        for(auto rule:dfa[i]){
            if(rule[2]!="")
                continue;
            if(rule==acr){
                pti it;
                it.acc=true;
                pt[i]["$"]=it;
                continue;
            }
            for(char x:rule[3]){                
                pti it;
                it.action=true;
                it.shift=false;
                it.rul=rule;
                string gg="";
                gg=gg+x;
                pt[i][gg]=it;
            }
        }
    }
    for(auto df:pt){
        string opp="I";
        opp=opp+to_string(df.first);
        cout<<left<<setw(15)<<opp;
        for(string s:term){
            if(df.second.find(s)!=df.second.end()){
                pti it=df.second[s];
                if(it.acc)
                    cout<<left<<setw(15)<<"ACC";
                else if(it.shift){
                    opp="S";
                    opp+=to_string(it.ns);
                    cout<<left<<setw(15)<<opp;
                }
                    
                else{
                    cout<<left<<setw(15);
                    string op="R(";
                    op+=it.rul[0];
                    op=op+"->";
                    op=op+it.rul[1];
                    op=op+")";
                    cout<<op;
                }
                    
                
                
            }
            else
                cout<<left<<setw(15)<<"";
        ;
        }
        for(string s:nterm){
            cout<<left<<setw(15);
            if(df.second.find(s)!=df.second.end())
                cout<<df.second[s].ns;
            else
                cout<<left<<setw(15)<<"";
           
        }
        cout<<endl;
        
    }
    if(flag)
        cout<<"Given Grammar is LR(1)"<<endl;
    else
        cout<<"Given Grammar is not LR(1)"<<endl;
}
bool mtch(vector<vector<string> > &a,vector<vector<string> > &b){
    for(vector<string> r1:a){
        bool match=false;
        for(vector<string> r2:b)
            if(r1[0]==r2[0]&&r1[1]==r2[1]&&r1[2]==r2[2]){
                match=true;
                break;
            }
        if(!match)
            return false;
    }
    return true;
}
void group(vector<vector<string> > &a,vector<vector<string> > &b){
    for(vector<string> r1:a){
        bool match=false;
        for(vector<string> r2:b)
            if(r1[0]==r2[0]&&r1[1]==r2[1]&&r1[2]==r2[2]){
                for(char x:r2[3]){
                    string hh="";
                    hh=hh+x;
                    if(r1[3].find(hh)==string::npos){
                        r1[3]=r1[3]+hh;
                    }
                }   
            }
    }
}
int main(){
    init();
    cout<<"Enter string to parse"<<endl;
    string s1;
    cin>>s1;
    cout<<left<<setw(15);
    cout<<"STACK"<<left<<setw(15)<<"INPUT"<<left<<setw(15)<<"ACTION"<<endl;
    s1=s1+"$";
    string stk="$0";
    while(1){
        cout<<left<<setw(15)<<stk<<left<<setw(15)<<s1;
        string cs(1,stk[stk.length()-1]);
        string nip(1,s1[0]);
        if(pt[stoi(cs)].find(nip)==pt[stoi(cs)].end()){
            cout<<"ERROR"<<endl;
            break;
        }
        pti it=pt[stoi(cs)][nip];
        if(it.acc){
            cout<<"ACCEPTED"<<endl;
            break;
        }
        else if(it.action&&it.shift){
            cout<<"SHIFT"<<endl;
            stk+=nip;
            stk+=to_string(it.ns);
            s1=s1.substr(1);
        }
        else{
            cout<<"REDUCE "<<it.rul[0]<<"->"<<it.rul[1]<<endl;
            int len=it.rul[1].length();
            for(int i=0;i<2*len;i++){
                stk.erase(stk.begin()+stk.length()-1);
            }
            string ccs(1,stk[stk.length()-1]);
            cs=ccs;
            stk+=it.rul[0];
            stk+=to_string(pt[stoi(cs)][it.rul[0]].ns);
        }
    }
    cout<<"----------------------------LALR-------------------------"<<endl;
    grp.resize(dfa.size());
    par.resize(dfa.size());
    for(int i=0;i<dfa.size();i++){
        int match=-1;
        for(int j=0;j<i;j++)
            if(mtch(dfa[i],dfa[j])){
                group(dfaR[grp[j]],dfa[i]);
                match=j;
                grp[i]=j;
                break;
            }
        if(match==-1){
            dfaR.push_back(dfa[i]);
            grp[i]=dfaR.size()-1;
            par[i]=1;
        }
    }


    cout<<"               ACTION"<<"                        GOTO"<<endl;
    cout<<"               ";
    for(string sy:term){
        cout<<setw(15)<<sy;
    }
    for(string sy:nterm)
        cout<<setw(15)<<sy;
    cout<<endl;
    
    bool valid=true;
    for(int i=0;i<dfa.size();i++){
        for(string sy:term){
            if(dfamp[i].find(sy)!=dfamp[i].end()){
                pti it;
                it.shift=true;
                it.action=true;
                it.ns=grp[dfamp[i][sy]];
                it.acc=false;
                ptR[grp[i]][sy]=it;
            }
        }
        for(string sy:nterm){
            if(dfamp[i].find(sy)!=dfamp[i].end()){
                pti it;
                it.shift=false;
                it.action=false;
                it.ns=grp[dfamp[i][sy]];
                it.acc=false;
                ptR[grp[i]][sy]=it;
            }
        }
    }
    vector<string> acr={"X","S","","$"};
    for(int i=0;i<dfaR.size();i++){
        for(auto rule:dfaR[i]){
            if(rule[2]!="")
                continue;
            if(rule==acr){
                pti it;
                it.acc=true;
                ptR[i]["$"]=it;
                continue;
            }
            for(char x:rule[3]){                
                pti it;
                it.action=true;
                it.shift=false;
                it.rul=rule;
                string gg="";
                gg=gg+x;
                ptR[i][gg]=it;
            }
        }
    }
    for(auto df:ptR){
        string opp="I";
        opp=opp+to_string(df.first);
        cout<<left<<setw(15)<<opp;
        for(string s:term){
            if(df.second.find(s)!=df.second.end()){
                pti it=df.second[s];
                if(it.acc)
                    cout<<left<<setw(15)<<"ACC";
                else if(it.shift){
                    opp="S";
                    opp+=to_string(it.ns);
                    cout<<left<<setw(15)<<opp;
                }
                    
                else{
                    cout<<left<<setw(15);
                    string op="R(";
                    op+=it.rul[0];
                    op=op+"->";
                    op=op+it.rul[1];
                    op=op+")";
                    cout<<op;
                }
                    
                
                
            }
            else
                cout<<left<<setw(15)<<"";
        ;
        }
        for(string s:nterm){
            cout<<left<<setw(15);
            if(df.second.find(s)!=df.second.end())
                cout<<df.second[s].ns;
            else
                cout<<left<<setw(15)<<"";
           
        }
        cout<<endl;
        
    }

}

operator precedence   

#include<bits/stdc++.h>
using namespace std;
char pec[256][256]={0};
int main(){
pec['I']['*']=pec['I']['+']=pec['I']['$']=pec['*']['*']=pec['*']['+']=pec['*']['$']=pec['+']['+']=pec['+']['$']='>';
pec['*']['I']=pec['+']['I']=pec['+']['*']=pec['$']['I']=pec['$']['*']=pec['$']['+']='<';
    string ip;
    cout<<"ENTER THE STRING TO BE PARSED"<<endl;
    cin>>ip;
    string ooo="$";
    ooo=ooo+ip;
    ip=ooo+"$";
    int count=0;
    for(int i=0;i<ip.length();i++)
        if(ip[i]=='I')
            count++;
    string cur="$";
    for(int i=1;i<ip.length();i++){
        cur+=pec[ip[i-1]][ip[i]];
        cur+=ip[i];
    }
    cout<<cur<<endl;
    size_t found=cur.find("<I>");
    while(found!=string::npos){
        cur.replace(found,3,"E");
        cout<<cur<<endl;
        found=cur.find("<I>");
    }
    found=cur.find("E");
    while(found!=string::npos){
        cur.replace(found,1,"");
        found=cur.find("E");
    }
    cout<<cur<<endl;
    ip=cur;
     cur="$";
    for(int i=1;i<ip.length();i++){
        cur+=pec[ip[i-1]][ip[i]];
        cur+=ip[i];
    }
    cout<<cur<<endl;
    size_t found2=cur.find("<+>");
    found=cur.find("<*>");
    while(found2!=string::npos||found!=string::npos){
        if(found==string::npos)
            cur.replace(found2,3,"");
        else if(found2==string::npos)
            cur.replace(found,3,"");
        else{
            if(found<found2){
                cur.replace(found,3,"");
            }
            else
                cur.replace(found2,3,"");
        }
        ip="";
        for(int i=0;i<cur.length();i++)
            if(cur[i]=='*'||cur[i]=='$'||cur[i]=='+')
                ip=ip+cur[i];
        cur="$";
        for(int i=1;i<ip.length();i++){
            cur+=pec[ip[i-1]][ip[i]];
            cur+=ip[i];
        }
        cout<<cur<<endl;
        found=cur.find("<*>");
        found2=cur.find("<+>");
    }

    return 0;
}

shhhhhhiffffffffft reduce    
#include<bits/stdc++.h>
using namespace std;
/*SHIFT REDUCE PARSER FOR FOLLOWING GRAMMAR
E->2E2
E->3E3
E->4
*/
string LHS[]={"E","E","E"};
string RHS[]={"22E","33E","4"};
void print(queue<char> q){
    while(q.size()){
        cout<<q.front();
        q.pop();
    }
    cout<<"\t\t";
}
int main(){
    string ip;
    cout<<"Enter the Input String to be parsed"<<endl;
    cin>>ip;
    queue<char> q;
    for(char x:ip)
        q.push(x);
    q.push('$');
    string Stack="$";
    cout<<"STACK\tINPUT BUFFER\tPARSING ACTION"<<endl;
    bool accept=false;
    while((q.size()>1)||(Stack.length()>=2)){
        //print output;
        cout<<Stack<<"\t";
        print(q);
        int act=-1;
        //can i reduce?
        int rule=-1;
        for(int i=0;i<3;i++){
            size_t found=Stack.rfind(RHS[i]);
            if(found!=string::npos){
                if(found+RHS[i].length()==Stack.length()){
                    rule=i;
                    Stack.replace(found,RHS[i].length(),LHS[i]);
                    break;
                }
            }
        }
        if(rule!=-1){
            cout<<"REDUCE USING RULE "<<LHS[rule]<<"->"<<RHS[rule]<<endl;
            continue;
        }
        //can i shift
        if(q.size()>1){
            Stack=Stack+q.front();
            q.pop();
            cout<<"SHIFT"<<endl;
            continue;
        }
        break;

    }
    if(Stack=="$E"&&q.size()==1){
        cout<<"ACCEPT"<<endl;
    }
    else{
        cout<<"ERROR! NOT A VALID EXPRESSION"<<endl;
    }
    return 0;
}

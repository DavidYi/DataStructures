//
//  main.cpp
//  2Sat
//
//  Created by David Yi on 2/4/18.
//  Copyright © 2018 DavidYi. All rights reserved.
//

#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <fstream>

using namespace std;
typedef vector<int> VI;

struct Vertex{
    Vertex(): seen(false), sc(-1), adj(){}
    VI adj;
    bool seen;
    int sc;
};

typedef vector<Vertex> Graph;


Graph g;
Graph gt;//transposed graph
Graph gsc;//satisfying
vector<VI> sc;//strong components - tells which vertices are in which strong component
vector<int> values;//since it defaults to 0, make 1 false and 2 true
stack<int> posto;
int counterSC;
int counterPost;

//Kosaraju's algorithm to find strong components
void dfsG(int v){//first dfs to label vertices with postorder
    g[v].seen = true;
    
    for (int i = 0; i < g[v].adj.size(); ++i){
        //cout << "dfsG of index" << i << endl;
        int to = g[v].adj[i];
        if (!g[to].seen)
            dfsG(to);
    }
    //post[counterPost] = v;
    posto.push(v);
    //++counterPost;
}

void dfsGt(int v){//second dfs of g transposed
    gt[v].seen = true;
    
    for (int i = 0; i < gt[v].adj.size(); ++i){
        int to = gt[v].adj[i];
        if (!gt[to].seen)
            dfsGt(to);
    }
    sc[counterSC].push_back(v);
    g[v].sc = counterSC;
}

//find strog components
void findSC(){
    //find post order
    //cout << "starting first dfs" << endl;
    //counterPost = 0;
    for (int i = 0; i < g.size(); ++i){
        //cout << "dfis:" << i << endl;
        if (!g[i].seen)
            dfsG(i);
    }
    counterSC = 0;
    
    //cout << "starting second dfs" << endl;
    int pos;
    while (!posto.empty()){
        pos = posto.top();
        posto.pop();
        
        if (!gt[pos].seen){
            dfsGt(pos);
            ++counterSC;
        }
    }
    /*
    for (int i = post.size() - 1; i >= 0; --i){//reverse post order
        pos = post[i];
        if (!gt[pos].seen){
            dfsGt(pos);
            ++counterSC;
        }
    }*/
}

//make condensation dag
void condensation(){
    for (int i = 0; i < g.size(); ++i){
        for (int j = 0; j < g[i].adj.size(); ++j){
            int neigh = g[i].adj[j];
            if (g[i].sc != g[neigh].sc){
                bool same = false;
                for (int k = 0; k < gsc[g[i].sc].adj.size(); ++k)
                    if (g[neigh].sc == gsc[g[i].sc].adj[k])
                        same = true;
                if (!same)
                    gsc[g[i].sc].adj.push_back(g[j].sc);
            }
        }
    }
}

int main(int argc, const char * argv[]) {
    //make the graph
    int nv, nc;
    
    //cout << "Enter the number of variables:" << endl;
    cin >> nv;
    //in the graph there will be twice as many vertices to account for negation
    //cout << "Enter the number of clauses:" << endl;
    cin >> nc;
    g.resize(2*nv);//0~(nv-1) will be the var, while nv~(2*nv-1) will be the negated var
    gt.resize(2*nv);
    sc.resize(2*nv);
    //post.resize(2*nv);
    values.resize(2*nv);
    
    int p, q;//p or q
    for (int i = 0; i < nc; ++i){
        //cout << "Enter the next clause:" << endl;
        cin >> p >> q;//if the clause includes !p, then !p will be -p, which p is an integer
        //p or q becomes !p-->q and !q-->p
        if (p > 0 && q > 0){//p or q
            g[p + nv - 1].adj.push_back(q - 1);
            g[q + nv - 1].adj.push_back(p - 1);
            //cout << "!p" << ": " << p + nv - 1 << "\tp:" << p - 1 << endl;
            //cout << "!q" << ": " << q + nv - 1 << "\tq:" << q - 1 << endl;
            
            gt[q - 1].adj.push_back(p + nv - 1);
            gt[p - 1].adj.push_back(q + nv - 1);
        } else if (p > 0 && q < 0){//p or !q
            g[p + nv - 1].adj.push_back(nv - q - 1);//nv + q - 1 since q is negative
            g[-q - 1].adj.push_back(p - 1);//negated of !q is q, so -q
            
            gt[nv - q - 1].adj.push_back(p + nv - 1);
            gt[p - 1].adj.push_back(-q - 1);
        } else if (p < 0 && q > 0){//!p or q
            g[-p - 1].adj.push_back(q - 1);
            g[q + nv - 1].adj.push_back(nv - p - 1);
            
            gt[q - 1].adj.push_back(-p - 1);
            gt[nv - p - 1].adj.push_back(q + nv - 1);
        }else{//!p or !q
            g[-p - 1].adj.push_back(nv - q - 1);
            g[-q - 1].adj.push_back(nv - p - 1);
            
            //cout << "!(!p)" << ": " << -p - 1 << "\t!p:" << nv - p - 1 << endl;
            //cout << "!(!q)" << ": " << -q - 1 << "\t!q:" << nv - q - 1 << endl;
            
            gt[nv - q - 1].adj.push_back(-p - 1);
            gt[nv - p - 1].adj.push_back(-q - 1);
        }
        //at this point made the graph and the transposed graph
    }
    //cout << "starting kosaraju's" << endl;
    //find kosaraju's algorithm
    findSC();
    
    //cout << "finished kosaraju's" << endl;
    gsc.resize(counterSC);
    
    //cout << "starting condesation" << endl;
    condensation();
    
    //cout << "condesation done" << endl;
    //now we have condesation dag where each s.c is a vertex
    //cout << "gsc.size()=" << gsc.size() << endl;
    for (int i = 0; i < gsc.size(); ++i) {
        //cout << "sc[" << i << "].size()=" << sc[i].size() << endl;
        for (int j = 0; j < sc[i].size(); ++j){
            int u = sc[i][j];
            //cout << "sc[" << i << "][" << j << "]=" << u << endl;
            //cout << "sc[" << i << ", " << j << "]=" << u << endl;
            if(u < nv){//vertex u is not negated (p)
                //cout << "g[" << u << "].sc=" << g[u].sc <<"\tg[" << u+nv << "].sc=" << g[u + nv].sc << endl;
                if(g[u].sc == g[u + nv].sc){
                    cout << "Not Satisfiable" << endl;
                    return 0;
                }else{
                    if (values[u] == 0){//means that it is empty
                        values[u] = 2;
                        values[u+nv] = 1;
                    }
                }
            } else {//vertex u is negated (!p)
                //cout << "g[" << u << "].sc=" << g[u].sc <<"\tg[" << u-nv << "].sc=" << g[i - nv].sc << endl;
                if(g[u].sc == g[u - nv].sc){
                    cout << "Not Satisfiable" << endl;
                    return 0;
                }else{
                    if (values[u] == 0){
                        values[u] = 2;
                        values[u - nv] = 1;
                    }
                }
            }
        }
    }
    
    for (int i = 0; i < nv; ++i){
        if (values[i] == 1){
            cout << "1";
        }else if (values[i] == 2){
            cout << "0";
        } else {
            cout << "error at v_" << i << " with assigning truth values";
        }
    }
    //if it reaches this point, the cnf is satisfiable
    return 0;
}

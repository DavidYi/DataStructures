//
//  main.cpp
//  ski
//
//  Created by David Yi on 1/19/18.
//  Copyright © 2018 DavidYi. All rights reserved.
//

//
//  main.cpp
//  ski
//
//  Created by David Yi on 1/19/18.
//  Copyright © 2018 DavidYi. All rights reserved.
//

#include <iostream>
#include <vector>
#include <fstream>

using namespace std;
typedef vector<int> VI;

struct Vertex{
    Vertex(): neighbors(){}
    VI neighbors;
};

typedef vector<Vertex> Graph;

Graph g;
int path = 0;

void numPath(int pos){
    for (int i = 0; i < g[pos].neighbors.size(); ++i){
        numPath(g[pos].neighbors[i]);
    }
    
    if (pos == g.size() - 1){//last item
        path = (path + 1) % 1000000007;
    }
    
    return;
}

int main() {
//    ofstream fout("ski.out");
//    ifstream fin("ski.in");
    
    int vertices, edges;
    
    //cout << "Enter the number of vertices:" << endl;
    cin >> vertices;
    //cout << "Enter the number of edges:" << endl;
    cin >> edges;
    g.resize(vertices);
    
    int to, from;
    for (int i = 0; i < edges; ++i){
        //cout << "Enter the next edge:" << endl;
        cin >> from >> to;
        g[from - 1].neighbors.push_back(to - 1);
    }
    
    path = 0;
    numPath(0);//start the search
    cout << path << endl;
    
    return 0;
}



//
//  main.cpp
//  Hanoi
//
//  Created by David Yi on 11/19/17.
//  Copyright © 2017 DavidYi. All rights reserved.
//

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
using namespace std;

typedef vector<int> VI;
typedef vector<long> VL;

vector<VL> n_hanoi;//n_hanoi[twr][dsks] should be the minimum # of moves to solve the problem
vector<VI> k_hanoi;//should give the corresponding # of disks k that should be moved to a spare tower first
// Initialize n_hanoi and k_hanoi

void find_e(int twr, int dsks) {
    for (int i = 1; i < dsks; i++) {
        if (k_hanoi[twr][i] == 0) {
            find_e(dsks, i);
        }
        if (k_hanoi[twr - 1][dsks - i] == 0) {
            find_e(twr - 1, dsks - i);
        }
        int n = n_hanoi[twr][i] + n_hanoi[twr][i] + n_hanoi[twr - 1][dsks - i];
        if (n < n_hanoi[twr][dsks]) {
            n_hanoi[twr][dsks] = n;
            k_hanoi[twr][dsks] = i;
        }
    }
}

void hanoi_init() {
    VL n(10001, pow(2, 62));
    VI k(10001, 0);
    for (int i = 0; i < 11; i++) {
        n_hanoi[i][1] = 1;
        n_hanoi.push_back(n);
        k_hanoi[i][1] = 1;
        k_hanoi.push_back(k);
    }

    for (int i = 1; i < 10000; i++) {
        k_hanoi[3][i] = i - 1;
    }

    for (int i = 2; i < 10000; i++) {
        if (i < 63)
            n_hanoi[3][i] = pow(2, i) - 1;
            
        find_e(4, i);
        find_e(5, i);
        find_e(6, i);
        find_e(7, i);
        find_e(8, i);
        find_e(9, i);
        find_e(10, i);
    }
}

void swap(VI& aux, int from, int to){
    int temp = aux[from];
    aux[from] = aux[to];
    aux[to] = temp;
}

// Fill moves so that each element is a two-integer VI describing the move
// You may assume that, initially, aux[] = {0, 1, 2, ..., n_disks - 1}
void hanoi(vector<VI>& moves, int n_twrs, int n_dsks, VI& aux) {
    // aux[0] is always origin and aux[1] is always destination
    if (n_dsks == 1) {
        VI move;
        move.resize(2);
        move[0] = aux[0]; // origin
        move[1] = aux[1]; // destination
        moves.push_back(move);
        return;
    }
    if (k_hanoi[n_twrs][n_dsks] > 0) {
        // moves k pegs from first to last tower
        swap(aux[1], aux[n_twrs-1]);
        hanoi(moves, n_twrs, k_hanoi[n_twrs][n_dsks], aux);
                
        // moves n-k pegs from first to destination tower
        swap(aux[1], aux[n_twrs-1]);
        hanoi(moves, n_twrs - 1, n_dsks - k_hanoi[n_twrs][n_dsks], aux);
                
        // moves k pegs from last to destination tower
        swap(aux[0], aux[n_twrs-1]);
        hanoi(moves, n_twrs, k_hanoi[n_twrs][n_dsks], aux);
        return;
    }
    
    hanoi(moves, n_twrs, n_dsks, aux);
}
/*
int main() {
    hanoi_init();
    
    ifstream in ("in.txt");
    ifstream out ("out.txt");
    
    int n, k;
    in >> n >> k;
    for (int i = 0; i < n; i++) {
        int a, b;
        long long ans;
        in >> a >> b;
        out >> ans;
        if (n_hanoi.size() <= a || n_hanoi[a].size() <= b) {
            cout << "your code can't handle " << a << " pillars and " << b << " disks\n";
            return 3735928559;
        }
        if (n_hanoi[a][b] != ans) {
            cout << "number of moves to do " << a << " pillars and " << b << " disks is " << ans << ", but you gave " << n_hanoi[a][b] << "\n";
            return 3735928559;
        }
    }
    cout << "Init works";
    
    for (int i = 0; i < k; i++) {
        int a, b;
        long long x;
        in >> a >> b;
        out >> x;
        
        if (n_hanoi.size() <= a || n_hanoi[a].size() <= b) {
            cout << "your code can't handle " << a << " pillars and " << b << " disks\n";
            return 3735928559;
        }
        if (n_hanoi[a][b] != x) {
            cout << "number of moves to do " << a << " pillars and " << b << " disks is " << x << ", but you gave " << n_hanoi[a][b] << "\n";
            return 3735928559;
        }
        
        
        vector<VI> v;
        VI aux(a);
        for (int j = 0; j < aux.size(); j++)
            aux[j] = j;
        hanoi(v, a, b, aux);
        vector<vector<int> > p(a);
        for (int j=0;j<b;j++)
            p[0].push_back(b-j-1);
        for (int j = 0; j < v.size(); j++) {
            int f = v[j][0], t = v[j][1];
            if (!p[f].size() || p[t].size() && p[f].back()>p[t].back()){
                cout << "right number of moves but moves are wrong for " << a << " pillars and " << b << " disks\n";
                cout << "tried to make invalid move "<<f<<" ==> "<<t<<" move number "<<j<<"\n";
                return 3735928559;
            }
            
            p[t].push_back(p[f].back());
            p[f].resize(p[f].size()-1);
        }
        
        if (p[0].size() || p[1].size()!=b){
            cout << "right number of moves but moves are wrong for " << a << " pillars and " << b << " disks\n";
            cout << "not all pillars moved\n";
        }
        
    }
    
    cout << "all clear :D !!!\n";
    
    cout << string(18, ' ') << string(1, 'x') <<"\n"; cout << string(17, ' ') << string(3, 'x') <<"\n"; cout << string(16, ' ') << string(5, 'x') <<"\n"; cout << string(15, ' ') << string(7, 'x') <<"\n"; cout << string(15, ' ') << string(7, 'x') <<"\n"; cout << string(13, ' ') << string(11, 'x') <<"\n"; cout << string(9, ' ') << string(20, 'x') << string(10, ' ') << string(1, 'H') << string(1, 'A') << string(2, 'P') << string(1, 'Y') <<"\n"; cout << string(12, ' ') << string(6, '/') << string(6, '\\') << string(13, ' ') << string(1, 'H') << string(1, 'A') << string(2, 'L') << string(1, 'O') << string(1, 'W') << string(2, 'E') << string(1, 'N') <<"\n"; cout << string(11, ' ') << string(3, '/') << string(2, ' ') << string(1, 'O') << string(3, ' ') << string(1, 'O') << string(2, ' ') << string(2, '\\') << string(14, ' ') << string(1, 'D') << string(1, 'U') << string(1, 'D') << string(1, 'E') <<"\n"; cout << string(11, ' ') << string(3, '/') << string(4, ' ') << string(1, 'U') << string(4, ' ') << string(3, '\\') <<"\n"; cout << string(11, ' ') << string(2, '/') << string(3, ' ') << string(1, '\\') << string(3, '_') << string(1, '/') << string(3, ' ') << string(2, '\\') << string(9, ' ') << string(1, '/') << string(1, '\\') << string(7, ' ') << string(1, '.') <<"\n"; cout << string(14, ' ') << string(1, 'a') << string(7, ' ') << string(1, 'a') << string(12, ' ') << string(1, '\\') << string(2, ' ') << string(1, '\\') << string(1, ' ') << string(2, '_') << string(1, ' ') << string(1, '/') << string(1, '|') <<"\n"; cout << string(12, ' ') << string(5, 'a') << string(3, ' ') << string(5, 'a') << string(12, ' ') << string(1, 'o') << string(3, ' ') << string(1, 'o') <<"\n"; cout << string(9, ' ') << string(19, 'a') << string(7, ' ') << string(3, ':') << string(1, ' ') << string(1, '@') << string(1, ' ') << string(3, ':') <<"\n"; cout << string(8, ' ') << string(7, 'a') << string(2, ' ') << string(4, 'a') << string(3, ' ') << string(6, 'a') << string(7, ' ') << string(1, '\\') << string(1, '/') << string(1, ' ') << string(1, '\\') << string(1, '/') << string(4, ' ') << string(1, '\\') << string(5, ' ') << string(1, 'M') << string(2, 'e') << string(1, '-') << string(1, ' ') << string(1, 'e') << string(2, 'o') << string(1, 'W') << string(1, ' ') << string(1, '!') <<"\n"; cout << string(7, ' ') << string(5, 'a') << string(15, ' ') << string(4, 'a') << string(9, ' ') << string(1, '\\') << string(4, ' ') << string(3, '\\') <<"\n"; cout << string(6, ' ') << string(5, 'a') << string(2, ' ') << string(5, 'a') << string(1, ' ') << string(7, 'a') << string(2, ' ') << string(3, 'a') << string(10, ' ') << string(1, '\\') << string(3, ' ') << string(2, '#') << string(2, ' ') << string(1, '\\') <<"\n"; cout << "\n"; cout << string(70, '#') <<"\n"; cout << "\n"; cout << string(1, '#') << string(2, ' ') << string(1, '#') << string(3, ' ') << string(1, '#') << string(3, ' ') << string(4, '#') << string(1, ' ') << string(4, '#') << string(1, ' ') << string(1, '#') << string(3, ' ') << string(1, '#') <<"\n"; cout << string(4, '#') << string(2, ' ') << string(3, '#') << string(2, ' ') << string(1, '#') << string(1, ' ') << string(2, '#') << string(1, ' ') << string(1, '#') << string(1, ' ') << string(2, '#') << string(2, ' ') << string(3, '#') <<"\n"; cout << string(1, '#') << string(2, ' ') << string(1, '#') << string(1, ' ') << string(1, '#') << string(1, ' ') << string(1, '#') << string(1, ' ') << string(1, '#') << string(1, ' ') << string(1, '#') << string(4, ' ') << string(1, '#') << string(6, ' ') << string(1, '#') <<"\n"; cout << "\n"; cout << string(7, ' ') << string(1, '#') << string(2, ' ') << string(1, '#') << string(3, ' ') << string(1, '#') << string(3, ' ') << string(1, '#') << string(3, ' ') << string(1, '#') << string(4, ' ') << string(2, '#') << string(2, ' ') << string(1, '#') << string(5, ' ') << string(1, '#') << string(1, ' ') << string(4, '#') << string(1, ' ') << string(4, '#') << string(1, ' ') << string(1, '#') << string(2, ' ') << string(2, '#') <<"\n"; cout << string(7, ' ') << string(4, '#') << string(2, ' ') << string(3, '#') << string(2, ' ') << string(1, '#') << string(3, ' ') << string(1, '#') << string(3, ' ') << string(1, '#') << string(2, ' ') << string(1, '#') << string(2, ' ') << string(1, '#') << string(1, ' ') << string(1, '#') << string(1, ' ') << string(1, '#') << string(2, ' ') << string(1, '#') << string(2, '-') << string(2, ' ') << string(1, '#') << string(2, '-') << string(2, ' ') << string(1, '#') << string(1, ' ') << string(1, '#') << string(1, ' ') << string(1, '#') <<"\n"; cout << string(7, ' ') << string(1, '#') << string(2, ' ') << string(1, '#') << string(1, ' ') << string(1, '#') << string(3, ' ') << string(1, '#') << string(1, ' ') << string(3, '#') << string(1, ' ') << string(3, '#') << string(2, ' ') << string(2, '#') << string(4, ' ') << string(3, '#') << string(3, ' ') << string(4, '#') << string(1, ' ') << string(4, '#') << string(1, ' ') << string(2, '#') << string(2, ' ') << string(1, '#') <<"\n"; cout << "\n"; cout << string(71, '#') <<"\n";
    
    in.close();
    out.close();
    return 0;
}
*/

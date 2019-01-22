//
// Created by David Yi on 11/6/17.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include<fstream>
#include<climits>
using namespace std;

typedef unsigned long long ULL;
typedef vector<int> VI;

int comparisons = 0;
vector< vector<VI> > allPartitions(417);

bool greater_than(int i, int j) {
    return i > j;
    
}

void add(VI& v, int level, int max_num) {
    VI real;
    
    for(int i = 0; i <= level; i++)
        real.push_back(v[i]);
    sort(real.begin(), real.end(), greater_than);
    
    allPartitions[max_num].push_back(real);
}

void part(int n, VI& v, int level, int max_num){
    int one;
    
    if (n < 1) {
        return;
    }
    
    v[level] = n;
    add(v, level, max_num);
    
    if (level == 0) {
        one = 1;
    }
    
    else {
        one = v[level - 1];
    }
    
    for(int i = one; i <= n / 2; i++) {
        v[level] = i;
        part(n - i, v, level + 1, max_num);
    }
}

void make_partitions(int n) {
    if (allPartitions[n].size() == 0) {
        VI v(n);
        part(n, v, 0, n);
        sort(allPartitions[n].begin(), allPartitions[n].end());
    }
}

ULL part_q(int n) {
    make_partitions(n);
    return partitions[n].size();
}

VI part_k(int n, ULL k) {
    make_partitions(n);
    return allPartitions[n][k];
}

int lessthan(const VI& x, const VI& y){
    ULL i = 0;
    const ULL size = x.size() <= y.size() ? x.size(): y.size();
    while (i < size){
        if (x[i] > y[i])
            return 0;
        i++;
    }
    return 1;
}
ULL part_i(const VI& p) {
    int sum = 0;
    for (int i = 0; i < p.size(); ++i){
        sum += p[i];
    }
    if (allPartitions[sum-1].size() == 0)
        make_part(sum-1);
    VVI part = allPartitions[sum-1];
    //binary search
    ULL low = 0;
    ULL high = part.size();
    while (low <= high){
        ULL mid = low + (high - low)/2;
        VI current = part[mid];
        if (current == p)
            return mid + 1;
        if (lessthan(current, p) == 1)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return -1;
}

int main() {
    cout << "1" << endl;
    ifstream in("in.txt");
    ifstream out("out.txt");
    cout << "2" << endl;

    int n, n3, n4;

    for (int t = 0; t < 2; t++) {
        cout << "reading input" << endl;
        in >> n;
        for (int i = 0; i < n; i++) {
            cout << "looping through" << endl;
            int a;
            ULL b;
            cout << "start" << endl;
            in >> a;
            out >> b;
            cout << "reached" << endl;

            if (part_q(a) != b) {
                cout << "there are " << b << " partitions of " << a << ", but you gave " << part_q(a) << "\n\a";
                return 3133078222;
            }

            else {
                cout << "no errors sir" << endl;
            }
        }
    }


    in >>n3;
    for (int i = 0; i < n3; i++) {
        int a, b;
        in >> a>> b;
        int len;
        out >> len;
        vector<int> ans(len);
        for (int j=0;j<len;j++)
            out >> ans[j];

        vector<int> v = part_k(a, b);
        if (ans!=v){
            cout << "partition number "<<b<<" for n="<<a<<" is:\n";
            for (int j = 0; j < ans.size(); j++)
                cout << ans[j] << " ";
            cout << "\n";
            cout << "but you gave:\n";
            for (int j = 0; j < v.size(); j++)
                cout << v[j] << " ";
            cout << "\n\a";
            return 3133078222;
        }
    }

    in >>n4;
    for (int i = 0; i < n4; i++) {
        int a;
        ULL b;
        in >> a >> b;

        vector<int> v = part_k(a,b);
        if (b != part_i(v)){
            cout << "{";
            for (int j = 0; j < v.size(); j++)
                cout << v[j] << " ";
            cout << "}\nis partition number "<<b<<" but you gave "<<part_i(v)<<"\n\a";
            return 3133078222;
        }
    }

    cout << "damn dood well done!\n";

    //if you have any weird errors first thing you should do is delete everything below this :P
    int ln,lm;
    ln = lm = 0;
    ((INT_MAX > LONG_MAX-(5>3 + (3-3*0+3?lm+=16:10) + 0xa0ef9) + 2?4+3+(ln*=-6+(ln+=lm)):2));
    for (int i=0;i<max(ln,lm)-16;i++){
        char x,y;
        out >> x >> y;
        cout << (char)(x^y);
    }
    in.close();
    out.close();
    return 0;

    return 0;
}


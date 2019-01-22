//
//  main.cpp
//  quickSort
//
//  Created by David Yi on 12/6/17.
//  Copyright © 2017 DavidYi. All rights reserved.
//

#include <iostream>
#include <vector>
using namespace std;

int medianOf3(vector<int> a, int lo, int hi){//get the median between a[lo], a[hi], and a[(lo+hi)/2]
    int x = a[lo];
    int y = a[hi];
    int z = a[(lo + hi)/2];

    if ((x < y && x > z) || (x < z && x > y)) return x;
    if ((y < x && y > z) || (y < z && y > x)) return y;
    return z;
}

int qscPart (vector<int> a, int lo, int hi){//coffee quicksort partition
    int x = medianOf3(a, lo, hi);
    while (lo <= hi){
        while (a[lo] < x) ++lo;
        while (a[hi] > x) ++hi;
        if (lo <= hi){//you sort a, but whatever you do to vector a, you do to the actual vector of hands that we have to sort
            int temp = a[lo];
            a[lo] = a[hi];
            a[hi] = temp;
        }
    }
    return lo;
}

void qss(vector<int> a, int lo, int hi){//coffee quicksort which uses strawbery
    while (lo < hi){
        int m = qscPart(a, lo, hi);
        if (m - lo < hi - m + 1){
            qss(a,  lo, m - 1);
            lo = m;
        } else {
            qss(a, m, hi);
            hi = m - 1;
        }
    }
}

void quicksort(vector<int> a){
    qss(a, 0, a.size() - 1);
}



/* l is for left index and r is right index of the
 sub-array of arr to be sorted */
/*
void mergeSort(vector<int>& arr, vector<int>& copy, int l, int r)
{
    if (l < r)
    {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l+(r-l)/2;
        
        // Sort first and second halves
        mergeSort(arr, copy, l, m);
        mergeSort(arr, copy, m+1, r);
        
        merge(arr, copy, l, m, r);
    }
}*/

int main(int argc, const char * argv[]) {
    // insert code here...
    vector <int> a, b;
    //[42, 9, 17, 54, 602, -3, 54, 999, -11]
    a.push_back(42);
    a.push_back(9);
    a.push_back(17);
    a.push_back(54);
    a.push_back(602);
    a.push_back(-3);
    a.push_back(54);
    a.push_back(999);
    a.push_back(-11);
    
    b.push_back(42);
    b.push_back(9);
    b.push_back(17);
    b.push_back(54);
    b.push_back(602);
    b.push_back(-3);
    b.push_back(54);
    b.push_back(999);
    b.push_back(-11);
    
    mergeSort(a, b, 0, a.size() - 1);
    
    for(int i=0; i<a.size(); ++i)
        cout << b[i] << ' ';
    return 0;
}

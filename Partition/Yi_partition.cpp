//
// Created by David Yi on 11/6/17.
//

#include <vector>
using std::vector;

typedef unsigned long long ULL;
typedef vector<int> VI; //a partition
typedef vector<VI> VVI; //a vector of partitions

vector<VVI> allPartitions(447, vector<VI>(0, VI(0)));//creates table to store all partitions of all integers

VVI make_part_of_n(int max, int remainder){
    VVI partitions;//all partitions of n
    
    
    return partitions;
}


void make_part(int n){//make all partitions for int n
    VVI partitions = make_part_of_n(n, n);
    allPartitions[n] = partitions;
}

vector<VI> make_part_f(int n){
    vector<VI> partitions; //all partitions of n
    int i = n;//the first number of each partition
    int j;
    
    while (i >= 1){
        j = n - i;//n - num
        VI* part = new VI(i);
        while (j >= 0){
            part->push_back(j);
            --j;
        }
        partitions.push_back(*part);
        --i;
    }
    
    return partitions;
}


ULL part_q(int n){
    if (allPartitions[n-1].size() == 0)// if the partitions of n have not been make, then make it
        make_part(n-1);
    return allPartitions[n-1].size();
}

VI part_k(int n, ULL k){
    if (allPartitions[n-1].size() == 0)
        make_part(n-1);
    return allPartitions[n-1][k];
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

ULL part_i(const VI& p){
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

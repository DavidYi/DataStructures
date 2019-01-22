//
// Created by David Yi on 9/15/17.
//

//#include "assignment1.h"
#include <vector>
#include <iostream>
#include <fstream>
using std::vector;
using std::cout;

template<class T>
struct Link {
    explicit Link(const T &info, Link *next = 0) : info(info), next(next) {}
    
    // This avoids stack overflow
    ~Link() {
        Link *p = next;
        while (p) {
            Link *q = p->next;
            p->next = 0;
            delete p;
            p = q;
        }
    }
    
    T info;
    Link *next;
};

vector<int> loopTail(Link<int>* head){
    int lenLoop = 0;
    int lenTail = 0;
    
    bool isLoop = false;
    vector<int> output;
    
    Link<int> *tortoise = head;
    Link<int> *hare = head;
    while (hare->next && hare->next->next){
        hare = hare->next->next;
        tortoise = tortoise->next;
        if (hare == tortoise){
            isLoop = true;
            break;
        }
    }
    
    if (isLoop){//if has loop in linked list
        
        do{
            hare = hare->next;
            lenLoop++;
        }while (tortoise != hare);
        
        
        hare = head;
        tortoise = head;
        for (int i = 0; i < lenLoop; i++){
            hare = hare->next;
        }
        while (tortoise != hare){
            hare = hare->next;
            tortoise = tortoise->next;
            lenTail++;
        }
    } else{//case if no loop in linked list
        lenLoop = 0;
        lenTail = 0;
        hare = head;
        while (hare){
            lenTail++;
            hare = hare->next;
        }
    }
    
    output.push_back(lenLoop);
    output.push_back(lenTail);
    
    //print out the output
    //cout << "[" << output[0] << ", " << output[1] << "] \n";
    
    return output;
};

vector<int> josephus(int n, int k) {
    vector<int> output;
    
    //create circular linked list
    Link<int>* head= new Link<int>(1);
    Link<int>* node = head;
    for (int i = 2; i <= n; ++i){
        node->next = new Link<int>(i);
        node = node->next;
    }
    node->next = head;
    
    
    //start to get rid of people
    int k_mod;
    while (n){
        //optimization
        k_mod = (k % n == 0) ? k : k % n;
        for (int i = 1; i < k_mod; ++i) {
            node = node->next;
        }
        output.push_back(node->next->info);
        
        node->next = node->next->next;
        
        //update info
        --n;
        
        
        //print out output
        //cout << output[pos - 1] << " -> ";
    }
    //cout << "all dead >:)\n";
    
    return output;
}


 #include "stdc++.h"
 
 //using external test cases
 int main() {
     std::cout << "testing josephus:\n";
     
     std::ifstream input("in.txt");
     std::ifstream output("out.txt");
     
     int n;
     input >> n;
     bool fail = false;
     for(int i = 0; i < n; i++) {
     int a, b;
     input >> a >> b;
     std::clock_t start = std::clock();
     vector<int> v = josephus(a, b);
     double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
     
     for (int j = 0; j < a; j++){
     int ans;
     output >> ans;
     if (v[j] != ans) {
     std::cout << v[j] << "," << ans << "\n";
     std::cout << "fail on case #" << i << " at index "<< j <<"\n";
     fail = true;
     break;
     }
     }
     std::cout << "case #"<<i<<" took "<<duration<<" secs to run\n";
     }
     
     if (!fail)
     std::cout << "all clear!\n";
     
     
     input.close();
     output.close();
     std::cout << "----------------------\n\n";
     
     std::ifstream input2("in2.txt");
     std::ifstream output2("out2.txt");
     
     std::cout << "testing floyd:\n";
     input2 >> n;
     fail = false;
     for(int i = 0; i < n; i++) {
     int a, b, c;
     input2 >> a >> c >> b;
     
     vector<Link<int>* > ls(a);
     ls[a-1]= new Link<int>(0);
     for (int j = a-2; j >= 0; j--)
     ls[j]= new Link<int>(0,ls[j + 1]);
     if (b != -1)
     ls[b]->next = ls[c];
     
     std::clock_t start = std::clock();
     vector<int> v = loopTail(ls[0]);
     double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
     
     for (int j = 0; j < v.size(); j++){
     int ans;
     output2 >> ans;
     if (v[j] != ans) {
     std::cout << v[j] << "," << ans << "\n";
     std::cout << "fail on case #" << i << " at index "<< j <<"\n";
     fail = true;
     break;
     }
     }
     std::cout << "case #"<<i<<" took "<<duration<<" secs to run\n";
     }
     
     if (!fail)
         std::cout << "all clear!\n";
     input2.close();
     output2.close();
 }
 /*
//using my test cases
int main() {
    
    Link<int>* head,* node,* temp;
    vector<int> out;
    cout << "testing josephus:\n";
    
    cout << "----------------------\n\n";
    
    //kill off every by 1
    out = josephus(419, 289);
    
    //kill off by every 5 for 1 soldier --> 1
    out = josephus(1,5);
    
    out = josephus(10,3);
    
    
    cout << "testing floyd:\n";
    
    cout  << "\n" <<  "----------------------\n\n";
    
    //tail = 3, loop = 5;
    head = new Link<int>(1);
    node = head;
    for (int i = 2; i <= 8; i++){
        node->next = new Link<int>(i);
        node = node->next;
        if (i == 4)
            temp = node;
    }
    
    node->next = temp;
    
    out = loopTail(head);
    
    //tail = 0, head = 5
    head= new Link<int>(1);
    node = head;
    for (int i = 2; i <= 5; i++){
        node->next = new Link<int>(i);
        node = node->next;
    }
    node->next = head;
    out = loopTail(head);
    
    //tail = 5 head = 0
    head= new Link<int>(1);
    node = head;
    for (int i = 2; i <= 5; i++){
        node->next = new Link<int>(i);
        node = node->next;
    }
    out = loopTail(head);
    
    //tail = 6 head = 0
    head= new Link<int>(1);
    node = head;
    for (int i = 2; i <= 6; i++){
        node->next = new Link<int>(i);
        node = node->next;
    }
    
    out = loopTail(head);
}
//*/

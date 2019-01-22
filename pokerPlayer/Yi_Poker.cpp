//
//  main.cpp
//  pokerPlayer
//
//  Created by David Yi on 5/1/18.
//  Copyright © 2018 DavidYi. All rights reserved.
//

#include <vector>
#include <iostream>
#include <fstream>
#include <random>

using namespace std;

int main(int argc, const char * argv[]) {
    random_device seed;
    mt19937 gen(seed());
    uniform_int_distribution<int> u (0, 4);
    vector<int> hand;
    
    int card;
    
    for (int i = 0; i < 5; ++i){
        cin >> card;
        hand.push_back(card);
    }
    
    int x = u(gen);
    cout << x << " ";
    
    for (int j = 0; j < x; ++j){
        cout << hand[j] << " ";
    }
    cout << endl;
    
    for (int j = 0; j < x; ++j){
        cin >> hand[j];
    }
}

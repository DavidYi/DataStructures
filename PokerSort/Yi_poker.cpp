//
//  Yi_poker.cpp
//  Datastructures
//
//  Created by David Yi on 12/5/17.
//  Copyright © 2017 DavidYi. All rights reserved.
//

#include <vector>
#include <iostream>
#include <fstream>
using std::cout;
using std::cin;
using std::vector;
using std::max;
using std::min;

void printHand(vector<int> hand){
    for(int i=0; i<hand.size(); ++i){
        int card = hand[i];
        int suit = hand[i] % 4;
        int rank = 2 + hand[i] / 4;
        
        cout << card;
        if (suit == 0) cout << "diamond";
        else if (suit == 1) cout << "clubs";
        else if (suit == 2) cout << "hearts";
        else if (suit == 3) cout << "spades";
        else cout << "suit error";
        
        if (rank < 11)
            cout << rank << '\n';
        else if (rank == 14)
            cout << "A" << '\n';
        else if (rank == 11)
            cout << "J" << '\n';
        else if (rank == 12)
            cout << "Q" << '\n';
        else if (rank == 13)
            cout << "K" << '\n';
    }
}

void insertSort(vector<int>& a) {
    for(int i = 1; i < a.size(); ++i) {
        for(int j = i; j > 0 && a[j-1] > a[j]; --j) {
            std::swap(a[j], a[j-1]);
        }
    }
}

int convert(int n){
    vector<int> hand;
    int i = 0;
    
    //cout << "\n\n*HAND TO CARD\n";
    while (i < 5) {
        //cout << "\n" << i << "th card\tN:" << n << "--> cardEncoded:" << n % 52 << "\n";
        hand.push_back(n % 52);
        n /= 52;
        i++;
    }
    insertSort(hand);
    
    bool isFlush = true, isAce = false;//isAce is exculsively to check for straight with an ace in it
    int prevS, r, s, repeatR, least = 13, great = 0;//least and great will be every other rank but ace
    vector<int> ranks; //occurences of the 13 cards
    
    for (int i = 0; i < 13; ++i){
        ranks.push_back(0);
    }
    
    //cout << "\n\n*CARD DECODED";
    int pair = 0;
    bool is4 = false, is3 = false;
    for (int i = 0; i < 5; ++i){
        r = hand[i]/4;
        //cout << "\nrank:"  << r << " from cardEncoded:" <<  hand[i] << "\n";
        ++ranks[r];
        if (ranks[r] == 4)
            is4 = true;
        else if (ranks[r] == 2){
            //cout << "pair of " << r;
            pair++;
        }
        else if (ranks[r] == 3){
            pair--;
            is3 = true;
        }
        //cout << r << "'s occurence:\t" << ranks[r] << "times\n";
        s = hand[i] % 4;
        
        if (r == 12)
            isAce = true;
        else{
            if (r < least) least = r;// if it is not an ace and the current rank is greater than the current least, then make least = current rank
            if (r > great) great = r;//same thing as above but for greatest
        }
        if (i != 0){
            if (s != prevS) isFlush = false;//if the previous suit is not the current suit then there is no way that it is a suit
        }
        prevS = s;
    }
    
    bool isStraight = false;
    int range = great - least;
    //check if it is a straight or not
    if (pair == 0 && !is3 && !is4){
        if (range == 4 && !isAce){
            isStraight = true;
        }else if (range == 3 && isAce){
            if (great == 3 && least == 0){
                isStraight = true;
                great = 3;
            } else if (great == 11 && least == 8){
                isStraight = true;
                great = 12;
            }
        }
    }
    
    //RETURN THE ENCODED VALUES
    
    //straight flush
    if (isFlush && isStraight){
        //cout << "straightFlush" << "\n";
        return 50000000*(1 + great);//at least 250M and at most 700M
    }
    
    //convert the hand to some number that can be sorted compared to the other hands
    if (is4){//four of a kind
        
        //cout << "Four of Kind" << "\n";
        
        int c1 = 1 + hand[0]/4, c2 = 1 + hand[4]/4;//since it is sorted, then either the first or the last card in the hand is four of kind
        if (ranks[c1-1] == 4)
            return 150000000 + 10000*c1 + 100*c2;//at least 150,010,200 and at most 150,131,200
        return 150000000 + 10000*c2 + 100*c1;
    }
    if (is3 && pair == 1){//full house
        
        //cout << "Full House" << "\n";
        
        int c1 = 1 + hand[0]/4, c2 = 1 + hand[4]/4;//one is a pair and the other is 3 of kind since its sorted
        if (ranks[c1-1] == 3)
            return 140000000 + 10000*c1 + 100*c2; //at least 140,010,200 at most 140,131,200
        return 140000000 + 10000*c2 + 100*c1;
    }
    if (isFlush){//flush
        
        //cout << "Flush" << "\n";
        
        //from lowest card to highest card
        int c1 = 1 + hand[0]/4, c2 = 1 + hand[1]/4, c3 = 1 + hand[2]/4, c4 = 1 + hand[3]/4, c5 = 1 + hand[4]/4;
        return c1 + 14*c2 + 14*14*c3 + 14*14*14*c4 + 10000000*c5 + 5000000;// at least 15,006,137 and at most 135,035,233
    }
    //straight
    if (isStraight){
        
        //cout << "straight" << "\n";
        return 14000000 + 100*(1+great);//at least 14,000,100 and most 14,001,400
    }
    
    if (is3){//3 of a kind
        
        //cout << "Three of Kind" << "\n";
        int sum = 0, past = -1, factor = 1, third;
        for (int i = 0; i < 5; ++i){
            int card = 1 + hand[i]/4;
            if (past == card){
                third = card;
            } else{
                sum += factor * card;
                factor *= 14;
            }
            past = card;
        }
        return sum + 10000*third + 10000000;//at least 10,100,227 at most 11,791,189
    }
    
    if (pair == 2){//2 pairs
        
        //cout << "Two Pairs" << "\n";
        
        int sum = 0, past = -1, factor = 1, pair1 = -1, pair2;
        for (int i = 0; i < 5; ++i){
            //cout << sum << " --> ";
            int card = 1 + hand[i]/4;
            if (past == card){
                if (pair1 == -1)
                    pair1 = card;
                else
                    pair2 = card;
            } else{
                sum += factor * card;
                factor *= 14;
                
                //cout << sum << "\n";
            }
            past = card;
        }
        if (pair1 > pair2)
            sum += 50000*14*pair1 + 50000*pair2;
        sum += 50000*14*pair2 + 50000*pair1; //at least 1,642,775 at most  9,218,260
        
        //cout << sum << "end";
        return sum;
    }
    
    if (pair == 1){//1 pair
        
        //cout << "Pair of";
        
        int sum = 0, past = -1, factor = 1;
        for (int i = 0; i < 5; ++i){
            int card = 1 + hand[i]/4;
            if (past == card){
                sum += card*50000;
            }else{
                sum += factor * card;
                factor *= 14;
            }
            past = card;
        }
        sum -= 5000000;
        return sum;//at least 584,496 at most 1,554,505
    }
    
    //cout << "Highcard" << "\n";
    //highcard
    int c1 = 1 + hand[0]/4, c2 = 1 + hand[1]/4, c3 = 1 + hand[2]/4, c4 = 1 + hand[3]/4, c5 = 1 + hand[4]/4;
    return c1 + 14*c2 + 14*14*c3 + 14*14*14*c4 + 14*14*14*14*c5 - 100000000;//at least -455,447 at most 34,641
    
}

void insertion_sort(vector<int>& a, vector<int>& copy) {
    for(int i = 1; i < a.size(); ++i) {
        for(int j = i; j > 0 && a[j-1] > a[j]; --j) {
            std::swap(a[j], a[j-1]);
            std::swap(copy[j], copy[j-1]);
        }
    }
}

int medianOf3(vector<int>& a, int lo, int hi) {
    return max( min( a[lo],a[hi] ), min( max(a[lo],a[hi]),a[ (lo+hi)/2 ] ) );
}

int qscPart(vector<int>& a, vector<int>& copy, int lo, int hi) {
    int x = medianOf3(a, lo, hi);
    
    while(lo <= hi) {
        while(a[lo] < x) ++lo;
        while(a[hi] > x) --hi;
        
        if(lo <= hi) {
            int t = a[lo];
            int t2 = copy[lo];
            
            a[lo] = a[hi];
            a[hi] = t;
            copy[lo++] = copy[hi];
            copy[hi--] = t2;
            
        }
    }
    return lo;
    
}

void qsch(vector<int>& a, vector<int>& copy, int lo, int hi) {
    while(hi - lo > 20) {
        int m = qscPart(a, copy, lo, hi);
        if(m - lo < hi - m + 1) {
            qsch(a, copy, lo, m-1);
            lo = m;
        }
        else {
            qsch(a, copy, m, hi);
            hi = m-1;
        }
    }
}

void quicksort(vector<int>& a, vector<int>& copy) {
    qsch(a, copy, 0, a.size() - 1);
    insertion_sort(a, copy);
}

void poker_sort(vector<int>& a){//a is an array of poker hands {h0, h1, h2, h3, h4, h5 ..., hn}
    //each hand is encoded as h = c0 + c1 * 52 + c2 * 52^2 + c3 * 52^3 + c4 * 52^4 where c is a card
    //each card is encoded as c = 4r + s
    vector<int> encode;
    encode.resize(a.size());
    //get the array of encoded
    for (int i = 0; i < a.size(); i++){
        encode[i] = convert(a[i]);
        //cout << "convert: " << encode[i] << '\t';
    }
   // cout << "<-- encode \n";
    
//    for(int i=0; i<a.size(); ++i){
//        cout << a[i] << '\t';
//    }
    
    //cout << " <-- a\n";
    quicksort(encode, a);
    
//    for(int i=0; i<a.size(); ++i)
//       // cout << a[i] << ":" << encode[i] << '\n';
//    return;
    
}



void printPoke(int x){
    vector<int> hand;
    int n = x;
    int i = 0;
    
    //cout <<"\n\nPRINT POKE\n";
    while (i < 5) {
        //cout << "\ni: " << i << "\tn: " << n << "--> hand:" << n % 52 << "";
        hand.push_back(n % 52);
        n /= 52;
        i++;
    }
    insertSort(hand);//sort the hand to better convert it to a value so that we can sort the hands
    
    //cout << "\n\n CONVERTED VALUE OF " << x << " IS: " << convert(x) << "!!\n\n\n";
    
    printHand(hand);
}

//#include<fstream>
//using std::ifstream;
//int main() {
//    ifstream in("in.txt");
//    int n, past;
//    in >> n >> past;
//    for (int i = 1; i < n; i++) {
//
//        cout << "\n\n********TESTCASE: " <<  i << "************\n";
//        int x;
//        in >> x;
//        vector<int> v;
//        v.push_back(past);
//        v.push_back(x);
//        poker_sort(v);
//
//        if (v[0] != past || v[1] != x){
//
//            cout << convert(past) << "\n";
//            printPoke(past);
//            cout << "next one" << "\n";
//            printPoke(x);
//
//            cout << "\nUNSORTED: {" << x << "," << past << "}\n";
//
//            cout << "SORTED: {" << v[0] << "," << v[1] << "}\n";
//
//            cout << "the hand " << past << " is worse than the hand " << x << ", your program either switched these or deleted one when sorting the vector {" << past << ", " << x << "}\n";
//
//            //if (convert(x) != convert(past))
//                return -1;
//        }
//        past = x;
//    }
//
//    cout << "well done!\n";
//    in.close();
//}
/*
using namespace std;
#include<fstream>
#include<ctime>
#include<vector>
#include<iostream>
#include<map>
int main() {
    ifstream in("in.txt");
    
    int m, n, rr, v;
    in >> m;
    for (int mm = 0; mm < m; mm++) {
        in >> n;
        cout << n << " elements... ";
        vector<int> a(n);
        map<int, int> r;
        for (int i = 0; i < n; i++) {
            in >> a[i] >> rr;
            r[a[i]] = rr;
        }
        
        clock_t start = std::clock();
        poker_sort(a);
        cout << "\t"<<( std::clock() - start ) / (double) CLOCKS_PER_SEC << " seconds\n";
        cout << "\tvalidating...\t";
        for (int i = 0; i < n - 1; i++)
            if (r[a[i]] > r[a[i + 1]]) {
                cout << "\n\n********TESTCASE: " <<  i << "************\n\n";
                cout << "\n\thand " << a[i] << " = (";
                v = a[i];
                for (int j = 0; j < 5; j++) {
                    if (j) cout << "\t";
                    cout << (v % 52) / 4 << " " << ((v % 4) == 0 ? 'A' : (v % 4) == 1 ? 'B' : (v % 4) == 2 ? 'C' : 'D');
                    v /= 52;
                }
                cout << ") is better than hand " << a[i + 1] << " = (";
                v = a[i+1];
                for (int j = 0; j < 5; j++) {
                    if (j) cout << "\t";
                    cout << (v % 52) / 4 << " " << ((v % 4) == 0 ? 'A' : (v % 4) == 1 ? 'B' : (v % 4) == 2 ? 'C' : 'D');
                    v /= 52;
                }
                
                cout << ") but you put these in the sorted array in the opposite order\n";
                
                
                //printPoke(a[i]);
//                cout << "vs. \n";
//                printPoke(a[i+1]);
//                cout << "converted value of the 'bigger' hand is" << convert(a[i]) << "\nconverted value of smaller hand: " << convert (a[i+1]);
                return -1;
            }
        cout << "valid!\n";
    }
    
}
*/

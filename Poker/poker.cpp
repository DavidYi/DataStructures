//
//  Yi_poker.cpp
//  Datastructures
//
//  Created by David Yi on 12/5/17.
//  Copyright © 2017 DavidYi. All rights reserved.
//

#include <vector>
#include <iostream>
using std::cout;
using std::cin;
using std::vector;

void insert_sort (vector<int>& h){//implementation of insertition sort
    int i, temp, j;
    for (i = 1; i < 5; i++){
        temp = h[i];
        j = i - 1;
        
        while (j >= 0 && h[j] > temp){
            h[j+1] = h[j];
            j = j-1;
        }
        h[j+1] = temp;
    }
}

int convert(int n){
    vector<int> hand;
    
    int div = 52*52*52*52;
    
    hand.push_back(n/div);
    n = n % div;
    div /= 52;
    
    hand.push_back(n/div);
    n = n % div;
    div /= 52;
    
    hand.push_back(n/div);
    n = n % div;
    div /= 52;
    
    hand.push_back(n/div);
    n = n % div;
    div /= 52;
    
    hand.push_back(n);
    
    insert_sort(hand);//sort the hand to better convert it to a value so that we can sort the hands
    
    bool isFlush = true, isAce = false;//isAce is exculsively to check for straight
    int prevS, r, s, repeatR, least = 13, great = 0;//least and great will be every other rank but ace
    vector<int> ranks; //occurences of the 13 cards
    
    for (int i = 0; i < 13; ++i){
        ranks.push_back(0);
    }
    
    for (int i = 0; i < 5; ++i){
        r = hand[i]/4;
        ++ranks[r];
        s = hand[i] % 4;
        
        if (i != 0){
            if (r == 0) isAce = true;//there is at least an ace
            else if (r < least) least = r;// if it is not an ace and the current rank is greater than the current least, then make least = current rank
            else if (r > great) great = r;//same thing as above but for greatest
            if (s != prevS) isFlush = false;//if the previous suit is not the current suit then there is no way that it is a suit
        }
        prevS = s;
    }
    bool isStraight = false;
    int range = great - least;
    
    //check if it is a straight or not
    if (range == 4)
        isStraight = true;
    else if (range == 3 && isAce){
        if (great == 12){
            isStraight = true;
            great = 13;
        } else if (least == 1)
            isStraight = true;
    }
    
    //straight flush
    if (isFlush && isStraight) return 50000000*(1 + great);//at least 250M and at most 700M
    
    int pair = 0;
    bool is4 = false, is3 = false;
    for (int i = 0; i < 13; ++i){
        if (ranks[i] == 4)
            is4 = true;
        else if (ranks[i] == 2)
            pair++;
        else if (ranks[i] == 3)
            is3 = true;
    }
    
    //convert the hand to some number that can be sorted compared to the other hands
    if (is4){//four of a kind
        int c1 = 1 + hand[0]/4, c2 = 1 + hand[4]/4;//since it is sorted, then either the first or the last card in the hand is four of kind
        if (ranks[c1] == 4)
            return 150000000 + 10000*c1 + 100*c2;//at least 150,010,200 and at most 150,131,200
        return 150000000 + 10000*c2 + 100*c1;
    }
    if (is3 && pair == 1){//full house
        int c1 = 1 + hand[0]/4, c2 = 1 + hand[4]/4;//one is a pair and the other is 3 of kind
        if (ranks[c1] == 3)
            return 140000000 + 10000*c1 + 100*c2; //at least 140,010,200 at most 140,131,200
        return 140000000 + 10000*c2 + 100*c1;
    }
    if (isFlush){//flush
        //from lowest card to highest card
        int c1 = 1 + hand[0]/4, c2 = 1 + hand[1]/4, c3 = 1 + hand[2]/4, c4 = 1 + hand[3]/4, c5 = 1 + hand[4]/4;
        return c1 + 14*c2 + 14*14*c3 + 14*14*14*c4 + 10000000*c5 + 5000000;// at least 15,006,137 and at most 135,035,233
    }
    //straight
    if (isStraight)
        return 14000000 + 100*(1+great);//at least 14,000,100 and most 14,001,400
    
    if (is3){//3 of a kind
        int sum = 0, past = -1, factor = 1, third;
        for (int i = 0; i < 5; ++i){
            int card = 1 + hand[i]/4;
            if (past == card){
                third = card;
            } else{
                sum += factor * card;
            }
            factor *= 14;
            past = card;
        }
        return sum + 10000*third + 5000000;//at least 5,100,227 at most 6,791,189
    }
    
    if (pair == 2){//2 pairs
        int sum = 0, past = -1, factor = 1, pair1 = -1, pair2;
        for (int i = 0; i < 5; ++i){
            int card = 1 + hand[i]/4;
            if (past == card){
                if (pair1 == -1)
                    pair1 = card;
                else
                    pair2 = card;
            } else{
                sum += factor * card;
            }
            factor *= 14;
            past = card;
        }
        if (pair1 > pair2)
            return sum + 10000*14*pair1 + 10000*pair2;
        return sum + 20000*14*pair2 + 20000*pair1; //at least 320,003 at most  4,302,576
    }
    
    if (pair == 1){//1 pair
        int sum = 0, past = -1, factor = 1;
        for (int i = 0; i < 5; ++i){
            int card = 1 + hand[i]/4;
            if (past == card)
                sum += card*10000;
            else{
                sum += factor * card;
            }
            factor *= 14;
            past = card;
        }
        return sum;//at least 84,496 at most 168,192
    }
    
    //highcard
    int c1 = 1 + hand[0]/4, c2 = 1 + hand[1]/4, c3 = 1 + hand[2]/4, c4 = 1 + hand[3]/4, c5 = 1 + hand[4]/4;
    return c1 + 14*c2 + 14*14*c3 + 14*14*14*c4 + 14*14*14*14*c5;//at least 44553 at most 534641
    
    
}

int medianOf3(vector<int> a, int lo, int hi){//get the median between a[lo], a[hi], and a[(lo+hi)/2]
    int x = a[lo];
    int y = a[hi];
    int z = a[(lo + hi)/2];
    
    if ((x < y && x > z) || (x < z && x > y)) return x;
    if ((y < x && y > z) || (y < z && y > x)) return y;
    return z;
}

int qscPart (vector<int> a, vector<int>& copy, int lo, int hi){//coffee quicksort partition
    int x = medianOf3(a, lo, hi);
    while (lo <= hi){
        while (a[lo] < x) ++lo;
        while (a[hi] > x) ++hi;
        if (lo <= hi){//you sort a, but whatever you do to vector a, you do to the actual vector of hands that we have to sort
            int temp = a[lo];
            int temp2 = copy[lo];
            a[lo] = a[hi];
            copy[lo++] = copy[hi];
            a[hi] = temp;
            copy[hi++] = temp2;
        }
    }
    return lo;
}

void qss(vector<int> a, vector<int>& copy, int lo, int hi){//coffee quicksort which uses strawbery
    while (lo < hi){
        int m = qscPart(a, copy, lo, hi);
        if (m - lo < hi - m + 1){
            qss(a, copy, lo, m - 1);
            lo = m;
        } else {
            qss(a, copy, m, hi);
            hi = m - 1;
        }
    }
}

void quicksort(vector<int> a, vector<int>& copy){
    cout << "hiiii";
    qss(a, copy, 0, a.size() - 1);
}

void poker_sort(vector<int>& a){//a is an array of poker hands {h0, h1, h2, h3, h4, h5 ..., hn}
    //each hand is encoded as h = c0 + c1 * 52 + c2 * 52^2 + c3 * 52^3 + c4 * 52^4 where c is a card
    //each card is encoded as c = 4r + s
    vector<int> encode;
    
    cout << "hiiii";
    //get the array of encoded
    for (int i = 0; i < a.size(); i++)
        encode[i] = convert(a[i]);
    
    quicksort(encode, a);
    
}


#include<fstream>
using std::ifstream;
int main() {
    cout << "hiiii";
    ifstream in("in.txt");
    int n, past;
    in >> n >> past;
    for (int i = 1; i < n; i++) {
        int x;
        in >> x;
        vector<int> v;
        v.push_back(past);
        v.push_back(x);
        poker_sort(v);
        if (v[0] != past || v[1] != x){
            cout << "the hand " << past << " is worse than the hand " << x << ", your program either switched these or deleted one when sorting the vector {" << past << ", " << x << "}\n";
            return -1;
        }
        past = x;
    }
    
    cout << "well done!\n";
    in.close();
}



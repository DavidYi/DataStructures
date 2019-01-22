//
// Created by David Yi on 9/30/17.
//

#include "Mod.h"
#include <fstream>
using std::ifstream;
using std::istream;
using std::endl;

//helper functions
long mod_reduce(long val, long modulus){
    long out = val % modulus;
    if (out < 0)
        out += modulus;
    return out;
}

long mod_reduce(const Mod& m){
    long out = m.val() % m.get_modulus();
    if (out < 0)
        out += m.get_modulus();
    return out;
}

long euclidean(long a, long b, long *x, long *y){//euclidian extended
    if (a == 0) {
        *x = 0;
        *y = 1;
        return b;
    }
    
    long tempx, tempy;
    long gcd = euclidean(mod_reduce(b, a), a, &tempx, &tempy);
    
    *x = tempy - (b/a) * tempx;
    *y = tempx;
    
    return gcd;
}

Mod& inverse(const Mod&m){
    long x, y;
    long a = mod_reduce(m.val(), m.get_modulus()), b = m.get_modulus();
    
    
    euclidean(a, b, &x, &y);
    //x and y at this
    long inv = mod_reduce((x % b + b), b);
    Mod* temp = new Mod(inv);
    temp->set_modulus(m.get_modulus());
    
    return *temp;
}

//implementations
Mod::Mod(long t){
    this->x = mod_reduce(t, this->modulus);
}

Mod::Mod(const Mod &m) {
    this->modulus = m.modulus;
    this->x = mod_reduce(m.x, m.get_modulus());
}

Mod& Mod::operator =(const Mod& m){
    this->x = mod_reduce(m.x, m.get_modulus());
    this->modulus = m.get_modulus();
    return *this;
}

Mod& Mod::operator+=(const Mod &m) {
    this->x = mod_reduce(this->val() + m.val(), this->get_modulus());
    
    return *this;
}

Mod& Mod::operator-=(const Mod &m) {
    this->x = mod_reduce(this->val() - m.val(), this->get_modulus());
    return *this;
}

Mod& Mod::operator*=(const Mod &m) {
    long outx = 0; // Initialize result
    long a = this->val(), b = m.val();
    while (b > 0){
        // If b is odd, add 'a' to result
        if (b % 2 == 1)
            outx = mod_reduce(outx + a, this->get_modulus());
        
        // Multiply 'a' with 2
        a = mod_reduce(a*2, this->get_modulus());
        
        // Divide b by 2
        b /= 2;
    }
    
    // Return result
    this->x = mod_reduce(outx, this->get_modulus());
    return *this;
    
}

Mod& Mod::operator/=(const Mod &m) {
    if (m.val() == 0){//takes into account if x is or congruent to 0 since val reduces it
        exit (-1);
    }
    this->x *= inverse(m).val();
    return *this;
}

Mod Mod::operator-() const {
    Mod* out = new Mod(*this);
    out->x = mod_reduce(0 - this->x, this->get_modulus());
    
    return *out;
}

Mod Mod::pwr(long e) const {
    if (e == 0) {
        return Mod(1);
    }
    
    if (e < 0) { // negative
        return inverse(x).pwr(-e);
    }
    
    Mod out(x);
    Mod temp(1);
    
    for (long i = e; i> 0; i /= 2) {
        if (i % 2) {
            temp *= out;
        }
        out *= out;
    }
    
    out = temp.val();
    return out;
}


long Mod::val() const {
    return this->x;
}

void Mod::set_modulus(long m) {
    if (m < 2){
        exit(-1);
    }
    modulus = m;
}


Mod operator+(const Mod& a, const Mod& b){
    return Mod(a) += b;
}

Mod operator+(long t, const Mod& m){
    return Mod(t) += m;
}

Mod operator-(const Mod& a, const Mod& b){
    return Mod(a) -= b;
}

Mod operator-(long t, const Mod& m){
    return Mod(t) -= m;
}

Mod operator*(const Mod& a, const Mod& b){
    return Mod(a) *= b;
}

Mod operator*(long t, const Mod& m){
    return Mod(t) *= m;
}

Mod operator/(const Mod& a, const Mod& b){
    return Mod(a) /= b;
}

Mod operator/(long t, const Mod& m){
    
    return Mod(t) /= m;
}

bool operator==(const Mod& a, const Mod& b){
    return (mod_reduce(a) == mod_reduce(b) && a.get_modulus() == b.get_modulus());
}

bool operator==(long t, const Mod& m){
    return (mod_reduce(t, m.get_modulus()) == m.val());
}

bool operator!=(const Mod& a, const Mod& b){
    return (mod_reduce(a) != mod_reduce(b) || a.get_modulus() != b.get_modulus());
}

bool operator!=(long t, const Mod& m){
    return (mod_reduce(t, m.get_modulus()) != mod_reduce(m));
}

istream& operator>>(istream& is, Mod& m){
    long x;
    is >> x;
    m = *new Mod(x);
    return is;
}

ostream& operator<<(ostream& os, const Mod& m){
    os << m.val() << "(mod" << m.get_modulus()<< ")";
    return os;
    
}


long Mod::modulus = 17;

/*
int main() {
    ifstream in ("in.txt");
    ifstream out ("out.txt");

    long n;
    in >> n;

    Mod::set_modulus(17);
    cout << "testing your conditionals\n";

    bool bad = true;

    if (!(Mod(-12) == Mod(22)))
        cout << "check your ==(Mod, Mod)\n";
    else if (!(Mod(-12) == 22))
        cout << "check your ==(long, Mod)\n";
    else if (Mod(22) != Mod(-12))
        cout << "check your !=(Mod, Mod)\n";
    else if (Mod(5) != -12)
        cout << "check your !=(long, Mod)\n";
    else
        bad = false;

    if (bad)
        return 0;

    cout << "conditionals all good\n";
    cout << "\ntesting arithmetic...\n";

    for (int i = 0; i < n; i++) {

        long prime;
        in >> prime;
        Mod::set_modulus(prime);


        for (int j = 0; j < 180; j++) {
            
            cout << "***TESTCASE " << j << '\n';
            long a, b;
            in >> a >> b;

            int c;
            in >> c;

            Mod ans(0);

            Mod A(a), B(b);

            int co = 0;
            for (int s = 0; s < 5; s++) {
                cout << "***TESTCASE " << s << '\n';

                if (s == 3 && c & 1) continue;
                if (s == 4 && c > 1) continue;
                co++;

                out >> ans;

                Mod myans(!s ? A + B : !(s - 1) ? A - B : !(s - 2) ? A*B : !(s - 3) ? A / B : A.pwr(b));

                if (myans != ans) {
                    cout << "wrong answer for " << a << " " << (!s ? "+" : !(s - 1) ? "-" : !(s - 2) ? "*" : !(s - 3) ? "/" : "pwr") << " " << b << " (mod " << prime << ")\n";
                    cout << "correct: " << ans << ", your answer: " << myans << endl;
                    bad = true;
                    break;
                }
            }
            if (bad) break;
        }
        if (bad) break;
    }
    if (!bad) cout << "all passed!!!\n";

    in.close();
    out.close();
    return 0;
}
*/

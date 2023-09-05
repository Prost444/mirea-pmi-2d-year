#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <set>

using namespace std;


class Fraction {
protected:
    int numerator;
    int denominator;
public:
    Fraction(int numerator = 0, int denominator = 1) : numerator(numerator), denominator(denominator) {};
    friend ostream& operator<<(ostream& out, const Fraction& f);
};

ostream& operator<<(ostream& out, const Fraction& f) {
    out << f.numerator << "/" << f.denominator;
    return out;
}


template <class T>
void push(list<T>& l, const T& t)
{
    typename list<T>::iterator it = l.begin();
    while (it!= l.end())
    {
        if (*it > t)
        {
            l.insert(it, t);
            return;
        }
        it++;
    }
    l.insert(it, t);
}

template <class T>
void pop(list<T>& l, const T& index = -1)
{
    if (index == -1)
        l.erase(l.end()--);
    typename list<T>::iterator it = l.begin();
    for (int i = 0; i < index; i++, it++)
        if (it == l.end()) return;
    l.erase(it);
}

template <class T>
void print(list<T>& l)
{
    typename list<T>::iterator it = l.begin();
    while (it!= l.end())
    {
        cout << *it << " ";
        it++;
    }
    cout << endl;
}

int main()
{
    list<int> l;
    push(l, 5);
    push(l, 20);
    push(l, 88);
    push(l, 1);
    push(l, 15);
    push(l, 6);
    push(l, -3);
    print(l);
    pop(l, 4);
    print(l);
    return 0;
}
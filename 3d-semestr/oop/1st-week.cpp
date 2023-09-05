#include <iostream>
#include <list>
#include <string>

using namespace std;


class Fraction {
protected:
    int numerator;
    int denominator;

public:

    Fraction(int numerator = 0, int denominator = 1) : numerator(numerator), denominator(denominator) {};

    bool operator > (Fraction other) {
        return numerator*other.denominator > other.numerator*denominator; }
    bool operator < (Fraction other) {
        return numerator*other.denominator < other.numerator*denominator; }
    bool operator >= (Fraction other) {
        return numerator*other.denominator >= other.numerator*denominator; }
    bool operator <= (Fraction other) {
        return numerator*other.denominator <= other.numerator*denominator; }
    bool operator == (Fraction other) {
        return numerator*other.denominator == other.numerator*denominator; }
    bool operator!= (Fraction other) {
        return numerator*other.denominator!= other.numerator*denominator; }

    bool operator > (int other) {
        return numerator > other*denominator; }
    bool operator < (int other) {
        return numerator < other*denominator; }
    bool operator >= (int other) {
        return numerator >= other*denominator; }
    bool operator <= (int other) {
        return numerator <= other*denominator; }
    bool operator == (int other) {
        return numerator == other*denominator; }
    bool operator!= (int other) {
        return numerator!= other*denominator; }

    bool operator > (double other) {
        return numerator > other*denominator; }
    bool operator < (double other) {
        return numerator < other*denominator; }
    bool operator >= (double other) {
        return numerator >= other*denominator; }
    bool operator <= (double other) {
        return numerator <= other*denominator; }
    bool operator == (double other) {
        return numerator == other*denominator; }
    bool operator!= (double other) {
        return numerator!= other*denominator; }


    friend ostream& operator<<(ostream& out, const Fraction& f);
    friend Fraction abs(Fraction f);
};


ostream& operator<<(ostream& out, const Fraction& f) 
{
    out << f.numerator << "/" << f.denominator;
    return out;
}


Fraction abs(Fraction f) { return Fraction(abs(f.numerator), abs(f.denominator)); }


template <class T>
list<Fraction> filter(list<Fraction>& l, T a)
{
    list<Fraction> res;
    list<Fraction>::iterator it = l.begin();
    while (it!= l.end())
    {
        if (abs(*it) < a)
            res.push_back(*it);
        it++;
    }
    return res;
}


class SportTeam {
protected:
    string name;
    string city;
    int num_wins;
    int num_draws;
    int num_losses;
    int score;

public:
    SportTeam(string name="NotStated", string city="NotStated", int num_wins=0, int num_draws=0, int num_losses=0, int score=0) :
     name(name), city(city), num_wins(num_wins), num_draws(num_draws), num_losses(num_losses), score(score) {};

    bool operator > (SportTeam other) {
        if (num_wins > other.num_wins)
            return true;
        if (num_wins == other.num_wins && num_draws > other.num_draws)
            return true;
        if (num_wins == other.num_wins && num_draws == other.num_draws && num_losses < other.num_losses)
            return true;
        if (num_wins == other.num_wins && num_draws == other.num_draws && num_losses == other.num_losses && name < other.name) 
            return true;
        return false;
    }

    bool operator < (SportTeam other) {
        if (num_wins < other.num_wins)
            return true;   
        if (num_wins == other.num_wins && num_draws < other.num_draws) 
            return true;
        if (num_wins == other.num_wins && num_draws == other.num_draws && num_losses > other.num_losses)
            return true;
        if (num_wins == other.num_wins && num_draws == other.num_draws && num_losses == other.num_losses && name > other.name)
            return true;
        return false;
    }

    bool operator == (SportTeam other) {
        return num_wins == other.num_wins && num_draws == other.num_draws && num_losses == other.num_losses && name == other.name
        && city == other.city && score == other.score;
    }

    friend ostream& operator<<(ostream& out, const SportTeam& s);
};



ostream& operator<<(ostream& out, const SportTeam& s) 
{
    out << s.name << " " << s.city << " " << s.num_wins << " " << s.num_draws << " " << s.num_losses << " " << s.score;
    return out;
}

template <class T>
void push(list<T>& l, const T& t)
{
    typename list<T>::iterator it = l.begin();
    if (typeid(T) == typeid(SportTeam))
    {
        while (it!= l.end())
        {
            if (*it < t)
            {
                l.insert(it, t);
                return;
            }
            it++;
        }
        l.insert(it, t);
    }
    else
    {
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

}

template <class T>
void pop(list<T>& l, T elem)
{
    for (typename list<T>::iterator it = l.begin(); it != l.end(); it++)
        if (*it == elem)
        {
            l.erase(it);
            return;
        }
}

template <class T>
ostream& operator<<(ostream& out, const list<T>& l) {
    typename list<T>::const_iterator it = l.begin();
    while (it!= l.end())
    {
        cout << *it << " ";
        it++;
    }
    return out;
}

ostream& operator<<(ostream& out, const list<SportTeam>& l) {
    typename list<SportTeam>::const_iterator it = l.begin();
    while (it!= l.end())
    {
        cout << *it << "\n";
        it++;
    }
    return out;
}


int main()
{
    list<Fraction> l;
    push(l, Fraction(1, 2));
    push(l, Fraction(7, 3));
    push(l, Fraction(-1, 5));
    push(l, Fraction(-10, 1));
    push(l, Fraction(6, 5));
    push(l, Fraction(-15, 19));
    push(l, Fraction(2, 5));
    cout << l << '\n';
    pop(l, Fraction(-10, 1));
    cout << l << '\n';
    cout << filter(l, 0.5) << "\n\n";

    list<SportTeam> teams;
    push(teams, SportTeam("Lions", "City1", 10, 5, 3, 30));
    push(teams, SportTeam("Tigers", "City2", 8, 7, 2, 29));
    push(teams, SportTeam("Bears", "City3", 12, 3, 2, 35));
    push(teams, SportTeam("Eagles", "City4", 8, 7, 2, 29));
    push(teams, SportTeam("Wolves", "City5", 10, 5, 3, 30));
    push(teams, SportTeam("Dolphins", "City6", 6, 6, 6, 24));
    push(teams, SportTeam("Panthers", "City7", 10, 5, 3, 30));

    cout << teams << '\n';

    pop(teams, SportTeam("Dolphins", "City6", 6, 6, 6, 24));
    cout << teams << '\n';

    return 0;
}
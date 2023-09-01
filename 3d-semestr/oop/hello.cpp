#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <set>

using namespace std;


int main()
{
    vector <int> v;
    for (int i = 0; i < 10; i++)
        v.push_back(i);
    for (int i = 0; i < 10; i++)
        cout << v[i] << " ";
    cout << '\n';
    list<int> l;
    for (int i = 0; i < 10; i++)
        l.push_back(i);
    list<int>::iterator it = l.begin();
    while (it != l.end())
    {
        cout << *it << ' ';
        it++;
    }

    cout << '\n';
    do
        cout << *--it << ' ';
    while (it != l.begin());
    cout << '\n';

    map <string, int> marks;
    marks["ivanov"] = 5; 
    marks["petrov"] = 4; 
    marks["sidorov"] = 3; 
    marks["kuznetsov"] = 4; 
    marks["karpov"] = 4; 

    map<string, int>::iterator it_m = marks.begin();
    while (it_m != marks.end())
    {
        cout << it_m->first << ": " << it_m->second << '\n';
        it_m++;
    }

    set<int> test_set;
    for (int i = 0; i<10; i++)
        for (int j=0; j < 10; j++)
            test_set.insert(i);
    set<int>::iterator it_set = test_set.begin(); cout << test_set.size() << ": ";
    while (it_set != test_set.end())
    {
        cout << *it_set<< ' ';
        it_set++;
    }
    
    cout << "\nHelloNigga3";

    return 0;
}
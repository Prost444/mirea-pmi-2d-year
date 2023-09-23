#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

class MapExeption : public exception
{
protected:
	//сообщение об ошибке
	char* str;
public:
	MapExeption(const char* s)
	{
		str = new char[strlen(s) + 1];
		strcpy(str, s);
	}
	MapExeption(const MapExeption& e)
	{
		str = new char[strlen(e.str) + 1];
		strcpy(str, e.str);
	}
	~MapExeption()
	{
		delete[] str;
	}

	//функцию вывода можно будет переопределить в производных классах, когда будет ясна конкретика
	virtual void print()
	{
		cout << "MapExeption: " << str << "; " << what();
	}
};

class MapKeyExeption : public MapExeption
{
public:
    MapKeyExeption(const char* s) : MapExeption(s) {}
    MapKeyExeption(const MapKeyExeption& e) : MapExeption(e) {}
	virtual void print()
	{
		cout << "MapKeyExeption: " << str << "; " << what();
	}
};

//Пишем свое добавление элемента для обработки исключения
template<class K, class V>
typename map<K, V>::iterator addItem(map<K, V>& m,  K key, V value) 
{
    if (m.find(key) != m.end())
        throw MapKeyExeption("Element with this key already exists.");
    m[key] = value;
    return m.find(key);
}

// Функция для поиска элемента по ключу
template<class K, class V>
V findValueByKey(map<K, V> m, K key) 
{
    typename map<K, V>::iterator it = m.find(key);
    if (it != m.end())
        return it->second;
    throw MapKeyExeption("Element with this key does not exist.");
}

// Функция для поиска ключей по значению
template<class K, class V>
set<K> findKeysByValue(map<K, V> m, V value) 
{
    set<K> keys;
    for (typename map<K, V>::iterator it = m.begin(); it != m.end(); it++)
        if (it->second == value)
            keys.insert(it->first);
    return keys;
}

//Вывод с помощью итератора
template<class K, class V>
ostream& operator << (ostream& ustream, map<K, V> m)
{
    for (typename map<K, V>::iterator it = m.begin(); it != m.end(); it++)
        ustream << "map[" << it->first << "]: " << it->second << ";\n";
    return ustream;
}

//Фильтр
template<class K, class V, class T>
map<K, V> filter(map<K, V> m, bool (*predicate)(V, T), T threshold)
{
    map<K, V> res;
    for (typename map<K, V>::iterator it = m.begin(); it != m.end(); it++)
        if (predicate(it->second, threshold))
            addItem(res, it->first, it->second);
    return res;
}

//Все встречающиеся в дереве элементы
template<class K, class V>
vector<V> getUniqueValues(map<K, V> m)
{
    vector<V> vals;
    for (typename map<K, V>::iterator it = m.begin(); it != m.end(); it++)
    {
        V cur = it->second;
        bool flag = true;
        for (int i = 0; i < vals.size(); i++)
        {
            if (vals[i] == cur)
            {
                flag = false;
                break;
            }
        }
        if (flag)
            vals.push_back(it->second);
    }
    return vals;
}

class Statistics {
protected:
    int num_wins;
    int num_draws;
    int num_losses;
    int score;

public:
    Statistics(int num_wins=0, int num_draws=0, int num_losses=0, int score=0) :
    num_wins(num_wins), num_draws(num_draws), num_losses(num_losses), score(score) {};

    bool operator == (const Statistics other) {
        return num_wins == other.num_wins && num_draws == other.num_draws && num_losses == other.num_losses && score == other.score;}

    friend ostream& operator<<(ostream& out, const Statistics& s);
    friend bool TeamsFilter (Statistics t, int threshold);
};



ostream& operator<<(ostream& out, const Statistics& s) 
{
    out << "wins: " << s.num_wins << ", draws: " << s.num_draws << ", loses: " << s.num_losses << ", score: " << s.score;
    return out;
}

template <class T>
ostream& operator<<(ostream& out, const set<T>& s)
{
    for (typename set<T>::iterator it = s.begin(); it != s.end(); it++)
        out << *it << ", ";
    return out;
}

template <class T>
ostream& operator<<(ostream& out, const vector<T>& s)
{
    for (int i = 0; i < s.size(); i++)
        out << s[i] << ";\n";
    return out;
}

bool TeamsFilter (Statistics t, int threshold)
{
    return t.num_wins > threshold;
}


// Функция для поиска элементов по ключу
template<class K, class V>
vector<V> findValuesByKey(multimap<K, V> m, K key) 
{
    vector<V> res;
    for(typename multimap<K, V>::iterator it = m.begin(); it != m.end(); it++)
        if (it->first == key)
            res.push_back(it->second);
    if (res.size())
        return res;
    throw MapKeyExeption("Element with this key does not exist.");
}

// Функция для поиска ключей по значению
template<class K, class V>
vector<K> findKeysByValue(multimap<K, V> m, V value) 
{
    vector<K> keys;
    for (typename multimap<K, V>::iterator it = m.begin(); it != m.end(); it++)
        if (it->second == value)
            keys.push_back(it->first);
    return keys;
}

//Пишем свое добавление элемента просто по приколу
template<class K, class V>
typename multimap<K, V>::iterator addItem(multimap<K, V>& m,  K key, V value) 
{
    return m.insert(pair(key, value));
}

//Вывод с помощью итератора
template<class K, class V>
ostream& operator << (ostream& ustream, multimap<K, V> m)
{
    for (typename multimap<K, V>::iterator it = m.begin(); it != m.end(); it++)
        ustream << "multimap[" << it->first << "]: " << it->second << ";\n";
    return ustream;
}

//Фильтр
template<class K, class V, class T>
multimap<K, V> filter(multimap<K, V> m, bool (*predicate)(V, T), T threshold)
{
    multimap<K, V> res;
    for (typename multimap<K, V>::iterator it = m.begin(); it != m.end(); it++)
        if (predicate(it->second, threshold))
            addItem(res, it->first, it->second);
    return res;
}

//Все встречающиеся в дереве элементы
template<class K, class V>
vector<V> getUniqueValues(multimap<K, V> m)
{
    vector<V> vals;
    for (typename multimap<K, V>::iterator it = m.begin(); it != m.end(); it++)
    {
        V cur = it->second;
        bool flag = true;
        for (int i = 0; i < vals.size(); i++)
        {
            if (vals[i] == cur)
            {
                flag = false;
                break;
            }
        }
        if (flag)
            vals.push_back(it->second);
    }
    return vals;
}

int main()
{
    map<string, Statistics> teams;
    addItem(teams, string("Lions City1"), Statistics(10, 5, 3, 30));
    addItem(teams, string("Tigers City2"), Statistics(8, 7, 2, 29));
    addItem(teams, string("Bears City3"), Statistics(12, 3, 2, 35));
    addItem(teams, string("Eagles City4"), Statistics(8, 7, 2, 29));
    addItem(teams, string("Wolves City5"), Statistics(10, 5, 3, 30));
    addItem(teams, string("Dolphins City6"), Statistics(6, 6, 6, 24));
    addItem(teams, string("Panthers City7"), Statistics(10, 5, 3, 30));
    cout << "\nSport teams:\n";
    cout << teams;

    cout << "\nFinding Dolphins City6: " << findValueByKey(teams, string("Dolphins City6"));
    cout << "\nFinding Statistics(10, 5, 3, 30): " << findKeysByValue(teams, Statistics(10, 5, 3, 30)) << "\n";

    cout << "\nUnique statistics:\n" << getUniqueValues(teams);

    cout << "\nFilterd tree with num_wins > 8:\n" << filter(teams, TeamsFilter, 8);


    multimap<string, Statistics> multi_teams;

    addItem(multi_teams, string("Lions City1"), Statistics(10, 5, 3, 30));
    addItem(multi_teams, string("Tigers City2"), Statistics(8, 7, 2, 29));
    addItem(multi_teams, string("Bears City3"), Statistics(12, 3, 2, 35));
    addItem(multi_teams, string("Eagles City4"), Statistics(8, 7, 2, 29));
    addItem(multi_teams, string("Wolves City5"), Statistics(10, 5, 3, 30));
    addItem(multi_teams, string("Dolphins City6"), Statistics(6, 6, 6, 24));
    addItem(multi_teams, string("Panthers City7"), Statistics(10, 5, 3, 30));
    addItem(multi_teams, string("Panthers City7"), Statistics(10, 5, 3, 30));
    addItem(multi_teams, string("Panthers City7"), Statistics(9, 4, 6, 20));

    cout << "\nMultimap tree of sport teams:\n" << multi_teams << endl;

    cout << "\nFinding Panthers City7:\n" << findValuesByKey(multi_teams, string("Panthers City7"));
    cout << "\nFinding Statistics(10, 5, 3, 30):\n" << findKeysByValue(multi_teams, Statistics(10, 5, 3, 30)) << "\n";

    cout << "\nUnique statistics:\n" << getUniqueValues(multi_teams);

    cout << "\nFilterd tree with num_wins > 8:\n" << filter(multi_teams, TeamsFilter, 8);

    return 0;
}

#include <iostream>
#include <fstream>
#include <cstring>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations" //Вырубает предупреждение про итераторы

using namespace std;


class Exception : public exception
{
protected:
	//сообщение об ошибке
	char* str;
public:
	Exception(const char* s)
	{
		str = new char[strlen(s) + 1];
		strcpy(str, s);
	}
	Exception(const Exception& e)
	{
		str = new char[strlen(e.str) + 1];
		strcpy(str, e.str);
	}
	~Exception()
	{
		delete[] str;
	}

	//функцию вывода можно будет переопределить в производных классах, когда будет ясна конкретика
	virtual void print()
	{
		cout << "Exception: " << str << "; " << what();
	}
};

class IteratorException : public Exception
{
public:
    IteratorException(const char* s) : Exception(s) {}
    IteratorException(const IteratorException& e) : Exception(e) {}
	virtual void print()
	{
		cout << "IteratorException: " << str << "; " << what();
	}
};


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
class Element
{
	//элемент связного списка
private:
	//указатель на предыдущий и следующий элемент
	Element* next;
	Element* prev;

	//информация, хранимая в поле
	T field;
public:
	//доступ к полю *next
	virtual Element* getNext() { return next; }
	virtual void setNext(Element* value) { next = value; }

	//доступ к полю *prev
	virtual Element* getPrevious() { return prev; }
	virtual void setPrevious(Element* value) { prev = value; }

	//доступ к полю с хранимой информацией field
	virtual T getValue() { return field; }
	virtual void setValue(T value) { field = value; }

	template<class T1> friend ostream& operator<< (ostream& ustream, Element<T1>& obj);

	Element(T value =NULL) { field = value; next = prev = NULL; }
};

template<class T>
ostream& operator << (ostream& ustream, Element<T>& obj)
{
	if (&obj != NULL)
		ustream << obj.field;
	return ustream;
}

template <class T>
class LinkedListParent
{
protected:
	//достаточно хранить начало и конец
	Element<T>* head;
	Element<T>* tail;
	//для удобства храним количество элементов
	int num;
public:
	virtual int Number() { return num; }

	virtual Element<T>* getBegin() { return head; }

	virtual Element<T>* getEnd() { return tail; }

	LinkedListParent()
	{
		//конструктор без параметров
		cout << "\nParent constructor";
		head = NULL; tail = NULL;
		num = 0;
	}

	//чисто виртуальная функция: пока не определимся с типом списка, не сможем реализовать добавление
	virtual Element<T>* push(T value) = 0;

	//чисто виртуальная функция: пока не определимся с типом списка, не сможем реализовать удаление
	virtual T pop() = 0;

	virtual ~LinkedListParent()
	{
		//деструктор - освобождение памяти
		cout << "\nParent destructor";
	}

	//получение элемента по индексу - какова асимптотическая оценка этого действия?
	virtual Element<T>* operator[](int i)
	{
		//индексация
		if (i<0 || i>num) return NULL;
		int k = 0;

		//ищем i-й элемент - вставем в начало и отсчитываем i шагов вперед
		Element<T>* cur = head;
		for (k = 0; k < i; k++)
		{
			cur = cur->getNext();
		}
		return cur;
	}

	virtual void UniversalFilter(LinkedListParent<T>* list, bool (*filter)(T))
	{
		for (Element<T>* current = head; current != NULL; current = current->getNext())
			if (filter(current->getValue()))
				list->push(current->getValue());
	}

	template<class T1> friend ostream& operator<< (ostream& ustream, LinkedListParent<T1>& obj);
	template<class T1> friend istream& operator>> (istream& ustream, LinkedListParent<T1>& obj);
};

template<class T>
ostream& operator << (ostream& ustream, LinkedListParent<T>& obj)
{
	if (typeid(ustream).name() == typeid(ofstream).name())
	{
		ustream << obj.num << "\n";
		for (Element<T>* current = obj.getBegin(); current != NULL; current = current->getNext())
			ustream << current->getValue() << " ";
		return ustream;
	}

	ustream << "\nLength: " << obj.num << "\n";
	int i = 0;
	for (Element<T>* current = obj.getBegin(); current != NULL; current = current->getNext(), i++)
		ustream << "arr[" << i << "] = " << current->getValue() << "\n";

	return ustream;
}

template<class T>
istream& operator >> (istream& ustream, LinkedListParent<T>& obj)
{
	//чтение из файла и консоли совпадают
	int len;
	ustream >> len;
	//здесь надо очистить память под obj, установить obj.num = 0
	double v = 0;
	for (int i = 0; i < len; i++)
	{
		ustream >> v;
		obj.push(v);
	}
	return ustream;
}

//дописать класс итератора по списку
template<typename ValueType>
class ListIterator : public iterator<input_iterator_tag, ValueType>
{
public:
	//конструкторы
	ListIterator() { ptr = NULL; }
	ListIterator(Element<ValueType>* p) { ptr = p; }
	ListIterator(const ListIterator& it) { ptr = it.ptr; }

	//методы работы с итераторами
	//присваивание
	ListIterator& operator=(const ListIterator& it) { ptr = it.ptr; return *this; }
	ListIterator& operator=(Element<ValueType>* p) { ptr = p; return *this; }

	//проверка итераторов на равенство
	bool operator!=(ListIterator const& other) const { return ptr != other.ptr; }
	bool operator==(ListIterator const& other) const { return ptr == other.ptr; }
	//получить значение
	Element<ValueType>& operator*()
	{
		if (ptr == NULL)
			throw IteratorException("The iterator is not associated with any element of the list");
		return *ptr;
	}
	//перемещение с помощью итераторов
	ListIterator& operator++() { ptr = ptr->getNext();  return *this; } //Префиксный ++
	ListIterator& operator++(int v) { ptr = ptr->getNext(); return *this; } //Постфиксный ++
	ListIterator& operator--() { ptr = ptr->getPrevious();  return *this; } //Префиксный --
	ListIterator& operator--(int v) { ptr = ptr->getPrevious(); return *this; } //Постфиксный --

	//текущий элемент
	Element<ValueType>* ptr;
};

//класс итерируемый список - наследник связного списка, родитель для Очереди и Стека
template <class T>
class IteratedLinkedList : public LinkedListParent<T>
{
public:
	IteratedLinkedList() : LinkedListParent<T>() { cout << "\nIteratedLinkedList constructor"; }
	virtual ~IteratedLinkedList() { cout << "\nIteratedLinkedList destructor"; }
	ListIterator<T> begin() { ListIterator<T> it = LinkedListParent<T>::head; return it; }
	ListIterator<T> end() { ListIterator<T> it = LinkedListParent<T>::tail; return it; }
	template <class T1> friend ostream& operator<< (ostream& ustream, IteratedLinkedList<T1>& obj);
};

template<class T>
ostream& operator << (ostream& ustream, IteratedLinkedList<T>& obj)
{
	if (typeid(ustream).name() == typeid(ofstream).name())
	{
		ustream << obj.num << "\n";
		for (ListIterator<T> current = obj.begin(); current != NULL; current++)
			ustream << (*current).getValue() << " ";
		return ustream;
	}

	ustream << "\nLength: " << obj.num << "\n";
	int i = 0;
	for (ListIterator<T> current = obj.begin(); current != NULL; current++, i++)
		ustream << "arr[" << i << "] = " << (*current).getValue() << "\n";

	return ustream;
}

template <class T>
class Stack : public IteratedLinkedList<T>
{
public:
	Stack() : IteratedLinkedList<T>() { cout << "\nStack constructor"; }

	Element<T>* push(T obj) //To the end
	{
		Element<T>* x = new Element<T>(obj);
		if (this->tail != NULL)
		{
			this->tail->setNext(x);
			x->setPrevious(this->tail);
			this->tail = x;
		}
		else
		{
			this->head = x;
			this->tail = this->head;
		}
		this->num++;
		return this->tail;
	}
	T pop()
	{
		T x;
		Element<T>* prev;
		x = this->tail->getValue();
		prev = this->tail->getPrevious();
		if (this->num>1)
		{
			prev->setNext(NULL);
			this->tail->setPrevious(NULL);
		}
		else
			this->head = NULL;
		this->tail = prev;
		this->num--;
		return x;
	}

	ListIterator<T> insert(T val, ListIterator<T> it)
	{
		Element<T>* cur = it.ptr;
		Element<T>* n = new Element<T>(val);
		if (this->tail == cur)
		{
			push(val); return this->end();
		}
		if (it == NULL)
		{
			n->setNext(this->head);
			this->head->setPrevious(n);
			this->head = n;
			return this->begin();
		}

		n->setNext(cur->getNext());
		cur->setNext(n);
		n->setPrevious(cur);
		n->getNext()->setPrevious(n);
		this->num++;
		return it++;
	}

	//Push, Pop, Remove, Insert
};

template <class T>
class Queue : public IteratedLinkedList<T>
{
public:
	Queue(): IteratedLinkedList<T>() { cout << "\nQueue constructor"; }

	Element<T>* push(T obj)
	{
		Element<T>* x = new Element<T>(obj);
		if (this->tail != NULL)
		{
			this->tail->setNext(x);
			x->setPrevious(this->tail);
			this->tail = x;
		}
		else
		{
			this->head = x;
			this->tail = this->head;
		}
		this->num++;
		return this->tail;
	}

	T pop()
	{
		T x;
		Element<T>* next;
		x = this->head->getValue();
		next = this->head->getNext();
		if (this->num>1)
		{
			next->setPrevious(NULL);
			this->head->setNext(NULL);
		}
		else
			this->tail = NULL;
		this->head = next;
		this->num--;
		return x;
	}

	Queue<T> Filter(bool (*filter)(T))
	{
		Queue<T> ans;
		//cout << *this;
		for (Element<T>* current = this->head; current != NULL; current = current->getNext())
			if (filter(current->getValue()))
				ans.push(current->getValue());
		return(ans);
	}

};

template <class T>
class SortedQueue : public Queue<T>
{
public: 
	SortedQueue() : Queue<T>() { cout << "\nSortedQueue constructor"; }

    Element<T>* push(T obj)
    {
		Element<T>* x = new Element<T>(obj);
		ListIterator<T> it = this->begin();
		while (it!=NULL && (*it).getValue()<obj)
			it++;
		if (it == NULL) return Queue<T>::push(obj);
		if (it == this->begin())
		{
			x->setNext(this->head);
            (*this->begin()).setPrevious(x);
            this->head = x;
			this->num++;
            return x;
		}
		x->setNext(it.ptr);
		x->setPrevious((--it).ptr);
		(*it).setNext(x);
		(it++)++;
		(*it).setPrevious(x);
		this->num++;
		return x;
	}
};

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


class SortedQueueSportTeam : public IteratedLinkedList<SportTeam>
{
public:
    SortedQueueSportTeam() : IteratedLinkedList<SportTeam>() { cout << "\nSortedQueueSportTeam constructor"; }

    Element<SportTeam>* push(SportTeam obj)
	{
		Element<SportTeam>* x = new Element<SportTeam>(obj);
		ListIterator<SportTeam> it = this->begin();
		while (it!=NULL && (*it).getValue()>obj)
			it++;

		if (it == NULL)
		{
			if (it == this->begin())
			{
                this->head = x;
                this->tail = x;
				this->num++;
				return x;
			}
			this->tail->setNext(x);
			x->setPrevious(this->tail);
			this->tail = x;
			this->num++;
			return x;
		}
		if (it == this->begin())
		{
			(*it).setPrevious(x);
			x->setNext(this->head);
			this->head = x;
			this->num++;
			return x;	
		}
		x->setNext(it.ptr);
		x->setPrevious((--it).ptr);
		(*it).setNext(x);
		(it++)++;
		(*it).setPrevious(x);
		this->num++;
		return x;
	}

	SportTeam pop()
	{
		SportTeam x;
		ListIterator<SportTeam> next = this->begin()++;
		x = (*(this->begin())).getValue();
		if (this->num>1)
		{
			(*next).setPrevious(NULL);
			this->head->setNext(NULL);
		}
		else
			this->tail = NULL;
		this->head = next.ptr;
		this->num--;
		return x;
	}
};


bool FractionFilter(Fraction f)
{
	double a=0.5;
	return abs(f) < a;
}


int main()
{
	try
	{
		Queue<int> Q;
		cout << "\n\nDemonstation of the queue work:\n";
		Q.push(1);
		cout << Q << "\n";
		Q.push(2);
		cout << Q << "\n";
		Q.push(3);
		cout << Q << "\n";
		Q.push(4);
		cout << Q << "\n";

		Q.pop();
		cout << Q << "\n";
		Q.pop();
		cout << Q << "\n";
		Q.pop();
		cout << Q << "\n";

		Queue<Fraction> l;
		l.push(Fraction(1, 2));
		l.push(Fraction(7, 3));
		l.push(Fraction(-1, 5));
		l.push(Fraction(-10, 1));
		l.push(Fraction(6, 5));
		l.push(Fraction(-15, 19));
		l.push(Fraction(2, 5));

		Queue<Fraction> filtered = l.Filter(FractionFilter);

		cout << "\n\nnew Queue:" << l << "\n";
		cout << "Filered Queue:\n"<< filtered;

		Stack<Fraction> *filtered2 = new Stack<Fraction>;
		l.UniversalFilter(filtered2, FractionFilter);
		cout << "\nnew Stack with filtered values from Queue:\n" << *filtered2 << "\n";

		cout << "\nDemonstation of the sorted queue work:\n";
		SortedQueue<int> QS;
		QS.push(5);
		QS.push(2);
		QS.push(3);
		QS.push(6);
		cout << QS << "\n";

        cout << "\npush with use of iterators:\n";
		SortedQueueSportTeam teams;
		teams.push(SportTeam("Lions", "City1", 10, 5, 3, 30)); cout << teams << '\n';
		teams.push(SportTeam("Tigers", "City2", 8, 7, 2, 29)); cout << teams << '\n';
		teams.push(SportTeam("Bears", "City3", 12, 3, 2, 35));
		teams.push(SportTeam("Eagles", "City4", 8, 7, 2, 29));
		teams.push(SportTeam("Wolves", "City5", 10, 5, 3, 30));
		teams.push(SportTeam("Dolphins", "City6", 6, 6, 6, 24));
		teams.push(SportTeam("Panthers", "City7", 10, 5, 3, 30));

		cout << teams << '\n';

		cout << "Popped with use of iterators: " << teams.pop() << "\n\n";
		cout << teams << '\n';
			return 0;
		}
	
	catch (IteratorException &e)
	{
		cout << "\nIteratorException has been caught: "; e.print();
	}
}

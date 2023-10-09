// Лазарев Александр КМБО-03-22
// ВАРИАТН 15
#include <iostream>
#include <queue>
#include <list>
#include <vector>
#include <algorithm>
    
using namespace std;


// 3.1
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
    friend ostream& operator<<(ostream& out, const SportTeam& s);
    friend bool SportTeam_less_priority (const SportTeam a, const SportTeam b);
    friend bool SportTeam_greater_priority (const SportTeam a, const SportTeam b);
    //friend bool equal_priority (const SportTeam a, const SportTeam b);
	friend struct SportTeam_PriorityKey;	
};

ostream& operator<<(ostream& out, const SportTeam& s) 
{
    out << s.name << " " << s.city << " " << s.num_wins << " " << s.num_draws << " " << s.num_losses << " " << s.score;
    return out;
}

bool SportTeam_less_priority (const SportTeam a, const SportTeam b)
{ 
	if (a.num_wins < b.num_wins)
    	return true;   
    if (a.num_wins == b.num_wins && a.num_draws < b.num_draws) 
		return true;
	if (a.num_wins == b.num_wins && a.num_draws == b.num_draws && a.num_losses > b.num_losses)
		return true;
	if (a.num_wins == b.num_wins && a.num_draws == b.num_draws && a.num_losses == b.num_losses && a.name > b.name)
		return true;
	return false;
}

bool SportTeam_greater_priority(const SportTeam a, const SportTeam b)
{
	if (a.num_wins > b.num_wins)
    	return true;   
    if (a.num_wins == b.num_wins && a.num_draws > b.num_draws) 
		return true;
	if (a.num_wins == b.num_wins && a.num_draws == b.num_draws && a.num_losses < b.num_losses)
		return true;
	if (a.num_wins == b.num_wins && a.num_draws == b.num_draws && a.num_losses == b.num_losses && a.name < b.name)
		return true;
	return false;
}

struct SportTeam_PriorityKey
{
    bool operator()(const SportTeam a, const SportTeam b) const
	{
		return SportTeam_less_priority(a, b);
	}
	
};

template <class Type, class Container, class Compare>
ostream& operator<<(ostream& out, priority_queue<Type, Container, Compare> q)
{
	out << "{\n";
	while (!q.empty())
    {
    	out << "  " << q.top() << '\n';
    	q.pop();
  	}
	out << '}';
	return out;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// 3.2
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class HeapException : public exception
{
protected:
    //сообщение об ошибке
    char* str;
public:
    HeapException(const char* s)
    {
        str = new char[strlen(s) + 1];
        strcpy(str, s);
    }
    HeapException(const HeapException& e)
    {
        str = new char[strlen(e.str) + 1];
        strcpy(str, e.str);
    }
    ~HeapException()
    {
        delete[] str;
    }

    //функцию вывода можно будет переопределить в производных классах, когда будет ясна конкретика
    virtual void print()
    {
        cout << "HeapException: " << str << "; " << what();
    }
};

class IndexOutOfBoundsException : public HeapException
{
protected:
    int index;
public:
    IndexOutOfBoundsException(const char* s, int i) : HeapException(s) { index = i; }
    IndexOutOfBoundsException(const IndexOutOfBoundsException& e) : HeapException(e) { index = e.index; }
    virtual void print()
    {
        cout << "IndexOutOfBoundsException: " << str << "; Index: " << index << "; " << what();
    }
};


template <class T>
class Node
{
private:
	T value;
public:
	//установить данные в узле
	T getValue() { return value; }
	void setValue(T v) { value = v; }

	//сравнение узлов
	int operator<(Node N)
	{
		return value < N.getValue();
	}

	int operator>(Node N)
	{
		return value > N.getValue();
	}

	//вывод содержимого одного узла
	void print()
	{
		cout << value;
	}
};

template <class T>
ostream& operator<<(ostream& out, const Node<T>& s)
{
	out << s->getValue() << "\n";
	return out;
}

template <class T>
bool base_less_priority(T a, T b)
{
	return a < b;
}

template <class T>
bool base_greater_priority(T a, T b)
{
	return a > b;
}

//куча (heap)
template <class T>
class Heap
{
private:
	//массив
	T* arr;
	//сравнение приоритетов
	bool (*less_priority)(T, T);
	bool (*greater_priority)(T, T);
	//сколько элементов добавлено
	int len;
	//сколько памяти выделено
	int size;
public:

	//доступ к вспомогательным полям кучи и оператор индекса
	int getCapacity() { return size; }
	int getCount() { return len; }

	T& operator[](int index)
	{
		if (index < 0)
			throw IndexOutOfBoundsException("Index can't be less then zero", index);
		if  (index >= len)
			throw IndexOutOfBoundsException("Index can't be greater then heap length-1", index);
		return arr[index];
	}

	//конструктор
	Heap<T>(int MemorySize = 100, bool(*less)(T a, T b)=base_less_priority, bool(*greater)(T a, T b) = base_greater_priority)
	{
		arr = new T[MemorySize];
		less_priority = less;
		greater_priority = greater;
		len = 0;
		size = MemorySize;
	}
	Heap<T>(bool(*less)(T a, T b), bool(*greater)(T a, T b))
	{
		arr = new T[100];
		less_priority = less;
		greater_priority = greater;
		len = 0;
		size = 100;
	}
	Heap<T>(const Heap<T>& other)
	{
		arr = new T[other.size];
		size = other.size;
		for (int i = 0; i<size;i++)
			arr[i] = other.arr[i];
		less_priority = other.less_priority;;
		greater_priority = other.greater_priority;;
		len = other.len;
	}

	//поменять местами элементы arr[index1], arr[index2]
	void Swap(int index1, int index2)
	{
		if (index1 <= 0 || index1 >= len)
		{
			if (index1 < 0)
				throw IndexOutOfBoundsException("Index can't be less then zero", index1);
			if  (index1 >= len)
				throw IndexOutOfBoundsException("Index can't be greater then heap length-1", index1);
		}
		if (index2 <= 0 || index2 >= len)
		{
			if (index2 < 0)
				throw IndexOutOfBoundsException("Index can't be less then zero", index2);
			if  (index2 >= len)
				throw IndexOutOfBoundsException("Index can't be greater then heap length-1", index2);
		}

		T temp;
		temp = arr[index1];
		arr[index1] = arr[index2];
		arr[index2] = temp;
	}

	//функции получения левого, правого дочернего элемента, родителя или их индексов в массиве
	int GetLeftChildIndex(int index)
	{
		if (index < 0 || index * 2 >= len)
		{
			if (index < 0)
				throw IndexOutOfBoundsException("Index can't be less then zero", index);
			if  (index >= len)
				throw IndexOutOfBoundsException("Index can't be greater then heap length-1", index);
		}
		return index * 2 + 1;
	}

	int GetRightChildIndex(int index)
	{
		if (index < 0 || index * 2 >= len)
		{
			if (index < 0)
				throw IndexOutOfBoundsException("Index can't be less then zero", index);
			if  (index >= len)
				throw IndexOutOfBoundsException("Index can't be greater then heap length-1", index);
		}
		return index * 2 + 2;
	}

	int GetParentIndex(int index)
	{
		if (index < 0 || index >= len)
		{
			if (index < 0)
				throw IndexOutOfBoundsException("Index can't be less then zero", index);
			if  (index >= len)
				throw IndexOutOfBoundsException("Index can't be greater then heap length-1", index);
		}

		if (index % 2 == 0)
			return index / 2 - 1;
		return index / 2;
	}

	//восстановление свойств кучи после удаления или добавления элемента
	void Heapify(int index = 0)
	{
		//то же, что и SiftDown
		int leftChild = 2*index + 1;;
		int rightChild = 2*index + 2;
		if (len <= leftChild) return;

		int largestChild=index;
		if (greater_priority(arr[leftChild], arr[largestChild]))
			largestChild = leftChild;
		if (rightChild < len && greater_priority(arr[rightChild], arr[largestChild]))
			largestChild = rightChild;

		if (largestChild != index)
		{
			Swap(index, largestChild);
			Heapify(largestChild);
		}		
	}

	//просеить элемент вверх
	void SiftUp(int index = -1)
	{
		if (index == -1) index = len - 1;
		if (index == 0) return;
		int parent_index = GetParentIndex(index);
		if (greater_priority(arr[index], arr[parent_index]))
		{
			Swap(index, parent_index);
			SiftUp(parent_index);
		}
	}

	//добавление элемента - вставляем его в конец массива и просеиваем вверх
	//удобный интерфейс для пользователя 
	void push(T v)
	{
		Node<T>* N = new Node<T>;
		N->setValue(v);
		push(N);
	}

	void push(Node<T>* N)
	{
		//добавить элемент и включить просеивание
		if (len < size)
		{
			arr[len] = N->getValue();
			len++;
			SiftUp();
		}
	}
	
	T ExtractMax()
	{
		//исключить максимум и запустить просеивание кучи
		T res = arr[0];
		Swap(0, len - 1);
		len--;
		if (len > 1)
			Heapify();
		return res;
	}

	T remove(int index)
    {
        if (index < 0 || index >= len)
            throw IndexOutOfBoundsException("Index less or more then available was submitted to remove: ", index);
		T ans = arr[index];
        if (index == len - 1)
            len--;
        else
        { 
            Swap(index, len - 1);
            len--;
			if (less_priority(arr[GetParentIndex(index)], arr[index]))
            	SiftUp(index);
			else if (greater_priority(arr[GetParentIndex(index)], arr[index]))
            	Heapify(index);
        }
		return ans;
    }

	template <class T1> friend ostream& operator<< (ostream& stream, Heap<T1> heap);

	~Heap()
	{
		if (arr != NULL)
			delete[] arr;
		len = 0;
		arr = NULL;
	}
};

template <class T>
ostream& operator<< (ostream& out, Heap<T> heap)
{
	out << "{\n";
	while(heap.getCount())
		out << "  " << heap.ExtractMax() << ";\n";
	out << "}";
	return out;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




// 3.3
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class V> class FibonacciHeap;

template <class V> struct FibNode {
private:
	typename list<FibNode<V>*>::iterator self;
	typename list<FibNode<V>*>::iterator parent;
	list<FibNode<V>*> children;
	V priority;
	int degree;
public:
	friend class FibonacciHeap<V>;
	typename list<FibNode<V>*>::iterator getSelf() { return self; }
	typename list<FibNode<V>*>::iterator getParent() { return parent; }
	list<FibNode<V>*> getChildren() { return children; }
	V getpriority() { return priority; }

	bool hasChildren() { return !children.empty(); }
	bool hasParent() { return parent != children.end(); }
};

template <class V> class FibonacciHeap
{
protected:
	list<FibNode<V>*> heap;
	typename list<FibNode<V>*>::iterator max;
	//сравнение приоритетов
	bool (*less_priority)(V, V);
	bool (*greater_priority)(V, V);
public:

	FibonacciHeap<V>(bool(*less)(V, V) = base_less_priority, bool (*greater)(V, V) = base_greater_priority)
	{
		heap.clear();
		max = heap.end();
		less_priority = less;
		greater_priority = greater;
	}
	virtual ~FibonacciHeap()
	{
		if (!heap.empty())
		{
			for(typename list<FibNode<V>*>::iterator it = heap.begin(); it != heap.end(); ++it)
				delete *it;
			heap.clear();
		}
	}
	FibNode<V>* push(V priority)
	{
		FibNode<V>* ret = new FibNode<V>;
		ret->priority = priority;
		ret->degree = 0;
		ret->children.clear();
		ret->parent = ret->children.end();
		ret->self = heap.insert(heap.end(), ret);
		if (heap.size()==1 || greater_priority(ret->priority, (*max)->priority))
			max = ret->self;
		return ret;
	}
	void merge(FibonacciHeap& other)
	{
		heap.splice(heap.end(), other.heap);
		if (greater_priority((*other.max)->priority, (*max)->priority))
			max = other.max;
	}

	bool isEmpty()
	{
		return heap.empty();
	}

	V getMax()
	{
        return (*max)->priority;
    }

	V ExtractMax() 
	{
		if (isEmpty())
			throw HeapException("Heap is empty.");

		FibNode<V>* maxNode = *max;
		V maxPriority = maxNode->priority;

		if (!maxNode->children.empty())
			heap.splice(heap.end(), maxNode->children);

		heap.erase(maxNode->self);
		delete maxNode;

		if (!heap.empty()) 
			consolidate();
		else
			max = heap.end();

		return maxPriority;
	}

	void consolidate() 
	{
		vector<FibNode<V>*> A(heap.size(), nullptr);
		for (typename list<FibNode<V>*>::iterator it = heap.begin(); it != heap.end(); ++it) 
		{
			FibNode<V>* x = *it;
			int d = x->degree;
			while (A[d] != nullptr) 
			{
				FibNode<V>* y = A[d];
				if (less_priority(x->priority, y->priority))
					swap(x, y);
				_childLink(x, y);
				A[d] = nullptr;
				d++;
			}
			A[d] = x;
		}

		heap.clear();
		max = heap.end();
		for (int i = 0; i < A.size(); i++) 
		{
			if (A[i] != nullptr) 
			{
				A[i]->self = heap.insert(heap.end(), A[i]);
				if (max == heap.end() || greater_priority(A[i]->priority, (*max)->priority))
					max = A[i]->self;
			}
		}
	}

	void _childLink(FibNode<V>* parent, FibNode<V>* child) 
	{
		parent->children.push_back(child);
		if (!child->children.empty())
			parent->children.splice(parent->children.end(), child->children);
		parent->degree += child->degree;
		child->children.clear();
		parent->degree++;
	}
	
	void printWithExtractMax()
	{
		cout << "{\n";
		int sz = heap.size();
		for (int i = 0; i < sz; i++)
		{
			cout <<  "  " << ExtractMax() << ";\n";
		}
		cout << "}";
	}
};


int main()
{
	priority_queue<SportTeam, vector<SportTeam>, SportTeam_PriorityKey> PQ;
	PQ.push(SportTeam("Lions", "City1", 10, 5, 3, 30));
	PQ.push(SportTeam("Tigers", "City2", 8, 7, 2, 29));
	PQ.push(SportTeam("Bears", "City3", 12, 3, 2, 35));
	PQ.push(SportTeam("Eagles", "City4", 8, 7, 2, 29));
	PQ.push(SportTeam("Wolves", "City5", 10, 5, 3, 30));
	PQ.push(SportTeam("Dolphins", "City6", 6, 6, 6, 24));
	PQ.push(SportTeam("Panthers", "City7", 10, 5, 3, 30));
	cout << "Priority queue with custom PriorityKey:\n" << PQ;


	Heap<SportTeam> Heap(SportTeam_less_priority, SportTeam_greater_priority);
	Heap.push(SportTeam("Lions", "City1", 10, 5, 3, 30));
	Heap.push(SportTeam("Tigers", "City2", 8, 7, 2, 29));
	Heap.push(SportTeam("Bears", "City3", 12, 3, 2, 35));
	Heap.push(SportTeam("Eagles", "City4", 8, 7, 2, 29));
	Heap.push(SportTeam("Wolves", "City5", 10, 5, 3, 30));
	Heap.push(SportTeam("Dolphins", "City6", 6, 6, 6, 24));
	Heap.push(SportTeam("Panthers", "City7", 10, 5, 3, 30));
	cout << "\n\nHeap:\n" << Heap;
	cout << "\n\nRemoving 3d element: " << Heap.remove(3);
	cout << "\n\nHeap after removind 3d element:\n" << Heap;

	FibonacciHeap<SportTeam> FH(SportTeam_less_priority, SportTeam_greater_priority);
	FH.push(SportTeam("Lions", "City1", 10, 5, 3, 30));
	FH.push(SportTeam("Tigers", "City2", 8, 7, 2, 29));
	FH.push(SportTeam("Bears", "City3", 12, 3, 2, 35));
	FH.push(SportTeam("Eagles", "City4", 8, 7, 2, 29));
	FH.push(SportTeam("Wolves", "City5", 10, 5, 3, 30));
	FH.push(SportTeam("Dolphins", "City6", 6, 6, 6, 24));
	FH.push(SportTeam("Panthers", "City7", 10, 5, 3, 30));
	cout << "\n\nPrinting fibonacci heap with use of ExtractMax()\n";
	FH.printWithExtractMax();
	return 0;
}
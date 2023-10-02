// Лазарев Александр КМБО-03-22
// ВАРИАТН 15
#include <iostream>
    
using namespace std;

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
		return (value < N.getValue());
	}

	int operator>(Node N)
	{
		return (value > N.getValue());
	}

	//вывод содержимого одного узла
	void print()
	{
		cout << value;
	}
};

template <class T>
void print(Node<T>* N)
{
	cout << N->getValue() << "\n";
}

//куча (heap)
template <class T>
class Heap
{
private:
	//массив
	T* arr;
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
		if (index < 0 || index >= len)
			;//?

		return arr[index];
	}

	//конструктор
	Heap<T>(int MemorySize = 100)
	{
		arr = new T[MemorySize];
		len = 0;
		size = MemorySize;
	}

	//поменять местами элементы arr[index1], arr[index2]
	void Swap(int index1, int index2)
	{
		if (index1 <= 0 || index1 >= len)
			;
		if (index2 <= 0 || index2 >= len)
			;
		//здесь нужна защита от дурака

		T temp;
		temp = arr[index1];
		arr[index1] = arr[index2];
		arr[index2] = temp;
	}

	//скопировать данные между двумя узлами

	//функции получения левого, правого дочернего элемента, родителя или их индексов в массиве

	int GetLeftChildIndex(int index)
	{
		if (index < 0 || index * 2 >= len)
			;
		//здесь нужна защита от дурака
		return index * 2 + 1;
	}

	int GetRightChildIndex(int index)
	{
		if (index < 0 || index * 2 >= len)
			;
		//здесь нужна защита от дурака

		return index * 2 + 2;
	}

	int GetParentIndex(int index)
	{
		if (index <= 0 || index >= len)
			;
		//здесь нужна защита от дурака

		if (index % 2 == 0)
			return index / 2 - 1;
		return index / 2;
	}

	//восстановление свойств кучи после удаления или добавления элемента
	void Heapify(int index = 0)
	{
		//то же, что и SiftDown
		int leftChild;
		int rightChild;
		int largestChild;

		leftChild = 2 * index + 1;
		rightChild = 2 * index + 2;

		//нужно сравнить элементы и при необходимости произвести просеивание вниз
		
	}

	//просеить элемент вверх
	void SiftUp(int index = -1)
	{
		if (index == -1) index = len - 1;

		//нужно сравнить элементы и при необходимости произвести просеивание вверх
		int parent_index = GetParentIndex(index);
		
	}

	//добавление элемента - вставляем его в конец массива и просеиваем вверх
	//удобный интерфейс для пользователя 
	void push(T v)
	{
		
	}

	
	T ExtractMax()
	{
		//исключить максимум и запустить просеивание кучи
		
	}

	
};


int main()
{
	Heap<int> Tree;

	Tree.push(1);
	Tree.push(-1);
	Tree.push(-2);
	Tree.push(2);
	Tree.push(5);
	Tree.push(6);
	Tree.push(-3);
	Tree.push(-4);
	Tree.push(4);
	Tree.push(3);

	cout << "\n-----\nExtractMax:";
	int i = 0;
	while (i < Tree.getCount())
	{
		int n = Tree.ExtractMax();
		cout<<n << "\n";
	}

	char c; cin >> c;
	return 0;
}


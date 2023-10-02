
#include <iostream>
#include <vector>
#include <string>

using namespace std;


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
    friend bool operator < (const SportTeam a, const SportTeam b);
    friend bool operator > (const SportTeam a, const SportTeam b);
    friend bool operator == (const SportTeam a, const SportTeam b);
    friend bool base_equals(vector<SportTeam> a, vector<SportTeam> b);

};

ostream& operator<<(ostream& out, const SportTeam& s) 
{
    out << s.name << " " << s.city << " " << s.num_wins << " " << s.num_draws << " " << s.num_losses << " " << s.score;
    return out;
}

bool operator > (const SportTeam a, const SportTeam b)
{
	if (a.name > b.name)
        return true;
    if (a.name == b.name && a.city > b.city)
        return true;
    return false;
}

bool operator < (const SportTeam a, const SportTeam b)
{
	if (a.name < b.name)
        return true;
    if (a.name == b.name && a.city < b.city)
        return true;
    return false;
}

bool operator == (const SportTeam a, const SportTeam b)
{
	return a.name == b.name && a.city == b.city;
}


template <class T>
bool base_equals(T a, T b)
{
	return a == b;
}

bool base_equals(vector<SportTeam> a, vector<SportTeam> b)
{
    return a[0].name == b[0].name && a[0].city == b[0].city;
}

template <class T>
ostream& operator<<(ostream& out, const vector<T>& s)
{
	out << "{ ";
    for (int i = 0; i < s.size(); i++)
        out << s[i] << "; ";
	out << '}';
    return out;
}

//узел
template<class T>
class Node
{
protected:
	//закрытые переменные Node N; N.data = 10 вызовет ошибку
	T data;

	//не можем хранить Node, но имеем право хранить указатель
	Node* left;
	Node* right;
	Node* parent;

	//переменная, необходимая для поддержания баланса дерева
	int height;
public:
	//доступные извне переменные и функции
	virtual void setData(T d) { data = d; }
	virtual T getData() { return data; }
	int getHeight() { return height; }

	virtual Node* getLeft() { return left; }
	virtual Node* getRight() { return right; }
	virtual Node* getParent() { return parent; }

	virtual void setLeft(Node* N) { left = N; }
	virtual void setRight(Node* N) { right = N; }
	virtual void setParent(Node* N) { parent = N; }

	//Конструктор. Устанавливаем стартовые значения для указателей
	Node<T>(T n)
	{
		data = n;
		left = right = parent = NULL;
		height = 1;
	}

	Node<T>()
	{
		left = NULL;
		right = NULL;
		parent = NULL;
		height = 1;
	}


	virtual void print()
	{
		cout << "\n" << data;
	}

	virtual void setHeight(int h)
	{
		height = h;
	}

	template<class T1> friend ostream& operator<< (ostream& stream, Node<T1>& N);

	Node* successor()
	{
		Node<T>* Current;
		if (right != NULL)
		{
			Current = right;
			while (Current->left != NULL)
				Current = Current->left;
		}
		else
		{
			Current = parent;
			while (Current != NULL && Current->data < data)
				Current = Current->parent;
		}
		return Current;
	}
	Node* predecessor()
	{
		Node<T>* Current;
		if (left != NULL)
		{
			Current = left;
			while (Current->right != NULL)
				Current = Current->right;
		}
		else
		{
			Current = parent;
			while (Current != NULL && Current->data > data)
				Current = Current->parent;
		}
		return Current;
	}
};

template<class T>
ostream& operator<< (ostream& stream, Node<T>& N)
{
	stream << "\nNode data: " << N.data << ", height: " << N.height;
	return stream;
}
template<class T>
void print(Node<T>* N) { cout << "\n" << N->getData() << ' ' << N->getHeight(); }

template<class T>
class Tree
{
protected:
	//корень - его достаточно для хранения всего дерева
	Node<T>* root;

	virtual Node<T>* push(Node<T>* N, Node<T>* Current)
	{
		//не передан добавляемый узел
		if (N == NULL) return NULL;

		//пустое дерево - добавляем в корень
		if (root == NULL) { root = N; return N; }

		if (Current->getData() > N->getData())
		{
			//идем влево
			if (Current->getLeft() != NULL) return push(N, Current->getLeft());
			else { Current->setLeft(N); N->setParent(Current); }
		}
		if (Current->getData() < N->getData())
		{
			//идем вправо
			if (Current->getRight() != NULL) return push(N, Current->getRight());
			else { Current->setRight(N); N->setParent(Current); }
		}
		//вернуть добавленный узел
		return N;
	}

	//поиск узла в дереве. Второй параметр - в каком поддереве искать, первый - что искать
	virtual Node<T>* Find_R(T data, Node<T>* Current, bool(*equals)(T, T))
	{
		//база рекурсии
		if (Current == NULL) return NULL;

		if (equals(Current->getData(), data)) return Current;

		//рекурсивный вызов
		if (Current->getData() > data) return Find_R(data, Current->getLeft(), equals);
		return Find_R(data, Current->getRight(), equals);
	}
public:
	//доступ к корневому элементу
	virtual Node<T>* getRoot() { return root; }

	//конструктор дерева: в момент создания дерева ни одного узла нет, корень смотрит в никуда
	Tree<T>() { root = NULL; }

	//рекуррентная функция добавления узла. Устроена аналогично, но вызывает сама себя - добавление в левое или правое поддерево
	virtual Node<T>* push(Node<T>* N)
	{
		return push(N, root);
	}

	//функция для добавления числа. Делаем новый узел с этими данными и вызываем нужную функцию добавления в дерево
	virtual Node<T>* push(T n)
	{
		Node<T>* N = new Node<T>;
		N->setData(n);
		return push(N);
	}

	virtual Node<T>* Find(T data, bool(*equals)(T, T) = NULL)
	{
		if (equals == NULL)
			return Find_R(data, root, base_equals);
		return Find_R(data, root, equals);
	}

	//удаление узла
	virtual Node<T>* Remove(Node<T>* N)
	{
		if (N == NULL) return NULL; // Если не нашли, то выходим

		// Если удаляемый узел имеет двух потомков
		if (N->getLeft() != NULL && N->getRight() != NULL)
		{
			Node<T>* successor = N->successor(); // Находим преемника
			N->setData(successor->getData()); // Копируем данные преемника в удаляемый узел
			N = successor; // Теперь удаляемый узел - преемник
		}

		// Если удаляемый узел имеет только одного потомка или не имеет их вообще
		Node<T>* child = NULL;

		if (N->getLeft() != NULL)
			child = N->getLeft();
		else if (N->getRight() != NULL)
			child = N->getRight();

		if (child != NULL)
			child->setParent(N->getParent()); // Устанавливаем родителя для потомка

		if (N == root)
			root = child; // Если удаляем корень, то новый корень - потомок удаляемого узла
		else
		{
			if (N == N->getParent()->getLeft())
				N->getParent()->setLeft(child); // Если удаляемый узел - левый потомок, то переставляем указатель на потомка
			else
				N->getParent()->setRight(child); // Если удаляемый узел - правый потомок, то переставляем указатель на потомка
		}
		// Обрываем связи узла с деревом
		N->setLeft(NULL);
		N->setRight(NULL);
		N->setParent(NULL); 
		N->setHeight(0);
		return N;
	}

	virtual Node<T>* Remove(T N)
	{
		return Remove(Find(N));
	}

	virtual Node<T>* Min(Node<T>* Current = NULL)
	{
		//минимум - это самый "левый" узел. Идём по дереву всегда влево
		if (root == NULL) return NULL;

		if (Current == NULL)
			Current = root;
		while (Current->getLeft() != NULL)
			Current = Current->getLeft();
		return Current;
	}

	virtual Node<T>* Max(Node<T>* Current = NULL)
	{
		//максимум - это самый "правый" узел. Идём по дереву всегда вправо
		if (root == NULL) return NULL;
		if (Current == NULL) Current = root;
		for (; Current->getRight() != NULL; Current = Current->getRight());
		return Current;
	}

	//три обхода дерева
	virtual void PreOrder(Node<T>* N, void (*f)(Node<T>*))
	{
		if (N != NULL)
			f(N);
		if (N != NULL && N->getLeft() != NULL)
			PreOrder(N->getLeft(), f);
		if (N != NULL && N->getRight() != NULL)
			PreOrder(N->getRight(), f);
	}

	//InOrder-обход даст отсортированную последовательность
	virtual void InOrder(Node<T>* N, void (*f)(Node<T>*))
	{
		if (N != NULL && N->getLeft() != NULL)
			InOrder(N->getLeft(), f);
		if (N != NULL)
			f(N);
		if (N != NULL && N->getRight() != NULL)
			InOrder(N->getRight(), f);
	}

	virtual void PostOrder(Node<T>* N, void (*f)(Node<T>*))
	{
		if (N != NULL && N->getLeft() != NULL)
			PostOrder(N->getLeft(), f);
		if (N != NULL && N->getRight() != NULL)
			PostOrder(N->getRight(), f);
		if (N != NULL)
			f(N);
	}
};

template<class T>
class AVL_Tree : public Tree<T>
{
protected:
	//определение разности высот двух поддеревьев
	int bfactor(Node<T>* p)
	{
		int hl = 0;
		int hr = 0;
		if (p->getLeft() != NULL)
			hl = p->getLeft()->getHeight();
		if (p->getRight() != NULL)
			hr = p->getRight()->getHeight();
		return (hr - hl);
	}

	//при добавлении узлов в них нет информации о балансе, т.к. не ясно, куда в дереве они попадут
	//после добавления узла рассчитываем его высоту (расстояние до корня) и редактируем высоты в узлах, где это
	//значение могло поменяться
	void fixHeight(Node<T>* p)
	{
		int hl = 0;
		int hr = 0;
		if (p->getLeft() != NULL)
			hl = p->getLeft()->getHeight();
		if (p->getRight() != NULL)
			hr = p->getRight()->getHeight();
		p->setHeight((hl > hr ? hl : hr) + 1);
	}

	//краеугольные камни АВЛ-деревьев - процедуры поворотов
	Node<T>* RotateRight(Node<T>* p) // правый поворот вокруг p
	{
		Node<T>* q = p->getLeft();

		p->setLeft(q->getRight());
	    q->setRight(p);

		if (p == Tree<T>::root)
			Tree<T>::root = q;
		else
		{
			if (p->getParent()->getLeft() == p)
				p->getParent()->setLeft(q);
			else
				p->getParent()->setRight(q);
		}

		q->setParent(p->getParent());
		p->setParent(q);
		if(p->getLeft()!=NULL) p->getLeft()->setParent(p);

		fixHeight(p);
		fixHeight(q);
		return q;
	}

	Node<T>* RotateLeft(Node<T>* q) // левый поворот вокруг q
	{
		Node<T>* p = q->getRight();

		q->setRight(p->getLeft());
		p->setLeft(q);
		
		if (q == Tree<T>::root)
			Tree<T>::root = p;
		else
		{
			if (q->getParent()->getLeft() == q)
				q->getParent()->setLeft(p);
			else
				q->getParent()->setRight(p);
		}

		p->setParent(q->getParent());
		q->setParent(p);
		if(q->getRight()!=NULL) q->getRight()->setParent(q);
		
		fixHeight(q);
		fixHeight(p);
		return p;
	}

	//балансировка поддерева узла p - вызов нужных поворотов в зависимости от показателя баланса
	Node<T>* Balance(Node<T>* p) // балансировка узла p
	{
		fixHeight(p);
		if (bfactor(p) == 2)
		{
			if(bfactor(p->getRight())<0) RotateRight(p->getRight());
			return RotateLeft(p);
		}
		if (bfactor(p) == -2)
		{
			if (bfactor(p->getLeft()) > 0) RotateLeft(p->getLeft());
			return RotateRight(p);
		}

		return p; // балансировка не нужна
	}

public:
	//конструктор AVL_Tree вызывает конструктор базового класса Tree
	AVL_Tree<T>() : Tree<T>() {}

	virtual Node<T>* push(Node<T>* N)
	{
		return push(N, Tree<T>::root);
	}

	//рекуррентная функция добавления узла. Устроена аналогично, но вызывает сама себя - добавление в левое или правое поддерево
	virtual Node<T>* push(Node<T>* N, Node<T>* Current)
	{
		//вызываем функцию push из базового класса
		Node<T>* pushedNode = Tree<T>::push(N, Current);
		//применяем к добавленному узлу балансировку
		if (Current != NULL)
			return Balance(Current);
		return pushedNode;
	}

	//функция для добавления числа. Делаем новый узел с этими данными и вызываем нужную функцию добавления в дерево
	virtual Node<T>* push(T n)
	{
		Node<T>* N = new Node<T>;
		N->setData(n);
		return push(N);
	}

	//удаление узла
	virtual Node<T>* remove(Node<T>* N)
	{
		Node<T>* parent = N->getParent();
		// Вызываем метод remove родительского класса для удаления узла
		Node<T>* removedNode = Tree<T>::Remove(N);
		// Балансировка дерева после удаления узла
		if (parent != NULL)
			Balance(parent);
		return removedNode;
	}
};


//класс итератор по дереву
template<typename ValueType>
class TreeIterator : public std::iterator<std::input_iterator_tag, ValueType>
{

public:
	Node<ValueType>* ptr;

	TreeIterator() { ptr = NULL; }
	TreeIterator(Node<ValueType>* p) { ptr = p; }
	TreeIterator(const TreeIterator& it) { ptr = it.ptr; }

	TreeIterator& operator=(const TreeIterator& it) { ptr = it.ptr; return *this ; }
	TreeIterator& operator=(Node<ValueType>* p) { ptr = p; return *this;  }

	bool operator!=(TreeIterator const& other) const { return ptr != other.ptr; }
	bool operator==(TreeIterator const& other) const { return ptr == other.ptr; }
	Node<ValueType>& operator*() { return *ptr; }
	TreeIterator& operator++() //++p;
	{
		if(ptr!=NULL)
			ptr = ptr->successor();
		return *this;
	}
	TreeIterator operator++(int v) //x = p++;
	{
		Node<ValueType>* p = ptr;
		if (ptr != NULL)
			ptr = ptr->successor();
		return TreeIterator<ValueType>(p);
	}
	TreeIterator& operator--() //--p;
	{
		if(ptr!=NULL)
			ptr = ptr->predecessor();
		return *this;
	}
	TreeIterator operator--(int v) //x = p--;
	{
		Node<ValueType>* p = ptr;
		if (ptr != NULL)
			ptr = ptr->predecessor();
		return TreeIterator<ValueType>(p);
	}
};

//класс итерируемое дерево поиска
template<class T>
class IteratedTree : public AVL_Tree<T>
{
protected:
	virtual TreeIterator<T> push_R(TreeIterator<T> N, TreeIterator<T> Current)
	{
		//не передан добавляемый узел
		if (N == NULL) return NULL;

		//пустое дерево - добавляем в корень
		if (Tree<T>::root == NULL) { Tree<T>::root = N.ptr; return N; }

		if ((*Current).getData() > (*N).getData())
		{
			//идем влево
			if ((*Current).getLeft() != NULL) return push(N, (*Current).getLeft());
			else { (*Current).setLeft(N.ptr); (*N).setParent(Current.ptr); }
		}
		if ((*Current).getData() < (*N).getData())
		{
			//идем вправо
			if ((*Current).getRight() != NULL) return push(N, (*Current).getRight());
			else { (*Current).setRight(N.ptr); (*N).setParent(Current.ptr); }
		}
		//вернуть добавленный узел
		return N;
	}

	//поиск узла в дереве. Второй параметр - в каком поддереве искать, первый - что искать
	virtual TreeIterator<T> Find_R(T data, TreeIterator<T> Current, bool(*equals)(T, T))
	{
		//база рекурсии
		if (Current == NULL) return NULL;

		if (equals((*Current).getData(), data)) return Current;

		//рекурсивный вызов
		if ((*Current).getData() > data) return Find_R(data, (*Current).getLeft(), equals);
		return Find_R(data, (*Current).getRight(), equals);
	}

	//определение разности высот двух поддеревьев
	int bfactor(TreeIterator<T> p)
	{
		int hl = 0;
		int hr = 0;
		if ((*p).getLeft() != NULL)
			hl = (*p).getLeft()->getHeight();
		if ((*p).getRight() != NULL)
			hr = (*p).getRight()->getHeight();
		return (hr - hl);
	}

	//при добавлении узлов в них нет информации о балансе, т.к. не ясно, куда в дереве они попадут
	//после добавления узла рассчитываем его высоту (расстояние до корня) и редактируем высоты в узлах, где это
	//значение могло поменяться
	void fixHeight(TreeIterator<T> p)
	{
		int hl = 0;
		int hr = 0;
		if ((*p).getLeft() != NULL)
			hl = (*p).getLeft()->getHeight();
		if ((*p).getRight() != NULL)
			hr = (*p).getRight()->getHeight();
		(*p).setHeight((hl > hr ? hl : hr) + 1);
	}

	//краеугольные камни АВЛ-деревьев - процедуры поворотов
	TreeIterator<T> RotateRight(TreeIterator<T> p) // правый поворот вокруг p
	{
		TreeIterator<T> q = (*p).getLeft();

		(*p).setLeft((*q).getRight());
	    (*q).setRight(p.ptr);

		if (p == Tree<T>::root)
			Tree<T>::root = q.ptr;
		else
		{
			if ((*p).getParent()->getLeft() == p.ptr)
				(*p).getParent()->setLeft(q.ptr);
			else
				(*p).getParent()->setRight(q.ptr);
		}

		(*q).setParent((*p).getParent());
		(*p).setParent(q.ptr);
		if((*p).getLeft()!=NULL) (*p).getLeft()->setParent(p.ptr);

		fixHeight(p);
		fixHeight(q);
		return q;
	}

	TreeIterator<T> RotateLeft(TreeIterator<T> q) // левый поворот вокруг q
	{
		TreeIterator<T> p = (*q).getRight();

		(*q).setRight((*p).getLeft());
		(*p).setLeft(q.ptr);
		
		if (q == Tree<T>::root)
			Tree<T>::root = p.ptr;
		else
		{
			if ((*q).getParent()->getLeft() == q.ptr)
				(*q).getParent()->setLeft(p.ptr);
			else
				(*q).getParent()->setRight(p.ptr);
		}

		(*p).setParent((*q).getParent());
		(*q).setParent(p.ptr);
		if((*q).getRight()!=NULL) (*q).getRight()->setParent(q.ptr);
		
		fixHeight(q);
		fixHeight(p);
		return p;
	}

	//балансировка поддерева узла p - вызов нужных поворотов в зависимости от показателя баланса
	TreeIterator<T> Balance(TreeIterator<T> p) // балансировка узла p
	{
		fixHeight(p);
		if (bfactor(p) == 2)
		{
			if(bfactor((*p).getRight())<0) RotateRight((*p).getRight());
			return RotateLeft(p);
		}
		if (bfactor(p) == -2)
		{
			if (bfactor((*p).getLeft()) > 0) RotateLeft((*p).getLeft());
			return RotateRight(p);
		}

		return p; // балансировка не нужна
	}

public:
	IteratedTree<T>() : AVL_Tree<T>() {}

	TreeIterator<T> begin() { return TreeIterator<T>(Tree<T>::Min()); }
	TreeIterator<T> end() { return TreeIterator<T>(Tree<T>::Max()); }

	//рекуррентная функция добавления узла. Устроена аналогично, но вызывает сама себя - добавление в левое или правое поддерево
	virtual TreeIterator<T> push(TreeIterator<T> N, TreeIterator<T> Current)
	{
		//вызываем функцию push из базового класса
		TreeIterator<T> pushedNode = push_R(N, Current);
		//применяем к добавленному узлу балансировку
		if (Current != NULL)
			return Balance(Current);
		return pushedNode;
	}

	//функция для добавления числа. Делаем новый узел с этими данными и вызываем нужную функцию добавления в дерево
	virtual TreeIterator<T> Push(T n)
	{
		TreeIterator<T> N = new Node<T>;
		(*N).setData(n);
		return push(N, TreeIterator<T> (Tree<T>::root));
	}

	//удаление узла
	virtual TreeIterator<T> remove(TreeIterator<T> N)
	{
		TreeIterator<T> parent = (*N).getParent();
		if (N == NULL) return NULL; // Если не нашли, то выходим

		// Если удаляемый узел имеет двух потомков
		if ((*N).getLeft() != NULL && (*N).getRight() != NULL)
		{
			TreeIterator<T> successor = N; // Находим преемника
			successor++;
			(*N).setData((*successor).getData()); // Копируем данные преемника в удаляемый узел
			N = successor; // Теперь удаляемый узел - преемник
		}

		// Если удаляемый узел имеет только одного потомка или не имеет их вообще
		TreeIterator<T> child = NULL;

		if ((*N).getLeft() != NULL)
			child = (*N).getLeft();
		else if ((*N).getRight() != NULL)
			child = (*N).getRight();

		if (child != NULL)
			(*child).setParent((*N).getParent()); // Устанавливаем родителя для потомка

		if (N == Tree<T>::root)
			Tree<T>::root = child.ptr; // Если удаляем корень, то новый корень - потомок удаляемого узла
		else
		{
			if (N == (*N).getParent()->getLeft())
				(*N).getParent()->setLeft(child.ptr); // Если удаляемый узел - левый потомок, то переставляем указатель на потомка
			else
				(*N).getParent()->setRight(child.ptr); // Если удаляемый узел - правый потомок, то переставляем указатель на потомка
		}
		// Обрываем связи узла с деревом
		(*N).setLeft(NULL);
		(*N).setRight(NULL);
		(*N).setParent(NULL);
		(*N).setHeight(0);
		if (parent != NULL)
			Balance(parent);
		return N;
	}

	virtual TreeIterator<T> remove(T N)
	{
		return remove(find(N));
	}

	virtual TreeIterator<T> Min(TreeIterator<T> Current = NULL)
	{
		//минимум - это самый "левый" узел. Идём по дереву всегда влево
		if (Tree<T>::root == NULL) return NULL;

		if (Current == NULL)
			Current = Tree<T>::root;
		while ((*Current).getLeft() != NULL)
			Current = (*Current).getLeft();
		return Current;
	}

	virtual TreeIterator<T> Max(TreeIterator<T> Current = NULL)
	{
		//максимум - это самый "правый" узел. Идём по дереву всегда вправо
		if (Tree<T>::root == NULL) return NULL;
		if (Current == NULL) Current = Tree<T>::root;
		for (; (*Current).getRight() != NULL; Current = (*Current).getRight());
		return Current;
	}


	virtual TreeIterator<T> find(T data, bool(*equals)(T, T) = NULL)
	{
		if (equals == NULL)
			return Find_R(data, Tree<T>::root, base_equals);
		return Find_R(data, Tree<T>::root, equals);
	}

	//три обхода дерева
	virtual void PreOrder(TreeIterator<T> N, void (*f)(Node<T>*))
	{
		if (N != NULL)
			f(N.ptr);
		if (N != NULL && (*N).getLeft() != NULL)
			PreOrder((*N).getLeft(), f);
		if (N != NULL && (*N).getRight() != NULL)
			PreOrder((*N).getRight(), f);
	}

	//InOrder-обход даст отсортированную последовательность
	virtual void InOrder(TreeIterator<T> N, void (*f)(Node<T>*))
	{
		if (N != NULL && (*N).getLeft() != NULL)
			InOrder((*N).getLeft(), f);
		if (N != NULL)
			f(N.ptr);
		if (N != NULL && (*N).getRight() != NULL)
			InOrder((*N).getRight(), f);
	}

	virtual void PostOrder(TreeIterator<T> N, void (*f)(Node<T>*))
	{
		if (N != NULL && (*N).getLeft() != NULL)
			PostOrder((*N).getLeft(), f);
		if (N != NULL && (*N).getRight() != NULL)
			PostOrder((*N).getRight(), f);
		if (N != NULL)
			f(N.ptr);
	}

	template<class T1> friend ostream& operator<< (ostream& stream, IteratedTree<T1>& N);

};

template<class T> ostream& operator<< (ostream& stream, IteratedTree<T>& N)
{
	for (TreeIterator<T> i = N.begin(); i != N.end(); ++i)
		stream << (*i).getData() << ' ';
	if (N.end() != NULL)
		stream << (*N.end()).getData();
	return stream;
}

template <class T>
class IteratedMultiKeyTree : public IteratedTree<vector<T>>
{
protected:
	virtual TreeIterator<vector<T>> push_R(TreeIterator<vector<T>> N, TreeIterator<vector<T>> Current, bool(*equals)(vector<T>, vector<T>))
	{
		//не передан добавляемый узел
		if (N == NULL) return NULL;

		//пустое дерево - добавляем в корень
		if (Tree<vector<T>>::root == NULL) { Tree<vector<T>>::root = N.ptr; return N; }

        if (equals((*Current).getData(), (*N).getData()))
		{
			vector<T> res = (*Current).getData();
			res.push_back((*N).getData()[0]);
			(*Current).setData(res);
            return Current;
		}
		if ((*Current).getData() > (*N).getData())
		{
			//идем влево
			if ((*Current).getLeft() != NULL) return push(N, (*Current).getLeft(), equals);
			else { (*Current).setLeft(N.ptr); (*N).setParent(Current.ptr); }
		}
		if ((*Current).getData() < (*N).getData())
		{
			//идем вправо
			if ((*Current).getRight() != NULL) return push(N, (*Current).getRight(), equals);
			else { (*Current).setRight(N.ptr); (*N).setParent(Current.ptr); }
		}
		//вернуть добавленный узел
		return N;
	}
public:
	virtual TreeIterator<vector<T>> push(TreeIterator<vector<T>> N, TreeIterator<vector<T>> Current, bool(*equals)(vector<T>, vector<T>))
	{
		//вызываем функцию push из базового класса
		TreeIterator<vector<T>> pushedNode = push_R(N, Current, equals);
		//применяем к добавленному узлу балансировку
		if (Current != NULL)
			return IteratedTree<vector<T>>::Balance(Current);
		return pushedNode;
	}

	//функция для добавления числа. Делаем новый узел с этими данными и вызываем нужную функцию добавления в дерево
	virtual TreeIterator<vector<T>> Push(T n, bool(*equals)(vector<T>, vector<T>)=NULL)
	{
		TreeIterator<vector<T>> N = new Node<vector<T>>;
		(*N).setData(vector<T>{n});
		if (equals == NULL)
			return push(N, TreeIterator<vector<T>> (Tree<vector<T>>::root), base_equals);
		return push(N, TreeIterator<vector<T>> (Tree<vector<T>>::root), equals);
	}

	template<class T1> friend ostream& operator<< (ostream& stream, IteratedMultiKeyTree<T1>& N);
};


template<class T>
class Splay1 : protected IteratedTree<T>
{
public:
	Splay1<T>() : IteratedTree<T>() {}
	TreeIterator<T> begin() { return IteratedTree<T>::begin(); }
	TreeIterator<T> end() { return IteratedTree<T>::end();}
	TreeIterator<T> push(T data)
	{
		if(Tree<T>::root==NULL) return TreeIterator<T>(Tree<T>::push(data));
		Node<T>* newElem = new Node<T>(data);
		if (data > Tree<T>::root->getValue())
			newElem->setLeft(Tree<T>::root);
		else
			newElem->setRight(Tree<T>::root);
		Tree<T>::root->setParent(newElem);
		Tree<T>::root = newElem;
		return TreeIterator<T>(Tree<T>::root);
	}
	TreeIterator<T> find(T data)
	{
		if (Tree<T>::root == NULL) return NULL;
		Node<T>* found = Tree<T>::find(data);
		if (found == NULL) return TreeIterator<T>();
		found = Tree<T>::remove(found);
		if (data > Tree<T>::root->getValue())
			found->setLeft(Tree<T>::root);
		else
			found->setRight(Tree<T>::root);
		Tree<T>::root->setParent(found);
		Tree<T>::root = found;
		return TreeIterator<T>(Tree<T>::root);
	}
};

template<class T> ostream& operator<< (ostream& stream, IteratedMultiKeyTree<T>& N)
{
	for (TreeIterator<vector<T>> i = N.begin(); i != N.end(); ++i)
		stream << (*i).getData() << '\n';
	if (N.end() != NULL)
		stream << (*N.end()).getData();
	return stream;
}

int main()
{
	IteratedTree<double> tree; //Сбалансированное дерево поиска, работающее на итераторах
	for (int i = 14; i >= 0; i--) { tree.Push(i); }
	void (*f_ptr)(Node<double>*); f_ptr = print;
	cout << "\n-----\nPreorder:";
	tree.PreOrder(tree.getRoot(), f_ptr);
	cout << "\n-----\nInorder:";
	tree.InOrder(tree.getRoot(), f_ptr);
	cout << "\n-----\nPostorder:";
	tree.PostOrder(tree.getRoot(), f_ptr);
	cout << "\n\nInorder standart output with use of iterators:\n" << tree;
	cout << "\n\nSearch for element 11:" << *(tree.find(11));
	cout << "\n\nRemoving element 7:" << (*tree.remove(7)); 
	cout << "\n\nInorder standart output with use of iterators:\n" << tree;

	
	IteratedMultiKeyTree<SportTeam> T;
	T.Push(SportTeam("Lions", "City1", 10, 5, 3, 30));
	T.Push(SportTeam("Tigers", "City2", 8, 7, 2, 29));
	T.Push(SportTeam("Bears", "City3", 12, 3, 2, 35));
	T.Push(SportTeam("Eagles", "City4", 8, 7, 2, 29));
	T.Push(SportTeam("Wolves", "City5", 10, 5, 3, 30));
	T.Push(SportTeam("Dolphins", "City6", 6, 6, 6, 24));
	T.Push(SportTeam("Panthers", "City7", 10, 5, 3, 30));
	T.Push(SportTeam("Panthers", "City7", 10, 5, 3, 30));
	T.Push(SportTeam("Panthers", "City7", 9, 4, 6, 20));

	cout << "\n\nSearch tree with matching keys\n" << T;
	return 0;
}
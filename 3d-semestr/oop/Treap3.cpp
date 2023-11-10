// Treap3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <math.h>
#include <time.h>

using namespace std;

//класс узел
class Node
{
private:
	//закрытые переменные Node N; N.key = 10 вызовет ошибку
	int key;
	int priority;

	int size;
	int node_function;
	int promise;

	//не можем хранить Node, но имеем право хранить указатель
	Node* left;
	Node* right;
	//Node* parent;
public:
	//доступные извне переменные и функции
	int getKey() { return key; }
	int getPriority() { return priority; }
	int getFunction() { return node_function + promise; }
	int getSize() { return size; }
	int getPromise() { return promise; }

	Node* getLeft() { return left; }
	Node* getRight() { return right; }
	//Node* getParent() { return parent; }

	void setLeft(Node* N) { left = N; }
	void setRight(Node* N) { right = N; }
	void setFunction(int value)
	{
		node_function = value;
	}
	void RecalculateFunction()
	{
		node_function = (left != NULL ? left->getFunction() : 0) + (right != NULL ? right->getFunction() : 0);
	}
	void setSize(int value) { size = value; }
	void setPromise(int value) { promise = value; }
	void FulfillPromise()
	{
		if (left != NULL) left->setPromise(left->getPromise() + promise);
		if (right != NULL) right->setPromise(right->getPromise() + promise);
		if (promise != 0)
		{
			cout << "\nPromise in the Node " << key << ", priority: " << priority << "\t" << node_function << ", " << promise << " ";
			node_function += promise;
			cout << "\nFulfilled promise in Node " << key << ", priority: " << priority << "\t" << node_function << " ";
		}
		promise = 0;
	}
	//void setParent(Node* N) { parent = N; }

	//конструктор устанавливает стартовые значения для указателей
	Node(int k, int p, int prom)
	{
		key = k;
		priority = p;
		left = right = NULL;
		node_function = key; //0
		size = 1;
		promise = prom;
		//parent = NULL;
	}

	Node(int k)
	{
		key = k;
		srand(time(NULL));
		priority = rand();
		left = right = NULL;
		node_function = key; //0
		size = 1;
		promise = 0;
		//parent = NULL;
	}

	Node()
	{
		left = right = NULL;
		node_function = 0;
		size = 1;
		promise = 0;
		//parent = NULL;
	}

	friend ostream& operator<<(ostream& stream, Node& N)
	{
		stream << N.key << "; " << N.priority << ";\tpromise: " << N.promise << ",\tf: " << N.getFunction() << "\, size: "<<N.size<<"\n";
		if (N.getLeft() != NULL)
			stream << *N.getLeft();
		if (N.getRight() != NULL)
			stream << *N.getRight();
		return stream;
	}
};


//класс дерево
class Treap
{
protected:
	//корень - его достаточно для хранения всего дерева
	Node* root;

	Node* merge(Node* L, Node* R)
	{
		if (L->getPriority() > R->getPriority())
		{
			if (L->getRight() != NULL)
				L->setRight(merge(L->getRight(), R));
			else
				L->setRight(R);
			L->setSize((R != NULL ? R->getSize() : 0) + (L->getLeft() != NULL ? L->getLeft()->getSize() : 0) + 1);
			//L->RecalculateFunction();
			return L;
		}
		else
		{
			if (R->getLeft() != NULL)
			{
				L = merge(L, R->getLeft());
				R->setLeft(NULL);
				R->setSize((L != NULL ? L->getSize() : 0) + 1);
				//R->RecalculateFunction();
				L = merge(L, R);
			}
			else
			{
				R->setLeft(L);
				R->setSize((L != NULL ? L->getSize() : 0) + (R->getRight() != NULL ? R->getSize() : 0) + 1);
				//R->RecalculateFunction();
				L = R;
			}
			return L;
		}
	}

	Node* merge_in_split(Node* L, Node* R)
	{
		Node* corrected_left = L; Node* corrected_right = R;
		bool bRootIsMoved = false;

		if (L->getKey() > R->getKey())
		{
			corrected_right = L;
			corrected_left = R;
			bRootIsMoved = true;
		}
		if (corrected_left->getPriority() > corrected_right->getPriority())
		{
			if (corrected_left->getRight() != NULL)
				corrected_left->setRight(merge_in_split(corrected_left->getRight(), corrected_right));
			else
				corrected_left->setRight(corrected_right);
			corrected_left->setSize((corrected_right != NULL ? corrected_right->getSize() : 0) + (corrected_left->getLeft() != NULL ? corrected_right->getLeft()->getSize() : 0) + 1);
			//corrected_left->RecalculateFunction();
			return corrected_left;
		}
		else
		{
			if (corrected_right->getLeft() != NULL)
			{
				corrected_left = merge_in_split(corrected_left, corrected_right->getLeft());
				corrected_right->setLeft(NULL);
				corrected_right->setSize((corrected_right->getRight() != NULL ? corrected_right->getRight()->getSize() : 0) + 1);
				//corrected_right->RecalculateFunction();
				corrected_left = merge_in_split(corrected_left, corrected_right);
			}
			else
			{
				corrected_right->setLeft(corrected_left);
				corrected_right->setSize((corrected_left != NULL ? corrected_left->getSize() : 0) + (corrected_right->getRight() != NULL ? corrected_right->getSize() : 0) + 1);
				//corrected_right->RecalculateFunction();
				corrected_left = corrected_right;
			}
			return corrected_left;
		}
	}

	Node* split(Node* Current, int x, Treap* Left, Treap* Right)
	{
		if (Current == NULL)
		{
			//Left = NULL;
			//Right = NULL;
			return NULL;
		}
		if (Current->getKey() == x)
		{
			Node* Res = Current;
			Left->Merge(Current->getLeft());
			Right->Merge(Current->getRight());
			Res->setLeft(NULL);
			Res->setRight(NULL);
			Res->setSize(1);
			Res->setPromise(0);
			Res->setFunction(0);
			//Res->setParent(NULL);
			return Res;
		}
		//Treap* newTreap;
		Current->FulfillPromise();
		if (Current->getKey() < x)
		{
			Left->MergeInSplit(Current->getKey(), Current->getPriority(), Current->getPromise());
			Left->MergeInSplit(Current->getLeft());
			Left->root->FulfillPromise();
			Left->root->setSize((Left->root->getLeft() != NULL ? Left->root->getLeft()->getSize() : 0) + (Left->root->getRight() != NULL ? Left->root->getRight()->getSize() : 0) + 1);
			Left->root->RecalculateFunction();
			return split(Current->getRight(), x, Left, Right);

			/*newTreap = split(Current->getRight(), x, Left, Right, Res);

			if (newTreap != NULL) Right = newTreap;
			return new Treap(Current, new Treap(Current->getLeft()), Right);*/
			//Treap* newRight = new Treap(Current, Left, Right);
			//Right = newRight;
			//return Right;
		}
		else
		{
			Right->MergeInSplit(Current->getKey(), Current->getPriority(), Current->getPromise());
			Right->MergeInSplit(Current->getRight());
			Right->root->FulfillPromise();
			Right->root->setSize((Right->root->getLeft() != NULL ? Right->root->getLeft()->getSize() : 0) + (Right->root->getRight() != NULL ? Right->root->getRight()->getSize() : 0) + 1);
			Right->root->RecalculateFunction();
			return split(Current->getLeft(), x, Left, Right);

			/*newTreap = split(Current->getLeft(), x, Left, Right, Res);
			if (newTreap != NULL) Left = newTreap;
			return new Treap(Current, Left, new Treap(Current->getRight()));*/

			//Treap* newLeft = new Treap(Current, Left, Right);
			//Left = newLeft;
			//return Left;
		}
	}
public:
	//доступ к корневому элементу
	Node* getRoot() { return root; }

	//конструктор дерева: в момент создания дерева ни одного узла нет, корень смотрит в никуда
	Treap() { root = NULL; }
	Treap(int root_key, int root_prior, Treap* Left, Treap* Right)
	{
		root = new Node(root_key, root_prior, 0);
		root->setLeft(Left->root);
		root->setRight(Right->root);
		//root->RecalculateFunction();
		root->setSize((root != NULL ? root->getLeft()->getSize() : 0) + (root->getRight() != NULL ? root->getRight()->getSize() : 0) + 1);
	}
	Treap(Node* N, Treap* Left, Treap* Right)
	{
		root = N;
		root->setLeft(Left->root);
		root->setRight(Right->root);
		//root->RecalculateFunction();
		root->setSize((root != NULL ? root->getLeft()->getSize() : 0) + (root->getRight() != NULL ? root->getRight()->getSize() : 0) + 1);
	}
	Treap(Node* N)
	{
		root = N;
	}

	void PreOrder(Node* N, void (*f)(Node*))
	{
		if (N != NULL)
			f(N);
		if (N != NULL && N->getLeft() != NULL)
			PreOrder(N->getLeft(), f);
		if (N != NULL && N->getRight() != NULL)
			PreOrder(N->getRight(), f);
	}

	void Merge(Node* N)
	{
		Treap* R = new Treap();
		R->root = N;
		Merge(R);
	}

	void Merge(int key, int prior, int prom)
	{
		Node* N = new Node(key, prior, prom);
		Merge(N);
	}

	void MergeInSplit(Node* N)
	{
		Treap* R = new Treap();
		R->root = N;
		MergeInSplit(R);
	}

	void MergeInSplit(int key, int prior, int prom)
	{
		Node* N = new Node(key, prior, prom);
		MergeInSplit(N);
	}

	void Merge(Treap* R)
	{
		if (root == NULL) { root = R->root; return; }
		if (R->root == NULL) return; //this;

		if (root->getPriority() > R->root->getPriority())
		{
			if (root->getRight() != NULL)
				root->setRight(merge(root->getRight(), R->root));
			else
				root->setRight(R->root);
			root->setSize((root->getLeft() != NULL ? root->getLeft()->getSize() : 0) + (root->getRight() != NULL ? root->getRight()->getSize() : 0) + 1);
			//root->RecalculateFunction();
			//return new Treap(L.x, L.y, L.Left, newR);
		}
		else
		{
			if (R->root->getLeft() != NULL)
			{
				//root = merge(root, R->root->getLeft());
				root = merge(root, R->root->getLeft());
				R->root->setLeft(root);
				R->root->setSize((R->root->getLeft() != NULL ? R->root->getLeft()->getSize() : 0) + (R->root->getRight() != NULL ? R->root->getRight()->getSize() : 0) + 1);
				//R->root->RecalculateFunction();
				root = R->root;
			}
			else
			{
				R->root->setLeft(root);
				R->root->setSize((R->root->getLeft() != NULL ? R->root->getLeft()->getSize() : 0) + (R->root->getRight() != NULL ? R->root->getRight()->getSize() : 0) + 1);
				//R->root->RecalculateFunction();
				root = R->root;
			}
			//return new Treap(R.x, R.y, newL, R.Right);
		}
	}

	void MergeInSplit(Treap* R)
	{
		if (root == NULL) { root = R->root; return; }
		if (R->root == NULL) return; //this;

		Node* corrected_right_root = R->root;
		Node* corrected_root = root;
		bool bRootIsMoved = false;

		if (root->getKey() > R->root->getKey())
		{
			corrected_right_root = root;
			corrected_root = R->root;
			bRootIsMoved = true;
		}
		if (corrected_root->getPriority() > corrected_right_root->getPriority())
		{
			if (corrected_root->getRight() != NULL)
				corrected_root->setRight(merge_in_split(corrected_root->getRight(), corrected_right_root));
			else
				corrected_root->setRight(corrected_right_root);
			corrected_root->setSize((corrected_root->getLeft() != NULL ? corrected_root->getRight()->getSize() : 0) + (corrected_root->getRight() != NULL ? corrected_root->getRight()->getSize() : 0) + 1);
			//corrected_root->RecalculateFunction();
			//return new Treap(L.x, L.y, L.Left, newR);
		}
		else
		{
			if (corrected_right_root->getLeft() != NULL)
			{
				//root = merge(root, R->root->getLeft());
				corrected_root = merge_in_split(corrected_root, corrected_right_root->getLeft());
				corrected_right_root->setLeft(corrected_root);
				corrected_right_root->setSize((corrected_right_root->getLeft() != NULL ? corrected_right_root->getLeft()->getSize() : 0) + (corrected_right_root->getRight() != NULL ? corrected_right_root->getRight()->getSize() : 0) + 1);
				//corrected_right_root->RecalculateFunction();
				corrected_root = corrected_right_root;

			}
			else
			{
				corrected_right_root->setLeft(corrected_root);
				corrected_right_root->setSize((corrected_right_root->getLeft() != NULL ? corrected_right_root->getLeft()->getSize() : 0) + (corrected_right_root->getRight() != NULL ? corrected_right_root->getRight()->getSize() : 0) + 1);
				//corrected_right_root->RecalculateFunction();
				corrected_root = corrected_right_root;
			}
			//return new Treap(R.x, R.y, newL, R.Right);
		}
		root = corrected_root;
	}

	Node* Split(int x, Treap* Left, Treap* Right)
	{
		Node* Res = NULL;
		Res = split(root, x, Left, Right);
		return Res;
	}


	friend ostream& operator<<(ostream& stream, Treap& T)
	{
		if (T.root != NULL)
			stream << *T.root;
		return stream;
	}

	void insert(int key, int prior = -1)
	{
		Node* N;
		if (prior == -1) N = new Node(key);
		else N = new Node(key, prior, 0);

		Treap* L = new Treap;
		Treap* R = new Treap;
		Split(key, L, R);
		if (L == NULL && R == NULL)
		{
			root = N; return;
		}

		Treap* KeyTreap = new Treap(); KeyTreap->Merge(N);
		if (L != NULL)
		{
			L->Merge(KeyTreap); L->Merge(R);
			root = L->root;
		}
		else
		{
			R->Merge(KeyTreap); root = R->root;
		}
	}

	Node* Remove(int key)
	{
		Treap L, R;
		Node* Res = Split(key, &L, &R);
		L.Merge(&R);
		*this = L;
		return Res;
	}

	void MultipleAdd(int add, int left = -1, int right = -1)
	{
		if (left == -1 && right == -1)
		{
			root->setPromise(add);
			return;
		}
		Treap L, R;
		Node* Res1 = Split(left, &L, &R); //Res1 = Treap(left)
		Treap RL, RR;
		Node* Res2 = R.Split(right, &RL, &RR); //Res2 = Treap(right)
		//RL: (left, right), RR: >right, L: <left
		Treap Middle;
		Middle.Merge(Res1);
		Middle.Merge(&RL);
		Middle.Merge(Res2);
		Middle.root->setPromise(add);
		L.Merge(&Middle);
		L.Merge(&RR);
		*this = L;
	}
};

void print(Node* N) { cout << N->getKey() << "; " << N->getPriority(); }

class ImplicitTreap : protected Treap
{
protected:
	Node* split(Node* Current, int x, ImplicitTreap* Left, ImplicitTreap* Right)
	{
		if (Current == NULL)
			return NULL;
		if (Current->getSize() == x)
		{
			Node* Res = Current;
			Left->Merge(Current->getLeft());
			//Right->Merge(Current->getRight());
			Left->Merge(Current->getRight()); //?
			Res->setLeft(NULL);
			Res->setRight(NULL);
			Res->setSize(1);
			Res->setPromise(0);
			Res->setFunction(0);
			return Res;
		}
		Current->FulfillPromise();
		if (Current->getSize() < x)
		{
			int left_size = (Current->getLeft()!=NULL? Current->getLeft()->getSize(): 0) + 1;
			Left->Merge(Current->getPriority(), Current->getPromise());
			Left->Merge(Current->getLeft());
			Left->root->FulfillPromise();
			Left->root->setSize((Left->root->getLeft() != NULL ? Left->root->getLeft()->getSize() : 0) + (Left->root->getRight() != NULL ? Left->root->getRight()->getSize() : 0) + 1);
			Left->root->RecalculateFunction();
			return split(Current->getRight(), x-left_size, Left, Right);
		}
		else
		{
			Right->Merge(Current->getPriority(), Current->getPromise());
			Right->Merge(Current->getRight());
			Right->root->FulfillPromise();
			Right->root->setSize((Right->root->getLeft() != NULL ? Right->root->getLeft()->getSize() : 0) + (Right->root->getRight() != NULL ? Right->root->getRight()->getSize() : 0) + 1);
			Right->root->RecalculateFunction();
			return split(Current->getLeft(), x, Left, Right);
		}
	}
public:
	ImplicitTreap() : Treap() {}
	void Merge(ImplicitTreap* R) { Treap::Merge(R); }
	void Merge(Node* N) { ImplicitTreap* newTreap = new ImplicitTreap(); newTreap->root = N; Merge(newTreap); }
	void Merge(int priority, int promise) { Node* N = new Node(0, priority, promise); Merge(N); }
	Node* Split(int x, ImplicitTreap* Left, ImplicitTreap* Right)
	{
		return split(root, x, Left, Right);
	}

	void insert(int index, int priority, int promise = 0)
	{
		Node* N;
		N = new Node(0, priority, promise);

		ImplicitTreap* L = new ImplicitTreap;
		ImplicitTreap* R = new ImplicitTreap;
		Split(index, L, R);
		if (L == NULL && R == NULL)
		{
			root = N; return;
		}

		ImplicitTreap* KeyTreap = new ImplicitTreap(); KeyTreap->Merge(N);
		if (L != NULL)
		{
			L->Merge(KeyTreap); L->Merge(R);
			root = L->root;
		}
		else
		{
			R->Merge(KeyTreap); root = R->root;
		}
	}

	Node* Remove(int index)
	{
		ImplicitTreap L, R;
		Node* Res = Split(index, &L, &R);
		L.Merge(&R);
		*this = L;
		return Res;
	}
	friend ostream& operator<<(ostream& stream, ImplicitTreap& T);
};

ostream& operator<<(ostream& stream, ImplicitTreap& T)
{
	if (T.root != NULL)
		stream << *T.root;
	return stream;
}

int main()
{
	ImplicitTreap T, TR;
	int arr[15];
	int i = 0;

	//заполнение дерева
	for (i = 0; i < 15; i++) arr[i] = (int)(100 * sin(15 * double(i)));
	for (i = 0; i < 5; i++)
	{
		T.Merge(arr[i], 0);
	}
	for (i = 5; i < 15; i++)
	{
		TR.Merge(arr[i], 0);
	}
	cout << T << "\n\n\n";
	T.Merge(&TR);
	//cout << "inserted";
	cout << T;
	ImplicitTreap* T1 = new ImplicitTreap(); ImplicitTreap* T2 = new ImplicitTreap();
	Node* N = T.Split(5, T1, T2);
	cout << "\n---\n" << *T1 << "\n---\n" << *T2;
	if (N != NULL)
		cout << "\nNode: " << *N;
	T1->Merge(T2);
	cout << "\n---\nBack merging without N:\n";
	cout << *T1;
	cout << "\n---\nInserting N:\n";
	T1->insert(5, N->getPriority()); cout << *T1;
	cout << "\n---\nRemoving N:\n";
	T1->Remove(N->getKey());
	if (N != NULL)
		cout << "\nNode: " << *N;
	cout << "\n" << *T1;

	/*Treap T, TR;
	int arr[15];
	int i = 0;

	//заполнение дерева
	for (i = 0; i < 15; i++) arr[i] = (int)(100 * sin(15 * double(i)));
	for (i = 0; i < 5; i++)
	{
		T.Merge(i, arr[i], 0);
	}
	for (i = 5; i < 15; i++)
	{
		TR.Merge(i, arr[i], 0);
	}
	cout << T << "\n\n\n";
	T.Merge(&TR);
	//cout << "inserted";
	cout << T;
	Treap* T1 = new Treap(); Treap* T2 = new Treap();
	Node* N = T.Split(5, T1, T2);
	cout << "\n---\n" << *T1 << "\n---\n" << *T2;
	if (N != NULL)
		cout << "\nNode: " << *N;
	T1->Merge(T2);
	cout << "\n---\nBack merging without N:\n";
	cout << *T1;
	cout << "\n---\nInserting N:\n";
	T1->insert(5, N->getPriority()); cout << *T1;
	cout << "\n---\nRemoving N:\n";
	T1->Remove(N->getKey());
	if (N != NULL)
		cout << "\nNode: " << *N;
	cout << "\n" << *T1;*/

	char c; cin >> c;
	return 0;
}
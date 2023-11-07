#include <iostream>
#include <cstdlib>

using namespace std;

template <class T>
class TreapNode
{
public:
    int priority;
    T data;
    T SumTreeData;
    int size;
    TreapNode* left;
    TreapNode* right;
    T Add;

    TreapNode(T dt = 0, T ad = 0) : priority(rand()), data(dt), SumTreeData(dt), 
                                                    size(1), left(nullptr), right(nullptr), Add(ad) {}
    // Конструктор копирования
    TreapNode(const TreapNode& other) :
        priority(other.priority), data(other.data), SumTreeData(other.SumTreeData), size(other.size),
        left(other.left), right(other.right), Add(other.Add) {}
    
    void recalc() 
    {
        size = 1 + SizeOf(left) + SizeOf(right);
        SumTreeData = data + CostOf(left) + CostOf(right) + Add*size;
    }

    T CostOf(TreapNode<T>* treap) { return treap == nullptr ? 0 : treap->SumTreeData + treap->Add; }
    int SizeOf(TreapNode<T>* treap) { return treap == nullptr ? 0 : treap->size; }


    template<class T1> friend ostream& operator<< (ostream& stream, const TreapNode<T1>& N);

};


template<class T>
ostream& operator<< (ostream& stream, const TreapNode<T>& N)
{
	stream << N.data;
	return stream;
}

template <class T>
class Treap 
{
public:
    TreapNode<T>* root;

    TreapNode<T>* merge(TreapNode<T>* left, TreapNode<T>* right) 
    {
        if (left == nullptr) return right;
        if (right == nullptr) return left;

        TreapNode<T>* answer;
        if (left->priority > right->priority) 
        {
            left->right = merge(left->right, right);
            answer = left;
        }
        else 
        {
            right->left = merge(left, right->left);
            answer = right;
        }

        return answer;
    }

    void split(TreapNode<T>* root, T key, TreapNode<T>*& left, TreapNode<T>*& right)
    {
        if (root == nullptr) 
        {
            left = nullptr;
            right = nullptr;
            return;
        }

        if (root->data <= key) 
        {
            TreapNode<T>* newTree = nullptr;
            split(root->right, key, newTree, right);
            left = new TreapNode<T>(*root); // Создаем новую вершину как копию root
            left->right = newTree;
        }
        else 
        {
            TreapNode<T>* newTree = nullptr;
            split(root->left, key, left, newTree);
            right = new TreapNode<T>(*root); // Создаем новую вершину как копию root
            right->left = newTree;
        }
    }

    Treap() : root(nullptr) {}

    Treap(T* arr, int len)
    {
        Treap<T> temp;
        for (int i = 0; i < len; i++)
            temp.push(arr[i]);
        root = temp.root;
    }

    void insert_at(T data, T pos)
    {
        TreapNode<T>* new_node = new TreapNode<T>(data);
        TreapNode<T>* left;
        TreapNode<T>* right;
        split(root, pos, left, right);

        root = merge(merge(left, new_node), right);
    }

    void push(T data)
    {
        TreapNode<T>* new_node = new TreapNode<T>(data);
        TreapNode<T>* left;
        TreapNode<T>* right;
        split(root, data, left, right);
        root = merge(merge(left, new_node), right);
    }

    void remove(int pos)
    {
        TreapNode<T>* l;
        TreapNode<T>* m;
        TreapNode<T>* r;
        split(root, pos, l, r);
        split(r, 1, m, r);
        root = merge(l, r);
        delete m;
    }


    void inorderTraversal() 
    {
        inorderTraversal(root);
    }

    void inorderTraversal(TreapNode<T>* node, ostream& stream) 
    {
        if (node == nullptr)
        {
            return;
        }

        inorderTraversal(node->left, stream);
        stream << *node << ' ';
        inorderTraversal(node->right, stream);
    }



    TreapNode<T>* search(T key)
    {
        return search(root, key);
    }

    TreapNode<T>* search(TreapNode<T>* node, T key) 
    {
        if (node == nullptr)
        {
            return nullptr;
        }
        if (node->data == key) return node;
        if (search(node->left, key)) return search(node->left, key);
        if (search(node->right, key)) return search(node->right, key);

    }

    template<class T1> friend ostream& operator<< (ostream& stream, Treap<T1>& N);

};

template<class T>
ostream& operator<< (ostream& stream, Treap<T>& N)
{
	N.inorderTraversal(N.root, stream);
	return stream;
}

int main() {
    unsigned random_value = 12;
    srand(random_value);

    Treap<int> treap;

    for (int i = 0; i < 30; i++)
        treap.push(i);
    cout << "\nInorder Traversal:\n" << treap;

    Treap<int> Left;
    Treap<int> Right;
    treap.split(treap.root, 14, Left.root, Right.root);

    cout << "\nInorder Traversal:\n" << Left;
    cout << "\nInorder Traversal:\n" << Right;

    /*treap.insert_at(18, 4);
    cout << "\nInorder Traversal after iserting 18 at pos 4:\n" << treap;

    treap.remove(3);   
    cout << "\nInorder Traversal after removing element from pos 3:\n" << treap;


    TreapNode<int>* found = treap.search(2);
    cout << "\nData 2 found: " << (found ? "Yes" : "No") ;
    found = treap.search(23);
    cout << "\nData 23 found: " << (found ? "Yes" : "No");

    int *a = new int[10]{0, 4, 8, 10, 0, 9, 4, 8, 4, 0};
    Treap<int> newtreap(a, 10);
    cout << "\nInorder Traversal for arraytreap:\n" << newtreap;*/
    return 0;
}
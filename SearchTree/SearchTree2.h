#ifndef SEARCHTREE_H_
#define SEARCHTREE_H_

#include <cstdlib>
#include <ctime>

template <typename Type>
class SearchTree
{
private:
    using unll = unsigned long long;
    struct TreeNode
    {
        Type Item;
        unll Times;
        TreeNode *Left;
        TreeNode *Right;
    };

    typedef TreeNode *Tree;
    typedef TreeNode *Position;

    Tree Root;

    // Account of Node
    unll NodeCount;

    // Account of Element
    unll EleCount;

    SearchTree(const Tree &T) = delete;
    SearchTree &operator=(const Tree &T) = delete;

protected:
    Tree MakeEmpty(Tree &);
    Tree Insert(Tree &, const Type &);
    Position Find(Position, const Type &) const;
    Position FindMax(Position) const;
    Position FindMin(Position) const;
    Tree RemoveAll(Tree &, const Type &);
    Tree Remove(Tree &, const Type &);

public:
    SearchTree() : Root(nullptr), NodeCount(0ULL), EleCount(0ULL) {}
    ~SearchTree();

    bool insert(const Type &);
    unll find(const Type &) const;
    unll findMax(Type &) const;
    unll findMin(Type &) const;
    bool removeAll(const Type &);
    bool remove(const Type &);
    bool isEmpty() const;
    const unll eleSize() const;
    const unll nodeSize() const;
};

template <typename Type>
typename SearchTree<Type>::Tree SearchTree<Type>::MakeEmpty(Tree &T)
{
    if (T != nullptr)
    {
        MakeEmpty(T->Left);
        MakeEmpty(T->Right);
        delete T;
    }
    NodeCount = 0;
    EleCount = 0;
    return nullptr;
}

template <typename Type>
SearchTree<Type>::~SearchTree()
{
    Root = MakeEmpty(Root);
    NodeCount = 0ULL;
    EleCount = 0ULL;
}

template <typename Type>
typename SearchTree<Type>::Tree SearchTree<Type>::Insert(Tree &T, const Type &x)
{
    if (T == nullptr)
    {
        T = new TreeNode;
        T->Item = x;
        T->Times = 1;
        T->Left = T->Right = nullptr;
        ++NodeCount;
        ++EleCount;
    }
    else if (x < T->Item)
        T->Left = Insert(T->Left, x);
    else if (x > T->Item)
        T->Right = Insert(T->Right, x);
    else
    {
        ++EleCount;
        ++(T->Times);
    }

    return T;
}

template <typename Type>
typename SearchTree<Type>::Position SearchTree<Type>::Find(Position T, const Type &x) const
{
    if (T != nullptr)
    {
        if (x < T->Item)
            return Find(T->Left, x);
        if (x > T->Item)
            return Find(T->Right, x);
    }
    return T;
}

template <typename Type>
typename SearchTree<Type>::Position SearchTree<Type>::FindMax(Position T) const
{
    if (T != nullptr)
        while (T->Right != nullptr)
            T = T->Right;
    return T;
}

template <typename Type>
typename SearchTree<Type>::Position SearchTree<Type>::FindMin(Position T) const
{
    if (T != nullptr)
        while (T->Left != nullptr)
            T = T->Left;
    return T;
}

template <typename Type>
typename SearchTree<Type>::Tree SearchTree<Type>::RemoveAll(Tree &T, const Type &x)
{
    Tree TempCell = T;
    if (T == nullptr)
        ;
    else if (x < T->Item)
        T->Left = RemoveAll(T->Left, x);
    else if (x > T->Item)
        T->Right = RemoveAll(T->Right, x);
    else if (T->Left != nullptr && T->Right != nullptr)
    {
        if (std::rand() & 1)
        {
            TempCell = FindMin(T->Right);
            T->Times = TempCell->Times;
            T->Item = TempCell->Item;
            T->Right = RemoveAll(T->Right, T->Item);
        }
        else
        {
            TempCell = FindMax(T->Left);
            T->Times = TempCell->Times;
            T->Item = TempCell->Item;
            T->Left = RemoveAll(T->Left, T->Item);
        }
    }
    else
    {
        TempCell = T;
        if (T->Left == nullptr)
            T = T->Right;
        else if (T->Right == nullptr)
            T = T->Left;
        EleCount -= TempCell->Times;
        delete TempCell;
        --NodeCount;
    }
    return T;
}

template <typename Type>
typename SearchTree<Type>::Tree SearchTree<Type>::Remove(Tree &T, const Type &x)
{
    Tree TempCell = T;
    if (T == nullptr)
        ;
    else if (x < T->Item)
        T->Left = Remove(T->Left, x);
    else if (x > T->Item)
        T->Right = Remove(T->Right, x);
    else if (T->Times == 1)
    {
        if (T->Left != nullptr && T->Right != nullptr)
        {
            TempCell = FindMin(T->Right);
            T->Item = TempCell->Item;
            T->Times = TempCell->Times;
            T->Right = Remove(T->Right, T->Item);
        }
        else
        {
            TempCell = T;
            if (T->Left == nullptr)
                T = T->Right;
            else if (T->Right == nullptr)
                T = T->Left;
            EleCount -= TempCell->Times;
            delete TempCell;
            --NodeCount;
        }
    }
    else
    {
        --EleCount;
        --(T->Times);
    }

    return T;
}

template <typename Type>
bool SearchTree<Type>::insert(const Type &x)
{
    Tree temp = Insert(Root, x);
    if (temp == nullptr)
        return false;

    Root = temp;
    return true;
}

template <typename Type>
typename SearchTree<Type>::unll SearchTree<Type>::find(const Type &x) const
{
    Position temp = Find(Root, x);

    if (temp == nullptr)
        return 0ULL;
    return temp->Times;
}

template <typename Type>
typename SearchTree<Type>::unll SearchTree<Type>::findMax(Type &x) const
{
    Position temp = FindMax(Root);
    if (temp == nullptr)
        return 0ULL;

    x = temp->Item;
    return temp->Times;
}

template <typename Type>
typename SearchTree<Type>::unll SearchTree<Type>::findMin(Type &x) const
{
    Position temp = FindMin(Root);
    if (temp == nullptr)
        return 0ULL;

    x = temp->Item;
    return temp->Times;
}

template <typename Type>
bool SearchTree<Type>::removeAll(const Type &x)
{
    Position temp = RemoveAll(Root, x);
    return temp != nullptr;
}

template <typename Type>
bool SearchTree<Type>::remove(const Type &x)
{
    Position temp = Remove(Root, x);
    if (temp != nullptr)
        Root = temp;
    return temp != nullptr;
}

template <typename Type>
bool SearchTree<Type>::isEmpty() const
{
    return NodeCount == 0;
}

template <typename Type>
const typename SearchTree<Type>::unll SearchTree<Type>::nodeSize() const
{
    return NodeCount;
}

template <typename Type>
const typename SearchTree<Type>::unll SearchTree<Type>::eleSize() const
{
    return EleCount;
}

#endif
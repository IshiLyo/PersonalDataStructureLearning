#ifndef SEARCHTREE_H_
#define SEARCHTREE_H_

template <typename Type>
class SearchTree
{
private:
    struct TreeNode
    {
        Type Item;
        TreeNode *Left;
        TreeNode *Right;
    };

    typedef TreeNode *Tree;
    typedef TreeNode *Position;

    Tree Root;

    unsigned long long size;

    SearchTree(const Tree &T) = delete;
    SearchTree &operator=(const Tree &T) = delete;

protected:
    Tree MakeEmpty(Tree &);
    Tree Insert(Tree &, const Type &);
    Position Find(Position, const Type &) const;
    Position FindMax(Position) const;
    Position FindMin(Position) const;
    Tree DeleteAll(Tree &, const Type &);
    Tree Delete(Tree &, const Type &);

public:
    SearchTree() : Root(nullptr), size(0ULL) {}
    ~SearchTree();

    bool insert(const Type &);
    bool find(const Type &) const;
    bool findMax(Type &) const;
    bool findMin(Type &) const;
    bool delAll(const Type &);
    bool del(const Type &);
    bool isEmpty() const;
    const unsigned long long showSize() const;
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
    size = 0;
    return nullptr;
}

template <typename Type>
SearchTree<Type>::~SearchTree()
{
    Root = MakeEmpty(Root);
    size = 0ULL;
}

template <typename Type>
typename SearchTree<Type>::Tree SearchTree<Type>::Insert(Tree &T, const Type &x)
{
    if (T == nullptr)
    {
        T = new TreeNode;
        T->Item = x;
        T->Left = T->Right = nullptr;
        ++size;
    }
    else if (x < T->Item)
        T->Left = Insert(T->Left, x);
    else if (x > T->Item)
        T->Right = Insert(T->Right, x);

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
typename SearchTree<Type>::Tree SearchTree<Type>::DeleteAll(Tree &T, const Type &x)
{
    Tree TempCell = T;
    if (T == nullptr)
        ;
    else if (x < T->Item)
        T->Left = DeleteAll(T->Left, x);
    else if (x > T->Item)
        T->Right = DeleteAll(T->Right, x);
    else if (T->Left != nullptr && T->Right != nullptr)
    {
        TempCell = FindMin(T->Right);
        T->Item = TempCell->Item;
        T->Right = DeleteAll(T->Right, T->Item);
    }
    else
    {
        TempCell = T;
        if (T->Left == nullptr)
            T = T->Right;
        else if (T->Right == nullptr)
            T = T->Left;
        delete TempCell;
        --size;
    }
    return T;
}

// template <typename Type>
// typename SearchTree<Type>::Tree Delete(Tree &T, const Type &x)
// {
//     Tree TempCell = T;
//     if (T == nullptr)
//         ;
//     else if (x < T->Item)
//         T->Left = DeleteAll(T->Left, x);
//     else if (x > T->Item)
//         T->Right = DeleteAll(T->Right, x);
//     else if (T->Left != nullptr && T->Right != nullptr)
//     {
//         TempCell = FindMin(T->Right);
//         T->Item = TempCell->Item;
//         T->Right = DeleteAll(T->Right, T->Item);
//     }
//     else
//     {
//         TempCell = T;
//         if (T->Left == nullptr)
//             T = T->Right;
//         else if (T->Right == nullptr)
//             T = T->Left;
//         delete TempCell;
//         --size;
//     }
//     return T;
// }

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
bool SearchTree<Type>::find(const Type &x) const
{
    Position temp = Find(Root, x);
    return temp != nullptr;
}

template <typename Type>
bool SearchTree<Type>::findMax(Type &x) const
{
    Position temp = FindMax(Root);
    if (temp == nullptr)
        return false;

    x = temp->Item;
    return true;
}

template <typename Type>
bool SearchTree<Type>::findMin(Type &x) const
{
    Position temp = FindMin(Root);
    if (temp == nullptr)
        return false;

    x = temp->Item;
    return true;
}

template <typename Type>
bool SearchTree<Type>::delAll(const Type &x)
{
    Position temp = DeleteAll(Root, x);
    return temp != nullptr;
}

template <typename Type>
bool SearchTree<Type>::del(const Type &x)
{
    Position temp = Delete(Root, x);
    return temp != nullptr;
}

template <typename Type>
bool SearchTree<Type>::isEmpty() const
{
    return size == 0;
}

template <typename Type>
const unsigned long long SearchTree<Type>::showSize() const
{
    return size;
}

#endif
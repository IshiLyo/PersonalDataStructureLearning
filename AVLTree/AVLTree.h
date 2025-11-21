#ifndef AVLTREE_H_
#define AVLTREE_H_

#include <algorithm>
#include <iostream>

template <typename Type>
class AVLTree
{
private:
    struct TreeNode
    {
        Type Item;
        unsigned int Times;
        int Height;
        TreeNode *Left;
        TreeNode *Right;
    };

    using Position = TreeNode *;
    using Tree = TreeNode *;

    Tree Root;

    unsigned long long NodeCount;
    unsigned long long ElementCount;

protected:
    Tree MakeEmpty(Tree &);
    int Height(Position) const;
    Position SingleRotateWithLeft(Position);
    Position SingleRotateWithRight(Position);
    Position DoubleRotateWithLeft(Position);
    Position DoubleRotateWithRight(Position);
    Tree Insert(Tree &, const Type &);
    Position Find(Tree, const Type &) const;
    Position FindMax(Tree) const;
    Position FindMin(Tree) const;
    // Tree Remove(Tree &, const Type &);
    // Tree RemoveAll(Tree &, const Type &);
    void Show(Tree) const;

public:
    AVLTree();
    ~AVLTree();

    bool insert(const Type &);
    unsigned int find(const Type &) const;
    unsigned int findMax(Type &) const;
    unsigned int findMin(Type &) const;
    // bool remove(const Type &);
    // bool removeAll(const Type &);
    unsigned long long nodeSize() const { return NodeCount; }
    unsigned long long eleSize() const { return ElementCount; }
    bool isEmpty() const { return NodeCount == 0; }
    void show() const;
};

template <typename Type>
AVLTree<Type>::AVLTree() : Root(nullptr), NodeCount(0ULL), ElementCount(0ULL)
{
}

template <typename Type>
typename AVLTree<Type>::Tree AVLTree<Type>::MakeEmpty(Tree &T)
{
    if (T != nullptr)
    {
        if (T->Left != nullptr)
            T->Left = MakeEmpty(T->Left);
        if (T->Right != nullptr)
            T->Right = MakeEmpty(T->Right);
        delete T;
        T = nullptr;
    }
    return T;
}

template <typename Type>
AVLTree<Type>::~AVLTree()
{
    Root = MakeEmpty(Root);
    NodeCount = 0ULL;
    ElementCount = 0ULL;
}

template <typename Type>
int AVLTree<Type>::Height(Position P) const
{
    if (P == nullptr)
        return -1;
    return P->Height;
}

template <typename Type>
typename AVLTree<Type>::Position AVLTree<Type>::SingleRotateWithLeft(Position K2)
{
    Position K1;

    K1 = K2->Left;
    K2->Left = K1->Right;
    K1->Right = K2;

    K2->Height = std::max(Height(K2->Left), Height(K2->Right)) + 1;
    K1->Height = std::max(Height(K1->Left), Height(K1->Right)) + 1;

    return K1;
}

template <typename Type>
typename AVLTree<Type>::Position AVLTree<Type>::SingleRotateWithRight(Position K1)
{
    Position K2;

    K2 = K1->Right;
    K1->Right = K2->Left;
    K2->Left = K1;

    K1->Height = std::max(Height(K1->Left), Height(K1->Right)) + 1;
    K2->Height = std::max(Height(K2->Left), Height(K2->Right)) + 1;

    return K2;
}

template <typename Type>
typename AVLTree<Type>::Position AVLTree<Type>::DoubleRotateWithLeft(Position K3)
{
    K3->Left = SingleRotateWithRight(K3->Left);

    return SingleRotateWithLeft(K3);
}

template <typename Type>
typename AVLTree<Type>::Position AVLTree<Type>::DoubleRotateWithRight(Position K3)
{
    K3->Right = SingleRotateWithLeft(K3->Right);

    return SingleRotateWithRight(K3);
}

// 还没写new未成功分配内存的逻辑
template <typename Type>
typename AVLTree<Type>::Tree AVLTree<Type>::Insert(Tree &T, const Type &x)
{
    if (T == nullptr)
    {
        T = new TreeNode;
        T->Item = x;
        T->Times = 1;
        T->Height = 0;
        T->Left = T->Right = nullptr;
        ++ElementCount;
        ++NodeCount;
    }
    else if (x < T->Item)
    {
        T->Left = Insert(T->Left, x);
        if (Height(T->Left) - Height(T->Right) == 2)
            if (x < T->Left->Item)
                T = SingleRotateWithLeft(T);
            else
                T = DoubleRotateWithLeft(T);
    }
    else if (x > T->Item)
    {
        T->Right = Insert(T->Right, x);
        if (Height(T->Right) - Height(T->Left) == 2)
            if (x > T->Right->Item)
                T = SingleRotateWithRight(T);
            else
                T = DoubleRotateWithRight(T);
    }
    else
    {
        // 使自身数量 + 1，同时使ElementCount + 1
        ++(T->Times);
        ++ElementCount;
    }

    T->Height = std::max(Height(T->Left), Height(T->Right)) + 1;

    return T;
}

template <typename Type>
typename AVLTree<Type>::Position AVLTree<Type>::Find(Tree T, const Type &x) const
{
    if (T != nullptr)
    {
        if (x < T->Item)
            T = Find(T->Left, x);
        else if (x > T->Item)
            T = Find(T->Right, x);
    }

    return T;
}

template <typename Type>
typename AVLTree<Type>::Position AVLTree<Type>::FindMax(Tree T) const
{
    if (T == nullptr)
        return nullptr;
    while (T->Right != nullptr)
        T = T->Right;
    return T;
}

template <typename Type>
typename AVLTree<Type>::Position AVLTree<Type>::FindMin(Tree T) const
{
    if (T == nullptr)
        return nullptr;
    while (T->Left != nullptr)
        T = T->Left;
    return T;
}

// 如果 x 不在树中，将返回 nullptr
// 如果在树中，则根节点的位置，同时根节点也被修改
// template <typename Type>
// typename AVLTree<Type>::Tree AVLTree<Type>::Remove(Tree &T, const Type &x)
// {
//     Tree TempCell = T;
//     if (T == nullptr)
//         return nullptr;
//     else if (x < T->Item)
//         T->Left = Remove(T->Left, x);
//     else if (x > T->Item)
//         T->Right = Remove(T->Right, x);
//     else if (T->Times == 1)
//     {
//         if (T->Left != nullptr && T->Right != nullptr)
//         {
//             if (std::rand() & 1)
//             {
//                 TempCell = FindMin(T->Right);
//                 T->Item = TempCell->Item;
//                 T->Times = TempCell->Times;
//                 T->Right = Remove(T->Right, T->Item);
//             }
//             else
//             {
//                 TempCell = FindMax(T->Left);
//                 T->Item = TempCell->Item;
//                 T->Times = TempCell->Times;
//                 T->Left = Remove(T->Left, T->Item);
//             }
//         }
//         else
//         {
//             TempCell = T;
//             if (T->Left == nullptr)
//                 T = T->Right;
//             else if (T->Right == nullptr)
//                 T = T->Left;
//             delete TempCell;

//             // 因为只有一次
//             --ElementCount;
//             --NodeCount;
//         }
//     }
//     else
//         --(T->Times);
//     return T;
// }

// template <typename Type>
// typename AVLTree<Type>::Tree AVLTree<Type>::RemoveAll(Tree &T, const Type &x)
// {
//     Tree TempCell = T;
//     if (T == nullptr)
//         return nullptr;
//     else if (x < T->Item)
//         T->Left = Remove(T->Left, x);
//     else if (x > T->Item, x)
//         T->Right = Remove(T->Right, x);

//     else if (T->Left != nullptr && T->Right != nullptr)
//     {
//         unsigned long long tempt;
//         if (std::rand() & 1)
//         {
//             TempCell = FindMin(T->Right);
//             T->Item = TempCell->Item;
//             tempt = T->Times;
//             T->Times = TempCell->Times;
//             TempCell->Times = tempt;
//             T->Right = Remove(T->Right, T->Item);
//         }
//         else
//         {
//             TempCell = FindMax(T->Left);
//             T->Item = TempCell->Item;
//             tempt = T->Times;
//             T->Times = TempCell->Times;
//             TempCell->Times = tempt;
//             T->Left = Remove(T->Left, T->Item);
//         }
//     }
//     else
//     {
//         TempCell = T;
//         if (T->Left == nullptr)
//             T = T->Right;
//         else if (T->Right == nullptr)
//             T = T->Left;
//         ElementCount -= T->Times;
//         delete TempCell;
//         --NodeCount;
//     }

//     return T;
// }

template <typename Type>
void AVLTree<Type>::Show(Tree T) const
{
    if (T == nullptr)
        return;
    Show(T->Left);
    for (int i = 0; i < T->Times; i++)
        std::cout << T->Item << ' ';
    Show(T->Right);
}

// 永远不可能为false
template <typename Type>
bool AVLTree<Type>::insert(const Type &x)
{
    Tree temp = Insert(Root, x);

    if (temp == nullptr)
        return false;
    Root = temp;
    return true;
}

template <typename Type>
unsigned int AVLTree<Type>::find(const Type &x) const
{
    Position temp = Find(Root, x);
    if (temp == nullptr)
        return 0;
    return temp->Times;
}

template <typename Type>
unsigned int AVLTree<Type>::findMax(Type &x) const
{
    Position temp = FindMax(Root);
    if (temp == nullptr)
        return 0;
    x = temp->Item;
    return temp->Times;
}

template <typename Type>
unsigned int AVLTree<Type>::findMin(Type &x) const
{
    Position temp = FindMin(Root);
    if (temp == nullptr)
        return 0;
    x = temp->Item;
    return temp->Times;
}

// template <typename Type>
// bool AVLTree<Type>::remove(const Type &x)
// {
//     return Remove(Root, x) != nullptr;
// }

// template <typename Type>
// bool AVLTree<Type>::removeAll(const Type &x)
// {
//     return RemoveAll(Root, x) != nullptr;
// }

template <typename Type>
void AVLTree<Type>::show() const
{
    Show(Root);
}

#endif
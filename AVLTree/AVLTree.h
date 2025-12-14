#ifndef AVLTREE_H_
#define AVLTREE_H_

#include <algorithm>
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <cstdlib>

template <typename Type>
class AVLTree
{
private:
    struct TreeNode
    {
        Type Item;
        unsigned int Times; // 支持重复元素
        int Height;         // AVL平衡所需
        TreeNode *Left, *Right;

        TreeNode() = delete;
        TreeNode(const Type &x) : Item(x), Times(1), Height(0), Left(nullptr), Right(nullptr) {}
        TreeNode(const TreeNode *TN) = delete;
        TreeNode(const TreeNode &TN) : Item(TN.Item), Times(TN.Times), Height(TN.Height), Left(nullptr), Right(nullptr) {}
    };

    using Position = TreeNode *;
    using Tree = TreeNode *;

    Tree Root;
    unsigned long long NodeCount;    // 节点数
    unsigned long long ElementCount; // 元素数（含重复）

    // 辅助函数的辅助函数

    Tree MakeEmpty(Tree);
    int Height(Position) const;
    Tree SingleRotateWithLeft(Position);
    Tree SingleRotateWithRight(Position);
    Tree DoubleRotateWithLeft(Position);
    Tree DoubleRotateWithRight(Position);
    Tree Balance(Position);

    // 辅助函数

    Tree Insert(Tree, const Type &);
    Tree Remove(Tree, const Type &);
    Tree RemoveAll(Tree, const Type &);
    Position Find(Tree, const Type &) const;
    Position FindMin(Tree) const;
    Position FindMax(Tree) const;

public:
    AVLTree() : Root(nullptr), NodeCount(0ULL), ElementCount(0ULL) {}
    AVLTree(const AVLTree &);
    AVLTree(AVLTree &&);
    AVLTree &operator=(const AVLTree &);
    AVLTree &operator=(AVLTree &&);
    ~AVLTree() { MakeEmpty(Root); }

    bool insert(const Type &);
    bool remove(const Type &);
    unsigned int removeAll(const Type &);
    unsigned int find(const Type &) const;
    unsigned int findMin(Type &) const;
    unsigned int findMax(Type &) const;

    bool isEmpty() const;
    unsigned long long nodeCount() const;
    unsigned long long elementCount() const;

    std::vector<Type> preOrder() const;
    std::vector<Type> inOrder() const;
    std::vector<Type> posOrder() const;
    std::vector<Type> preOrderAll() const;
    std::vector<Type> inOrderAll() const;
    std::vector<Type> posOrderAll() const;
    std::vector<Type> bfs() const;
    std::vector<Type> bfsAll() const;
};

template <typename Type>
typename AVLTree<Type>::Tree AVLTree<Type>::MakeEmpty(Tree T)
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

/**
 * @brief 获取节点的高度
 *
 * ##AVL树空节点高度定义为-1，叶子节点高度为0
 * ##这种定义使得：
 * ##1.空树高度为-1，符合递归定义
 * ##2.平衡因子计算直观：Height(Left) - Height(Right)
 * ##3.平衡状态：-1, 0, +1
 * ##4.失衡状态：<= -2 或 >= +2
 *
 * @tparam Type 树中存储的元素类型
 * @param P 要获取高度的节点位置
 * @return int 节点的高度，空节点返回-1
 */
template <typename Type>
int AVLTree<Type>::Height(Position P) const
{
    // 空节点高度定义为-1（AVL树标准定义）
    if (P == nullptr)
        return -1;

    // 返回节点记录的高度值
    return P->Height;
}

/**
 * @brief 执行左左单旋转（右旋转）
 *
 * 用于处理左子树过高导致的不平衡情况
 *
 * 为左旋转的逆操作
 *
 * ##旋转前结构：K2(失衡节点) -> K1(左子节点) -> 左子树A
 * ##旋转后结构：K1(新根节点) -> 左子树A, 右子节点K2
 *
 * @tparam Type 树中存储的元素类型
 * @param K2 失衡节点的位置（最小不平衡子树的根节点）
 * @return Tree 旋转后新的根节点位置K1
 */
template <typename Type>
typename AVLTree<Type>::Tree AVLTree<Type>::SingleRotateWithLeft(Position K2)
{
    Position K1;

    // 保存K2的左子节点K1（将称为新的根节点）
    // 因为是左左失衡，K2->Left 必不为空
    K1 = K2->Left;

    // 将K1的右子树挂接到K2的左子树位置
    // 无所谓 K1->Right 是否为空
    K2->Left = K1->Right;

    // 将K2作为K1的右子节点
    K1->Right = K2;

    // 更新K2的高度（先更新子节点）
    K2->Height = std::max(Height(K2->Left), Height(K2->Right)) + 1;

    // 更新K1的高度（后更新父节点）
    K1->Height = std::max(Height(K1->Left), Height(K1->Right)) + 1;

    return K1; // 返回新的根节点K1
}

/**
 * @brief 执行右右旋转（左旋转）
 *
 * 用于处理右子树过高导致的不平衡情况
 *
 * 为右旋转的逆操作
 *
 * ##旋转前结构：K1（失衡节点）-> K2（右子节点）-> 右子树A
 * ##旋转后结构：K2（新根节点）-> 右子树K1, 左子树A
 *
 * @tparam Type 树中存储的元素类型
 * @param K1 失衡节点的位置（最小不平衡子树的根节点）
 * @return Tree 旋转后新的根节点位置K2
 */
template <typename Type>
typename AVLTree<Type>::Tree AVLTree<Type>::SingleRotateWithRight(Position K1)
{
    Position K2;

    // 保存K1的右子节点K2（将称为新的根节点）
    // 因为是右右失衡，K2->Right 必不为空
    K2 = K1->Right;

    // 将K2的左子树挂接到K1的右子树位置
    // 无所谓 K2->Left 是否为空
    K1->Right = K2->Left;

    // 将K1作为K2的左子节点
    K2->Left = K1;

    // 更新K1的高度（先更新子节点）
    K1->Height = std::max(Height(K1->Left), Height(K1->Right)) + 1;

    // 更新K2的高度（后更新父节点）
    K2->Height = std::max(Height(K2->Left), Height(K2->Right)) + 1;

    return K2; // 返回新的根节点K1
}

/**
 * @brief 执行左右双旋转（先左旋后右旋）
 *
 * 用于处理左子树的右子树过高导致的不平衡情况
 *
 * 为左左旋转和右右旋转的组合操作
 *
 * ##旋转前结构：K3（失衡节点）-> K1（左子节点，右重）-> K2（右子节点）
 * ##旋转后结构：K2（新根节点）-> 左子节点K1, 右子节点K3
 *
 * @tparam Type 树中存储的元素类型
 * @param K3 失衡节点的位置（最小不平衡子树的根节点）
 * @return Tree 旋转后新的根节点位置K2
 */
template <typename Type>
typename AVLTree<Type>::Tree AVLTree<Type>::DoubleRotateWithLeft(Position K3)
{
    // 第一步：对K3的左子节点K1进行右右旋转（左旋）
    // 将K1的右子节点K2提升，修复K1的右重情况
    K3->Left = SingleRotateWithRight(K3->Left);

    // 第二步：对K3进行左左旋转（右旋）
    // 将修复后的左子树提升，修复K3的左重情况
    return SingleRotateWithLeft(K3);
}

/**
 * @brief 执行右左双旋转（先右旋后左旋）
 *
 * 用于处理右子树的左子树过高导致的不平衡情况
 *
 * 为右右旋转和左左旋转的组合操作
 *
 * ##旋转前结构：K1（右重失衡）-> K3（左重）-> K2（平衡因子任意）
 * ##旋转后结构：K2（新根节点）-> 左子节点K1, 右子节点K3
 *
 * @tparam Type 树中存储的元素类型
 * @param K1 失衡节点的位置（最小不平衡子树的根节点）
 * @return Tree 旋转后新的根节点位置K2
 */
template <typename Type>
typename AVLTree<Type>::Tree AVLTree<Type>::DoubleRotateWithRight(Position K1)
{
    // 第一步：对K1的右子节点K3进行左左旋转（右旋）
    // 将K3的左子节点K2提升，修复K3的左重情况
    K1->Right = SingleRotateWithLeft(K1->Right);

    // 第二步：对K1进行右右旋转（左旋）
    // 将修复后的右子树提升，修复K1的右重情况
    return SingleRotateWithRight(K1);
}

/**
 * @brief 平衡以Node为根的子树
 *
 * 根据节点的平衡因子执行相应的旋转操作，恢复AVL树的平衡性质
 * 自动判断需要单旋转还是双旋转，处理四种失衡情况
 *
 * 平衡因子 = 左子树高度 - 右子树高度
 * ##失衡情况：
 * ##1.左左失衡 (平衡因子 > 1 且 左子节点平衡因子 >= 0)：右旋转
 * ##2.左右失衡 (平衡因子 > 1 且 左子节点平衡因子 < 0)：先左旋后右旋
 * ##3.右右失衡 (平衡因子 < -1 且 右子节点平衡因子 <= 0)：左旋转
 * ##4.右左失衡 (平衡因子 < -1 且 右子节点平衡因子 > 0)：先右旋后左旋
 *
 * @tparam Type 树中存储的元素类型
 * @param Node 需要平衡的子树根节点
 * @return Tree 平衡后的新根节点
 */
template <typename Type>
typename AVLTree<Type>::Tree AVLTree<Type>::Balance(Position Node)
{
    if (Node == nullptr)
        return nullptr;

    // 既然Node不为空，那么就必然有平衡因子
    int banlance = Height(Node->Left) - Height(Node->Right);

    if (banlance > 1) // 左重
    {
        // 左重保证Node->Left非空
        int left_balance = Height(Node->Left->Left) - Height(Node->Left->Right);

        // 左左, 孙子树高度一致时, 优先选择单旋转
        if (left_balance >= 0)
            return SingleRotateWithLeft(Node);
        else
            return DoubleRotateWithLeft(Node);
    }
    else if (banlance < -1) // 右重
    {
        int right_balance = Height(Node->Right->Left) - Height(Node->Right->Right);

        if (right_balance <= 0)
            return SingleRotateWithRight(Node);
        else
            return DoubleRotateWithRight(Node);
    }

    // balance处在-1到1之间，不用平衡
    return Node;
}

/**
 * @brief 向AVL树中插入元素
 *
 * 递归地向AVL树中插入元素，维护树的平衡性质
 * 支持重复元素插入，通过Times计数实现
 * 插入后自动更新节点高度并执行平衡操作
 *
 * ##插入情况：
 * ##1.空树情况：创建新节点作为根节点
 * ##2.小于当前节点：递归插入左子树
 * ##3.大于当前节点：递归插入右子树
 * ##4.等于当前节点：增加重复计数
 *
 * @tparam Type 树中存储的元素类型
 * @param T 当前子树根节点
 * @param x 要插入的元素
 * @return Tree 插入并平衡后的新根节点
 */
template <typename Type>
typename AVLTree<Type>::Tree AVLTree<Type>::Insert(Tree T, const Type &x)
{
    // 空树情况：创建新节点
    if (T == nullptr)
    {
        // 内存分配失败将会抛出std::bad_alloc, 然后栈展开
        T = new TreeNode(x);
        ++ElementCount;
        ++NodeCount;
        return T; // 新节点直接返回，无需平衡
    }

    // 插入左子树情况
    if (x < T->Item)
    {
        T->Left = Insert(T->Left, x);

        // 操作之后高度发生改变
        T->Height = std::max(Height(T->Left), Height(T->Right)) + 1;
        T = Balance(T); // 平衡左子树
    }
    // 插入右子树情况
    else if (x > T->Item)
    {
        T->Right = Insert(T->Right, x);

        // 操作之后高度发生改变
        T->Height = std::max(Height(T->Left), Height(T->Right)) + 1;
        T = Balance(T); // 平衡右子树
    }
    // 重复元素情况
    else
    {
        // 使自身数量 + 1，同时使ElementCount + 1
        ++(T->Times);
        ++ElementCount;
        // 重复元素不改变树结构，无需更新高度和平衡
    }

    return T; // 返回当前（可能平衡后的）根节点
}

/**
 * @brief 从AVL树中删除元素
 *
 * 递归地从AVL树中删除元素，维护树的平衡性质
 * 支持重复元素删除，通过Times计数实现
 * 删除后自动更新节点高度并执行平衡操作
 *
 * ##删除情况：
 * ##1.元素不存在：返回空指针
 * ##2.小于当前节点：递归删除左子树
 * ##3.大于当前节点：递归删除右子树
 * ##4.等于当前节点且Times>1：减少重复计数
 * ##5.等于当前节点且Times=1：真正删除节点
 *   ##5.1 有两个子节点：用右子树最小值替换
 *   ##5.2 有0或1个子节点：直接删除并连接子树
 *
 * @tparam Type 树中存储的元素类型
 * @param T 当前子树根节点
 * @param x 要删除的元素
 * @return Tree 删除并平衡后的新根节点
 */
template <typename Type>
typename AVLTree<Type>::Tree AVLTree<Type>::Remove(Tree T, const Type &x)
{
    // 元素不存在情况
    if (T == nullptr)
        return nullptr;

    // 递归查找要删除的元素
    if (x < T->Item)
        T->Left = Remove(T->Left, x); // 在左子树中删除
    else if (x > T->Item)
        T->Right = Remove(T->Right, x); // 在右子树中删除
    // 找到要删除的元素且需要真正删除节点
    else if (T->Times == 1)
    {
        // 有两个子节点的情况：用右子树最小值替换当前节点
        if (T->Left != nullptr && T->Right != nullptr)
        {
            Tree temp = FindMin(T->Right);        // 找到右子树最小节点
            unsigned tempt = T->Times;            // 保存临时计数
            T->Item = temp->Item;                 // 替换元素值
            T->Times = temp->Times;               // 替换计数
            temp->Times = tempt;                  // 恢复临时计数
            T->Right = Remove(T->Right, T->Item); // 删除右子树中的重复节点
        }
        // 有0或1个子节点的情况：直接删除
        else
        {
            Tree temp = T;
            if (T->Left == nullptr)
                T = T->Right; // 只有右子树或无子树
            else
                T = T->Left; // 只有左子树
            delete temp;     // 释放节点内存

            --ElementCount; // 更新元素计数
            --NodeCount;    // 更新节点计数
        }
    }
    // 重复元素情况：减少计数
    else
    {
        --(T->Times);   // 减少重复计数
        --ElementCount; // 更新元素计数
        return T;       // 重复元素不改变树结构，直接返回
    }

    // 更新高度并平衡树结构
    if (T != nullptr)
    {
        T->Height = std::max(Height(T->Left), Height(T->Right)) + 1; // 更新当前节点高度
        T = Balance(T);                                              // 执行平衡操作
    }
    return T; // 返回当前（可能平衡后的）根节点
}

/**
 * @brief 从AVL树中删除元素的所有出现
 *
 * 递归地从AVL树中删除元素的所有出现次数，维护树的平衡性质
 * 无论元素的重复次数多少，都完全删除该元素对应的节点
 * 删除后自动更新节点高度并执行平衡操作
 *
 * ##删除情况：
 * ##1.元素不存在：返回空指针
 * ##2.小于当前节点：递归删除左子树
 * ##3.大于当前节点：递归删除右子树
 * ##4.等于当前节点：完全删除该节点
 *   ##4.1 有两个子节点：用右子树最小值替换当前节点
 *   ##4.2 有0或1个子节点：直接删除并连接子树
 *
 * @tparam Type 树中存储的元素类型
 * @param T 当前子树根节点
 * @param x 要完全删除的元素
 * @return Tree 删除并平衡后的新根节点
 */
template <typename Type>
typename AVLTree<Type>::Tree AVLTree<Type>::RemoveAll(Tree T, const Type &x)
{
    // 元素不存在情况
    if (T == nullptr)
        return nullptr;

    // 递归查找要删除的元素
    if (x < T->Item)
        T->Left = RemoveAll(T->Left, x); // 在左子树中删除
    else if (x > T->Item)
        T->Right = RemoveAll(T->Right, x); // 在右子树中删除
    // 找到要删除的元素：完全删除该节点
    else if (T->Left != nullptr && T->Right != nullptr)
    {
        // 有两个子节点的情况：用右子树最小值替换当前节点
        Tree temp = FindMin(T->Right);           // 找到右子树最小节点
        unsigned tempt = T->Times;               // 保存临时计数
        T->Item = temp->Item;                    // 替换元素值
        T->Times = temp->Times;                  // 替换计数
        temp->Times = tempt;                     // 恢复临时计数
        T->Right = RemoveAll(T->Right, T->Item); // 删除右子树中的重复节点
    }
    // 有0或1个子节点的情况：直接删除
    else
    {
        Tree temp = T;
        if (T->Left == nullptr)
            T = T->Right; // 只有右子树或无子树
        else
            T = T->Left;             // 只有左子树
        ElementCount -= temp->Times; // 减去该节点的所有重复计数
        delete temp;                 // 释放节点内存

        --NodeCount; // 更新节点计数
    }

    // 更新高度并平衡树结构
    if (T != nullptr)
    {
        T->Height = std::max(Height(T->Left), Height(T->Right)) + 1; // 更新当前节点高度
        T = Balance(T);                                              // 执行平衡操作
    }

    return T; // 返回当前（可能平衡后的）根节点
}

/**
 * @brief 在AVL树中查找元素
 *
 * 递归地在AVL树中查找指定元素
 * 利用AVL树的二叉搜索树性质进行高效查找
 * 时间复杂度：O(log n)
 *
 * ##查找情况：
 * ##1.空树或到达叶子节点：返回nullptr
 * ##2.小于当前节点：递归查找左子树
 * ##3.大于当前节点：递归查找右子树
 * ##4.等于当前节点：返回当前节点指针
 *
 * @tparam Type 树中存储的元素类型
 * @param T 当前子树根节点
 * @param x 要查找的元素
 * @return Position 找到的节点位置，未找到返回nullptr
 */
template <typename Type>
typename AVLTree<Type>::Position AVLTree<Type>::Find(Tree T, const Type &x) const
{
    if (T != nullptr)
    {
        if (x < T->Item)
            return Find(T->Left, x); // 在左子树中递归查找
        else if (x > T->Item)
            return Find(T->Right, x); // 在右子树中递归查找
    }
    return T; // 找到目标节点或未找到返回nullptr
}

/**
 * @brief 查找AVL树中的最大元素
 *
 * 在AVL树中查找最大元素节点
 * 利用AVL树的二叉搜索树性质，最大元素位于最右节点
 * 使用迭代方式实现，时间复杂度：O(log n)
 *
 * ##查找过程：
 * ##1.空树情况：返回nullptr
 * ##2.非空树：沿右子树一直向下遍历
 * ##3.终止条件：当前节点的右子树为空
 *
 * @tparam Type 树中存储的元素类型
 * @param T 当前子树根节点
 * @return Position 最大元素节点位置，空树返回nullptr
 */
template <typename Type>
typename AVLTree<Type>::Position AVLTree<Type>::FindMax(Tree T) const
{
    // 空树情况
    if (T == nullptr)
        return nullptr;

    // 沿右子树迭代查找最右节点
    while (T->Right != nullptr)
        T = T->Right;

    return T; // 返回最大元素节点
}

template <typename Type>
AVLTree<Type>::AVLTree(const AVLTree<Type> &avl) : Root(nullptr), NodeCount(avl.NodeCount), ElementCount(avl.ElementCount)
{
    if (avl.Root != nullptr)
    {
        Root = new TreeNode(*(avl.Root));
        Tree srcRoot = avl.Root;
        Tree copyRoot = Root;
        std::queue<Tree> srcQueue;
        std::queue<Tree> copyQueue;
        srcQueue.push(srcRoot);
        copyQueue.push(copyRoot);
        while (!srcQueue.empty())
        {
            srcRoot = srcQueue.front();
            srcQueue.pop();
            copyRoot = copyQueue.front();
            copyQueue.pop();
            if (srcRoot->Left != nullptr)
            {
                srcQueue.push(srcRoot->Left);
                copyRoot->Left = new TreeNode(*(srcRoot->Left));
                copyQueue.push(copyRoot->Left);
            }
            if (srcRoot->Right != nullptr)
            {
                srcQueue.push(srcRoot->Right);
                copyRoot->Right = new TreeNode(*(srcRoot->Right));
                copyQueue.push(copyRoot->Right);
            }
        }
    }
}

template <typename Type>
AVLTree<Type>::AVLTree(AVLTree<Type> &&avl) : Root(avl.Root), NodeCount(avl.NodeCount), ElementCount(avl.ElementCount)
{
    avl.Root = nullptr;
    avl.NodeCount = 0;
    avl.ElementCount = 0;
}

template <typename Type>
AVLTree<Type> &AVLTree<Type>::operator=(const AVLTree<Type> &avl)
{
    if (this == &avl)
        return *this;
    Root = MakeEmpty(Root);
    NodeCount = avl.NodeCount;
    ElementCount = avl.ElementCount;

    if (avl.Root != nullptr)
    {
        Root = new TreeNode(*(avl.Root));
        Tree srcRoot = avl.Root;
        Tree copyRoot = Root;
        std::queue<Tree> srcQueue;
        std::queue<Tree> copyQueue;
        srcQueue.push(srcRoot);
        copyQueue.push(copyRoot);
        while (!srcQueue.empty())
        {
            srcRoot = srcQueue.front();
            srcQueue.pop();
            copyRoot = copyQueue.front();
            copyQueue.pop();
            if (srcRoot->Left != nullptr)
            {
                srcQueue.push(srcRoot->Left);
                copyRoot->Left = new TreeNode(*(srcRoot->Left));
                copyQueue.push(copyRoot->Left);
            }
            if (srcRoot->Right != nullptr)
            {
                srcQueue.push(srcRoot->Right);
                copyRoot->Right = new TreeNode(*(srcRoot->Right));
                copyQueue.push(copyRoot->Right);
            }
        }
    }
    return *this;
}

template <typename Type>
AVLTree<Type> &AVLTree<Type>::operator=(AVLTree<Type> &&avl)
{
    if (this == &avl)
        return *this;
    Root = MakeEmpty(Root);

    NodeCount = avl.NodeCount;
    ElementCount = avl.ElementCount;
    Root = avl.Root;

    avl.Root = nullptr;
    avl.NodeCount = 0;
    avl.ElementCount = 0;

    return *this;
}

/**
 * @brief 查找AVL树中的最小元素
 *
 * 在AVL树中查找最小元素节点
 * 利用AVL树的二叉搜索树性质，最小元素位于最左节点
 * 使用迭代方式实现，时间复杂度：O(log n)
 *
 * ##查找过程：
 * ##1.空树情况：返回nullptr
 * ##2.非空树：沿左子树一直向下遍历
 * ##3.终止条件：当前节点的左子树为空
 *
 * @tparam Type 树中存储的元素类型
 * @param T 当前子树根节点
 * @return Position 最小元素节点位置，空树返回nullptr
 */
template <typename Type>
typename AVLTree<Type>::Position AVLTree<Type>::FindMin(Tree T) const
{
    // 空树情况
    if (T == nullptr)
        return nullptr;

    // 沿左子树迭代查找最左节点
    while (T->Left != nullptr)
        T = T->Left;

    return T; // 返回最小元素节点
}

/**
 * @brief 向AVL树中插入元素的公共接口
 *
 * 提供安全的元素插入接口，处理内存分配异常
 * 调用内部递归插入函数并更新根节点
 * 确保插入操作的事务性：要么完全成功，要么完全失败
 *
 * ##执行流程：
 * ##1.调用内部Insert函数进行递归插入
 * ##2.更新树的根节点引用
 * ##3.返回插入成功状态
 * ##4.捕获内存分配异常，保证树状态一致性
 *
 * @tparam Type 树中存储的元素类型
 * @param x 要插入的元素
 * @return bool 插入成功返回true，内存分配失败返回false
 */
template <typename Type>
bool AVLTree<Type>::insert(const Type &x)
{
    Tree temp;
    try
    {
        // 调用内部递归插入函数
        temp = Insert(Root, x);
        // 更新根节点引用
        Root = temp;
        return true; // 插入成功
    }
    catch (std::bad_alloc &e)
    {
        // 内存分配失败处理
        std::cerr << e.what() << '\n';
        return false; // 插入失败
    }
}

/**
 * @brief 用来查找元素 x 是否存在于树中
 *
 * @param 待查找的元素
 *
 * @return 返回存在于树中的个数，0就是没有
 */
template <typename Type>
unsigned int AVLTree<Type>::find(const Type &x) const
{
    Position temp = Find(Root, x);
    if (temp == nullptr)
        return 0;
    return temp->Times;
}

/**
 * @brief 用来获取树中的最大值
 *
 * 如果不存在最大值, 返回 0, 不修改 x
 *
 * @param 用来传递最大值容器
 *
 * @return 返回最大值出现的次数, x 带回最大值
 */
template <typename Type>
unsigned int AVLTree<Type>::findMax(Type &x) const
{
    Position temp = FindMax(Root);
    if (temp == nullptr)
        return 0;
    x = temp->Item;
    return temp->Times;
}

/**
 * @brief 用来获取树中的最小值
 *
 * 如果不存在最小值, 返回 0, 不修改 x
 *
 * @param 用来传递最小值容器
 *
 * @return 返回最小值出现的次数, x 带回最小值
 */
template <typename Type>
unsigned int AVLTree<Type>::findMin(Type &x) const
{
    Position temp = FindMin(Root);
    if (temp == nullptr)
        return 0;
    x = temp->Item;
    return temp->Times;
}

/**
 * @brief 删除树中的一个元素
 *
 * @return 不存在则删除失败返回 false, 否则返回 true
 */
template <typename Type>
bool AVLTree<Type>::remove(const Type &x)
{
    if (Root == nullptr)
        return false;

    Tree temp = Remove(Root, x);
    if (temp == nullptr && ElementCount)
        return false;

    Root = temp;
    return true;
}

/**
 * @brief 删除树中所有与 x 相同的元素
 *
 * @return 不存在则删除失败返回 false, 否则返回 true
 */
template <typename Type>
unsigned int AVLTree<Type>::removeAll(const Type &x)
{
    if (Root == nullptr)
        return 0U;
    unsigned long long tempe = ElementCount;
    Tree temp = RemoveAll(Root, x);
    if (temp == nullptr && NodeCount)
        return 0U;
    Root = temp;
    return tempe - ElementCount;
}

/**
 * @brief 判断树是否为空
 */
template <typename Type>
bool AVLTree<Type>::isEmpty() const
{
    return ElementCount == 0;
}

/**
 * @brief 返回树中节点的数量
 */
template <typename Type>
unsigned long long AVLTree<Type>::nodeCount() const
{
    return NodeCount;
}

/**
 * @brief 返回树中元素的数量
 */
template <typename Type>
unsigned long long AVLTree<Type>::elementCount() const
{
    return ElementCount;
}

/**
 * @brief 先序遍历树，将（不重复）元素返回给一个 vector
 *
 * 使用栈来避免递归
 * 先处理当前栈顶的节点的元素
 * 然后使右子树先压入到栈中以保证左子树先于右子树被遍历
 *
 * @return 返回储存先序遍历树（不重复）元素的 vector 容器
 */
template <typename Type>
std::vector<Type> AVLTree<Type>::preOrder() const
{
    std::vector<Type> result;
    if (Root != nullptr)
    {
        // 这是为了防止原树被改变
        Tree root = Root;
        std::stack<Tree> stk;
        stk.push(root);
        while (!stk.empty())
        {
            root = stk.top();
            stk.pop();
            result.push_back(root->Item);
            if (root->Right != nullptr)
                stk.push(root->Right);
            if (root->Left != nullptr)
                stk.push(root->Left);
        }
    }
    return result;
}

/**
 * @brief 中序遍历树, 将（不重复）元素返回给一个 vector
 *
 * 使用栈来避免递归
 * 对于任何一个子树的根节点，先使左子树入栈
 * 当左子树为空时, 才弹出栈顶节点, 并保存节点元素
 * 然后再来到右子树重复上述操作
 *
 * @return 返回储存中序遍历树（不重复）元素的 vector 容器
 */
template <typename Type>
std::vector<Type> AVLTree<Type>::inOrder() const
{
    std::vector<Type> result;
    // 这是为了防止原树被改变
    Tree root = Root;
    std::stack<Tree> stk;
    while (root != nullptr || !stk.empty())
    {
        // root 可以是根节点, 左子树, 右子树
        // 循环后, 所有 root 最左边的左子树都已入栈
        while (root != nullptr)
        {
            stk.push(root);
            root = root->Left;
        }
        // 有前面两个判断保证 stk 此时必不为空
        root = stk.top();
        stk.pop();
        result.push_back(root->Item);
        // 遍历完左子树和自身, 再中序遍历自身
        root = root->Right;
    }
    return result;
}

/**
 * @brief 后序遍历树, 将（不重复）元素返回给一个 vector
 *
 * 使用栈来避免递归
 *
 * @return 返回储存后序遍历树（不重复）元素的 vector 容器
 */
template <typename Type>
std::vector<Type> AVLTree<Type>::posOrder() const
{
    std::vector<Type> result;
    if (Root != nullptr)
    {
        Tree head = nullptr, cur;
        std::stack<Tree> stk;
        stk.push(Root);
        while (!stk.empty())
        {
            cur = stk.top();
            if (cur->Left != nullptr && cur->Left != head && cur->Right != head)
                stk.push(cur->Left);
            else if (cur->Right != nullptr && cur->Right != head)
                stk.push(cur->Right);
            else
            {
                result.push_back(cur->Item);
                // 其实就是 cur
                head = stk.top();
                stk.pop();
            }
        }
    }
    return result;
}

/**
 * @brief 先序遍历树的所有（含重复）元素
 *
 * @return 返回带有遍历结果的 vector
 */
template <typename Type>
std::vector<Type> AVLTree<Type>::preOrderAll() const
{
    std::vector<Type> result;
    if (Root != nullptr)
    {
        // 这是为了防止原树被改变
        Tree root = Root;
        std::stack<Tree> stk;
        unsigned int times;
        stk.push(root);
        while (!stk.empty())
        {
            root = stk.top();
            times = root->Times;
            stk.pop();
            while (times--)
                result.push_back(root->Item);
            if (root->Right != nullptr)
                stk.push(root->Right);
            if (root->Left != nullptr)
                stk.push(root->Left);
        }
    }
    return result;
}

/**
 * @brief 中序遍历树的所有（含重复）元素
 *
 * @return 返回带有遍历结果的 vector
 */
template <typename Type>
std::vector<Type> AVLTree<Type>::inOrderAll() const
{
    std::vector<Type> result;
    std::stack<Tree> stk;
    Tree root = Root;
    unsigned int times;
    while (root != nullptr || !stk.empty())
    {
        while (root != nullptr)
        {
            stk.push(root);
            root = root->Left;
        }
        root = stk.top();
        stk.pop();
        times = root->Times;
        while (times--)
            result.push_back(root->Item);
        root = root->Right;
    }
    return result;
}

/**
 * @brief 后序遍历树的所有（含重复）元素
 *
 * @return 返回带有遍历结果的 vector
 */
template <typename Type>
std::vector<Type> AVLTree<Type>::posOrderAll() const
{
    std::vector<Type> result;
    if (Root != nullptr)
    {
        Tree head = nullptr, cur;
        unsigned int times;
        std::stack<Tree> stk;
        stk.push(Root);
        while (!stk.empty())
        {
            cur = stk.top();
            if (cur->Left != nullptr && cur->Left != head && cur->Right != head)
                stk.push(cur->Left);
            else if (cur->Right != nullptr && cur->Right != head)
                stk.push(cur->Right);
            else
            {
                times = cur->Times;
                while (times--)
                    result.push_back(cur->Item);
                head = stk.top();
                stk.pop();
            }
        }
    }
    return result;
}

/**
 * @brief 广度优先遍历树的所有（不重复）元素
 *
 * @return 返回带有遍历结果的 vector
 */
template <typename Type>
std::vector<Type> AVLTree<Type>::bfs() const
{
    std::vector<Type> result;
    if (Root != nullptr)
    {
        std::queue<Tree> q;
        Tree root = Root;
        q.push(root);
        while (!q.empty())
        {
            root = q.front();
            q.pop();
            result.push_back(root->Item);
            if (root->Left != nullptr)
                q.push(root->Left);
            if (root->Right != nullptr)
                q.push(root->Right);
        }
    }
    return result;
}

/**
 * @brief 广度优先遍历树的所有（含重复）元素
 *
 * @return 返回带有遍历结果的 vector
 */
template <typename Type>
std::vector<Type> AVLTree<Type>::bfsAll() const
{
    std::vector<Type> result;
    if (Root != nullptr)
    {
        Tree root = Root;
        unsigned int times;
        std::queue<Tree> q;
        q.push(root);
        while (!q.empty())
        {
            root = q.top();
            q.pop();
            times = root->Times;
            while (times--)
                result.push_back(root->Item);
            if (root->Left != nullptr)
                q.push(root->Left);
            if (root->Right != nullptr)
                q.push(root->Right);
        }
    }
    return result;
}

#endif
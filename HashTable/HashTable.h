#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include <cstdint>

// 分离链接法
template <typename Type>
class HashTable
{
private:
    struct ListNode
    {
        Type Item;
        ListNode *Next;
        ListNode() : Next(nullptr) {}
        ListNode(const Type &x) : Item(x), Next(nullptr) {}
    };

    std::size_t TableSize;
    ListNode **HashTbl;
    std::size_t Hash(const char[]);
    void MakeEmpty();

public:
    HashTable(std::size_t = 131);
    HashTable(const HashTable &);
    HashTable(HashTable &&);
    ~HashTable();
    HashTable<Type> &operator=(const HashTable &);
    HashTable<Type> &operator=(HashTable &&);

    void insert(const char[], const Type &);
    bool remove(const char[], const Type &);
    bool remove(const char[]);
    bool find(const char[]);
    bool find(const char[], const Type &);
};

template <typename Type>
void HashTable<Type>::MakeEmpty()
{
    if (HashTbl == nullptr)
        return;
    for (std::size_t i = 0; i < TableSize; ++i)
    {
        ListNode *cur = HashTbl[i];
        while (cur != nullptr)
        {
            ListNode *temp = cur;
            cur = cur->Next;
            delete temp;
        }
        HashTbl[i] = nullptr;
    }
}

template <typename Type>
std::size_t HashTable<Type>::Hash(const char key[])
{
    std::size_t hashVal = 0;
    const char *ptr = key;
    while (*ptr != '\0')
    {
        hashVal = (hashVal << 5) + static_cast<unsigned char>(*ptr);
        ++ptr;
    }
    return hashVal % TableSize;
}

template <typename Type>
HashTable<Type>::HashTable(std::size_t ts) : TableSize(ts > 0 ? ts : 101)
{
    HashTbl = new ListNode *[TableSize];
    for (std::size_t i = 0; i < TableSize; ++i)
        HashTbl[i] = nullptr;
}

template <typename Type>
HashTable<Type>::HashTable(const HashTable<Type> &ht) : TableSize(ht.TableSize)
{
    HashTbl = new ListNode *[TableSize];
    for (std::size_t i = 0; i < TableSize; ++i)
    {
        HashTbl[i] = nullptr;
        ListNode *raw = ht.HashTbl[i];
        ListNode **tail = &HashTbl[i];

        while (raw != nullptr)
        {
            *tail = new ListNode(raw->Item);
            tail = &((*tail)->Next);
            raw = raw->Next;
        }
    }
}

template <typename Type>
HashTable<Type>::HashTable(HashTable<Type> &&ht) : TableSize(ht.TableSize)
{
    HashTbl = ht.HashTbl;
    ht.HashTbl = nullptr;
}

template <typename Type>
HashTable<Type> &HashTable<Type>::operator=(const HashTable<Type> &ht)
{
    if (this == &ht)
        return *this;

    MakeEmpty();
    delete[] HashTbl;

    TableSize = ht.TableSize;
    HashTbl = new ListNode *[TableSize];

    for (std::size_t i = 0; i < TableSize; ++i)
    {
        HashTbl[i] = nullptr;
        ListNode *raw = ht.HashTbl[i];
        ListNode **tail = &HashTbl[i];

        while (raw != nullptr)
        {
            *tail = new ListNode(raw->Item);
            tail = &((*tail)->Next);
            raw = raw->Next;
        }
    }

    return *this;
}

template <typename Type>
HashTable<Type> &HashTable<Type>::operator=(HashTable<Type> &&ht)
{
    if (this == &ht)
        return *this;
    MakeEmpty();
    TableSize = ht.TableSize;
    HashTbl = ht.HashTbl;
    ht.HashTbl = nullptr;
    return *this;
}

template <typename Type>
HashTable<Type>::~HashTable()
{
    MakeEmpty();
    delete[] HashTbl;
}

template <typename Type>
void HashTable<Type>::insert(const char key[], const Type &x)
{
    if (find(key, x))
        return;

    std::size_t index = Hash(key);
    ListNode *newNode = new ListNode(x);
    newNode->Next = HashTbl[index];
    HashTbl[index] = newNode;
}

template <typename Type>
bool HashTable<Type>::remove(const char key[], const Type &x)
{
    std::size_t index = Hash(key);
    ListNode *pos = HashTbl[index];
    ListNode *pre = nullptr;

    while (pos != nullptr && pos->Item != x)
    {
        pre = pos;
        pos = pos->Next;
    }

    if (pos == nullptr)
        return false;

    if (pre == nullptr)
        HashTbl[index] = pos->Next;
    else
        pre->Next = pos->Next;

    delete pos;
    return true;
}

template <typename Type>
bool HashTable<Type>::remove(const char key[])
{
    std::size_t index = Hash(key);
    ListNode *pos = HashTbl[index];

    if (pos == nullptr)
        return false;

    ListNode *temp;
    while (pos != nullptr)
    {
        temp = pos;
        pos = pos->Next;
        delete temp;
    }

    HashTbl[index] = nullptr;
    return true;
}

template <typename Type>
bool HashTable<Type>::find(const char key[])
{
    std::size_t index = Hash(key);
    return HashTbl[index] != nullptr;
}

template <typename Type>
bool HashTable<Type>::find(const char key[], const Type &x)
{
    std::size_t index = Hash(key);
    ListNode *pos = HashTbl[index];

    while (pos != nullptr && pos->Item != x)
        pos = pos->Next;

    return pos != nullptr;
}

#endif
#ifndef MYDOUBLYLIST
#define MYDOUBLYLIST

#include <vector>
#include <iostream>

template <typename Type>
class MyDoublyList {
private: 
  struct Node {
    Node* pre = nullptr;
    Type value;
    Node* next = nullptr;
    Node(const Type& value) {
      // 托管给 C++ 的 = 重载
      this->value = value;
    }
  };

  Node* head = nullptr;
  Node* rear = nullptr;
  size_t size = 0;
  size_t length = 0;

  Node* getFromIndex(size_t index) {
    if (this->size >= index) {
      return nullptr;
    }

    Node* curptr = this->head;
    for (size_t i = 0; i < this->size; ++i) {
      curptr = curptr->next;
    }

    return curptr;
  }

public:
  MyDoublyList() {};

  MyDoublyList(const std::vector<Type>& values) {
    for (size_t i = 0; i < values.size; ++i) {
      this->push_back(values[i]);
    }
  }

  ~MyDoublyList() {
    Node* curptr = this->head;
    while (this->head) {
      this->head = this->head->next;
      delete curptr;
      curptr = this->head;
    }
    this->rear = nullptr;
    this->size = 0;
    this->length = 0;
  }
  
  void push_back(const Type& value) {
    const Node* newNode = new Node(value);

    if (this->head == nullptr) {
      this->head = newNode;
      this->rear = newNode;
    } else {
      const Node* preNode = this->rear;
      newNode->pre = preNode;
      preNode->next = newNode;
      this->rear = newNode;
    }

    ++this->size;
    ++this->length;
  }

  void push_front(const Type& value) {
    const Node* newNode = new Node(value);

    if (this->head == nullptr) {
      this->head = newNode;
      this->rear = newNode;
    } else {
      const Node* nextNode = this->head;
      newNode->next = preNode;
      nextNode->pre = newNode;
      this->head = newNode;
    }

    ++this->size;
    ++this->length;
  }

  void pop_back() {
    if (!this->size) {
      return;
    }

    const removedNode = this->rear;
    if (this->head == this->rear) {
      this->head == nullptr;
      this->rear = nullptr;
    } else {
      this->rear = removedNode->pre;
    }

    delete removedNode;
    --this->size;
    --this->length;
  }

  void pop_front() {
    if (!this->size) {
      return;
    }

    const removedNode = this->head;
    if (this->head == this->rear) {
      this->head = nullptr;
      this->rear = nullptr;
    } else {
      this->head = removedNode->next;
    }

    delete removedNode;
    --this->size;
    --this->length;
  }

  const Type* find(size_t index) {
    const pos = this->getFromIndex(index);
    if (!pos) {
      return nullptr;
    }

    return &(pos->value);
  }

  bool findFirst(const Type& value) {
    const Node* curptr = this->head;
    while (curptr) {
      // 托管给 C++ 运算重载
      if (curptr->value == value) {
        break;
      }
      curptr = curptr->next;
    }

    return curptr != nullptr;
  }

  bool inset(size_t index, const Type& value) {
    if (index > this->size) {
      return false;
    }

    if (index == 0) {
      this->push_front(value);
    } else if (index == this->size - 1) {
      this->push_back(value);
    } else {
      // 前面判过越界了
      const pos = this->getFromIndex(index);

      const newNode = new Node(value);
      const Node* nextNode = pos->next;

      pos->next = newNode;
      newNode->pre = newNode;

      ++this->size;
      ++this->length;
    }
  }

  void reverse() {
    Node* curptr = nullptr;
    this->rear = this->head;

    while (this->head) {
      this->head->pre = curptr;
      curptr = this->head;
      this->head = this->head->next;
    }

    this->head = curptr;
  }

  const std::vector<Type> values() {
    const std::vector<Type> resultArray = std::vector<Type>();
    Node* curptr = this->head;
    while (curptr) {
      resultArray.push_back(curptr->value);
      curptr = curptr->next;
    }

    return resultArray;
  }

  friend std::ostream& operator<<(std::ostream& os, const MyDoublyList& list) {
    Node* curptr = this->head;
    while (curptr) {
      if (curptr->next) {
        os << this->value << ", ";
      } else {
        os << this->value << std::endl;
      }
    }
    return os;
  }
}

#endif
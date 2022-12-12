#include "ThreadSafeSet.h"

using namespace std;

template <class T>
bool ThreadSafeSet<T>::insert(T val){
    auto current = atomic_load(&head);
    //head is null or head value is bigger than the inserted value
    if (current == nullptr || (!(current->value < val) && !(current->value ==val))){
        if (current!= nullptr){
            atomic_store(&head, make_shared<Node>(val, head));
        }else
            atomic_store(&head, make_shared<Node>(val));
        size++;
        return true;
    }
    // check inserted value is intermediate node
    shared_ptr<Node> prevNode = nullptr;
    while (current != nullptr && !(current->value == val)){
        if (!(current->value < val)){
            atomic_store(&prevNode->next, make_shared<Node>(val, current));
            size++;
            return true;
        }
        prevNode = current;
        current = current->next;
    }
    //check inserted is new last node
    if (current== nullptr && prevNode->value < val){
        atomic_store(&prevNode->next, make_shared<Node>(val,current));
        size++;
        return true;
    }
    
    return false;
}

template <class T>
bool ThreadSafeSet<T>::remove(T val){
    auto current = atomic_load(&head);
    if (current == nullptr)
        return false;
    //if head node is to be deleted
    if (current->value == val){
        atomic_store(&head, head->next);
        size--;
        return true;
    }
    //child node is to be deleted
    shared_ptr<Node> prevNode=nullptr;
    while (current!=nullptr){
        if (current->value == val){
            atomic_store(&prevNode->next, current->next);
            size--;
            return true;
        }
        if (!(current->value < val))
            break;
        prevNode=current;
        current = current->next;
    }
    return false;
}

template <class T>
void ThreadSafeSet<T>::print(){
    shared_ptr<Node> current = atomic_load(&head);
    while (current!= nullptr){
        cout << current->value << endl;
        current = current->next;
    }
}

template <class T>
bool ThreadSafeSet<T>::find(T val){
    shared_ptr<Node> current = atomic_load(&head);
    if (current == nullptr)
        return false;

    while (current != nullptr){
        if (current->value==val)
            return true;
        if (!(current->value < val))
            break;
        current = current->next;
    }
    return false;
}

template <class T>
bool ThreadSafeSet<T>::clear(){
    atomic_store(&head, shared_ptr<Node>{});
    size = 0;
    return true;
}

template <class T>
int ThreadSafeSet<T>::count(){
    return size;
}

template <class T>
ThreadSafeSet<T>::ThreadSafeSet(ThreadSafeSet<T> &&other){
    atomic_store(&head, other.head);
    atomic_store(&other.head, shared_ptr<Node>{});
    size=other.size;
    other.size=0;
}

template <class T>
ThreadSafeSet<T> &ThreadSafeSet<T>::operator=(ThreadSafeSet<T> &&other){
    atomic_store(&head, other.head);
    atomic_store(&other.head, shared_ptr<Node>{});
    size = other.size;
    other.size = 0;
    return &this;
}

template <class T>
void ThreadSafeSet<T>::iterate(function<void(const T&)> f){
    auto currnt = head;
    while(currnt != nullptr){
        f(currnt->value);
        currnt = currnt->next;
    }
}
template <class T>
ThreadSafeSet<T>::Node::Node(T _value) : value(_value), next(nullptr) {}

template <class T>
ThreadSafeSet<T>::Node::Node(T _value, shared_ptr<Node> _next) : value(_value), next(_next) {}

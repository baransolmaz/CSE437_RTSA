#ifndef _THREADSAFESET_H_
#define _THREADSAFESET_H_

#include <atomic>
#include <iostream>
#include <memory>
#include <functional>

using namespace std;

template <class T>
struct ThreadSafeSet{

    public:
        ThreadSafeSet() : head(nullptr), size(0){};
        ~ThreadSafeSet(){
            atomic_store(&head, shared_ptr<Node>{});
        }

        ThreadSafeSet(const ThreadSafeSet &) = delete;               // Copy Constructor
        ThreadSafeSet<T> &operator=(const ThreadSafeSet &) = delete; // Copy Assigment Constructor
        
        void operator=(ThreadSafeSet<T> &) = delete;

        ThreadSafeSet(ThreadSafeSet<T> &&other);                     // Move Constructor
        ThreadSafeSet<T> &operator=(ThreadSafeSet<T> &&other);       // Move Assigment Constructor

        //Prints all elements in increasing order
        void print();

        // If T val exists returns false, if not adds and returns true
        bool insert(T val);

        //If T val exists returns true, if not false
        bool find(T val);

        //If T val exists removes from set and returns true, if not false
        bool remove(T val);

        //Removes all elements and returns true
        bool clear();

        //Returns the size of set
        int count();

        //Iterates over elements in ascending order
        void iterate(function<void(const T&)> f);

    private :
        class Node {
            public:
                T value;
                shared_ptr<Node> next;
                Node(T _value);

                Node(T _value, shared_ptr<Node> _next);
        };

        shared_ptr<Node> head;
        atomic<int> size=0;
};
#endif
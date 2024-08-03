//
// Created by yasas on 8/3/24.
//


#pragma once

#include <atomic>

namespace async::lock_free {


    template<typename T>
    struct Node {

        T data;
        std::atomic<Node *> next;

        explicit Node(T val) : data(val), next(nullptr) {}

    };

    template<typename T>
    class LockFreeQueue {

    public :

        using LockFreeNode = Node<T>;

        LockFreeQueue() : _tail(nullptr), _head(nullptr) {}

        void enqueue(T item) {

            auto *new_node = new LockFreeNode(item);

            while (true) {
                auto tail_node = _tail.load();
                if (tail_node == nullptr) {
                    if (_tail.compare_exchange_weak(tail_node, new_node)) {
                        _head.store(new_node);
                        return;
                    }
                } else {
                    auto next_node = tail_node->next.load();
                    if (next_node == nullptr) {
                        if (tail_node->next.compare_exchange_weak(next_node, new_node)) {
                            _tail.compare_exchange_weak(tail_node, new_node);
                            return;
                        }
                    }
                }
            }
        }

        bool dequeue(T &result) {

            LockFreeNode *head_node;
            LockFreeNode *tail_node;
            LockFreeNode *next_node;

            while (true) {

                head_node = _head.load();
                if (head_node == nullptr) {
                    return false;
                }

                next_node = head_node->next.load();
                tail_node = _tail.load();

                if (head_node == _head.load()) {
                    if (next_node == nullptr) {
                        result = head_node->data;
                        if (_tail.compare_exchange_weak(tail_node, nullptr)) {
                            _head.store(nullptr);
                            delete head_node;
                            return true;
                        }
                    } else {
                        result = head_node->data;
                        if (_head.compare_exchange_weak(head_node, next_node)) {
                            delete head_node; // Clean up the old head
                            return true;
                        }
                    }
                }
            }


        }

    private:
        std::atomic<LockFreeNode *> _head;
        std::atomic<LockFreeNode *> _tail;
    };


}

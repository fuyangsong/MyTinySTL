#pragma once
#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "deque.h"
#include "functional.h"
#include "vector.h"

namespace myTinySTL
{
    template <class T, class Sequence = myTinySTL::deque<T>>
    class queue
    {
    public:
        // 使用底层容器的型别
        typedef typename Sequence::value_type value_type;
        typedef typename Sequence::size_type size_type;
        typedef typename Sequence::reference reference;
        typedef typename Sequence::const_reference const_reference;
        typedef Sequence container_type;

    private:
        Sequence s; // 用底层容器来表现 queue

    public:
        // 构造、复制、移动函数
        queue() : s() {}
        explicit queue(size_type n) : s(n) {}
        queue(size_type n, const value_type& value) : s(n, value) {}
        template <class InputIterator>
        queue(InputIterator first, InputIterator last) : s(first, last) {}

        queue(const Sequence& s) : s(s) {}
        queue(Sequence&& s) : s(std::move(s)) {}

        queue(const queue& rhs) : s(rhs.s) {}
        queue(queue&& rhs) : s(std::move(rhs.s)) {}

        queue& operator=(const queue& rhs)
        {
            s = rhs.s;
            return *this;
        }
        queue& operator=(queue&& rhs)
        {
            s = std::move(rhs.s);
            return *this;
        }

        // 以下操作使用底层容器的操作
        bool empty() const { return s.empty(); }
        size_type size() const { return s.size(); }

        reference front() { return s.front(); }
        const_reference front() const { return s.front(); }
        reference back() { return s.back(); }
        const_reference back() const { return s.back(); }

        void push(const value_type& value) { s.push_back(value); }
        void pop() { s.pop_front(); }
        void clear()
        {
            while (!empty())
                pop();
        }
        void swap(queue& rhs) { myTinySTL::swap(s, rhs.s); }

    public:
        friend bool operator==(const queue& lhs, const queue& rhs) { return lhs.s == rhs.s; }
    };

    // 重载比较操作符
    template <class T, class Sequence>
    bool operator==(const queue<T, Sequence>& lhs, const queue<T, Sequence>& rhs)
    {
        return lhs == rhs;
    }

    template <class T, class Sequence>
    bool operator!=(const queue<T, Sequence>& lhs, const queue<T, Sequence>& rhs)
    {
        return !(lhs == rhs);
    }


    // 重载 swap
    template <class T, class Sequence>
    void swap(queue<T, Sequence>& lhs, queue<T, Sequence>& rhs)
    {
        lhs.swap(rhs);
    }
}

#endif // !_QUEUE_H_

#pragma once
#ifndef _STACK_H_
#define _STACK_H_

#include "deque.h"

namespace myTinySTL
{

    // the class of stack
    template <class T, class Sequence = myTinySTL::deque<T>>
    class stack
    {
    public:
        typedef typename Sequence::value_type value_type;
        typedef typename Sequence::size_type size_type;
        typedef typename Sequence::reference reference;
        typedef typename Sequence::const_reference const_reference;

    private:
        Sequence s; // 底层容器

    public:
        // 构造、复制、移动函数
        stack() : s() {}
        explicit stack(size_type n) : s(n) {}
        stack(size_type n, const value_type& value) : s(n, value) {}
        template <class InputIterator>
        stack(InputIterator first, InputIterator last) : s(first, last) {}

        explicit stack(const Sequence& s) : s(s) {}
        explicit stack(Sequence&& s) : s(std::move(s)) {}

        stack(const stack& rhs) : s(rhs.s) {}
        stack(stack&& rhs) : s(std::move(rhs.s)) {}

        stack& operator=(const stack& rhs)
        {
            s = rhs.s;
            return *this;
        }
        stack& operator=(stack&& rhs)
        {
            s = std::move(rhs.s);
            return *this;
        }

        // 以下操作使用底层容器的操作
        bool empty() const { return s.empty(); }
        size_type size() const { return s.size(); }

        reference top() { return s.back(); }
        const_reference top() const { return s.back(); }
        void push(const value_type& value) { s.push_back(value); }
        void pop() { s.pop_back(); }

        void clear()
        {
            while (!empty())
                pop();
        }
        void swap(stack& rhs) { myTinySTL::swap(s, rhs.s); }

    public:
        friend bool operator==(const stack& lhs, const stack& rhs) { return lhs.s == rhs.s; }
    };

    // 重载比较操作符
    template <class T, class Sequence>
    bool operator==(const stack<T, Sequence>& lhs, const stack<T, Sequence>& rhs)
    {
        return lhs == rhs;
    }

    template <class T, class Sequence>
    bool operator!=(const stack<T, Sequence>& lhs, const stack<T, Sequence>& rhs)
    {
        return !(lhs == rhs);
    }

    // 重载swap
    template <class T, class Sequence>
    void swap(stack<T, Sequence>& lhs, stack<T, Sequence>& rhs)
    {
        lhs.swap(rhs);
    }

}

#endif // !_STACK_H_


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
        // ʹ�õײ��������ͱ�
        typedef typename Sequence::value_type value_type;
        typedef typename Sequence::size_type size_type;
        typedef typename Sequence::reference reference;
        typedef typename Sequence::const_reference const_reference;
        typedef Sequence container_type;

    private:
        Sequence s; // �õײ����������� queue

    public:
        // ���졢���ơ��ƶ�����
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

        // ���²���ʹ�õײ������Ĳ���
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

    // ���رȽϲ�����
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


    // ���� swap
    template <class T, class Sequence>
    void swap(queue<T, Sequence>& lhs, queue<T, Sequence>& rhs)
    {
        lhs.swap(rhs);
    }
}

#endif // !_QUEUE_H_

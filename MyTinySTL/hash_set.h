#pragma once
#ifndef _HASH_SET_H_
#define _HASH_SET_H_

#include "hashtable.h"

namespace myTinySTL
{
    // ģ���� hash_set
    // ����һ����ʵֵ���ͣ������������ϣ������ȱʡʹ�� myTinySTL �� hash
    // �����������ֵ�ȽϷ�ʽ��ȱʡʹ�� myTinySTL �� equal_to�������Ĵ���ռ����������ͣ�ȱʡʹ�� myTinySTL �� alloc
    // ʹ�÷����� set ���ƣ��� hashtable ��Ϊ�ײ���ƣ����� hash_set �е�Ԫ�ز����Զ�����
    template <class Value, class HashFcn = myTinySTL::hash<Value>,
        class EqualKey = myTinySTL::equal_to<Value>, class Alloc = myTinySTL::alloc>
        class hash_set
    {
    private:
        // ʹ�� hash_table ��Ϊ�ײ����
        typedef hashtable<Value, Value, HashFcn,
            myTinySTL::identity<Value>, EqualKey, Alloc>
            rep_type;
        rep_type ht_;

    public:
        // ʹ�� hash_table ���ͱ�
        typedef typename rep_type::key_type key_type;
        typedef typename rep_type::value_type value_type;
        typedef typename rep_type::hasher hasher;
        typedef typename rep_type::key_equal key_equal;

        typedef typename rep_type::size_type size_type;
        typedef typename rep_type::difference_type difference_type;
        typedef typename rep_type::const_pointer pointer;
        typedef typename rep_type::const_pointer const_pointer;
        typedef typename rep_type::const_reference reference;
        typedef typename rep_type::const_reference const_reference;

        typedef typename rep_type::const_iterator iterator;
        typedef typename rep_type::const_iterator const_iterator;
        typedef typename rep_type::allocator_type allocator_type;

        hasher hash_fcn() const { return ht_.hash_fcn(); }
        key_equal key_eq() const { return ht_.key_eq(); }
        allocator_type get_allocator() const { return ht_.get_allocator(); }

    public:
        // ���졢���ơ��ƶ�����
        hash_set() : ht_(100, hasher(), key_equal()) {} // ȱʡʹ�ô�СΪ 100 �ı��
        explicit hash_set(size_type n) : ht_(n, hasher(), key_equal()) {}
        hash_set(size_type n, const hasher& hf) : ht_(n, hf, key_equal()) {}
        hash_set(size_type n, const hasher& hf, const key_equal& keq) : ht_(n, hf, keq) {}

        hash_set(const hash_set& rhs) : ht_(rhs.ht_) {}
        hash_set(hash_set&& rhs) : ht_(std::move(rhs.ht_)) {}

        hash_set& operator=(const hash_set& rhs)
        {
            ht_ = rhs.ht_;
            return *this;
        }
        hash_set& operator=(hash_set&& rhs)
        {
            ht_ = std::move(rhs.ht_);
            return *this;
        }

        // ȫ��ʹ�� hashtable �� insert_unique����ֵ�������ظ�
        template <class InputIterator>
        hash_set(InputIterator first, InputIterator last)
            : ht_(100, hasher(), key_equal())
        {
            ht_.insert_unique(first, last);
        }
        template <class InputIterator>
        hash_set(InputIterator first, InputIterator last, size_type n)
            : ht_(n, hasher(), key_equal())
        {
            ht_.insert_unique(first, last);
        }
        template <class InputIterator>
        hash_set(InputIterator first, InputIterator last, size_type n,
            const hasher& hf) : ht_(n, hf, key_equal())
        {
            ht_.insert_unique(first, last);
        }
        template <class InputIterator>
        hash_set(InputIterator first, InputIterator last, size_type n,
            const hasher& hf, const key_equal& keq) : ht_(n, hf, keq)
        {
            ht_.insert_unique(first, last);
        }

        // ��ؽӿ�
        iterator begin() const { return ht_.begin(); }
        iterator end() const { return ht_.end(); }

        bool empty() const { return ht_.empty(); }
        size_type size() const { return ht_.size(); }
        size_type max_size() const { return ht_.max_size(); }

        pair<iterator, bool> insert(const value_type& value)
        {
            auto p = ht_.insert_unique(value);
            return pair<iterator, bool>(p.first, p.second);
        }
        template <class InputIterator>
        void insert(InputIterator first, InputIterator last)
        {
            ht_.insert_unique(first, last);
        }
        pair<iterator, bool> insert_noresize(const value_type& value)
        {
            auto p = ht_.insert_unique_noresize(value);
            return pair<iterator, bool>(p.first, p.second);
        }

        size_type erase(const key_type& key) { return ht_.erase(key); }
        void erase(iterator it) { ht_.erase(it); }
        void erase(iterator first, iterator last) { ht_.erase(first, last); }
        void clear() { ht_.clear(); }

        // hash_set ��ز���
        iterator find(const key_type& key) const { return ht_.find(key); }
        size_type count(const key_type& key) const { return ht_.count(key); }
        pair<iterator, iterator> equal_range(const key_type& key) const
        {
            return ht_.equal_range(key);
        }
        void reserve(size_type hint) { ht_.reserve(hint); }
        size_type bucket_count() const { return ht_.bucket_count(); }
        size_type max_bucket_count() const { return ht_.max_bucket_count(); }
        size_type elems_in_bucket(size_type n) const { return ht_.elems_in_bucket(n); }
        void swap(hash_set& rhs) { ht_.swap(rhs.ht_); }

    public:
        // ���� operator==, operator!=
        friend bool operator==(const hash_set& lhs, const hash_set& rhs) { return lhs.ht_ == rhs.ht_; }
        friend bool operator!=(const hash_set& lhs, const hash_set& rhs) { return lhs.ht_ != rhs.ht_; }
    };

    // ���رȽϲ�����
    template <class Value, class HashFcn, class EqualKey, class Alloc>
    inline bool operator==(const hash_set<Value, HashFcn, EqualKey, Alloc>& lhs,
        const hash_set<Value, HashFcn, EqualKey, Alloc>& rhs)
    {
        return lhs == rhs;
    }

    template <class Value, class HashFcn, class EqualKey, class Alloc>
    inline bool operator!=(const hash_set<Value, HashFcn, EqualKey, Alloc>& lhs,
        const hash_set<Value, HashFcn, EqualKey, Alloc>& rhs)
    {
        return lhs != rhs;
    }

    // ���� myTinySTL �� swap
    template <class Value, class HashFcn, class EqualKey, class Alloc>
    void swap(hash_set<Value, HashFcn, EqualKey, Alloc>& lhs,
        hash_set<Value, HashFcn, EqualKey, Alloc>& rhs)
    {
        lhs.swap(rhs);
    }

    /*****************************************************************************************/

    // ģ���� hash_multiset
    // ��ֵ�����ظ��������� hash_set ��ͬ
    template <class Value, class HashFcn = myTinySTL::hash<Value>,
        class EqualKey = myTinySTL::equal_to<Value>, class Alloc = myTinySTL::alloc>
        class hash_multiset
    {
    private:
        // ʹ�� hash_table ��Ϊ�ײ����
        typedef hashtable<Value, Value, HashFcn,
            myTinySTL::identity<Value>, EqualKey, Alloc>
            rep_type;
        rep_type ht_;

    public:
        // ʹ�� hash_table ���ͱ�
        typedef typename rep_type::key_type key_type;
        typedef typename rep_type::value_type value_type;
        typedef typename rep_type::hasher hasher;
        typedef typename rep_type::key_equal key_equal;

        typedef typename rep_type::size_type size_type;
        typedef typename rep_type::difference_type difference_type;
        typedef typename rep_type::const_pointer pointer;
        typedef typename rep_type::const_pointer const_pointer;
        typedef typename rep_type::const_reference reference;
        typedef typename rep_type::const_reference const_reference;

        typedef typename rep_type::const_iterator iterator;
        typedef typename rep_type::const_iterator const_iterator;
        typedef typename rep_type::allocator_type allocator_type;

        hasher hash_fcn() const { return ht_.hash_fcn(); }
        key_equal key_eq() const { return ht_.key_eq(); }
        allocator_type get_allocator() const { return ht_.get_allocator(); }

    public:
        // ���졢���ơ��ƶ�����
        hash_multiset() : ht_(100, hasher(), key_equal()) {} // ȱʡʹ�ô�СΪ 100 �ı��
        explicit hash_multiset(size_type n) : ht_(n, hasher(), key_equal()) {}
        hash_multiset(size_type n, const hasher& hf) : ht_(n, hf, key_equal()) {}
        hash_multiset(size_type n, const hasher& hf, const key_equal& keq) : ht_(n, hf, keq) {}

        hash_multiset(const hash_multiset& rhs) : ht_(rhs.ht_) {}
        hash_multiset(hash_multiset&& rhs) : ht_(std::move(rhs.ht_)) {}

        hash_multiset& operator=(const hash_multiset& rhs)
        {
            ht_ = rhs.ht_;
            return *this;
        }
        hash_multiset& operator=(hash_multiset&& rhs)
        {
            ht_ = std::move(rhs.ht_);
            return *this;
        }

        // ȫ��ʹ�� insert_equal����ֵ�����ظ�
        template <class InputIterator>
        hash_multiset(InputIterator first, InputIterator last)
            : ht_(100, hasher(), key_equal())
        {
            ht_.insert_equal(first, last);
        }
        template <class InputIterator>
        hash_multiset(InputIterator first, InputIterator last, size_type n)
            : ht_(n, hasher(), key_equal())
        {
            ht_.insert_equal(first, last);
        }
        template <class InputIterator>
        hash_multiset(InputIterator first, InputIterator last, size_type n,
            const hasher& hf) : ht_(n, hf, key_equal())
        {
            ht_.insert_equal(first, last);
        }
        template <class InputIterator>
        hash_multiset(InputIterator first, InputIterator last, size_type n,
            const hasher& hf, const key_equal& keq) : ht_(n, hf, keq)
        {
            ht_.insert_equal(first, last);
        }

        // ��ؽӿ�
        iterator begin() const { return ht_.begin(); }
        iterator end() const { return ht_.end(); }

        bool empty() const { return ht_.empty(); }
        size_type size() const { return ht_.size(); }
        size_type max_size() const { return ht_.max_size(); }

        iterator insert(const value_type& value)
        {
            return ht_.insert_equal(value);
        }
        template <class InputIterator>
        void insert(InputIterator first, InputIterator last)
        {
            ht_.insert_equal(first, last);
        }
        iterator insert_noresize(const value_type& value)
        {
            return ht_.insert_equal_noresize(value);
        }

        size_type erase(const key_type& key) { return ht_.erase(key); }
        void erase(iterator it) { ht_.erase(it); }
        void erase(iterator first, iterator last) { ht_.erase(first, last); }
        void clear() { ht_.clear(); }

        // hash_multiset ��ز���
        iterator find(const key_type& key) const { return ht_.find(key); }
        size_type count(const key_type& key) const { return ht_.count(key); }
        pair<iterator, iterator> equal_range(const key_type& key) const
        {
            return ht_.equal_range(key);
        }
        void reserve(size_type hint) { ht_.reserve(hint); }
        size_type bucket_count() const { return ht_.bucket_count(); }
        size_type max_bucket_count() const { return ht_.max_bucket_count(); }
        size_type elems_in_bucket(size_type n) const { return ht_.elems_in_bucket(n); }
        void swap(hash_multiset& rhs) { ht_.swap(rhs.ht_); }

    public:
        // ���� operator==, operator!=
        friend bool operator==(const hash_multiset& lhs, const hash_multiset& rhs)
        {
            return lhs.ht_ == rhs.ht_;
        }
        friend bool operator!=(const hash_multiset& lhs, const hash_multiset& rhs)
        {
            return lhs.ht_ != rhs.ht_;
        }
    };

    // ���رȽϲ�����
    template <class Value, class HashFcn, class EqualKey, class Alloc>
    inline bool operator==(const hash_multiset<Value, HashFcn, EqualKey, Alloc>& lhs,
        const hash_multiset<Value, HashFcn, EqualKey, Alloc>& rhs)
    {
        return lhs == rhs;
    }

    template <class Value, class HashFcn, class EqualKey, class Alloc>
    inline bool operator!=(const hash_multiset<Value, HashFcn, EqualKey, Alloc>& lhs,
        const hash_multiset<Value, HashFcn, EqualKey, Alloc>& rhs)
    {
        return lhs != rhs;
    }

    // ���� myTinySTL �� swap
    template <class Value, class HashFcn, class EqualKey, class Alloc>
    void swap(hash_multiset<Value, HashFcn, EqualKey, Alloc>& lhs,
        hash_multiset<Value, HashFcn, EqualKey, Alloc>& rhs)
    {
        lhs.swap(rhs);
    }
}

#endif
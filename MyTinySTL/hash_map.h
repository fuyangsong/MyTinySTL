#pragma once
#ifndef _HASH_MAP_H_
#define _HASH_MAP_H_

#include "hashtable.h"

namespace myTinySTL {
    // ����һ�����ֵ���ͣ�����������ʵֵ���ͣ������������ϣ������ȱʡʹ�� myTinySTL �� hash
    // �����Ĵ����ֵ�ȽϷ�ʽ��ȱʡʹ�� myTinySTL �� equal_to�����������ռ����������ͣ�ȱʡʹ�� myTinySTL �� alloc
    // ʹ�÷����� map ���ƣ�ʹ�� hashtable ��Ϊ�ײ���ƣ����� hash_map �ڵ�Ԫ�ز����Զ�����
    template <class Key, class T, class HashFcn = myTinySTL::hash<Key>,
        class EqualKey = myTinySTL::equal_to<Key>, class Alloc = myTinySTL::alloc>
        class hash_map
    {
    private:
        // ʹ�� hash_table ��Ϊ�ײ����
        typedef hashtable<myTinySTL::pair<const Key, T>, Key, HashFcn,
            myTinySTL::selectfirst<myTinySTL::pair<const Key, T>>, EqualKey, Alloc>  rep_type;
        rep_type ht_;

    public:
        // ʹ�� hash_table ���ͱ�
        typedef typename rep_type::key_type key_type;
        typedef T data_type;
        typedef T mapped_type;
        typedef typename rep_type::value_type value_type;
        typedef typename rep_type::hasher hasher;
        typedef typename rep_type::key_equal key_equal;

        typedef typename rep_type::size_type size_type;
        typedef typename rep_type::difference_type difference_type;
        typedef typename rep_type::pointer pointer;
        typedef typename rep_type::const_pointer const_pointer;
        typedef typename rep_type::reference reference;
        typedef typename rep_type::const_reference const_reference;

        typedef typename rep_type::iterator iterator;
        typedef typename rep_type::const_iterator const_iterator;
        typedef typename rep_type::allocator_type allocator_type;

        hasher hash_fcn() const { return ht_.hash_fcn(); }
        key_equal key_eq() const { return ht_.key_eq(); }
        allocator_type get_allocator() const { return ht_.get_allocator(); }

    public:
        // ���졢���ơ��ƶ�����
        hash_map() : ht_(100, hasher(), key_equal()) {} // ȱʡʹ�ô�СΪ 100 �ı��
        explicit hash_map(size_type n) : ht_(n, hasher(), key_equal()) {}
        hash_map(size_type n, const hasher& hf) : ht_(n, hf, key_equal()) {}
        hash_map(size_type n, const hasher& hf, const key_equal& keq) : ht_(n, hf, keq) {}

        hash_map(const hash_map& rhs) : ht_(rhs.ht_) {}
        hash_map(hash_map&& rhs) : ht_(std::move(rhs.ht_)) {}

        hash_map& operator=(const hash_map& rhs) { ht_ = rhs.ht_; return *this; }
        hash_map& operator=(hash_map&& rhs) { ht_ = std::move(rhs.ht_); return *this; }
        // ȫ��ʹ�� insert_unique����ֵ�������ظ�
        template <class InputIterator>
        hash_map(InputIterator first, InputIterator last)
            : ht_(100, hasher(), key_equal())
        {
            ht_.insert_unique(first, last);
        }
        template <class InputIterator>
        hash_map(InputIterator first, InputIterator last, size_type n)
            : ht_(n, hasher(), key_equal())
        {
            ht_.insert_unique(first, last);
        }
        template <class InputIterator>
        hash_map(InputIterator first, InputIterator last, size_type n,
            const hasher& hf) : ht_(n, hf, key_equal())
        {
            ht_.insert_unique(first, last);
        }
        template <class InputIterator>
        hash_map(InputIterator first, InputIterator last, size_type n,
            const hasher& hf, const key_equal& keq) : ht_(n, hf, keq)
        {
            ht_.insert_unique(first, last);
        }

        // ��ؽӿ�
        iterator begin() { return ht_.begin(); }
        const_iterator begin() const { return ht_.begin(); }
        iterator end() { return ht_.end(); }
        const_iterator end() const { return ht_.end(); }

        bool empty() const { return ht_.empty(); }
        size_type size() const { return ht_.size(); }
        size_type max_size() const { return ht_.max_size(); }

        pair<iterator, bool> insert(const value_type& value)
        {
            return ht_.insert_unique(value);
        }
        template <class InputIterator>
        void insert(InputIterator first, InputIterator last)
        {
            ht_.insert_unique(first, last);
        }
        pair<iterator, bool> insert_noresize(const value_type& value)
        {
            return ht_.insert_unique_noresize(value);
        }

        size_type erase(const key_type& key) { return ht_.erase(key); }
        void erase(iterator it) { ht_.erase(it); }
        void erase(iterator first, iterator last) { ht_.erase(first, last); }
        void clear() { ht_.clear(); }

        data_type& operator[](const key_type& key)
        {
            return ht_.find_or_insert(value_type(key, T())).second;
        }

        // hash_map ��ز���
        iterator find(const key_type& key) { return ht_.find(key); }
        const_iterator find(const key_type& key) const { return ht_.find(key); }
        size_type count(const key_type& key) const { return ht_.count(key); }
        pair<iterator, iterator> equal_range(const key_type& key)
        {
            return ht_.equal_range(key);
        }
        pair<const_iterator, const_iterator> equal_range(const key_type& key) const
        {
            return ht_.equal_range(key);
        }
        void reserve(size_type hint) { ht_.reserve(hint); }
        size_type bucket_count() const { return ht_.bucket_count(); }
        size_type max_bucket_count() const { return ht_.max_bucket_count(); }
        size_type elems_in_bucket(size_type n) const { return ht_.elems_in_bucket(n); }
        void swap(hash_map& rhs) { ht_.swap(rhs.ht_); }

    public:
        // ���� operator==, operator!=
        friend bool operator==(const hash_map& lhs, const hash_map& rhs) { return lhs.ht_ == rhs.ht_; }
        friend bool operator!=(const hash_map& lhs, const hash_map& rhs) { return lhs.ht_ != rhs.ht_; }
    };

    // ���رȽϲ�����
    template <class Key, class T, class HashFcn, class EqualKey, class Alloc>
    inline bool operator==(const hash_map<Key, T, HashFcn, EqualKey, Alloc>& lhs,
        const hash_map<Key, T, HashFcn, EqualKey, Alloc>& rhs)
    {
        return lhs == rhs;
    }

    template <class Key, class T, class HashFcn, class EqualKey, class Alloc>
    inline bool operator!=(const hash_map<Key, T, HashFcn, EqualKey, Alloc>& lhs,
        const hash_map<Key, T, HashFcn, EqualKey, Alloc>& rhs)
    {
        return lhs != rhs;
    }

    // ���� myTinySTL �� swap
    template <class Key, class T, class HashFcn, class EqualKey, class Alloc>
    void swap(hash_map<Key, T, HashFcn, EqualKey, Alloc>& lhs,
        hash_map<Key, T, HashFcn, EqualKey, Alloc>& rhs)
    {
        lhs.swap(rhs);
    }

    /*****************************************************************************************/

    // ģ���� hash_multimap
    // ��ֵ�����ظ��������� hash_map ��ͬ
    template <class Key, class T, class HashFcn = myTinySTL::hash<Key>,
        class EqualKey = myTinySTL::equal_to<Key>, class Alloc = myTinySTL::alloc>
        class hash_multimap
    {
    private:
        // ʹ�� hash_table ��Ϊ�ײ����
        typedef hashtable<pair<const Key, T>, Key, HashFcn,
            myTinySTL::selectfirst<pair<const Key, T>>, EqualKey, Alloc>  rep_type;
        rep_type ht_;

    public:
        // ʹ�� hash_table ���ͱ�
        typedef typename rep_type::key_type key_type;
        typedef T data_type;
        typedef T mapped_type;
        typedef typename rep_type::value_type value_type;
        typedef typename rep_type::hasher hasher;
        typedef typename rep_type::key_equal key_equal;

        typedef typename rep_type::size_type size_type;
        typedef typename rep_type::difference_type difference_type;
        typedef typename rep_type::pointer pointer;
        typedef typename rep_type::const_pointer const_pointer;
        typedef typename rep_type::reference reference;
        typedef typename rep_type::const_reference const_reference;

        typedef typename rep_type::iterator iterator;
        typedef typename rep_type::const_iterator const_iterator;
        typedef typename rep_type::allocator_type allocator_type;

        hasher hash_fcn() const { return ht_.hash_fcn(); }
        key_equal key_eq() const { return ht_.key_eq(); }
        allocator_type get_allocator() const { return ht_.get_allocator(); }

    public:
        // ���졢���ơ��ƶ�����
        hash_multimap() : ht_(100, hasher(), key_equal()) {} // ȱʡʹ�ô�СΪ 100 �ı��
        explicit hash_multimap(size_type n) : ht_(n, hasher(), key_equal()) {}
        hash_multimap(size_type n, const hasher& hf) : ht_(n, hf, key_equal()) {}
        hash_multimap(size_type n, const hasher& hf, const key_equal& keq) : ht_(n, hf, keq) {}

        hash_multimap(const hash_multimap& rhs) : ht_(rhs.ht_) {}
        hash_multimap(hash_multimap&& rhs) : ht_(std::move(rhs.ht_)) {}

        hash_multimap& operator=(const hash_multimap& rhs) { ht_ = rhs.ht_; return *this; }
        hash_multimap& operator=(hash_multimap&& rhs) { ht_ = std::move(rhs.ht_); return *this; }

        // ȫ��ʹ�� insert_equal����ֵ�����ظ�
        template <class InputIterator>
        hash_multimap(InputIterator first, InputIterator last)
            : ht_(100, hasher(), key_equal())
        {
            ht_.insert_equal(first, last);
        }
        template <class InputIterator>
        hash_multimap(InputIterator first, InputIterator last, size_type n)
            : ht_(n, hasher(), key_equal())
        {
            ht_.insert_equal(first, last);
        }
        template <class InputIterator>
        hash_multimap(InputIterator first, InputIterator last, size_type n,
            const hasher& hf) : ht_(n, hf, key_equal())
        {
            ht_.insert_equal(first, last);
        }
        template <class InputIterator>
        hash_multimap(InputIterator first, InputIterator last, size_type n,
            const hasher& hf, const key_equal& keq) : ht_(n, hf, keq)
        {
            ht_.insert_equal(first, last);
        }

        // ��ؽӿ�
        iterator begin() { return ht_.begin(); }
        const_iterator begin() const { return ht_.begin(); }
        iterator end() { return ht_.end(); }
        const_iterator end() const { return ht_.end(); }

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

        data_type& operator[](const key_type& key)
        {
            return ht_.find_or_insert(value_type(key, T())).second;
        }

        // hash_multimap ��ز���
        iterator find(const key_type& key) { return ht_.find(key); }
        const_iterator find(const key_type& key) const { return ht_.find(key); }
        size_type count(const key_type& key) const { return ht_.count(key); }
        pair<iterator, iterator> equal_range(const key_type& key)
        {
            return ht_.equal_range(key);
        }
        pair<const_iterator, const_iterator> equal_range(const key_type& key) const
        {
            return ht_.equal_range(key);
        }
        void reserve(size_type hint) { ht_.reserve(hint); }
        size_type bucket_count() const { return ht_.bucket_count(); }
        size_type max_bucket_count() const { return ht_.max_bucket_count(); }
        size_type elems_in_bucket(size_type n) const { return ht_.elems_in_bucket(n); }
        void swap(hash_multimap& rhs) { ht_.swap(rhs.ht_); }

    public:
        // ���� operator==, operator!=
        friend bool operator==(const hash_multimap& lhs, const hash_multimap& rhs)
        {
            return lhs.ht_ == rhs.ht_;
        }
        friend bool operator!=(const hash_multimap& lhs, const hash_multimap& rhs)
        {
            return lhs.ht_ != rhs.ht_;
        }
    };

    // ���رȽϲ�����
    template <class Key, class T, class HashFcn, class EqualKey, class Alloc>
    inline bool operator==(const hash_multimap<Key, T, HashFcn, EqualKey, Alloc>& lhs,
        const hash_multimap<Key, T, HashFcn, EqualKey, Alloc>& rhs)
    {
        return lhs == rhs;
    }

    template <class Key, class T, class HashFcn, class EqualKey, class Alloc>
    inline bool operator!=(const hash_multimap<Key, T, HashFcn, EqualKey, Alloc>& lhs,
        const hash_multimap<Key, T, HashFcn, EqualKey, Alloc>& rhs)
    {
        return lhs != rhs;
    }

    // ���� myTinySTL �� swap
    template <class Key, class T, class HashFcn, class EqualKey, class Alloc>
    void swap(hash_multimap<Key, T, HashFcn, EqualKey, Alloc>& lhs,
        hash_multimap<Key, T, HashFcn, EqualKey, Alloc>& rhs)
    {
        lhs.swap(rhs);
    }

} // namespace myTinySTL
#endif // !MYTINYSTL_HASH_MAP_H_


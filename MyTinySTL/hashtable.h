#pragma once
#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include "algorithm.h"
#include "functional.h"
#include "hash_functional.h"
#include "vector.h"

namespace myTinySTL
{
    // hashtable ʹ�ÿ������������ǽڵ㶨��
    template <class T>
    struct __hashtable_node
    {
        __hashtable_node* next; // ָ����һ���ڵ�
        T value;                // ����ʵֵ
        __hashtable_node() : next(nullptr), value(0) {}
    };

    // ǰ������
    template <class Val, class Key, class HashFun,
        class ExtractKey, class EqualKey, class Alloc>
        class hashtable;

    template <class Val, class Key, class HashFun,
        class ExtractKey, class EqualKey, class Alloc>
        struct __hashtable_iterator;

    template <class Val, class Key, class HashFun,
        class ExtractKey, class EqualKey, class Alloc>
        struct __hashtable_const_iterator;

    // __hashtable_iterator ����
    template <class Val, class Key, class HashFcn,
        class ExtractKey, class EqualKey, class Alloc>
        struct __hashtable_iterator
    {
        typedef myTinySTL::hashtable<
            Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>
            hashtable;
        typedef myTinySTL::__hashtable_iterator<
            Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>
            iterator;
        typedef myTinySTL::__hashtable_const_iterator<
            Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>
            const_iterator;
        typedef __hashtable_node<Val> node;

        // __hashtable_iterator ����Ƕ�ͱ�
        typedef forward_iterator_tag iterator_category;
        typedef Val value_type;
        typedef Val* pointer;
        typedef Val& reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        node* cur;     // ��������ǰ��ָ�ڵ�
        hashtable* ht; // ����������������

        // ���캯��
        __hashtable_iterator() {}
        __hashtable_iterator(node* n, hashtable* t) : cur(n), ht(t) {}
        __hashtable_iterator(const iterator& rhs) : cur(rhs.cur), ht(rhs.ht) {}

        // ���ز�����
        reference operator*() const { return cur->value; }
        pointer operator->() const { return &(operator*()); }

        iterator& operator++();
        iterator operator++(int)
        {
            iterator tmp = *this;
            ++* this;
            return tmp;
        }

        // ���رȽϲ�����
        bool operator==(const iterator& rhs) const { return cur == rhs.cur; }
        bool operator!=(const iterator& rhs) const { return cur != rhs.cur; }
    };

    // ���� __hashtable_iterator �� operator++
    template <class Val, class Key, class HashFcn,
        class ExtractKey, class EqualKey, class Alloc>
        __hashtable_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>&
        __hashtable_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator++()
    {
        const node* old = cur; // ԭ���Ľڵ�
        cur = cur->next;
        if (!cur)
        {                                           // �����һ��λ��Ϊ�գ�������һ�� bucket ����ʼ��
            auto index = ht->__bkt_num(old->value); // �ҵ���һ�� bucket ��λ��
            while (!cur && ++index < ht->buckets_.size())
                cur = ht->buckets_[index];
        }
        return *this;
    }

    // __hashtable_const_iterator ����
    template <class Val, class Key, class HashFcn,
        class ExtractKey, class EqualKey, class Alloc>
        struct __hashtable_const_iterator
    {
        typedef myTinySTL::hashtable<
            Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>
            hashtable;
        typedef myTinySTL::__hashtable_iterator<
            Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>
            iterator;
        typedef myTinySTL::__hashtable_const_iterator<
            Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>
            const_iterator;
        typedef __hashtable_node<Val> node;

        // __hashtable_const_iterator ����Ƕ�ͱ�
        typedef forward_iterator_tag iterator_category;
        typedef Val value_type;
        typedef const Val* pointer;
        typedef const Val& reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        const node* cur;     // ������Ŀǰ��ָ�ڵ�
        const hashtable* ht; // ����������������

        // ���캯��
        __hashtable_const_iterator() {}
        __hashtable_const_iterator(const node* n, const hashtable* t) : cur(n), ht(t) {}
        __hashtable_const_iterator(const iterator& rhs) : cur(rhs.cur), ht(rhs.ht) {}

        // ���ز�����
        reference operator*() const { return cur->value; }
        pointer operator->() const { return &(operator*()); }
        const_iterator& operator++();
        const_iterator operator++(int)
        {
            const_iterator tmp = *this;
            ++* this;
            return tmp;
        }

        // ���رȽϲ�����
        bool operator==(const const_iterator& rhs) const { return cur == rhs.cur; }
        bool operator!=(const const_iterator& rhs) const { return cur != rhs.cur; }
    };

    // ���� __hashtable_const_iterator �� operator++
    template <class Val, class Key, class HashFcn,
        class ExtractKey, class EqualKey, class Alloc>
        __hashtable_const_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>&
        __hashtable_const_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator++()
    {
        const node* old = cur; // ԭ���Ľڵ�
        cur = cur->next;
        if (!cur)
        {                                           // �����һ��λ��Ϊ�գ�������һ�� bucket ����ʼ��
            auto index = ht->__bkt_num(old->value); // �ҵ���һ�� bucket ��λ��
            while (!cur && ++index < ht->buckets_.size())
                cur = ht->buckets_[index];
        }
        return *this;
    }

    // �� 28 ����������Ʊ���С
    constexpr int kPrimeNum = 28;

    static const unsigned long __prime_list[kPrimeNum] = {
        53ul, 97ul, 193ul, 389ul, 769ul,
        1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
        49157ul, 98317ul, 196613ul, 393241ul, 786433ul,
        1572869ul, 3145739ul, 6291469ul, 12582917ul, 25165843ul,
        50331653ul, 100663319ul, 201326611ul, 402653189ul, 805306457ul,
        1610612741ul, 3221225473ul, 4294967291ul };

    // ���º��������ҳ���ӽ������ڵ��� n ���Ǹ�����
    inline unsigned long __next_prime(unsigned long n)
    {
        const unsigned long* first = __prime_list;
        const unsigned long* last = __prime_list + kPrimeNum;
        const unsigned long* pos = myTinySTL::lower_bound(first, last, n);
        return pos == last ? *(last - 1) : *pos;
    }

    // ģ���� hashtable
    // ����һ����ʵֵ���ͣ������������ֵ���ͣ������������ϣ����
    // �����Ĵ���ȡ����ֵ�ķ�ʽ��ȱʡʹ�� myTinySTL �� identity
    // ����������ֵ�ȽϷ�ʽ��ȱʡʹ�� myTinySTL �� equal_to������������ռ����������ͣ�ȱʡʹ�� myTinySTL �� alloc
    // hashtable �ڵ�Ԫ�ز����Զ�����
    template <class Val, class Key, class HashFcn, class ExtractKey = myTinySTL::identity<Key>,
        class EqualKey = myTinySTL::equal_to<Key>, class Alloc = myTinySTL::alloc>
        class hashtable
    {
    public:
        // hashtable ���ͱ���
        typedef Val value_type;
        typedef Key key_type;
        typedef HashFcn hasher;
        typedef EqualKey key_equal;
        typedef Alloc allocator_type;

        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef Val* pointer;
        typedef const Val* const_pointer;
        typedef Val& reference;
        typedef const Val& const_reference;
        typedef __hashtable_node<Val> node;
        typedef myTinySTL::vector<node*, Alloc> bucket_type;

        friend struct myTinySTL::__hashtable_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
        friend struct myTinySTL::__hashtable_const_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
        typedef myTinySTL::__hashtable_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>
            iterator;
        typedef myTinySTL::__hashtable_const_iterator<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>
            const_iterator;

        typedef allocator<node> node_allocator;
        allocator_type get_allocator() const { return allocator_type(); }

        hasher hash_fcn() const { return hash_; }   // ��ȡ hash function ���ͱ�
        key_equal key_eq() const { return equal_; } // ��ȡ�ӽڵ���ȡ����ֵ�ķ���

    private:
        // ������������������� hashtable
        hasher hash_;
        key_equal equal_;
        ExtractKey get_key_;
        size_type element_nums_;
        bucket_type buckets_;

    public:
        // ���졢���ơ��ƶ�����������
        explicit hashtable(size_type n,
            const HashFcn& hf = HashFcn(),
            const EqualKey& eqk = EqualKey(),
            const ExtractKey& exk = ExtractKey())
            : hash_(hf), equal_(eqk), get_key_(exk), element_nums_(0)
        {
            __hashtable_initialize(n);
        }

        hashtable(const hashtable& rhs) : hash_(rhs.hash_), equal_(rhs.equal_),
            get_key_(rhs.get_key_), element_nums_(0)
        {
            __copy_from(rhs);
        }
        hashtable(hashtable&& rhs) : hash_(rhs.hash_), equal_(rhs.equal_),
            get_key_(rhs.get_key_), element_nums_(rhs.element_nums_)
        {
            buckets_ = std::move(rhs.buckets_);
            rhs.element_nums_ = 0;
        }

        hashtable& operator=(const hashtable& rhs);
        hashtable& operator=(hashtable&& rhs);

        ~hashtable() { clear(); }

        // ��������ز���
        iterator begin();
        const_iterator begin() const;
        iterator end() { return iterator(static_cast<node*>(0), this); }
        const_iterator end() const { return const_iterator(static_cast<node*>(0), this); }

        // ������ز���
        bool empty() const { return element_nums_ == 0; }
        size_type size() const { return element_nums_; }
        size_type max_size() const { return static_cast<size_type>(-1); }
        size_type bucket_count() const { return buckets_.size(); }
        size_type max_bucket_count() const { return __prime_list[kPrimeNum - 1]; }
        size_type elems_in_bucket(size_type n) const;

        // ����ɾ����ز���
        pair<iterator, bool> insert_unique(const value_type& value)
        {
            reserve(element_nums_ + 1);
            return insert_unique_noresize(value);
        }
        iterator insert_equal(const value_type& value)
        {
            reserve(element_nums_ + 1);
            return insert_equal_noresize(value);
        }

        pair<iterator, bool> insert_unique_noresize(const value_type& value);
        iterator insert_equal_noresize(const value_type& value);

        template <class InputIterator>
        void insert_unique(InputIterator first, InputIterator last)
        {
            insert_unique(first, last, iterator_category(first));
        }
        template <class InputIterator>
        void insert_equal(InputIterator first, InputIterator last)
        {
            insert_equal(first, last, iterator_category(first));
        }

        template <class InputIterator>
        void insert_unique(InputIterator first, InputIterator last, input_iterator_tag);
        template <class InputIterator>
        void insert_equal(InputIterator first, InputIterator last, input_iterator_tag);
        template <class ForwardIterator>
        void insert_unique(ForwardIterator first, ForwardIterator last, forward_iterator_tag);
        template <class ForwardIterator>
        void insert_equal(ForwardIterator first, ForwardIterator last, forward_iterator_tag);

        size_type erase(const key_type& k);
        void erase(const iterator& it);
        void erase(iterator first, iterator last);
        void erase(const const_iterator& it);
        void erase(const_iterator first, const_iterator last);
        void clear();
        void reserve(size_type num_elements_hint);

        // hashtable ��ز���
        reference find_or_insert(const value_type& value);
        iterator find(const key_type& k);
        const_iterator find(const key_type& k) const;
        size_type count(const key_type& k) const;
        pair<iterator, iterator> equal_range(const key_type& k);
        pair<const_iterator, const_iterator> equal_range(const key_type& k) const;
        void swap(hashtable& rhs);

    private:
        // hashtable ��Ա����
        node* __get_node() { return node_allocator::allocate(1); }
        void __put_node(node* p) { node_allocator::deallocate(p, 1); }
        size_type __next_size(size_type n) const;
        void __hashtable_initialize(size_type n);
        size_type __bkt_num(const value_type& value, size_type n) const;
        size_type __bkt_num(const value_type& value) const;
        size_type __bkt_num_key(const key_type& key, size_type n) const;
        size_type __bkt_num_key(const key_type& key) const;
        node* __create_node(const value_type& value);
        void __delete_node(node* n);
        void __erase_bucket(const size_type n, node* first, node* last);
        void __erase_bucket(const size_type n, node* last);
        void __copy_from(const hashtable& ht);
    };

    /*****************************************************************************************/

    // ���Ƹ�ֵ�����
    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>&
        hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator=(const hashtable& rhs)
    {
        if (this != &rhs)
        {
            clear();
            hash_ = rhs.hash_;
            equal_ = rhs.equal_;
            get_key_ = rhs.get_key_;
            buckets_ = rhs.buckets_;
            element_nums_ = rhs.element_nums_;
        }
        return *this;
    }

    // �ƶ���ֵ�����
    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>&
        hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator=(hashtable&& rhs)
    {
        if (this != &rhs)
        {
            clear();
            hash_ = rhs.hash_;
            equal_ = rhs.equal_;
            get_key_ = rhs.get_key_;
            buckets_ = std::move(rhs.buckets_);
            element_nums_ = rhs.element_nums_;
            rhs.element_nums_ = 0;
        }
        return *this;
    }

    // begin ����ָ���һ���ڵ�ĵ�����
    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator
        hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::begin()
    {
        for (size_type n = 0; n < buckets_.size(); ++n)
        {
            if (buckets_[n]) // �ҵ���һ���нڵ��λ�þͷ���
                return iterator(buckets_[n], this);
        }
        return end(); // û���ҵ����� end
    }

    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator
        hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::begin() const
    {
        for (size_type n = 0; n < buckets_.size(); ++n)
        {
            if (buckets_[n]) // �ҵ���һ���нڵ��λ�þͷ���
                return const_iterator(buckets_[n], this);
        }
        return end(); // û���ҵ����� end
    }

    // ��ĳ�� bucket �ڵ�ĸ���
    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type
        hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::elems_in_bucket(size_type n) const
    {
        size_type result = 0;
        for (auto cur = buckets_[n]; cur; cur = cur->next)
            ++result;
        return result;
    }

    // �ڲ���Ҫ�ؽ���������²����½ڵ㣬��ֵ�������ظ�
    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    pair<typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator, bool>
        hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
        insert_unique_noresize(const value_type& value)
    {
        const auto n = __bkt_num(value); // value λ�ڵ� n �� bucket
        auto first = buckets_[n];        // first ָ�� �� n �� bucket ��ͷ��
        // ��� bucket[n] ��ռ�ã�first��Ϊ�գ�����ѭ�����߷�����
        for (auto cur = first; cur; cur = cur->next)
        {
            if (equal_(get_key_(cur->value), get_key_(value))) // ��������д�����ͬ��ֵ�Ľڵ�ͷ���
                return pair<iterator, bool>(iterator(cur, this), false);
        }
        auto tmp = __create_node(value); // �����½ڵ�
        tmp->next = first;
        buckets_[n] = tmp; // ���½ڵ��Ϊ����ĵ�һ���ڵ�
        ++element_nums_;
        return pair<iterator, bool>(iterator(tmp, this), true);
    }

    // �ڲ���Ҫ�ؽ���������²����½ڵ㣬��ֵ�����ظ�
    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator
        hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
        insert_equal_noresize(const value_type& value)
    {
        const auto n = __bkt_num(value); // value λ�ڵ� n �� bucket
        auto first = buckets_[n];        // first ָ�� �� n �� bucket ��ͷ��
        // ��� bucket[n] ��ռ�ã�first��Ϊ�գ�����ѭ�����߷�����
        for (auto cur = first; cur; cur = cur->next)
        {
            if (equal_(get_key_(cur->value), get_key_(value)))
            {
                // ��������д�����ͬ��ֵ�Ľڵ�����ϲ��룬Ȼ�󷵻�
                auto tmp = __create_node(value); // �����½ڵ�
                tmp->next = cur->next;           // ���½ڵ���뵽Ŀ��ڵ��
                cur->next = tmp;
                ++element_nums_;
                return iterator(tmp, this);
            }
        }
        auto tmp = __create_node(value); // �����½ڵ�
        tmp->next = first;
        buckets_[n] = tmp; // ���½ڵ��Ϊ����ĵ�һ���ڵ�
        ++element_nums_;
        return iterator(tmp, this);
    }

    // insert_unique �� input_iterator_tag �汾
    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    template <class InputIterator>
    void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
        insert_unique(InputIterator first, InputIterator last, input_iterator_tag)
    {
        for (; first != last; ++first)
            insert_unique(*first);
    }

    // insert_equal �� input_iterator_tag �汾
    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    template <class InputIterator>
    void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
        insert_equal(InputIterator first, InputIterator last, input_iterator_tag)
    {
        for (; first != last; ++first)
            insert_equal(*first);
    }

    // insert_unique �� forward_iterator_tag �汾
    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    template <class ForwardIterator>
    void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
        insert_unique(ForwardIterator first, ForwardIterator last, forward_iterator_tag)
    {
        auto n = static_cast<size_type>(distance(first, last));
        reserve(element_nums_ + n); // ������С
        for (; n > 0; --n, ++first)
            insert_unique_noresize(*first);
    }

    // insert_equal �� forward_iterator_tag �汾
    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    template <class ForwardIterator>
    void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
        insert_equal(ForwardIterator first, ForwardIterator last, forward_iterator_tag)
    {
        auto n = static_cast<size_type>(distance(first, last));
        reserve(element_nums_ + n); // ������С
        for (; n > 0; --n, ++first)
            insert_equal_noresize(*first);
    }

    // ɾ����ֵΪ key �Ľڵ�
    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type
        hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
        erase(const key_type& k)
    {
        const auto n = __bkt_num_key(k); // ��ֵΪ k �Ľڵ�λ�ڵ� n �� bucket
        auto first = buckets_[n];        // first ָ��� n �� bucket ��ͷ��
        size_type erased = 0;            // ��ɾ����Ԫ�ظ���
        if (first)
        {
            auto cur = first;
            auto next = cur->next;
            while (next)
            {
                if (equal_(get_key_(next->value), k))
                { // ����ҵ���ֵΪ k �Ľڵ�
                    cur->next = next->next;
                    __delete_node(next);
                    next = cur->next;
                    ++erased;
                    --element_nums_;
                }
                else
                {
                    cur = next;
                    next = cur->next;
                }
            }
            if (equal_(get_key_(first->value), k))
            { // �����һ���ڵ��ֵΪ k
                buckets_[n] = first->next;
                __delete_node(first);
                ++erased;
                --element_nums_;
            }
        }
        return erased;
    }

    // ɾ����������ָ�Ľڵ�
    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase(const iterator& it)
    {
        auto p = it.cur;
        if (p)
        {
            const auto n = __bkt_num(p->value);
            auto cur = buckets_[n];
            if (cur == p)
            { // p λ������ͷ��
                buckets_[n] = cur->next;
                __delete_node(cur);
                --element_nums_;
            }
            else
            {
                auto next = cur->next;
                while (next)
                { // ��û������β��
                    if (next == p)
                    { // ����ҵ� p
                        cur->next = next->next;
                        __delete_node(next);
                        --element_nums_;
                        break;
                    }
                    else
                    {
                        cur = next;
                        next = cur->next;
                    }
                }
            }
        }
    }

    // ɾ��[first, last)�ڵĽڵ�
    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
        erase(iterator first, iterator last)
    {
        auto first_bucket = first.cur ? __bkt_num(first.cur->value) : buckets_.size();
        auto last_bucket = last.cur ? __bkt_num(last.cur->value) : buckets_.size();
        if (first.cur == last.cur)
        {
            return;
        }
        else if (first_bucket == last_bucket)
        { // ����� bucket ��ͬһ��λ��
            __erase_bucket(first_bucket, first.cur, last.cur);
        }
        else
        {
            __erase_bucket(first_bucket, first.cur, static_cast<node*>(0)); // ɾ�� first_bucket
            for (auto n = first_bucket + 1; n < last_bucket; ++n)
                __erase_bucket(n, static_cast<node*>(0)); // ɾ�� first �� last �м�� bucket
            if (last_bucket != buckets_.size())
                __erase_bucket(last_bucket, last.cur); // ɾ��ʣ�ಿ��
        }
    }

    // ɾ����������ָ�Ľڵ�
    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    inline void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
        erase(const const_iterator& it)
    {
        erase(iterator(const_cast<node*>(it.cur), const_cast<hashtable*>(it.ht)));
    }

    // ɾ��[first, last)�ڵĽڵ�
    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    inline void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
        erase(const_iterator first, const_iterator last)
    {
        erase(iterator(const_cast<node*>(first.cur), const_cast<hashtable*>(first.ht)),
            iterator(const_cast<node*>(last.cur), const_cast<hashtable*>(last.ht)));
    }

    // ��� hashtable
    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::clear()
    {
        if (element_nums_ != 0)
        {
            for (size_type i = 0; i < buckets_.size(); ++i)
            {
                node* cur = buckets_[i];
                while (cur != nullptr)
                {
                    node* next = cur->next;
                    __delete_node(cur);
                    cur = next;
                }
                buckets_[i] = nullptr;
            }
            element_nums_ = 0;
        }
    }

    // �������� hashtable ��С
    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
        reserve(size_type num_elements_hint)
    {
        // ������Ԫ�غ�Ԫ�ظ������� bucket vector �Ĵ�С�����ؽ����
        const auto old_num = buckets_.size();
        if (num_elements_hint > old_num)
        {                                                  // �����Ҫ�ؽ�
            const auto n = __next_size(num_elements_hint); // �ҵ���һ������
            if (n > old_num)
            {
                bucket_type tmp(n); //�����µ� buckets
                try
                {
                    for (size_type index = 0; index < old_num; ++index)
                    {
                        auto first = buckets_[index];
                        while (first != nullptr)
                        {
                            auto new_bucket = __bkt_num(first->value, n);
                            buckets_[index] = first->next; // �ɵ� buckets_ ָ����һ���ڵ�
                            first->next = tmp[new_bucket]; // ��ǰ�ڵ���뵽�µ� bucket
                            tmp[new_bucket] = first;       // ʹ��ǰ�ڵ��Ϊ�� bucket �ĵ�һ���ڵ�
                            first = buckets_[index];
                        }
                    }
                    buckets_.swap(tmp); // ���µ� bucket ����
                }
                catch (...)
                {
                    for (size_type index = 0; index < tmp.size(); ++index)
                    {
                        while (tmp[index] != nullptr)
                        {
                            auto next = tmp[index]->next;
                            __delete_node(tmp[index]);
                            tmp[index] = next;
                        }
                    }
                }
            }
        }
    }

    // ����ҵ�ʵֵΪ value ��Ԫ�أ��ͷ�����������Ͳ�����Ԫ�غ��ٷ���
    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::reference
        hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
        find_or_insert(const value_type& value)
    {
        reserve(element_nums_ + 1);
        auto n = __bkt_num(value);
        auto first = buckets_[n];
        for (auto cur = first; cur; cur = cur->next)
        {
            if (equal_(get_key_(cur->value), get_key_(value)))
                return cur->value;
        }
        auto tmp = __create_node(value); //�½��ڵ�
        tmp->next = first;
        buckets_[n] = tmp; //���½ڵ��Ϊ����ĵ�һ���ڵ�
        ++element_nums_;
        return tmp->value;
    }

    // ���Ҽ�ֵΪ k �Ľڵ㣬�����������
    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator
        hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
        find(const key_type& k)
    {
        auto n = __bkt_num_key(k);
        node* first = buckets_[n];
        for (; first && !equal_(get_key_(first->value), k); first = first->next)
        {
        }
        return iterator(first, this);
    }

    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator
        hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
        find(const key_type& k) const
    {
        auto n = __bkt_num_key(k);
        const node* first = buckets_[n];
        for (; first && !equal_(get_key_(first->value), k); first = first->next)
        {
        }
        return const_iterator(first, this);
    }

    // ���Ҽ�ֵΪ k ���ֵĴ���
    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type
        hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
        count(const key_type& k) const
    {
        const auto n = __bkt_num_key(k);
        size_type result = 0;
        for (const node* cur = buckets_[n]; cur; cur = cur->next)
        {
            if (equal_(get_key_(cur->value), k))
                ++result;
        }
        return result;
    }

    // �������ֵ k ��ȵ����䣬����һ�� pair��ָ������������β
    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    pair<typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator,
        typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator>
        hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::equal_range(const key_type& k)
    {
        typedef pair<iterator, iterator> pii;
        const auto n = __bkt_num_key(k);
        for (auto first = buckets_[n]; first; first = first->next)
        {
            if (equal_(get_key_(first->value), k))
            { // ���������ȵļ�ֵ
                for (auto cur = first->next; cur; cur = cur->next)
                {
                    if (!equal_(get_key_(cur->value), k)) // ������ֲ���ȵļ�ֵ
                        return pii(iterator(first, this), iterator(cur, this));
                }
                for (auto m = n + 1; m < buckets_.size(); ++m)
                { // ����������ȣ�������һ��������ֵ�λ��
                    if (buckets_[m])
                        return pii(iterator(first, this), iterator(buckets_[m], this));
                }
                return pii(iterator(first, this), end());
            }
        }
        return pii(end(), end()); // û���ҵ�
    }

    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    pair<typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator,
        typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator>
        hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::equal_range(const key_type& k) const
    {
        typedef pair<const_iterator, const_iterator> pii;
        const auto n = __bkt_num_key(k);
        for (const node* first = buckets_[n]; first; first = first->next)
        {
            if (equal_(get_key_(first->value), k))
            { // ���������ȵļ�ֵ
                for (const node* cur = first->next; cur; cur = cur->next)
                {
                    if (!equal_(get_key_(cur->value), k)) // ������ֲ���ȵļ�ֵ
                        return pii(const_iterator(first, this), const_iterator(cur, this));
                }
                for (auto m = n + 1; m < buckets_.size(); ++m)
                { // ����������ȣ�������һ��������ֵ�λ��
                    if (buckets_[m])
                        return pii(const_iterator(first, this), const_iterator(buckets_[m], this));
                }
                return pii(const_iterator(first, this), end());
            }
        }
        return pii(end(), end()); // û���ҵ�
    }

    // ���� hashtable
    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::swap(hashtable& rhs)
    {
        myTinySTL::swap(hash_, rhs.hash_);
        myTinySTL::swap(equal_, rhs.equal_);
        myTinySTL::swap(get_key_, rhs.get_key_);
        buckets_.swap(rhs.buckets_);
        myTinySTL::swap(element_nums_, rhs.element_nums_);
    }

    // __next_size ����
    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type
        hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::__next_size(size_type n) const
    {
        return __next_prime(static_cast<unsigned long>(n));
    }

    // __hashtable_initialize ����
    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
        __hashtable_initialize(size_type n)
    {
        const auto bucket_nums = __next_size(n);
        buckets_.reserve(bucket_nums);
        buckets_.insert(buckets_.end(), bucket_nums, static_cast<node*>(nullptr));
        element_nums_ = 0;
    }

    // __bkt_num ����
    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type
        hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
        __bkt_num(const value_type& value, size_type n) const
    {
        return __bkt_num_key(get_key_(value), n);
    }

    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type
        hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
        __bkt_num(const value_type& value) const
    {
        return __bkt_num_key(get_key_(value));
    }

    // __bkt_num_key ����
    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type
        hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
        __bkt_num_key(const key_type& key, size_type n) const
    {
        return hash_(key) % n;
    }

    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type
        hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
        __bkt_num_key(const key_type& key) const
    {
        return __bkt_num_key(key, buckets_.size());
    }

    // __create_node ����
    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::node*
        hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::__create_node(const value_type& value)
    {
        auto tmp = __get_node();
        tmp->next = nullptr;
        try
        {
            myTinySTL::construct(&tmp->value, value);
        }
        catch (...)
        {
            __put_node(tmp);
        }
        return tmp;
    }

    // __delete_node ����
    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::__delete_node(node* n)
    {
        myTinySTL::destroy(&n->value);
        __put_node(n);
    }

    // __erase_bucket ����
    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
        __erase_bucket(const size_type n, node* first, node* last)
    {
        auto cur = buckets_[n];
        if (cur == first)
        {
            __erase_bucket(n, last);
        }
        else
        {
            node* next;
            for (next = cur->next; next != first; cur = next, next = cur->next)
                ; // �ҵ�first
            while (next != last)
            {
                cur->next = next->next;
                __delete_node(cur);
                next = cur->next;
                --element_nums_;
            }
        }
    }

    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
        __erase_bucket(const size_type n, node* last)
    {
        auto cur = buckets_[n];
        while (cur != last)
        {
            auto next = cur->next;
            __delete_node(cur);
            cur = next;
            buckets_[n] = cur;
            --element_nums_;
        }
    }

    // __copy_from ����
    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    void hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
        __copy_from(const hashtable& ht)
    {
        buckets_.clear();
        buckets_.reserve(ht.buckets_.size());
        buckets_.insert(buckets_.end(), ht.buckets_.size(), static_cast<node*>(0));
        try
        {
            for (size_type i = 0; i < ht.buckets_.size(); ++i)
            {
                const node* cur = ht.buckets_[i];
                if (cur)
                { // ���ĳ bucket ��������
                    auto copy = __create_node(cur->value);
                    buckets_[i] = copy;
                    for (auto next = cur->next; next; cur = next, next = cur->next)
                    { //��������
                        copy->next = __create_node(next->value);
                        copy = copy->next;
                    }
                }
            }
            element_nums_ = ht.element_nums_;
        }
        catch (...)
        {
            clear();
        }
    }

    // ���رȽϲ�����
    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    bool operator==(const hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>& lhs,
        const hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>& rhs)
    {
        if (lhs.buckets_.size() != rhs.buckets_.size())
            return false;
        for (auto n = 0; n < lhs.buckets_.size(); ++n)
        {
            auto cur1 = lhs.buckets_[n];
            auto cur2 = rhs.buckets_[n];
            for (; cur1 && cur2 && cur1->value == cur2->value;
                cur1 = cur1->next, cur2 = cur2->next)
            {
            }
            if (cur1 || cur2) // cur1 �� cur2 ����Ԫ��
                return false;
        }
        return true;
    }

    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    inline bool operator!=(const hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>& lhs,
        const hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>& rhs)
    {
        return !(lhs == rhs);
    }

    // ���� myTinySTL �� swap
    template <class Val, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    void swap(hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>& lhs,
        hashtable<Val, Key, HashFcn, ExtractKey, EqualKey, Alloc>& rhs)
    {
        lhs.swap(rhs);
    }
}

#endif
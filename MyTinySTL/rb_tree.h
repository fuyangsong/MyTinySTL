#pragma once
#ifndef _RB_TREE_H_
#define _RB_TREE_H_

#include "functional.h"
#include "iterator.h"
#include "allocator.h"
#include "reverseiterator.h"
#include "utility.h"
#include "construct.h"

namespace myTinySTL
{

    // rb tree �ڵ���ɫ������
    typedef enum
    {
        __rb_tree_red = false,
        __rb_tree_black = true
    } __rb_tree_color_type;

    // rb tree �Ľڵ����
    struct __rb_tree_node_base
    {
        typedef __rb_tree_color_type color_type;
        typedef __rb_tree_node_base* base_ptr;
        color_type color; // �ڵ���ɫ���Ǻ켴��
        base_ptr parent;  // RB����������������֪�����ڵ�
        base_ptr left;    // ���ӽڵ�
        base_ptr right;   // ���ӽڵ�

        static base_ptr minimum(base_ptr x)
        {
            while (x->left != nullptr)
                x = x->left; // һֱ�����ߣ��ҵ���Сֵ
            return x;
        }
        static base_ptr maximum(base_ptr x)
        {
            while (x->right != nullptr)
                x = x->right; // һֱ�����ߣ��ҵ����ֵ
            return x;
        }
    };

    template <class T>
    struct __rb_tree_node : public __rb_tree_node_base
    {
        typedef __rb_tree_node<T>* link_type;
        T value; // �ڵ�ֵ
    };

    // rb tree �ĵ��������
    struct __rb_tree_iterator_base
    {
        typedef __rb_tree_node_base::base_ptr base_ptr;
        typedef bidirectional_iterator_tag iterator_category;
        typedef ptrdiff_t difference_type;

        base_ptr node; // ������������֮�����һ�������ϵ

        // �ҵ��ȵ�ǰ�ڵ�����С�ڵ�
        void increment()
        {
            if (node->right != nullptr)
            {                                     // ��������ӽڵ㡣״����1��
                node = node->right;               // ��������
                while (node->left != nullptr)     // Ȼ��һֱ���������ߵ���
                    node = node->left;            // ���ǽ��
            }
            else
            {                                    // ���û�����ӽڵ㡣״����2��
                base_ptr y = node->parent;       // �ҵ����ڵ�
                while (y->right == node)
                {                                // ������нڵ㱾���Ǹ����ӽڵ�
                    node = y;                    // ��һֱ���ݣ�ֱ���ڵ㲻Ϊ���ӽڵ�
                    y = y->parent;
                }
                if (node->right != y)            // ����ʱ�����ӽڵ㲻���ڴ�ʱ�ĸ��ڵ�
                    node = y;                    // ״����3����ʱ�ĸ��ڵ㼴Ϊ���
                                                 // �����ʱ��nodeΪ���״����4��
            }
        }

        // �ҵ��ȵ�ǰ�ڵ�С�����ڵ�
        void decrement()
        {
            if (node->color == __rb_tree_red && node->parent->parent == node) // ����Ǻ�ڵ㣬
            {                                                                 // �Ҹ��ڵ�ĸ��ڵ�����Լ�
                node = node->right;                         // ״����1�����ӽڵ㼴Ϊ���
            }
            // �������������nodeΪheaderʱ(�༴nodeΪend()ʱ)
            // ע�⣬header֮�ҽڵ㼴mostright��ָ����������max�ڵ�
            else if (node->left != nullptr)
            {                                        // ��������ӽڵ㡣״����2��
                base_ptr y = node->left;             // ��yָ�����ӽڵ�
                while (y->right != nullptr)          // ��y�����ӽڵ�ʱ
                    y = y->right;                    // һֱ�����ӽڵ��ߵ���
                node = y;                            // ���Ϊ��
            }
            else
            {                              // �ȷǸ��ڵ㣬���������ӽڵ�
                base_ptr y = node->parent; // ״����3���ҵ����ڵ�
                while (node == y->left)
                {                          // ����ýڵ�Ϊ���ڵ�����ӽڵ�
                    node = y;              // һֱ����ֱ����Ϊ���ӽڵ�
                    y = y->parent;
                }
                node = y;                  // ��ʱ���ڵ㼴Ϊ��
            }
        }

        bool operator==(const __rb_tree_iterator_base& rhs) { return node == rhs.node; }
        bool operator!=(const __rb_tree_iterator_base& rhs) { return node != rhs.node; }
    };

    template <class T, class Ref, class Ptr>
    struct __rb_tree_iterator : public __rb_tree_iterator_base
    {
        typedef T value_type;
        typedef Ref reference;
        typedef Ptr pointer;
        typedef __rb_tree_iterator<T, T&, T*> iterator;
        typedef __rb_tree_iterator<T, const T&, const T*> const_iterator;
        typedef __rb_tree_iterator<T, Ref, Ptr> self;
        typedef __rb_tree_node<T>* link_type;

        // ���캯��
        __rb_tree_iterator() {}
        __rb_tree_iterator(link_type x) { node = x; }
        __rb_tree_iterator(const iterator& rhs) { node = rhs.node; }

        // ���ز�����
        reference operator*() const { return ((link_type)node)->value; }
        pointer operator->() const { return &(operator*()); }

        self& operator++()
        {
            increment();
            return *this;
        }
        self operator++(int)
        {
            self tmp(*this);
            increment();
            return tmp;
        }
        self& operator--()
        {
            decrement();
            return *this;
        }
        self operator--(int)
        {
            self tmp(*this);
            decrement();
            return tmp;
        }
    };

    // �����ĸ�ȫ�ֺ��������ڵ��� rb tree �ڵ��λ������ɫ
    // ����������һΪ�����㣬������Ϊ���ڵ�
    inline void __rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base*& root)
    {
        auto y = x->right;  // ���� y Ϊ��ת������ӽڵ�
        x->right = y->left; // x �����ӽڵ�Ϊ y �����ӽڵ�
        if (y->left != nullptr)
            y->left->parent = x; // ���� y �����ӽڵ�ĸ��ڵ�Ϊ x
        y->parent = x->parent;   // ���� y �ĸ��ڵ�

        if (x == root)
        { // ��� x Ϊ���ڵ�
            root = y;
        }
        else if (x == x->parent->left)
        {                        // ��� x �������ڵ�����ӽڵ�
            x->parent->left = y; // ���� x �ĸ��ڵ�����ӽڵ�Ϊ y
        }
        else
        {                         // ��� x �������ڵ�����ӽڵ�
            x->parent->right = y; // ���� x �ĸ��ڵ�����ӽڵ�Ϊ y
        }
        y->left = x; // ���� x �� y �Ĺ�ϵ
        x->parent = y;
    }

    // ����������һΪ�����㣬������Ϊ���ڵ�
    inline void __rb_tree_rotate_right(__rb_tree_node_base* x, __rb_tree_node_base*& root)
    {
        auto y = x->left;   // ���� y Ϊ����������ӽڵ�
        x->left = y->right; // ���� x �����ӽڵ�Ϊ y �����ӽڵ�
        if (y->right != nullptr)
            y->right->parent = x; // ���� y ���ӽڵ�ĸ��ڵ�Ϊ x
        y->parent = x->parent;    // ���� y �ĸ��ڵ�Ϊ x �ĸ��ڵ�
        if (x == root)
        {             // ��� x Ϊ���ڵ�
            root = y; // ���ڵ����Ϊ y
        }
        else if (x == x->parent->right)
        {                         // ��� x �������ڵ�����ӽڵ�
            x->parent->right = y; // �� x �ĸ��ڵ�����ӽڵ����ó� y
        }
        else
        {                        // ��� x �������ڵ�����ӽڵ�
            x->parent->left = y; // �� x �ĸ��ڵ�����ӽڵ����ó� y
        }
        y->right = x; // ���� x �� y �Ĺ�ϵ
        x->parent = y;
    }

    // ����ʹ rb tree ƽ�⣬����һΪ�����ڵ㣬������Ϊ���ڵ�
    inline void __rb_tree_rebalance(__rb_tree_node_base* x, __rb_tree_node_base*& root)
    {
        x->color = __rb_tree_red; // �����ڵ�Ϊ��ɫ
        while (x != root && x->parent->color == __rb_tree_red)
        { // �����ڵ�Ϊ��
            if (x->parent == x->parent->parent->left)
            { // ���ڵ����游�ڵ�֮���ӽڵ�
                auto y = x->parent->parent->right;            // ��yΪ�����ڵ�
                if (y && y->color == __rb_tree_red)
                {                                             // �����ڵ������Ϊ��
                    x->parent->color = __rb_tree_black;       // ���ĸ��ڵ�Ϊ��
                    y->color = __rb_tree_black;               // ���Ĳ����ڵ�Ϊ��
                    x->parent->parent->color = __rb_tree_red; // �����游�ڵ�Ϊ��
                    x = x->parent->parent;
                }
                else
                { // �޲����ڵ�򲮸��ڵ�Ϊ��
                    if (x == x->parent->right)
                    { // ��� x Ϊ���ڵ�����ӽڵ�
                        x = x->parent;
                        __rb_tree_rotate_left(x, root);
                    }
                    x->parent->color = __rb_tree_black; // ������ɫ
                    x->parent->parent->color = __rb_tree_red;
                    __rb_tree_rotate_right(x->parent->parent, root);
                }
            }
            else
            { // ������ڵ������ӽڵ㣬�Գƴ���
                auto y = x->parent->parent->left;
                if (y && y->color == __rb_tree_red)
                {                                             // �����ڵ������Ϊ��
                    x->parent->color = __rb_tree_black;       // ���ĸ��ڵ�Ϊ��
                    y->color = __rb_tree_black;               // ���Ĳ����ڵ�Ϊ��
                    x->parent->parent->color = __rb_tree_red; // �����游�ڵ�Ϊ��
                    x = x->parent->parent;
                }
                else
                { // �޲����ڵ�򲮸��ڵ�Ϊ��
                    if (x == x->parent->left)
                    { // ���xΪ���ڵ�����ӽڵ�
                        x = x->parent;
                        __rb_tree_rotate_right(x, root);
                    }
                    x->parent->color = __rb_tree_black; // ������ɫ
                    x->parent->parent->color = __rb_tree_red;
                    __rb_tree_rotate_left(x->parent->parent, root);
                }
            }
        }
        root->color = __rb_tree_black; // ���ڵ���ԶΪ��
    }

    // ��ɾ���ڵ��Ҫʹ rb tree ����ƽ�⣬����һΪҪɾ���Ľڵ㣬������Ϊ���ڵ㣬
    // ������Ϊ��С�ڵ㣬������Ϊ���ڵ�
    inline __rb_tree_node_base* __rb_tree_rebalance_for_erase(__rb_tree_node_base* z,
        __rb_tree_node_base*& root, __rb_tree_node_base*& leftmost, __rb_tree_node_base*& rightmost)
    {
        auto y = z;
        __rb_tree_node_base* x = nullptr;
        __rb_tree_node_base* x_parent = nullptr;
        // Ѱ�ҿ��Դ���Ľڵ�
        if (y->left == nullptr)
        { // �����ɾ�ڵ�����ӽڵ�Ϊ��
            x = y->right;
        }
        else
        {
            if (y->right == nullptr)
            { // �����ɾ�ڵ�����ӽڵ�Ϊ��
                x = y->left;
            }
            else
            { // �����ɾ�ڵ�����ҽڵ㶼��Ϊ��
                y = y->right;
                while (y->left != nullptr) // �ӱ�ɾ�ڵ�����ӽڵ�һֱ�����ߣ��ҵ�Ҫ����Ľڵ�
                    y = y->left;
                x = y->right;
            }
        }
        // ��������ڵ�Ľڵ��ϵ
        if (y != z)
        {                        // y �任��
            z->left->parent = y; // �� y �����ɾ�ڵ�
            y->left = z->left;   // �����µ����ӽڵ�
            if (y != z->right)
            {
                x_parent = y->parent;
                if (x)
                    x->parent = y->parent; // ��������ڵ�����ӽڵ�ĸ��ӹ�ϵ
                y->parent->left = x;
                y->right = z->right; // �����µ����ӽڵ�
                z->right->parent = y;
            }
            else
            {
                x_parent = y;
            }
            if (root == z)
            {             // �����ɾ�ڵ�Ϊ���ڵ�
                root = y; // ������ڵ�Ϊ���ڵ�
            }
            else if (z->parent->left == z)
            { // ��������ڵ�ĸ��ӹ�ϵ
                z->parent->left = y;
            }
            else
            {
                z->parent->right = y;
            }
            y->parent = z->parent;
            myTinySTL::swap(y->color, z->color);
            y = z; // �� y ָ��Ҫɾ���Ľڵ�
        }
        else
        { // y == z��x Ϊ����ڵ�
            x_parent = y->parent;
            if (x)
                x->parent = y->parent;
            if (root == z)
            { // �����ɾ�ڵ�Ϊ���ڵ�
                root = x;
            }
            else
            { // ��������ڵ�ĸ��ӹ�ϵ
                if (z->parent->left == z)
                    z->parent->left = x;
                else
                    z->parent->right = x;
            }
            if (leftmost == z)
            { // ��ɾ�ڵ�Ϊ��С�ڵ㣬������Сֵ
                if (z->right == nullptr)
                    leftmost = z->parent;
                else
                    leftmost = __rb_tree_node_base::minimum(x);
            }
            if (rightmost == z)
            { // ��ɾ�ڵ�Ϊ���ڵ㣬�������ֵ
                if (z->left == nullptr)
                    rightmost = z->parent;
                else
                    rightmost = __rb_tree_node_base::maximum(x);
            }
        }
        // �����ڵ���ɫ
        if (y->color != __rb_tree_red)
        { // �����ɾ�ڵ���ɫΪ��
            while (x != root && (x == nullptr || x->color == __rb_tree_black))
            {
                if (x == x_parent->left)
                { // �������ڵ������ӽڵ�
                    auto w = x_parent->right;
                    if (w->color == __rb_tree_red)
                    {
                        w->color = __rb_tree_black;
                        x_parent->color = __rb_tree_red;
                        __rb_tree_rotate_left(x_parent, root);
                        w = x_parent->right;
                    }
                    if ((w->left == nullptr || w->left->color == __rb_tree_black) &&
                        (w->right == nullptr || w->right->color == __rb_tree_black))
                    {
                        w->color = __rb_tree_red;
                        x = x_parent;
                        x_parent = x_parent->parent;
                    }
                    else
                    {
                        if (w->right == nullptr || w->right->color == __rb_tree_black)
                        {
                            if (w->left)
                                w->left->color = __rb_tree_black;
                            w->color = __rb_tree_red;
                            __rb_tree_rotate_right(w, root);
                            w = x_parent->right;
                        }
                        w->color = x_parent->color;
                        x_parent->color = __rb_tree_black;
                        if (w->right)
                            w->right->color = __rb_tree_black;
                        __rb_tree_rotate_left(x_parent, root);
                        break;
                    }
                }
                else
                { // �������ڵ�Ϊ���ӽڵ㣬�Գƴ���
                    auto w = x_parent->left;
                    if (w->color == __rb_tree_red)
                    {
                        w->color = __rb_tree_black;
                        x_parent->color = __rb_tree_red;
                        __rb_tree_rotate_right(x_parent, root);
                        w = x_parent->left;
                    }
                    if ((w->left == nullptr || w->left->color == __rb_tree_black) &&
                        (w->right == nullptr || w->right->color == __rb_tree_black))
                    {
                        w->color = __rb_tree_red;
                        x = x_parent;
                        x_parent = x_parent->parent;
                    }
                    else
                    {
                        if (w->left == nullptr || w->left->color == __rb_tree_black)
                        {
                            if (w->right)
                                w->right->color = __rb_tree_black;
                            w->color = __rb_tree_red;
                            __rb_tree_rotate_left(w, root);
                            w = x_parent->left;
                        }
                        w->color = x_parent->color;
                        x_parent->color = __rb_tree_black;
                        if (w->left)
                            w->left->color = __rb_tree_black;
                        __rb_tree_rotate_right(x_parent, root);
                        break;
                    }
                }
            }
            if (x)
                x->color = __rb_tree_black;
        }
        return y;
    }

    // ģ���� rb_tree
    // ����һ�����ֵ���ͣ�����������ʵֵ���ͣ�����������ʵֵת���ɼ�ֵ�ķ�ʽ��ȱʡʹ�� mystl �� identity
    // �����Ĵ����ֵ�Ƚϵķ�����ȱʡʹ�� mystl �� less����������Ϊ�ռ������������ͣ�ȱʡʹ�� mystl �� alloc
    template <class Key, class Value, class KeyOfValue = myTinySTL::identity<Key>,
        class Compare = myTinySTL::less<Key>, class Alloc = myTinySTL::allocator<__rb_tree_node<Value>>>
        class rb_tree
    {
    private:
        typedef __rb_tree_node_base* base_ptr;
        typedef __rb_tree_node<Value> rb_tree_node;
        typedef __rb_tree_color_type color_type;

    public:
        // rb tree ��Ƕ���ͱ���
        typedef Alloc allocator_type;
        typedef Key key_type;
        typedef Value value_type;
        typedef value_type* pointer;
        typedef const value_type* const_pointer;
        typedef value_type& reference;
        typedef const value_type& const_reference;
        typedef rb_tree_node* link_type;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        typedef __rb_tree_iterator<Value, Value&, Value*> iterator;
        typedef __rb_tree_iterator<Value, const Value&, const Value*> const_iterator;
        typedef myTinySTL::reverse_iterator_t<iterator> reverse_iterator;
        typedef myTinySTL::reverse_iterator_t<const_iterator> const_reverse_iterator;

        typedef myTinySTL::allocator<rb_tree_node> rb_tree_node_allocator;
        allocator_type get_allocator() const { return allocator_type(); }

    private:
        // �������������ݱ��� rb tree
        size_type node_count_; // rb tree �Ľڵ���Ŀ
        link_type header_;     // ����ڵ㣬����ڵ㻥Ϊ�Է��ĸ��ڵ�
        Compare key_compare;   // �ڵ��ֵ�Ƚϵ�׼��

    private:
        // �ڵ���ز���
        link_type __get_node() { return rb_tree_node_allocator::allocate(1); }
        void __put_node(link_type p) { rb_tree_node_allocator::deallocate(p, 1); }
        link_type __create_node(const value_type& x);
        link_type __clone_node(link_type x);
        void __destroy_node(link_type p);

    private:
        // ����������������ȡ�ø��ڵ㣬��С�ڵ�����ڵ�
        link_type& root() const { return reinterpret_cast<link_type&>(header_->parent); }
        link_type& leftmost() const { return reinterpret_cast<link_type&>(header_->left); }
        link_type& rightmost() const { return reinterpret_cast<link_type&>(header_->right); }

        // ����������������ȡ�ýڵ�ĳ�Ա
        static link_type& left(link_type x) { return reinterpret_cast<link_type&>(x->left); }
        static link_type& right(link_type x) { return reinterpret_cast<link_type&>(x->right); }
        static link_type& parent(link_type x) { return reinterpret_cast<link_type&>(x->parent); }
        static reference value(link_type x) { return x->value; }
        static const key_type& key(link_type x) { return KeyOfValue()(value(x)); }
        static color_type& color(link_type x) { return x->color; }

        // ����������������ȡ�û���ڵ�ĳ�Ա
        static link_type& left(base_ptr x) { return reinterpret_cast<link_type&>(x->left); }
        static link_type& right(base_ptr x) { return reinterpret_cast<link_type&>(x->right); }
        static link_type& parent(base_ptr x) { return reinterpret_cast<link_type&>(x->parent); }
        static reference value(base_ptr x) { return reinterpret_cast<link_type>(x)->value; }
        static const key_type& key(base_ptr x) { return KeyOfValue()(value(reinterpret_cast<link_type>(x))); }
        static color_type& color(base_ptr x) { return reinterpret_cast<link_type&>(x->color); }

        // ����Сֵ�����ֵ
        static link_type minimum(link_type x)
        {
            return reinterpret_cast<link_type>(__rb_tree_node_base::minimum(x));
        }
        static link_type maximum(link_type x)
        {
            return reinterpret_cast<link_type>(__rb_tree_node_base::maximum(x));
        }

    public:
        // ���졢���ơ���������
        rb_tree() { __rb_tree_initialize(); }

        rb_tree(const rb_tree& rhs);
        rb_tree(rb_tree&& rhs);

        rb_tree& operator=(const rb_tree& rhs);
        rb_tree& operator=(rb_tree&& rhs);

        ~rb_tree() { clear(); }

    public:
        // ��������ز���
        iterator begin() { return leftmost(); } // rb������ͷΪ����(��С)�ڵ㴦
        const_iterator begin() const { return leftmost(); }
        iterator end() { return header_; } // rb�����յ�Ϊheader��ָ��
        const_iterator end() const { return header_; }
        reverse_iterator rbegin() { return reverse_iterator(end()); }
        const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
        reverse_iterator rend() { return reverse_iterator(begin()); }
        const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

        // ������ز���
        bool empty() const { return node_count_ == 0; }
        size_type size() const { return node_count_; }
        size_type max_size() const { return static_cast<size_type>(-1); }

        // ����ɾ����ز���
        iterator insert_equal(const value_type& value); // ����ڵ�ֵ�ظ�
        iterator insert_equal(iterator position, const value_type& value);
        template <class InputIterator>
        void insert_equal(InputIterator first, InputIterator last);
        pair<iterator, bool> insert_unique(const value_type& value); // ���ֽڵ�ֵ��һ�޶�
        iterator insert_unique(iterator position, const value_type& value);
        template <class InputIterator>
        void insert_unique(InputIterator first, InputIterator last);
        void erase(iterator position);
        size_type erase(const key_type& k);
        void erase(iterator first, iterator last);
        void clear();

        // rb_tree ��ز���
        Compare key_comp() const { return key_compare; }
        iterator find(const key_type& k);
        const_iterator find(const key_type& k) const;
        size_type count(const key_type& k) const;
        iterator lower_bound(const key_type& k);
        const_iterator lower_bound(const key_type& k) const;
        iterator upper_bound(const key_type& k);
        const_iterator upper_bound(const key_type& k) const;
        pair<iterator, iterator> equal_range(const key_type& k);
        pair<const_iterator, const_iterator> equal_range(const key_type& k) const;
        void swap(rb_tree& rhs);

    private:
        // rb_tree ��˽�г�Ա����
        void __rb_tree_initialize();
        link_type __copy(link_type x, link_type p);
        iterator __insert(base_ptr x_, base_ptr y_, const value_type& value);
        void __erase(link_type x);
    };

    /*****************************************************************************************/

    // ����һ�����
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::link_type
        rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__create_node(const value_type& x)
    {
        auto tmp = __get_node(); // ���ÿռ�
        try
        {
            myTinySTL::construct(&tmp->value, x);
        }
        catch (...)
        {
            __put_node(tmp);
        }
        return tmp;
    }

    // ����һ�����
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::link_type
        rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__clone_node(link_type x)
    {
        auto tmp = __create_node(x->value);
        tmp->color = x->color;
        tmp->left = nullptr;
        tmp->right = nullptr;
        return tmp;
    }

    // ����һ�����
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__destroy_node(link_type p)
    {
        myTinySTL::destroy(&p->value); // ��������
        __put_node(p);  // �ͷ��ڴ�
    }

    // �������캯��
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree(const rb_tree& rhs)
    {
        __rb_tree_initialize();
        if (rhs.root() != nullptr)
        {
            color(header_) = __rb_tree_red;
            root() = __copy(rhs.root(), header_);
            leftmost() = minimum(root());
            rightmost() = maximum(root());
        }
        node_count_ = rhs.node_count_;
        key_compare = rhs.key_compare;
    }

    // �ƶ����캯��
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree(rb_tree&& rhs)
    {
        __rb_tree_initialize();
        if (rhs.root() != nullptr)
        {
            color(header_) = __rb_tree_red;
            root() = __copy(rhs.root(), header_);
            leftmost() = minimum(root());
            rightmost() = maximum(root());
            rhs.root() = nullptr;
            rhs.leftmost() = rhs.header_;
            rhs.rightmost() = rhs.header_;
            rhs.node_count_ = 0;
        }
        node_count_ = rhs.node_count_;
        key_compare = rhs.key_compare;
    }

    // ���Ƹ�ֵ������
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>&
        rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::operator=(const rb_tree& rhs)
    {
        if (this != &rhs)
        {
            if (root() == nullptr)
                __rb_tree_initialize();
            clear();
            if (rhs.root() != nullptr)
            {
                root() = __copy(rhs.root(), header_);
                leftmost() = minimum(root());
                rightmost() = maximum(root());
            }
            node_count_ = rhs.node_count_;
            key_compare = rhs.key_compare;
        }
        return *this;
    }

    // �ƶ���ֵ������
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>&
        rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::operator=(rb_tree&& rhs)
    {
        if (this != &rhs)
        {
            if (root() == nullptr)
                __rb_tree_initialize();
            clear();
            if (rhs.root() != nullptr)
            {
                root() = __copy(rhs.root(), header_);
                leftmost() = minimum(root());
                rightmost() = maximum(root());
                node_count_ = rhs.node_count_;
                rhs.root() = nullptr;
                rhs.leftmost() = rhs.header_;
                rhs.rightmost() = rhs.header_;
                rhs.node_count_ = 0;
            }
            node_count_ = rhs.node_count_;
            key_compare = rhs.key_compare;
        }
        return *this;
    }

    // ������ֵ���ڵ��ֵ�����ظ�
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
        rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::
        insert_equal(const value_type& value)
    {
        auto y = header_;
        auto x = root();
        while (x != nullptr)
        { //�Ӹ��ڵ㿪ʼ�����������ʵĲ����
            y = x;
            x = key_compare(KeyOfValue()(value), key(x)) ? left(x) : right(x); // С��������
        }
        return __insert(x, y, value);
    }

    // �� position λ�ÿ�ʼ������ֵ���ڵ��ֵ�����ظ�
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
        rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::
        insert_equal(iterator position, const value_type& value)
    {
        if (position.node == header_->left)
        { // λ�� begin ��
            if (size() > 0 && !key_compare(key(position.node), KeyOfValue()(value)))
                // position �ڵ��ֵ���ڵ�����ֵ��ֵ
                return __insert(position.node, position.node, value);
            else
                return insert_equal(value); // Ѱ�Һ��ʵĲ����
        }
        else if (position.node == header_)
        { // λ�� end ��
            if (!key_compare(KeyOfValue()(value), key(rightmost())))
                // ��ֵ��ֵ���ڵ������ڵ��ֵ
                return __insert(static_cast<base_ptr>(nullptr), rightmost(), value);
            else
                return insert_equal(value); // Ѱ�Һ��ʵĲ����
        }
        else
        {
            auto before = position;
            --before;
            if (!key_compare(KeyOfValue()(value), key(before.node)) &&
                !key_compare(key(position.node), KeyOfValue()(value)))
            {
                // before <= value <= position
                if (right(before.node) == nullptr)
                    return __insert(static_cast<base_ptr>(nullptr), before.node, value);
                else
                    return __insert(position.node, position.node, value);
            }
            else
            {
                return insert_equal(value); // Ѱ�Һ��ʵĲ����
            }
        }
    }

    // ����[first, last)�����ڵ�ֵ����ֵ�����ظ�
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    template <class InputIterator>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(InputIterator first, InputIterator last)
    {
        for (; first != last; ++first)
            insert_equal(*first);
    }

    // ������ֵ���ڵ��ֵ�������ظ�������һ�� pair��������ɹ���pair �ĵڶ�����Ϊ true������Ϊ false
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool>
        rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type& value)
    {
        auto y = header_;
        auto x = root();
        bool comp = true;
        while (x != nullptr)
        {
            y = x;
            comp = key_compare(KeyOfValue()(value), key(x)); // value ��ֵС�ڵ�ǰ�ڵ��ֵ
            x = comp ? left(x) : right(x);                   // С�������ߣ�����������
        }
        iterator j = iterator(y); // ��ʱ y Ϊ�����ĸ��ڵ�
        if (comp)
        {                     // �뿪ѭ��ʱ comp Ϊ true���������
            if (j == begin()) // ���ڵ�Ϊ����ڵ�
                return pair<iterator, bool>(__insert(x, y, value), true);
            else
                --j;
        }
        if (key_compare(key(j.node), KeyOfValue()(value))) // �½ڵ�û���ظ�
            return pair<iterator, bool>(__insert(x, y, value), true);
        // �������ˣ���ʾ�½ڵ������нڵ��ֵ�ظ�
        return pair<iterator, bool>(j, false);
    }

    // �� position ��ʼ������ֵ���ڵ��ֵ�������ظ�
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
        rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(iterator position,
            const value_type& value)
    {
        if (position.node == header_->left)
        {                                                                       // λ�� begin ��
            if (size() && key_compare(KeyOfValue()(value), key(position.node))) // ��ֵ��ֵС�� position �ڵ�ļ�ֵ
                return __insert(position.node, position.node, value);
            else
                return insert_unique(value).first; // Ѱ�Һ��ʵĲ����
        }
        else if (position.node == header_)
        {
            if (key_compare(key(rightmost()), KeyOfValue()(value))) // ���ڵ��ֵС����ֵ��ֵ
                return __insert(static_cast<base_ptr>(nullptr), rightmost(), value);
            else
                return insert_unique(value).first; // Ѱ�Һ��ʵĲ����
        }
        else
        {
            auto before = position;
            --before;
            if (key_compare(key(before.node), KeyOfValue()(value)) &&
                key_compare(KeyOfValue()(value), key(position.node)))
            {
                // before < value < position
                if (right(before.node) == nullptr)
                    return __insert(static_cast<base_ptr>(nullptr), before.node, value);
                else
                    return __insert(position.node, position.node, value);
            }
            else
            {
                return insert_unique(value).first; // Ѱ�Һ��ʵĲ����
            }
        }
    }

    // ����[first, last)�ڵ�ֵ����ֵ�������ظ�
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    template <class InputIterator>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(InputIterator first, InputIterator last)
    {
        for (; first != last; ++first)
            insert_unique(*first);
    }

    // ɾ�� position λ�õĽڵ�
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator position)
    {
        auto y = reinterpret_cast<link_type>(__rb_tree_rebalance_for_erase(position.node,
            header_->parent, header_->left, header_->right));
        __destroy_node(y);
        --node_count_;
    }

    // ɾ���� x ��ֵ��ȵ�Ԫ�أ�����ɾ���ĸ���
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type
        rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(const key_type& x)
    {
        auto p = equal_range(x); // �ҳ��� x ��ֵ��ȵ�����
        auto n = static_cast<size_type>(distance(p.first, p.second));
        erase(p.first, p.second);
        return n;
    }

    // ɾ��[first, last)�����ڵ�Ԫ��
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator first, iterator last)
    {
        if (first == begin() && last == end())
        {
            clear();
        }
        else
        {
            while (first != last)
                erase(first++);
        }
    }

    // ��� rb tree
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::clear()
    {
        if (node_count_ != 0)
        {
            __erase(root());
            leftmost() = header_;
            root() = nullptr;
            rightmost() = header_;
            node_count_ = 0;
        }
    }

    // ���Ҽ�ֵΪ k �Ľڵ㣬����ָ�����ĵ�����
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
        rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const key_type& k)
    {
        auto y = header_; // ���һ����С�� k �Ľڵ�
        auto x = root();
        while (x != nullptr)
        {
            if (!key_compare(key(x), k))
            { // k С�ڵ��� x ��ֵ��������
                y = x, x = left(x);
            }
            else
            { // k ���� x ��ֵ��������
                x = right(x);
            }
        }
        iterator j = iterator(y);
        return (j == end() || key_compare(k, key(j.node))) ? end() : j;
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
        rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const key_type& k) const
    {
        auto y = header_; // ���һ����С�� k �Ľڵ�
        auto x = root();
        while (x != nullptr)
        {
            if (!key_compare(key(x), k))
            { // k С�ڵ��� x ��ֵ��������
                y = x, x = left(x);
            }
            else
            { // k ���� x ��ֵ��������
                x = right(x);
            }
        }
        const_iterator j = const_iterator(y);
        return (j == end() || key_compare(k, key(j.node))) ? end() : j;
    }

    // ���Ҽ�ֵΪ k �Ľڵ���ֵĴ���
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type
        rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::count(const key_type& k) const
    {
        auto p = equal_range(k);
        auto n = static_cast<size_type>(distance(p.first, p.second));
        return n;
    }

    // ��ֵ��С�� k �ĵ�һ��λ��
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
        rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const key_type& k)
    {
        auto y = header_;
        auto x = root();
        while (x != nullptr)
        {
            if (!key_compare(key(x), k))
            { // k <= x
                y = x, x = left(x);
            }
            else
            {
                x = right(x);
            }
        }
        return iterator(y);
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
        rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const key_type& k) const
    {
        auto y = header_;
        auto x = root();
        while (x != nullptr)
        {
            if (!key_compare(key(x), k))
            { // k <= x
                y = x, x = left(x);
            }
            else
            {
                x = right(x);
            }
        }
        return const_iterator(y);
    }

    // ��ֵ��С�� k �����һ��λ��
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
        rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const key_type& k)
    {
        auto y = header_;
        auto x = root();
        while (x != nullptr)
        {
            if (key_compare(k, key(x)))
            { // k < x
                y = x, x = left(x);
            }
            else
            {
                x = right(x);
            }
        }
        return iterator(y);
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
        rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const key_type& k) const
    {
        auto y = header_;
        auto x = root();
        while (x != nullptr)
        {
            if (key_compare(k, key(x)))
            { // k < x
                y = x, x = left(x);
            }
            else
            {
                x = right(x);
            }
        }
        return const_iterator(y);
    }

    // �������ֵ k ��ȵ����䣬����һ�� pair �ֱ�ָ������������β
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    inline pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator,
        typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator>
        rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::equal_range(const key_type& k)
    {
        return pair<iterator, iterator>(lower_bound(k), upper_bound(k));
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    inline pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator,
        typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator>
        rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::equal_range(const key_type& k) const
    {
        return pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k));
    }

    // ���� rb tree
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::swap(rb_tree& rhs)
    {
        myTinySTL::swap(header_, rhs.header_);
        myTinySTL::swap(node_count_, rhs.node_count_);
        myTinySTL::swap(key_compare, rhs.key_compare);
    }

    // __rb_tree_initialize ����
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__rb_tree_initialize()
    {
        header_ = __get_node();      // ����һ���սڵ㣬��headerָ����
        color(header_) = __rb_tree_red; // header�ڵ���ɫΪ�죬��root����
        root() = nullptr;
        leftmost() = header_;
        rightmost() = header_;
        node_count_ = 0;
    }

    // __copy ����
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::link_type
        rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__copy(link_type x, link_type p)
    {
        auto top = __clone_node(x);
        top->parent = p;
        try
        {
            if (x->right)
                top->right = __copy(right(x), top);
            p = top;
            x = left(x);
            while (x != nullptr)
            {
                auto y = __clone_node(x);
                p->left = y;
                y->parent = p;
                if (x->right)
                    y->right = __copy(right(x), y);
                p = y;
                x = left(x);
            }
        }
        catch (...)
        {
            __erase(top);
        }
        return top;
    }

    // __insert ����
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
        rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::
        __insert(base_ptr x_, base_ptr y_, const value_type& value)
    {
        // x_ Ϊ��ֵ����㣬y_ Ϊ�����ĸ��ڵ㣬value Ϊ��ֵ
        auto x = reinterpret_cast<link_type>(x_);
        auto y = reinterpret_cast<link_type>(y_);
        link_type z;
        if (y == header_ || x != nullptr || key_compare(KeyOfValue()(value), key(y)))
        {
            z = __create_node(value);
            left(y) = z; // ʹ�� y Ϊ header_ ʱ��leftmost() = z
            if (y == header_)
            {
                root() = z;
                rightmost() = z;
            }
            else if (y == leftmost())
            {                   // ��� y Ϊ����ڵ�
                leftmost() = z; // ά�� leftmost(),ʹ����Զָ������ڵ�
            }
        }
        else
        {
            z = __create_node(value);
            right(y) = z; // ���½ڵ��Ϊ����㸸�ڵ�����ӽڵ�
            if (y == rightmost())
                rightmost() = z; // ά�� rightmost(),ʹ����Զָ�����ҽڵ�
        }
        parent(z) = y;      // �����½ڵ�ĸ��ڵ�
        left(z) = nullptr;  // �����½ڵ�����ӽڵ�
        right(z) = nullptr; // �����½ڵ�����ӽڵ�
        __rb_tree_rebalance(z, header_->parent);
        ++node_count_;
        return iterator(z);
    }

    // __erase ����
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__erase(link_type x)
    {
        while (x != nullptr)
        {
            __erase(right(x));
            auto y = left(x);
            __destroy_node(x);
            x = y;
        }
    }

    // ���رȽϲ�����
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    inline bool operator==(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& lhs,
        const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& rhs)
    {
        return lhs.size() == rhs.size() && myTinySTL::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    inline bool operator<(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& lhs,
        const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& rhs)
    {
        return myTinySTL::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    inline bool operator!=(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& lhs,
        const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& rhs)
    {
        return !(lhs == rhs);
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    inline bool operator>(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& lhs,
        const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& rhs)
    {
        return rhs < lhs;
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    inline bool operator<=(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& lhs,
        const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& rhs)
    {
        return !(rhs < lhs);
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    inline bool operator>=(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& lhs,
        const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& rhs)
    {
        return !(lhs < rhs);
    }

    // ����swap
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    inline void swap(rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& lhs,
        rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& rhs)
    {
        lhs.swap(rhs);
    }

}


#endif // !_RB_TREE_H_


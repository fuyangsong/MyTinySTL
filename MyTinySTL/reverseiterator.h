#pragma once
#ifndef _REVERSEITERATOR_H_
#define _REVERSEITERATOR_H_

#include "iterator.h"

namespace myTinySTL 
{
	template<class Iterator>
	class reverse_iterator_t 
	{
	public:
		typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
		typedef typename iterator_traits<Iterator>::value_type        value_type;
		typedef typename iterator_traits<Iterator>::difference_type   difference_type;
		typedef typename iterator_traits<Iterator>::pointer           pointer;
		typedef typename iterator_traits<Iterator>::reference         reference;

		typedef const pointer                                         const_pointer;
		typedef const reference                                       const_reference;

		typedef Iterator                                              iterator_type;
		typedef reverse_iterator_t<Iterator>                          self;
	private:
		Iterator base_; // 记录对应之正向迭代器
		Iterator cur_; // 反向迭代器比正向迭代器小1
	public:
		//构造。复制，析构相关
		reverse_iterator_t() :base_(0), cur_(0) {}
		
		explicit reverse_iterator_t(const iterator_type& it) :base_(it) 
		{
			auto temp = it;
			cur_ = --temp;
		}

		reverse_iterator_t(const self& rev_it) 
		{
			base_ = (iterator_type)rev_it.base();
			auto temp = base_;
			cur_ = --temp;
		};

		// 取出对应的正向迭代器
		iterator_type base() { return base_; }
		reference operator*() { return (*cur_); }
		const_reference operator*()const { return(*cur_); }
		pointer operator->() { return &(operator *()); }
		const_pointer operator->()const { return &(operator*()); }
		self& operator ++() { // 前置++
			--base_;
			--cur_;
			return *this;
		}
		self operator ++(int) { // 后置++
			self temp = *this;
			--base_;
			--cur_;
			return temp;
		}
		self& operator--() { // 前置--
			++base_;
			++cur_;
			return *this;
		}
		self operator--(int) { // 后置--
			self temp = *this;
			++base_;
			++cur_;
			return temp;
		}
		reference operator[] (difference_type n) {
			return *(*this + n);
		}
		self operator + (difference_type n)const;
		self& operator += (difference_type n);
		self operator - (difference_type n)const;
		self& operator -= (difference_type n);
	private:
		Iterator advanceNStep(Iterator it,
			difference_type n,
			bool right,//true -> \ false <-
			random_access_iterator_tag) 
		{
			if (right) {
				it += n;
			}
			else {
				it -= n;
			}
			return it;
		}
		
		Iterator advanceNStep(Iterator it,
			difference_type n,
			bool right,//true -> \ false <-
			bidirectional_iterator_tag) 
		{
			difference_type i;
			difference_type absN = n >= 0 ? n : -n;
			if ((right && n > 0) || (!right && n < 0)) {// ->
				for (i = 0; i != absN; ++i) {
					it = it + 1;
				}
			}
			else if ((!right && n > 0) || (right && n < 0)) {// <-
				for (i = 0; i != absN; ++i) {
					it = it - 1;
				}
			}
			return it;
		}
	public:
		template <class Iterator>
		friend bool operator == (const reverse_iterator_t<Iterator>& lhs, 
			                     const reverse_iterator_t<Iterator>& rhs);
		
		template <class Iterator>
		friend bool operator != (const reverse_iterator_t<Iterator>& lhs,
			                     const reverse_iterator_t<Iterator>& rhs);
		
		template <class Iterator>
		friend bool operator < (const reverse_iterator_t<Iterator>& lhs,
			                    const reverse_iterator_t<Iterator>& rhs);
		
		template <class Iterator>
		friend bool operator <= (const reverse_iterator_t<Iterator>& lhs,
			                     const reverse_iterator_t<Iterator>& rhs);
		
		template <class Iterator>
		friend bool operator > (const reverse_iterator_t<Iterator>& lhs,
			                    const reverse_iterator_t<Iterator>& rhs);
		
		template <class Iterator>
		friend bool operator >= (const reverse_iterator_t<Iterator>& lhs,
			                     const reverse_iterator_t<Iterator>& rhs);

		template <class Iterator>
		friend reverse_iterator_t<Iterator> operator + (
			typename reverse_iterator_t<Iterator>::difference_type n,
			const reverse_iterator_t<Iterator>& rev_it);
		
		template <class Iterator>
		friend typename reverse_iterator_t<Iterator>::difference_type operator- (
			const reverse_iterator_t<Iterator>& lhs,
			const reverse_iterator_t<Iterator>& rhs);
	};// end of reverse_iterator_t

	template<class Iterator>
	reverse_iterator_t<Iterator>& reverse_iterator_t<Iterator>::operator += (difference_type n) 
	{
		base_ = advanceNStep(base_, n, false, iterator_category());
		cur_ = advanceNStep(cur_, n, false, iterator_category());
		return *this;
	}

	template<class Iterator>
	reverse_iterator_t<Iterator>& reverse_iterator_t<Iterator>::operator -= (difference_type n) 
	{
		base_ = advanceNStep(base_, n, true, iterator_category());
		cur_ = advanceNStep(cur_, n, true, iterator_category());
		return *this;
	}

	template<class Iterator>
	reverse_iterator_t<Iterator> reverse_iterator_t<Iterator>::operator + (difference_type n)const 
	{
		reverse_iterator_t<Iterator> res = *this;
		res += n;
		return res;
	}
	template<class Iterator>
	reverse_iterator_t<Iterator> reverse_iterator_t<Iterator>::operator - (difference_type n)const 
	{
		reverse_iterator_t<Iterator> res = *this;
		res -= n;
		return res;
	}

	template <class Iterator>
	bool operator == (const reverse_iterator_t<Iterator>& lhs, const reverse_iterator_t<Iterator>& rhs) 
	{
		return lhs.cur_ == rhs.cur_;
	}

	template <class Iterator>
	bool operator != (const reverse_iterator_t<Iterator>& lhs, const reverse_iterator_t<Iterator>& rhs) 
	{
		return !(lhs == rhs);
	}
	template <class Iterator>
	bool operator < (const reverse_iterator_t<Iterator>& lhs, const reverse_iterator_t<Iterator>& rhs) 
	{
		return lhs.cur_ < rhs.cur_;
	}

	template <class Iterator>
	bool operator > (const reverse_iterator_t<Iterator>& lhs, const reverse_iterator_t<Iterator>& rhs) 
	{
		return lhs.cur_ > rhs.cur_;
	}

	template <class Iterator>
	bool operator >= (const reverse_iterator_t<Iterator>& lhs, const reverse_iterator_t<Iterator>& rhs) 
	{
		return !(lhs < rhs);
	}

	template <class Iterator>
	bool operator <= (const reverse_iterator_t<Iterator>& lhs, const reverse_iterator_t<Iterator>& rhs) 
	{
		return !(lhs > rhs);
	}

	template <class Iterator>
	reverse_iterator_t<Iterator> operator + (
		typename reverse_iterator_t<Iterator>::difference_type n,
		const reverse_iterator_t<Iterator>& rev_it) 
	{
		return rev_it + n;
	}

	template <class Iterator>
	typename reverse_iterator_t<Iterator>::difference_type operator - (
		const reverse_iterator_t<Iterator>& lhs,
		const reverse_iterator_t<Iterator>& rhs) 
	{
		return lhs.cur_ - rhs.cur_;
	}
}

#endif // !_REVERSEITERATOR_H_


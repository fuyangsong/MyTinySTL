#pragma once
#ifndef _TYPE_TRAITS_H_
#define _TYPE_TRAITS_H_

// 用于类型萃取，定义三个类型萃取模板
// _type_traits
// _is_integer
// _char_type

namespace myTinySTL 
{

	struct _true_type { };
	struct _false_type { };

	/*
	** 萃取传入的T类型的类型特性
	*/
	template<class T>
	struct _type_traits
	{
		typedef _false_type		has_trivial_default_constructor;
		typedef _false_type		has_trivial_copy_constructor;
		typedef _false_type		has_trivial_assignment_operator;
		typedef _false_type		has_trivial_destructor;
		typedef _false_type		is_POD_type; // POD: Plain Old Data
	};

	template<>
	struct _type_traits<bool>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	template<>
	struct _type_traits<char>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	template<>
	struct _type_traits<unsigned char>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	template<>
	struct _type_traits<signed char>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	template<>
	struct _type_traits<wchar_t>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	template<>
	struct _type_traits<short>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	template<>
	struct _type_traits<unsigned short>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	template<>
	struct _type_traits<int>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	template<>
	struct _type_traits<unsigned int>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	template<>
	struct _type_traits<long>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	template<>
	struct _type_traits<unsigned long>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	template<>
	struct _type_traits<long long>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	template<>
	struct _type_traits<unsigned long long>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	template<>
	struct _type_traits<float>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	template<>
	struct _type_traits<double>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	template<>
	struct _type_traits<long double>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	template<class T>
	struct _type_traits<T*>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	template<class T>
	struct _type_traits<const T*>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	template<>
	struct _type_traits<char*>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	template<>
	struct _type_traits<unsigned char*>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	template<>
	struct _type_traits<signed char*>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	template<>
	struct _type_traits<const char*>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	template<>
	struct _type_traits<const unsigned char*>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	template<>
	struct _type_traits<const signed char*>
	{
		typedef _true_type		has_trivial_default_constructor;
		typedef _true_type		has_trivial_copy_constructor;
		typedef _true_type		has_trivial_assignment_operator;
		typedef _true_type		has_trivial_destructor;
		typedef _true_type		is_POD_type;
	};

	/*****************************************************************************************/
	// _is_integer
	// 整型型别萃取
	template <class T>
	struct _is_integer
	{
		typedef _false_type is_integer;
	};

	template <>
	struct _is_integer<bool>
	{
		typedef _true_type is_integer;
	};

	template <>
	struct _is_integer<char>
	{
		typedef _true_type is_integer;
	};

	template <>
	struct _is_integer<signed char>
	{
		typedef _true_type is_integer;
	};

	template <>
	struct _is_integer<unsigned char>
	{
		typedef _true_type is_integer;
	};

	template <>
	struct _is_integer<short>
	{
		typedef _true_type is_integer;
	};

	template <>
	struct _is_integer<unsigned short>
	{
		typedef _true_type is_integer;
	};

	template <>
	struct _is_integer<int>
	{
		typedef _true_type is_integer;
	};

	template <>
	struct _is_integer<unsigned int>
	{
		typedef _true_type is_integer;
	};

	template <>
	struct _is_integer<long>
	{
		typedef _true_type is_integer;
	};

	template <>
	struct _is_integer<unsigned long>
	{
		typedef _true_type is_integer;
	};

	template <>
	struct _is_integer<long long>
	{
		typedef _true_type is_integer;
	};

	template <>
	struct _is_integer<unsigned long long>
	{
		typedef _true_type is_integer;
	};

	/*****************************************************************************************/
	// _char_type
	// 字符型型别萃取

	struct char_type
	{
	};
	struct wchar_type
	{
	};

	template <class T>
	struct _char_type
	{
		typedef _false_type value_type;
	};

	template <>
	struct _char_type<char>
	{
		typedef char_type value_type;
	};

	template <>
	struct _char_type<wchar_t>
	{
		typedef wchar_type value_type;
	};
}


#endif // !_TYPE_TRAITS_H_


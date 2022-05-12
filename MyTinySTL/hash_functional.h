#pragma once
#ifndef _HASH_FUNCTIONAL_H_
#define _HASH_FUNCTIONAL_H_

#include <cstddef>

#include <string>

namespace myTinySTL
{
    //�󲿷����ͣ�hash����Ϊ��ɶҲ����
    template <class Key>
    struct hash
    {
    };

    inline size_t __hash_string(const char* s)
    {
        size_t h = 0;
        for (; *s; ++s)
        {
            h = 3 * h + static_cast<size_t>(*s);
        }
        return h;
    }

    template <>
    struct hash<char*>
    {
        size_t operator()(const char* s) const { return __hash_string(s); }
    };

    template <>
    struct hash<const char*>
    {
        size_t operator()(const char* s) const { return __hash_string(s); }
    };

    // ��� mystl::string ���ػ��汾
    template <>
    struct hash<std::string>
    {
        size_t operator()(const std::string& str) const { return __hash_string(str.data()); }
    };

    // ��� char��int��long �������ͱ�ֻ�Ƿ���ԭֵ

    template <>
    struct hash<char>
    {
        size_t operator()(char x) const { return x; }
    };

    template <>
    struct hash<unsigned char>
    {
        size_t operator()(unsigned char x) const { return x; }
    };

    template <>
    struct hash<signed char>
    {
        size_t operator()(unsigned char x) const { return x; }
    };

    template <>
    struct hash<short>
    {
        size_t operator()(short x) const { return x; }
    };

    template <>
    struct hash<unsigned short>
    {
        size_t operator()(unsigned short x) const { return x; }
    };

    template <>
    struct hash<int>
    {
        size_t operator()(int x) const { return x; }
    };

    template <>
    struct hash<unsigned int>
    {
        size_t operator()(unsigned int x) const { return x; }
    };

    template <>
    struct hash<long>
    {
        size_t operator()(long x) const { return x; }
    };

    template <>
    struct hash<unsigned long>
    {
        size_t operator()(unsigned long x) const { return x; }
    };

}

#endif

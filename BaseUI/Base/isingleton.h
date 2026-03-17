#ifndef ISINGLETON_H
#define ISINGLETON_H

/**
 * @brief 客户端公共库中的单例模板类
 *
 * version 1.0
 * @date 2022-5-17
 *
 * Copyright (c) 2007-2022, Co.,Ltd.
 * All Rights Reserved
 * Histrory:
 * Revision
 */

#include <iostream>
template<typename T>
class Singleton
{
public:
    static T& get_instance() noexcept(std::is_nothrow_constructible<T>::value)
    {
        static T instance{token()};
        return instance;
    }
    virtual ~Singleton() = default;
    Singleton(const Singleton&) = delete;
    Singleton& operator =(const Singleton&) = delete;

protected:
    struct token{}; // helper class
    Singleton() noexcept=default;
};

#endif // ISINGLETON_H

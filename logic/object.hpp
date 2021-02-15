#pragma once

template <class I>
class Object {
    std::any m_storage;
    I &(*m_getter)(std::any &);

public:
    template <class T>
    Object(T &&object)
    : m_storage{std::forward<T>(object)}
    , m_getter{[](std::any &m_storage) -> I & {
        return std::any_cast<T &>(m_storage);
    }} {}

    I *operator->() {
        return &m_getter(m_storage);
    }
};

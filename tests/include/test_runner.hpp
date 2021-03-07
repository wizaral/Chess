#pragma once

#include <array>
#include <deque>
#include <forward_list>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace TestRunnerPrivate {
template <class K, class V, template <class, class> class Map>
std::ostream &print_map(std::ostream &os, const Map<K, V> &map) {
    os << '[';
    if (auto it = map.begin(), end = map.end(); it != end) {
        os << '{' << it->first << ':' << it->second << '}';
        for (++it; it != end; ++it)
            os << ", {" << it->first << ':' << it->second << '}';
    }
    return os << ']';
}
template <class Container>
std::ostream &print_container(std::ostream &os, const Container &cont) {
    os << '[';
    if (auto it = cont.begin(), end = cont.end(); it != end) {
        for (os << *it, ++it; it != end; ++it)
            os << ", " << *it;
    }
    return os << ']';
}

#define OPEN_CONTAINER(cont)                                                                           \
    template <class T>                                                                                 \
    std::ostream &print_##cont(std::ostream &os, const std::cont<T> &q) {                              \
        struct open_##cont : std::cont<T> {                                                            \
            decltype(auto) get_c() {                                                                   \
                return std::cont<T>::c;                                                                \
            }                                                                                          \
        };                                                                                             \
        return print_container(os, static_cast<open_##cont &>(const_cast<std::cont<T> &>(q)).get_c()); \
    }

OPEN_CONTAINER(priority_queue)
OPEN_CONTAINER(queue)
OPEN_CONTAINER(stack)
#undef OPEN_CONTAINER

} // namespace TestRunnerPrivate

template <class T, size_t S>
std::ostream &operator<<(std::ostream &os, const std::array<T, S> &array) {
    return TestRunnerPrivate::print_container(os, array);
}

#define PRINT_MAP(map)                                                      \
    template <class K, class V>                                             \
    std::ostream &operator<<(std::ostream &os, const std::map<K, V> &map) { \
        return TestRunnerPrivate::print_map(os, map);                       \
    }

PRINT_MAP(map)
PRINT_MAP(multimap)
PRINT_MAP(unordered_map)
PRINT_MAP(unordered_multimap)
#undef PRINT_MAP

#define PRINT_CONTAINER(cont)                                              \
    template <class T>                                                     \
    std::ostream &operator<<(std::ostream &os, const std::cont<T> &cont) { \
        return TestRunnerPrivate::print_container(os, cont);               \
    }

PRINT_CONTAINER(deque)
PRINT_CONTAINER(vector)
PRINT_CONTAINER(list)
PRINT_CONTAINER(forward_list)
PRINT_CONTAINER(set)
PRINT_CONTAINER(multiset)
PRINT_CONTAINER(unordered_set)
PRINT_CONTAINER(unordered_multiset)
#undef PRINT_CONTAINER

#define PRINT_OPEN_CONTAINER(cont)                                         \
    template <class T>                                                     \
    std::ostream &operator<<(std::ostream &os, const std::cont<T> &cont) { \
        return TestRunnerPrivate::print_##cont(os, cont);                  \
    }

PRINT_OPEN_CONTAINER(priority_queue)
PRINT_OPEN_CONTAINER(queue)
PRINT_OPEN_CONTAINER(stack)
#undef PRINT_OPEN_CONTAINER

namespace TestRunnerCompare {
#define CHECK_COMPARE(name, op)                                                                      \
    template <class T, class, class = void>                                                          \
    struct name : std::false_type {};                                                                \
    template <class T, class U>                                                                      \
    struct name<T, U, std::void_t<decltype(std::declval<const T &>() op std::declval<const U &>())>> \
        : std::true_type {};                                                                         \
    template <class T, class U>                                                                      \
    inline constexpr bool name##_v = name<T, U>::value

CHECK_COMPARE(has_eq_op, ==);
CHECK_COMPARE(has_ne_op, !=);
CHECK_COMPARE(has_lt_op, <);
CHECK_COMPARE(has_gt_op, >);
CHECK_COMPARE(has_le_op, <=);
CHECK_COMPARE(has_ge_op, >=);
#undef CHECK_COMPARE

template <class T, class U>
bool Compare(const T &t, const U &u) {
    if constexpr (has_eq_op_v<T, U>) {
        return !(t == u);
    } else if constexpr (has_ne_op_v<T, U>) {
        return t != u;
    } else if constexpr (has_lt_op_v<T, U>) {
        return t < u || u < t;
    } else if constexpr (has_gt_op_v<T, U>) {
        return t > u || u > t;
    } else if constexpr (has_le_op_v<T, U>) {
        return !(t <= u) || !(u <= t);
    } else if constexpr (has_ge_op_v<T, U>) {
        return !(t >= u) || !(u >= t);
    } else {
        constexpr auto has_any_comparison_operator = has_eq_op_v<T, U> || has_ne_op_v<T, U>
            || has_lt_op_v<T, U> || has_gt_op_v<T, U> || has_le_op_v<T, U> || has_ge_op_v<T, U>;
        static_assert(has_any_comparison_operator, "No comparison operator for this operands!");
        return false;
    }
}

} // namespace TestRunnerCompare

template <class os, class, class = void>
struct has_os_op : std::false_type {};
template <class os, class T>
struct has_os_op<os, T, std::void_t<decltype(std::declval<os &>() << std::declval<const T &>())>>
    : std::true_type {};
template <class os, class T>
inline constexpr bool has_os_op_v = has_os_op<os, T>::value;

template <class T, class U>
void AssertEqual(const T &t, const U &u, const std::string &hint = {}) {
    static_assert(has_os_op_v<std::ostream, T>, "No output operators for this operand!");
    static_assert(has_os_op_v<std::ostream, U>, "No output operators for this operand!");

    if (TestRunnerCompare::Compare(t, u)) {
        std::ostringstream os;
        os << "Assertion failed: " << t << " != " << u;
        if (!hint.empty())
            os << "\nHint: " << hint;
        throw std::runtime_error(os.str());
    }
}

inline void Assert(bool b, const std::string &hint = {}) {
    AssertEqual(b, true, hint);
}

class TestRunner {
    int m_fails = 0;
    bool m_should_terminate = true;
public:
    explicit TestRunner() = default;
    explicit TestRunner(bool should_terminate)
        : m_should_terminate(should_terminate) {}

    template <class TestFunc>
    void RunTest(TestFunc func, const std::string &test_name = {}) {
        try {
            func();
            std::cerr << test_name << " OK" << std::endl;
        } catch (std::exception &ex) {
            ++m_fails;
            std::cerr << test_name << " fail: " << ex.what() << std::endl;
        } catch (...) {
            ++m_fails;
            std::cerr << "Unknown exception caught" << std::endl;
        }
    }

    ~TestRunner() {
        if (std::cerr.flush(); m_fails > 0) {
            std::cerr << m_fails << " unit tests failed. Terminate" << std::endl;
            if (m_should_terminate)
                exit(EXIT_FAILURE);
        }
    }
};

#ifndef FILE_NAME
#define FILE_NAME __FILE__
#endif

#define ASSERT_EQUAL(x, y)                     \
    do {                                       \
        std::ostringstream assert_os___;       \
        assert_os___                           \
            << #x << " != " << #y << std::endl \
            << FILE_NAME << " : " << __LINE__; \
        AssertEqual(x, y, assert_os___.str()); \
    } while (0)

#define ASSERT(x)                              \
    do {                                       \
        std::ostringstream assert_os___;       \
        assert_os___                           \
            << #x << " is false" << std::endl  \
            << FILE_NAME << " : " << __LINE__; \
        Assert(x, assert_os___.str());         \
    } while (0)

#define RUN_TEST(tr, func) tr.RunTest(func, #func)

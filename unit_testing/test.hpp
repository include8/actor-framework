#ifndef TEST_HPP
#define TEST_HPP

#include <vector>
#include <string>
#include <cstddef>
#include <iostream>
#include <type_traits>

#include "cppa/util/enable_if.hpp"
#include "cppa/util/disable_if.hpp"

template<typename T1, typename T2>
inline bool cppa_check_value_fun_eq(T1 const& value1, T2 const& value2,
                                    typename cppa::util::disable_if_c<std::is_integral<T1>::value && std::is_integral<T2>::value>::type* = 0)
{
    return value1 == value2;
}

template<typename T1, typename T2>
inline bool cppa_check_value_fun_eq(T1 value1, T2 value2,
                                    typename cppa::util::enable_if_c<std::is_integral<T1>::value && std::is_integral<T2>::value>::type* = 0)
{
    return value1 == static_cast<T1>(value2);
}

template<typename T1, typename T2>
inline void cppa_check_value_fun(T1 const& value1, T2 const& value2,
                                 char const* file_name,
                                 int line_number,
                                  size_t& error_count)
{
    if (cppa_check_value_fun_eq(value1, value2) == false)
    {
        std::cerr << "ERROR in file " << file_name << " on line " << line_number
                  << " => expected value: " << value1
                  << ", found: " << value2
                  << std::endl;
        ++error_count;
    }
}

#define CPPA_TEST(name)                                                        \
struct cppa_test_scope                                                         \
{                                                                              \
    size_t error_count;                                                        \
    cppa_test_scope() : error_count(0) { }                                     \
    ~cppa_test_scope()                                                         \
    {                                                                          \
        std::cout << error_count << " error(s) detected" << std::endl;         \
    }                                                                          \
} cppa_ts;

#define CPPA_TEST_RESULT cppa_ts.error_count

#ifdef CPPA_VERBOSE_CHECK
#define CPPA_IF_VERBOSE(line_of_code) line_of_code
#define CPPA_CHECK(line_of_code)                                               \
if (!(line_of_code))                                                           \
{                                                                              \
    std::cerr << "ERROR in file " << __FILE__ << " on line " << __LINE__       \
              << " => " << #line_of_code << std::endl;                         \
    ++cppa_ts.error_count;                                                     \
}                                                                              \
else                                                                           \
{                                                                              \
    std::cout << "line " << __LINE__ << " passed" << endl;                     \
}                                                                              \
((void) 0)
#else
#define CPPA_IF_VERBOSE(line_of_code) ((void) 0)
#define CPPA_CHECK(line_of_code)                                               \
if (!(line_of_code))                                                           \
{                                                                              \
    std::cerr << "ERROR in file " << __FILE__ << " on line " << __LINE__       \
              << " => " << #line_of_code << std::endl;                         \
    ++cppa_ts.error_count;                                                     \
} ((void) 0)
#endif

#define CPPA_ERROR(err_msg)                                                    \
std::cerr << err_msg << std::endl;                                             \
++cppa_ts.error_count

#define CPPA_CHECK_EQUAL(lhs_loc, rhs_loc)                                     \
  cppa_check_value_fun((lhs_loc), (rhs_loc), __FILE__, __LINE__,               \
                       cppa_ts.error_count)

#define CPPA_CHECK_NOT_EQUAL(lhs_loc, rhs_loc) CPPA_CHECK(((lhs_loc) != (rhs_loc)))

typedef std::pair<std::string, std::string> string_pair;

size_t test__yield_interface();
size_t test__remote_actor(char const* app_path, bool is_client,
                          std::vector<string_pair> const& args);
size_t test__ripemd_160();
size_t test__uniform_type();
size_t test__type_list();
size_t test__atom();
size_t test__tuple();
size_t test__match();
size_t test__spawn();
size_t test__pattern();
size_t test__intrusive_ptr();
size_t test__serialization();
size_t test__local_group();
size_t test__primitive_variant();
size_t test__fixed_vector();
size_t test__intrusive_containers();

void test__queue_performance();

using std::cout;
using std::endl;
using std::cerr;

#endif // TEST_HPP

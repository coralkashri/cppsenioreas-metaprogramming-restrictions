/**
 * All examples are taken from C++ Senioreas blog
 * https://cppsenioreas.wordpress.com/
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <type_traits>
#include <string>
#include <chrono>

class goblin {};

#if __cplusplus <= 201103L // C++11

/*
 * Examples from - CppSenioreas::TheExactSolutionForAGenericProblem
 * https://cppsenioreas.wordpress.com/2020/08/14/the-exact-solution-for-a-generic-problem-part-1/
 * https://cppsenioreas.wordpress.com/2020/08/18/the-exact-solution-for-a-generic-problem-part-2/
 */

template <bool... B>
struct conjunction {};

template <bool Head, bool... Tail>
struct conjunction<Head, Tail...> : std::integral_constant<bool, Head && conjunction<Tail...>::value>{};

template <bool B>
struct conjunction<B> : std::integral_constant<bool, B> {};

template <typename T>
T sum(T t) {
    return t;
}

template <typename T, typename ...Args, typename = typename std::enable_if<conjunction<std::is_arithmetic<Args>::value...>::value>::type>
typename std::common_type<T, Args...>::type sum(T t, Args ...args) {
    return t + sum(args...);
}

#elif __cplusplus > 201103L && __cplusplus <= 201402L // C++14

/*
 * Examples from - CppSenioreas::TheExactSolutionForAGenericProblem
 * https://cppsenioreas.wordpress.com/2020/08/14/the-exact-solution-for-a-generic-problem-part-1/
 * https://cppsenioreas.wordpress.com/2020/08/18/the-exact-solution-for-a-generic-problem-part-2/
 */

template <bool... B>
struct conjunction {};

template <bool Head, bool... Tail>
struct conjunction<Head, Tail...> : std::integral_constant<bool, Head && conjunction<Tail...>::value>{};

template <bool B>
struct conjunction<B> : std::integral_constant<bool, B> {};

template <typename T>
auto sum(T t) {
    return t;
}

template <typename T, typename ...Args>
auto sum(T t, Args ...args) {
    static_assert(conjunction<std::is_arithmetic<Args>::value...>::value, "All parameters should be arithmetic params.");
    return t + sum(args...);
}

#elif __cplusplus > 201402L && __cplusplus <= 201703L // C++17

/*
 * Examples from - CppSenioreas::TheExactSolutionForAGenericProblem
 * https://cppsenioreas.wordpress.com/2020/08/14/the-exact-solution-for-a-generic-problem-part-1/
 * https://cppsenioreas.wordpress.com/2020/08/18/the-exact-solution-for-a-generic-problem-part-2/
 */

template <typename ...Args>
typename std::enable_if_t<(std::is_arithmetic_v<Args> && ...), decltype((Args() + ...))> sum(Args ...args) {
    return (args + ...);
}

/*
 * Examples from - CppSenioreas::SubstitutionFailureIsNotAnError_SFINAE
 * // TODO: add a link
 */

class base { public: virtual void func() = 0; };
class derived : public base { public: void func() override {} };

template <typename T, typename = std::enable_if_t<std::is_base_of_v<base, T>>>
void call_func(T &&derived) {
    derived.func();
}

// Simple Sum

template <typename T, typename U>
auto my_sum(T num1, U num2) {
    static_assert(std::is_arithmetic_v<T> && std::is_arithmetic_v<U>, "Arguments have to be of arithmetic types.");

    return num1 + num2;
}

struct my_int {
    explicit my_int(int a) : val(a) {}
    operator int() const { return val; }

    int val;
};

template <typename T, typename U, typename = std::enable_if_t<std::is_same_v<T, my_int>>>
auto my_sum(T num1, U num2) {
    return num1 + num2;
}

#else // C++20

/*
 * Examples from - CppSenioreas::TheExactSolutionForAGenericProblem
 * https://cppsenioreas.wordpress.com/2020/08/14/the-exact-solution-for-a-generic-problem-part-1/
 * https://cppsenioreas.wordpress.com/2020/08/18/the-exact-solution-for-a-generic-problem-part-2/
 */
template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

template <Arithmetic ...Args>
constexpr auto sum(Args ...args) {
    return (args + ...);
}

template <typename T>
concept LandType = std::is_arithmetic_v<T>;

namespace code_kingdom {
    template <LandType T>
    class land {
    public:
        explicit land(T t) {
            vec.push_back(t);
        };

        void insert(T t) {
            vec.push_back(t);
        }

        T sum() {
            return std::accumulate(vec.begin(), vec.end(), T());
        }

    private:
        std::vector<T> vec;
    };

    template <LandType T>
    land<T> generate_land() {
        return land<T>();
    }
}

/*
 * Examples from - CppSenioreas::SubstitutionFailureIsNotAnError_SFINAE
 * // TODO: add a link
 */

// Stringable

namespace std {
    string to_string(string &&val) {
        return val;
    }
}

template <typename T>
concept Stringable = requires(T type) {
    { std::to_string(type) };
};

template <typename ...Args>
std::string chain_strings(Args ...args) {
    static_assert((Stringable<Args> && ...), "Error: can chain some of those params");
}

template <Stringable ...Args>
std::string chain_strings(Args ...args) {
    return (std::string() + ... + std::to_string(args));
}


// Explicit expression

struct my_goblin {
    my_goblin() {}
    my_goblin(int) {}
};

template <typename T>
struct my_goblin_wrapper {
    explicit(!std::is_same_v<my_goblin, T>)
    my_goblin_wrapper(T &&type) : base (type) {}

    T &base;
};

#endif

int main() {
    using namespace std::string_literals;

#if __cplusplus > 201402L && __cplusplus <= 201703L // C++17
    /// Example 1
    call_func(derived());
    // call_func(goblin()); // Compile-time error - candidate template ignored: requirement 'std::is_base_of_v<base, goblin>' was not satisfied [with T = goblin]

    /// Example 2
    auto res = my_sum(1, 2.3);
    // auto res1 = my_sum(my_int(3), 2.3); // An ambitious call
#endif

#if __cplusplus > 201703L
    /// Example 1
    // code_kingdom::land l("1"s);  // Compilation error
    // l.insert("2.3"s);
    // l.insert("4"s);
    // std::cout << l.sum() << std::endl;

    code_kingdom::land l2(1.5);
    l2.insert(2.3);
    l2.insert(4);
    std::cout << l2.sum() << std::endl;

    goblin g;
    //code_kingdom::land l3(g); // Compilation error
    // l3.sum();

    /// Example 2
    std::cout << chain_strings(2, " + ", 3.5, " = ", 2 + 3.5) << std::endl;

    /// Example 3
    my_goblin_wrapper mgw = my_goblin();
    // my_goblin_wrapper mgw1 = int(); // Without the explicit this line will compile.
#endif

    return EXIT_SUCCESS;
}
#include <iostream>
#include <vector>
#include <numeric>
#include <type_traits>
#include <string>
#include <chrono>

/*template <typename T>
concept LandType = requires() {
    std::is_arithmetic_v<T>;
};

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
}*/

class goblin {};

#if __cplusplus <= 201103L // C++11

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

template <typename ...Args>
typename std::enable_if_t<(std::is_arithmetic_v<Args> && ...), decltype((Args() + ...))> sum(Args ...args) {
    return (args + ...);
}

#else // C++20

template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

template <Arithmetic ...Args>
constexpr auto sum(Args ...args) {
    return (args + ...);
}

#endif

int main() {
    int a = 90;
    auto start = std::chrono::high_resolution_clock::now();
    //double res = sum(a, 200.3, 8, 4, 5, 3.2, 1, 0.2, 200);
    constexpr double res = sum(90.0, 200.3, 8, 4, 5, 3.2, 1, 0.2, 200);
    auto stop = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count() << std::endl;
    std::cout << res << std::endl;

    /*using namespace std::string_literals;

    code_kingdom::land l("1"s);
    l.insert("2.3"s);
    l.insert("4"s);
    std::cout << l.sum() << std::endl;*/

    /*code_kingdom::land l2(1.5);
    l2.insert(2.3);
    l2.insert(4);
    std::cout << l2.sum() << std::endl;

    goblin g;
    code_kingdom::land l(g);
    l.sum();*/

    return res;
}
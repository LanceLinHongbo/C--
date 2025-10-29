// MyAlgorithms.cppm - C++ Module Interface Unit
export module MyAlgorithms;
import std;

export namespace mylib
{
    template <typename Container, typename Fp>
    // C++20 约束: Container可使用for-range 且 Fp可被调用
    requires std::ranges::range<Container> &&
             std::invocable<Fp, std::ranges::range_value_t<Container> &>
    void myForEach(Container &container, Fp func)
    {
        for (auto &element : container)
        {
            func(element);
        }
    }
} // namespace mylib

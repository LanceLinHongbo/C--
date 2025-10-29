import std;
import MyAlgorithms;

auto main() -> int
{
    try
    {
        int sum = 0;
        std::vector<int> numbers = {1, 2, 3, 4, 5};
        mylib::myForEach(numbers, [&sum](int num) { sum += num; });
        std::println("sum: {}", sum);
    }
    catch (...)
    {
        std::cerr << "error";
    }

    return 0;
}
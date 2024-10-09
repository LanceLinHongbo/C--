import std;

constexpr int alphSize = 3;

// 子字符串查找
struct KMP
{
    explicit KMP(const std::string &strpat, char chr)
        : pat(strpat)
        , dfa(alphSize, std::vector<int>(strpat.size()))
    {
        std::size_t patSize = pat.size();
        dfa[pat.at(0) - chr][0] = 1;

        for (int X = 0, patPtr = 1; patPtr != patSize; ++patPtr)
        {
            for (int i = 0; i != alphSize; ++i)
            {
                dfa[i][patPtr] = dfa[i][X];
            }
            dfa[pat.at(patPtr) - chr][patPtr] = patPtr + 1;
            X = dfa[pat.at(patPtr) - chr][X];
        }

        std::for_each(pat.begin(), pat.end(),
                      [](char a) { std::printf("%c\t", a); });

        std::printf("\n");
    }

    auto search(const std::string &txt, char chr) -> std::size_t
    {
        std::size_t txtPtr = 0;
        std::size_t patPtr = 0;
        std::size_t txtSize = txt.size();
        std::size_t patSize = pat.size();
        while (txtPtr != txtSize && patPtr != patSize)
        {
            patPtr = dfa[txt.at(txtPtr) - chr][patPtr];
            ++txtPtr;
        }
        if (patPtr == patSize)
        {
            return txtPtr - patSize;
        }
        return txtSize;
    }

    void printDfa()
    {
        for (auto i : dfa)
        {
            for (auto j : i)
            {
                std::printf("%d\t", j);
            }
            std::printf("\n");
        }
    }

  private:
    std::string pat;
    std::vector<std::vector<int>> dfa;
};

auto main() -> int
{
    KMP test("ababac", 'a');
    test.printDfa();
    std::print("find sub at: {}", test.search("baabababac", 'a'));
    return 0;
}

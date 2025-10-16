#include <unicode/ucnv.h>
#include <unicode/utypes.h>
import std;

namespace
{
    // 将毫秒数转换为 SRT 时间格式 (hh:mm:ss,mmm)
    auto formatTime(int ms) -> std::string
    {
        int hours = ms / 3600000;
        ms %= 3600000;

        int minutes = ms / 60000;
        ms %= 60000;

        int seconds = ms / 1000;
        int millis = ms % 1000;

        // 格式化写入 buffer，保留前导零
        return std::format("{:02d}:{:02d}:{:02d},{:03d}", hours, minutes,
                           seconds, millis);
    }

    // 抽取字幕内容和起始点
    void extractQuotedStrings(const std::string &text,
                              std::vector<std::string> &textVec,
                              std::vector<int> &in, std::vector<int> &out)
    {
        std::string captionText{"captionText"};
        std::string inPoint{"inPoint"};
        std::string outPoint{"outPoint"};

        std::string temp;
        temp.reserve(128);

        std::string current;
        current.reserve(128);

        for (auto chr : text)
        {
            if (chr != '"' && chr != '[' && chr != ']' && chr != '{' &&
                chr != '}' && chr != ',' && chr != ':')
            {
                current += chr;
            }
            else if (!current.empty())
            {
                if (temp == captionText)
                {
                    textVec.push_back(current);
                }
                if (temp == inPoint)
                {
                    in.push_back(std::stoi(current));
                }
                if (temp == outPoint)
                {
                    out.push_back(std::stoi(current));
                }
                temp = current;
                current.clear();
            }
        }
    }

    namespace fs = std::filesystem;
    // 获取必剪字幕文件名
    auto getLatestMaxBJsonFile(const std::string &rootPath) -> fs::path
    {
        fs::path root(rootPath);

        // 检查根目录是否存在且是目录
        if (!fs::exists(root) || !fs::is_directory(root))
        {
            std::cerr << "Error: directory does not exist\n";
            return "";
        }

        // 用于存储子目录
        std::vector<fs::directory_entry> subdirs;

        // 遍历根目录下的所有条目，筛选出目录
        for (const auto &entry : fs::directory_iterator(root))
        {
            if (entry.is_directory())
            {
                subdirs.push_back(entry);
            }
        }

        // 如果没有子目录
        if (subdirs.empty())
        {
            std::cerr << "No subdirectories found in " << rootPath << '\n';
            return "";
        }

        // 按修改时间降序排序，最新的目录在前
        std::ranges::sort(subdirs, [](const fs::directory_entry &entryA,
                                      const fs::directory_entry &entryB) {
            return entryA.last_write_time() > entryB.last_write_time();
        });

        // 获取最新的目录
        fs::path latestDir = subdirs.front().path();
        std::cout << "Latest directory: " << latestDir << '\n';

        // 遍历最新目录下的所有文件，查找 .bjson 文件
        std::vector<fs::directory_entry> bjsonFiles;
        for (const auto &entry : fs::directory_iterator(latestDir))
        {
            if (entry.is_regular_file() && entry.path().extension() == ".bjson")
            {
                bjsonFiles.push_back(entry);
            }
        }

        // 如果没有 .bjson 文件
        if (bjsonFiles.empty())
        {
            std::cerr << "No .bjson files found in the latest directory\n";
            return "";
        }

        // 找出最大的 .bjson 文件
        auto maxFileIt = std::ranges::max_element(
            bjsonFiles, [](const fs::directory_entry &entryA,
                           const fs::directory_entry &entryB) {
                return fs::file_size(entryA.path()) <
                       fs::file_size(entryB.path());
            });

        auto maxFileName = maxFileIt->path();
        std::cout << "Largest .bjson file: " << maxFileName << '\n';

        return maxFileName;
    }

    auto utf8ToGbk(const std::string &utf8) -> std::string
    {
        // 预留足够大的空间
        const auto cap = (utf8.size() + 1) * 2;
        std::string gbkStr(cap, 0);

        UErrorCode error = U_ZERO_ERROR;
        int strSize =
            ucnv_convert("GBK", "UTF-8", const_cast<char *>(gbkStr.c_str()),
                         static_cast<int>(cap), utf8.c_str(),
                         static_cast<int>(utf8.size()), &error);

        // 校正字符串长度
        gbkStr.resize(strSize);

        if (U_FAILURE(error) != 0)
        {
            throw "Failed to convert string";
        }

        return gbkStr;
    }
} // namespace

auto main() -> int
{
    auto jsonPath = getLatestMaxBJsonFile("D:\\BcutBilibili\\Sucai");
    std::ifstream file(jsonPath);
    if (file.is_open())
    {
        std::string content((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());
        file.close();

        std::vector<std::string> textVec;
        textVec.reserve(1024);

        std::vector<int> in;
        in.reserve(1024);
        std::vector<int> out;
        out.reserve(1024);

        extractQuotedStrings(content, textVec, in, out);

        std::size_t cnt = textVec.size();
        // std::string name = utf8ToGbk("D:\\B站\\srt.srt");
        // std::ofstream outfile(name);
        std::ofstream outfile(L"D:\\B站\\srt.srt");

        if (outfile.is_open())
        {
            for (std::size_t i = 0; i != cnt; ++i)
            {
                std::println(outfile, "{}", i + 1);
                std::println(outfile, "{} --> {}", formatTime(in[i]),
                             formatTime(out[i]));
                std::println(outfile, "{}\n", textVec[i]);
            }
            outfile.close();
        }
        else
        {
            std::println("无法打开srt文件");
        }
    }
    else
    {
        std::println("无法打开json文件");
    }

    std::getchar();

    return 0;
}
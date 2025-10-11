#include <unicode/ucnv.h>
#include <unicode/unistr.h>
#include <windows.h>

import std;

namespace
{
    //  std::u8string -> std::string
    void convToStr(const std::u8string &u8str, std::string &res)
    {
        // std::u8string 已经是 UTF-8 编码
        // 直接转换为 std::string (假设 char 和 char8_t 大小相同，通常成立)
        res = {reinterpret_cast<const char *>(u8str.c_str()), u8str.length()};
    }

    //  std::u16string -> std::string
    void convToStr(const std::u16string &u16str, std::string &res)
    {
        // 将 std::u16string (UTF-16) 转换为 ICU 的 UnicodeString
        icu::UnicodeString(reinterpret_cast<const UChar *>(u16str.c_str()),
                           static_cast<int32_t>(u16str.length()))
            .toUTF8String(res);
    }

    //  std::u32string -> std::string
    void convToStr(const std::u32string &u32str, std::string &res)
    {
        icu::UnicodeString::fromUTF32(
            reinterpret_cast<const UChar32 *>(u32str.c_str()),
            static_cast<int>(u32str.size()))
            .toUTF8String(res);
    }

    // std::u8string -> std::u16string
    auto u8ToU16(const std::u8string &u8str) -> std::u16string
    {
        auto str = icu::UnicodeString::fromUTF8(u8str);

        return {str.getBuffer(), static_cast<size_t>(str.length())};
    }

    // std::u8string -> std::u32string
    auto u8ToU32(const std::u8string &u8str) -> std::u32string
    {
        auto str = icu::UnicodeString::fromUTF8(u8str);
        std::u32string u32str(str.length() + 1, 0);

        UErrorCode error = U_ZERO_ERROR;
        auto strSize = str.toUTF32(
            reinterpret_cast<UChar32 *>(const_cast<char32_t *>(u32str.c_str())),
            str.length() + 1, error);

        if (U_FAILURE(error) != 0)
        {
            throw "Failed to convert string";
        }

        u32str.resize(strSize);

        return u32str;
    }

    auto gbkToUtf8(const std::string &gbk) -> std::string
    {
        // 预留足够大的空间
        const auto cap = (gbk.size() + 1) * 2;
        std::string u8str(cap, 0);

        UErrorCode error = U_ZERO_ERROR;
        int strSize =
            ucnv_convert("UTF-8", "GBK", const_cast<char *>(u8str.c_str()),
                         static_cast<int>(cap), gbk.c_str(),
                         static_cast<int>(gbk.size()), &error);

        // 校正字符串长度
        u8str.resize(strSize);

        if (U_FAILURE(error) != 0)
        {
            throw "Failed to convert string";
        }

        return u8str;
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

// --- 主函数：演示转换 ---
auto main() -> int
{
    try
    {
        // 测试文本：包含 ASCII 和 中文
        std::u8string u8str = u8"Hello, 世界! (u8)";

        std::u16string u16str = u"Hello";
        u16str = u8ToU16(u8str);

        std::u32string u32str = U"Hello";
        u32str = u8ToU32(u8str);

        std::wstring locstr = L"Hello, 世界! (local)";

        // 执行转换
        std::string s8;
        convToStr(u8str, s8);
        std::string s16;
        convToStr(u16str, s16);
        std::string s32;
        convToStr(u32str, s32);
        std::string sLoc;
        convToStr(
            {reinterpret_cast<const UChar *>(locstr.c_str()), locstr.size()},
            sLoc);

        // 输出结果
        std::print("std::u8string  -> std::string: {}\n", s8);
        std::print("std::u16string -> std::string: {}\n", s16);
        std::print("std::u32string -> std::string: {}\n", s32);
        std::print("std::locstring -> std::string: {}\n", sLoc);

        std::cout << std::string("Hello, 世界! (local)") << '\n';
        std::cout << utf8ToGbk(std::string("Hello, 世界! (local)")) << '\n';
        std::cout << gbkToUtf8(utf8ToGbk(std::string("Hello, 世界! (local)")))
                  << '\n';
    }
    catch (const std::exception &e)
    {
        std::print(std::cerr, "Error: {}\n", e.what());
        return 1;
    }

    return 0;
}

#include <clocale>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <string>
#include <windows.h>

#include <sapi.h>

struct myVoice
{
    myVoice(int argc, char *argv[])
    {
        CoInitialize(nullptr);
        hr = CoCreateInstance(CLSID_SpVoice, nullptr, CLSCTX_ALL, IID_ISpVoice,
                              reinterpret_cast<void **>(&pVoice));

        if (SUCCEEDED(hr))
        {
            if (argc >= 2)
            {
                std::ifstream speak_file(argv[2]);
                if (speak_file.fail())
                {
                    fprintf(stderr, "%s: failed to open speak_file\n",
                            __func__);
                }
                else
                {
                    std::string content(
                        (std::istreambuf_iterator<char>(speak_file)),
                        std::istreambuf_iterator<char>());
                    speak_file.close();
                    temp = std::move(content);
                }
            }
        }
        setlocale(LC_CTYPE, "");
    }

    ~myVoice()
    {
        pVoice->Release();
        pVoice = nullptr;
        CoUninitialize();
    }

    void speak()
    {
        if (temp.size() < 4000)
        {
            speak(temp.data());
        }
    }

  private:
    ISpVoice *pVoice = nullptr;
    HRESULT hr;
    wchar_t text[4096] = L"<VOICE REQUIRED='NAME=Microsoft Zira Desktop'/>";
    std::string temp;

    void speak(char *words)
    {
        mbstowcs(text + 47, words, 4000);
        if (SUCCEEDED(hr))
        {
            pVoice->Speak(text, SPF_DEFAULT | SPF_IS_XML, nullptr);
        }
    }
};

auto main(int argc, char *argv[]) -> int
{
    myVoice mvc(argc, argv);

    mvc.speak();

    return 0;
}

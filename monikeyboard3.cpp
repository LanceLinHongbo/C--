#include <iostream>
#include <vector>
#include <windows.h>
#include <string>
#include <map>
#include <regex>

using namespace std;
using std::map;
using std::pair;
using std::vector;

void down(int vk) //封装为函数：按下某键：
{
    keybd_event(vk, 0, 0, 0);
}

void up(int vk) //抬起某键：
{
    keybd_event(vk, 0, 2, 0);
}

void press(int vk) //按下弹起：
{
    down(vk);
    up(vk);
}

void ctrl_key(int vk) //组合键：例如模拟Ctrl+V：
{
    down(162); //按下Ctrl键
    press(vk); //按下V键，并放开
    up(162);   //放开ctrl键
}

void alt_key(int vk) //组合键：例如模拟Alt+V：
{
    down(164); //按下Alt键
    press(vk); //按下V键，并放开
    up(164);   //放开Alt键
}

void ctrl_alt_key(int vk) //组合键：例如模拟Ctrl+Alt+V：
{
    down(162);
    down(164);
    press(vk);
    up(164);
    up(162);
}

void shift_ctrl_key(int vk) //组合键：例如模拟Shift+Ctrl+V：
{
    down(160);
    down(162);
    press(vk);
    up(162);
    up(160);
}

void shift_ctrl_alt_key(int vk) //组合键：例如模拟Shift+Ctrl+Alt+V：
{
    down(160);
    down(162);
    down(164);
    press(vk);
    up(164);
    up(162);
    up(160);
}

const map<string, int> moNiJianPanJianZhiBiao() //得到虚拟键盘的键值字典
{
    const map<string, int> jianZhiBiao = {
        {"0", 48},
        {"1", 49},
        {"2", 50},
        {"3", 51},
        {"4", 52},
        {"5", 53},
        {"6", 54},
        {"7", 55},
        {"8", 56},
        {"9", 57},
        {"a", 65},
        {"b", 66},
        {"c", 67},
        {"d", 68},
        {"e", 69},
        {"f", 70},
        {"g", 71},
        {"h", 72},
        {"i", 73},
        {"j", 74},
        {"k", 75},
        {"l", 76},
        {"m", 77},
        {"n", 78},
        {"o", 79},
        {"p", 80},
        {"q", 81},
        {"r", 82},
        {"s", 83},
        {"t", 84},
        {"u", 85},
        {"v", 86},
        {"w", 87},
        {"x", 88},
        {"y", 89},
        {"z", 90},
        {"space", 32},
        {"pgup", 33},
        {"pgdn", 34},
        {"end", 35},
        {"home", 36},
        {"left", 37},
        {"up", 38},
        {"right", 39},
        {"down", 40},
    };
    return jianZhiBiao;
}

const vector<string> anJianFangShiXuanZeBiao()
{
    const vector<string> anJianFangShiXuanZeVec = {"1", "2", "3", "4", "5", "6"};
    return anJianFangShiXuanZeVec;
}

vector<pair<int, string>> shuRuXuLieMap(const map<string, int> jianZhiBiao, const vector<string> anJianFangShiXuanZeVec)
{
    cout << "---------------------------------------------------------------------------------------------------"
         << "\n"
         << "输入按键:'0-9';'a-z';'space';'pgup';'pgdn';'end';'home';'left';'up';'right';'down';"
         << "\n"
         << "---------------------------------------------------------------------------------------------------"
         << "\n"
         << "\n"
         << "按键方式："
         << "\n"
         << "\n"
         << "普通按键:'1'"
         << "\n"
         << "\n"
         << "ctrl组合键:'2'"
         << "\n"
         << "\n"
         << "alt组合键:'3'"
         << "\n"
         << "\n"
         << "ctrl+alt组合键:'4'"
         << "\n"
         << "\n"
         << "shift+ctrl组合键:'5'"
         << "\n"
         << "\n"
         << "shift+ctrl+alt组合键:'6'"
         << "\n"
         << "\n"
         << "---------------------------------------------------------------------------------------------------"
         << "\n"
         << "输入按键，回车，输入按键方式，以 { Ctrl + Z + 回车 } 结束，程序会等待 3 秒钟，用以切换操作窗口！"
         << "\n"
         << "---------------------------------------------------------------------------------------------------" << endl;

    string shuRuAnJian = {""};

    string anJianFangShiXuanZe = {""};

    vector<pair<int, string>> shuRuXuLieBiao;

    while (cin >> shuRuAnJian && cin >> anJianFangShiXuanZe)
    {
        int queRenShuRuAnJian = jianZhiBiao.find(shuRuAnJian)->second;
        auto anJianFangShiJianCha = find(anJianFangShiXuanZeVec.cbegin(), anJianFangShiXuanZeVec.cend(), anJianFangShiXuanZe);
        if (queRenShuRuAnJian < 200 && anJianFangShiJianCha != anJianFangShiXuanZeVec.cend())
        {
            shuRuXuLieBiao.push_back({queRenShuRuAnJian, anJianFangShiXuanZe});
        }
        else
        {
            cout << "输入有误，请重新输入！" << endl;
        }
    }

    return shuRuXuLieBiao;
}

void moNiAnJianCaoZuo(const vector<pair<int, string>> shuRuXuLieBiao, const int chongFuCiShu) //根据重复和按键列表进行重复按键
{
    Sleep(3000);

    for (int i = 0; i < chongFuCiShu; i++)
    {
        Sleep(200);
        for (auto it = shuRuXuLieBiao.cbegin(); it != shuRuXuLieBiao.cend(); ++it)
        {
            Sleep(100);
            if (it->second == "1")
            {
                press(it->first);
            }
            else if (it->second == "2")
            {
                ctrl_key(it->first);
            }
            else if (it->second == "3")
            {
                alt_key(it->first);
            }
            else if (it->second == "4")
            {
                ctrl_alt_key(it->first);
            }
            else if (it->second == "5")
            {
                shift_ctrl_key(it->first);
            }
            else if (it->second == "6")
            {
                shift_ctrl_alt_key(it->first);
            }
        }
    }
}

const int chongFuCiShu()
{
    int xunHuanTiaoJian = 1;
    while (xunHuanTiaoJian == 1)
    {
        string chongFuCiShuZhi = {""};
        cout << "输入重复次数" << endl;
        cin >> chongFuCiShuZhi;

        regex zhengZeCanShu("^[0-9]+$");
        bool panDuanShiFouWeiShuZi = regex_match(chongFuCiShuZhi, zhengZeCanShu);

        if (panDuanShiFouWeiShuZi)
        {
            const int chongFuCiShu = stoi(chongFuCiShuZhi);
            cout << "\n"
                 << "重复次数是" << chongFuCiShu << "\n"
                 << endl;
            xunHuanTiaoJian = 2;
            Sleep(1000);
            return chongFuCiShu;
        }
        else
        {
            cout << "输入错误"
                 << "\n"
                 << endl;
            xunHuanTiaoJian = 1;
        }
    }
    return 0;
}

int main()
{
    SetConsoleOutputCP(936);
    SetConsoleCP(936);

    moNiAnJianCaoZuo(
        shuRuXuLieMap(
            moNiJianPanJianZhiBiao(),
            anJianFangShiXuanZeBiao()),
        chongFuCiShu());

    return 0;
}

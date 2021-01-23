#include <iostream>
#include <vector>
#include <windows.h>
#include <string>
#include <map>

using namespace std;
using std::map;
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

map<string, int> moNiJianPanJianZhiBiao() //得到虚拟键盘的键值字典
{
    map<string, int> jianZhiBiao;
    jianZhiBiao["0"] = 48;
    jianZhiBiao["1"] = 49;
    jianZhiBiao["2"] = 50;
    jianZhiBiao["3"] = 51;
    jianZhiBiao["4"] = 52;
    jianZhiBiao["5"] = 53;
    jianZhiBiao["6"] = 54;
    jianZhiBiao["7"] = 55;
    jianZhiBiao["8"] = 56;
    jianZhiBiao["9"] = 57;
    jianZhiBiao["a"] = 65;
    jianZhiBiao["b"] = 66;
    jianZhiBiao["c"] = 67;
    jianZhiBiao["d"] = 68;
    jianZhiBiao["e"] = 69;
    jianZhiBiao["f"] = 70;
    jianZhiBiao["g"] = 71;
    jianZhiBiao["h"] = 72;
    jianZhiBiao["i"] = 73;
    jianZhiBiao["j"] = 74;
    jianZhiBiao["k"] = 75;
    jianZhiBiao["l"] = 76;
    jianZhiBiao["m"] = 77;
    jianZhiBiao["n"] = 78;
    jianZhiBiao["o"] = 79;
    jianZhiBiao["p"] = 80;
    jianZhiBiao["q"] = 81;
    jianZhiBiao["r"] = 82;
    jianZhiBiao["s"] = 83;
    jianZhiBiao["t"] = 84;
    jianZhiBiao["u"] = 85;
    jianZhiBiao["v"] = 86;
    jianZhiBiao["w"] = 87;
    jianZhiBiao["x"] = 88;
    jianZhiBiao["y"] = 89;
    jianZhiBiao["z"] = 90;
    jianZhiBiao["space"] = 32;
    jianZhiBiao["pgup"] = 33;
    jianZhiBiao["pgdn"] = 34;
    jianZhiBiao["end"] = 35;
    jianZhiBiao["home"] = 36;
    jianZhiBiao["left"] = 37;
    jianZhiBiao["up"] = 38;
    jianZhiBiao["right"] = 39;
    jianZhiBiao["down"] = 40;
    return jianZhiBiao;
}

vector<int> kuaiJieJianZhuanHuanBiao(const map<string, int> jianZhiBiao) //得到要运行的按键列表
{
    vector<int> shuRuAnJianBiao;
    string shuRuAnJian;
    cout << "输入按键:'0-9';'a-z';'space';'pgup';'pgdn';'end';'home';'left';'up';'right';'down'.以ctrl+z+回车结束！" << endl;
    while (cin >> shuRuAnJian)
    {
        int queRenShuRuAnJian = jianZhiBiao.find(shuRuAnJian)->second;
        if (queRenShuRuAnJian < 200)
        {
            shuRuAnJianBiao.push_back(queRenShuRuAnJian);
        }
    }

    cin.clear();
    cin.ignore();
    return shuRuAnJianBiao;
}

void moNiAnJianCaoZuo(const vector<int> shuRuAnJianBiao, const int chongFuCiShu, const int anJianFangShiXuanZe) //根据重复和按键列表进行重复按键
{
    Sleep(3000);

    int vtn = shuRuAnJianBiao.size();

    for (int i = 0; i < chongFuCiShu; i++)
    {
        Sleep(200);
        for (int i2 = 0; i2 < vtn; i2++)
        {
            Sleep(100);
            if (anJianFangShiXuanZe == 1)
            {
                press(shuRuAnJianBiao.at(i2));
            }
            else if (anJianFangShiXuanZe == 2)
            {
                ctrl_key(shuRuAnJianBiao.at(i2));
            }
            else if (anJianFangShiXuanZe == 3)
            {
                alt_key(shuRuAnJianBiao.at(i2));
            }
            else if (anJianFangShiXuanZe == 4)
            {
                ctrl_alt_key(shuRuAnJianBiao.at(i2));
            }
            else if (anJianFangShiXuanZe == 5)
            {
                shift_ctrl_key(shuRuAnJianBiao.at(i2));
            }
            else if (anJianFangShiXuanZe == 6)
            {
                shift_ctrl_alt_key(shuRuAnJianBiao.at(i2));
            }
        }
    }
}

int main()
{
    SetConsoleOutputCP(936);
    SetConsoleCP(936);

    cout << "输入重复次数" << endl;
    int chongFuCiShu = 0;
    cin >> chongFuCiShu;

    cout << "选择普通按键'1';"
         << "\n"
         << "选择ctrl组合键'2';"
         << "\n"
         << "选择alt组合键'3';"
         << "\n"
         << "选择ctrl+alt组合键'4';"
         << "\n"
         << "选择shift+ctrl组合键'5'"
         << "\n"
         << "选择shift+ctrl+alt组合键'6':" << endl;
    int anJianFangShiXuanZe;
    cin >> anJianFangShiXuanZe;

    moNiAnJianCaoZuo(kuaiJieJianZhuanHuanBiao(moNiJianPanJianZhiBiao()), chongFuCiShu, anJianFangShiXuanZe);

    return 0;
}

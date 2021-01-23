#include <iostream>
#include <windows.h> // 解决问题的关键，告诉windows终端编码方式：GBK；同时编码方式保存也要用GBK
#include <string>

using namespace std;

int main()
{
    SetConsoleOutputCP(936); //终端输出的是GBK编码
    SetConsoleCP(936);       //终端输入的是GBK编码
    int v1 = 0, v2 = 0;
    string v3;
    cout << "输入两个数字几个汉字" << endl;
    cin >> v1 >> v2;
    cin >> v3;
    cout << v1 << "及" << v2 << "的乘积是：" << v1 * v2;
    cout << "汉字是：";
    cout << v3 << endl;
    return 0;
}
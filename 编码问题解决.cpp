#include <iostream>
#include <windows.h> // �������Ĺؼ�������windows�ն˱��뷽ʽ��GBK��ͬʱ���뷽ʽ����ҲҪ��GBK
#include <string>

using namespace std;

int main()
{
    SetConsoleOutputCP(936); //�ն��������GBK����
    SetConsoleCP(936);       //�ն��������GBK����
    int v1 = 0, v2 = 0;
    string v3;
    cout << "�����������ּ�������" << endl;
    cin >> v1 >> v2;
    cin >> v3;
    cout << v1 << "��" << v2 << "�ĳ˻��ǣ�" << v1 * v2;
    cout << "�����ǣ�";
    cout << v3 << endl;
    return 0;
}
#include "PipeSender.h"
#include <iostream>
using namespace std;

int main() {

    //���������ܵ�
    PipeSender sender;
    sender.create(L"test");

    //�ȴ��ͻ��˵�����
    sender.connect();

    //��ͻ��˷�������
    string str;
    cin >> str;
    sender.write(str);

    //��ȡ�ͻ�������
    cout << sender.read() << endl;

    //�رչܵ�
    sender.close();

    return 0;
}
#include "PipeReceiver.h"
#include <iostream>
using namespace std;

int main() {
    //�򿪹ܵ�
    PipeReceiver receiver;
    receiver.open(L"test");

    //��ȡ����˷���������
    cout << receiver.read() << endl;

    //�����˷�������
    string str;
    cin >> str;
    receiver.write(str);

    //�رչܵ�
    receiver.close();

    return 0;
}

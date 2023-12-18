#include "PipeReceiver.h"
#include <iostream>
using namespace std;

int main() {
    //打开管道
    PipeReceiver receiver;
    receiver.open(L"test");

    //读取服务端发来的数据
    cout << receiver.read() << endl;

    //向服务端发送数据
    string str;
    cin >> str;
    receiver.write(str);

    //关闭管道
    receiver.close();

    return 0;
}

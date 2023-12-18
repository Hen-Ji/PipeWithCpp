#include "PipeSender.h"
#include <iostream>
using namespace std;

int main() {

    //创建命名管道
    PipeSender sender;
    sender.create(L"test");

    //等待客户端的连接
    sender.connect();

    //向客户端发送数据
    string str;
    cin >> str;
    sender.write(str);

    //读取客户端数据
    cout << sender.read() << endl;

    //关闭管道
    sender.close();

    return 0;
}
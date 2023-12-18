# PipeWithCpp
PipeWithCpp: 基于C++的双向命名管道，可实现进程间通信

例:
客户端:
```
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
```

服务端:
```
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
```
先运行服务端，再运行客户端即可

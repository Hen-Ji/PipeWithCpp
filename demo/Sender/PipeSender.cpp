#include "PipeSender.h"

PipeSender::PipeSender() {
	pipeHandle = NULL;
	pipeSize = 4096;
}
HANDLE PipeSender::create(wstring name, int size, int timeout) {
	pipeName = name; //设置管道名称和路径
	pipePath = L"\\\\.\\pipe\\" + name; //貌似必须有前缀 "\\\\.\\pipe\\"
    pipeSize = size; //设置管道大小

    pipeHandle = CreateNamedPipe( //创建管道，成功则返回服务端句柄，失败则返回INVALID_HANDLE_VALUE
        pipePath.c_str(), //管道名称（不区分大小写）
        PIPE_ACCESS_DUPLEX, //打开模式, PIPE_ACCESS_DUPLEX: 双向的，即客户端和服务端都可以进行读写
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, //管道模式
        PIPE_UNLIMITED_INSTANCES, //可为此管道创建的最大实例数（1 到 255）PIPE_UNLIMITED_INSTANCES: 255
        size, //要为输出缓冲区保留的字节数
        size, //要为输入缓冲区保留的字节数
        timeout, //超时值，值为0则默认超时为 50 毫秒
        NULL); //指向 SECURITY_ATTRIBUTES 结构的指针，NULL则为默认安全描述符
    return pipeHandle;
}
bool PipeSender::connect() {
    return ConnectNamedPipe( //等待连接，如果操作是同步的，成功则返回非0值，失败则返回0
        pipeHandle, //服务端句柄
        NULL); //指向 OVERLAPPED 结构的指针，没有使用FILE_FLAG_OVERLAPPED可以写null
}
bool PipeSender::write(string str) {
    DWORD dwReturn = 0;
    char *buffer = new char[pipeSize](); //后面加括号可以在申请空间时就将这块内存初始化(若是char，int类型则初始化为0)

    stringstream ss(str); // 写入缓冲
    ss >> buffer;

    BOOL write = WriteFile( //写入文件，如果操作是同步的，成功则返回非0值，失败则返回0
        pipeHandle, //文件句柄
        buffer, //指向缓冲区的指针
        strlen(buffer), //要写入的字节数
        &dwReturn, //一个指针，接收写入的字节数
        NULL); //指向 OVERLAPPED 结构的指针，没有使用FILE_FLAG_OVERLAPPED可以写null

    delete[] buffer; //释放内存
    buffer = nullptr;
    return write;
}
string PipeSender::read() {
    DWORD dwReturn = 0;
    char* buffer = new char[pipeSize]();

    BOOL read = ReadFile( //读取文件
        pipeHandle, //文件句柄
        buffer, //指向缓冲区的指针
        pipeSize, //要读取的最大字节数
        &dwReturn, //一个指针，接收读取的字节数
        NULL); //指向 OVERLAPPED 结构的指针，没有使用FILE_FLAG_OVERLAPPED可以写null
    if (!read) { //读取失败, 则释放内存，返回空
        delete[] buffer;
        buffer = nullptr;
        return "";
    }

    string res;
    stringstream ss(buffer); // 输出缓冲
    ss >> res;

    delete[] buffer; //释放内存
    buffer = nullptr;
    return res;
}
void PipeSender::disconnect() {
    DisconnectNamedPipe(pipeHandle); //断开连接
}
void PipeSender::close() {
    DisconnectNamedPipe(pipeHandle); //断开连接
    CloseHandle(pipeHandle); //关闭打开的对象句柄
}

wstring PipeSender::getName() {
    return pipeName;
}

int PipeSender::getSize() {
    return pipeSize;
}

#include "PipeReceiver.h"

PipeReceiver::PipeReceiver() {
	pipeHandle = NULL;
	pipeSize = 4096;
}
HANDLE PipeReceiver::open(wstring name, int size) {
    pipeName = name; //设置管道名称,路径,大小
    pipePath = L"\\\\.\\pipe\\" + name;
    pipeSize = size;

    BOOL accessible = WaitNamedPipe( //等待目标管道，在超时前管道可用则返回非0值，否则返回0
        pipePath.c_str(), //管道名称
        NMPWAIT_USE_DEFAULT_WAIT); //等待管道可用的毫秒数，NMPWAIT_USE_DEFAULT_WAIT为CreateNamedPipe中设定的超时数，NMPWAIT_WAIT_FOREVER为一直等待
    if (!accessible)
    {
        return NULL;
    }

    // 打开可用的命名管道 
    pipeHandle = CreateFile( //创建或打开文件(管道)，成功则返回文件(管道)句柄, 失败则返回 INVALID_HANDLE_VALUE
        pipePath.c_str(), //文件(管道)名称
        GENERIC_READ | GENERIC_WRITE, //访问权限, GENERIC_READ: 可读, GENERIC_WRITE: 可写
        FILE_SHARE_READ | FILE_SHARE_WRITE, //共享模式，不懂就这么写
        NULL, //指向 SECURITY_ATTRIBUTES 结构的指针，为 NULL 则为默认的安全描述符
        OPEN_EXISTING, //某情况下需要执行的操作，OPEN_EXISTING：仅当文件或设备存在时才打开它
        0, //文件或设备属性和标志，不懂就写0
        NULL); //具有 GENERIC_READ 访问权限的模板文件的有效句柄，打开现有文件时将忽略此参数，写null就行
    return pipeHandle;
}
bool PipeReceiver::write(string str) {
    DWORD dwReturn = 0;
    char* buffer = new char[pipeSize](); //后面加括号可以在申请空间时就将这块内存初始化(若是char，int类型则初始化为0)

    stringstream ss(str); // 写入缓冲
    ss >> buffer;

    BOOL write = WriteFile(pipeHandle, buffer, strlen(buffer), &dwReturn, NULL); //写入文件

    delete[] buffer; //释放内存
    buffer = nullptr;
    return write;
}
string PipeReceiver::read() {
    DWORD dwReturn = 0;
    char* buffer = new char[pipeSize]();

    BOOL read = ReadFile(pipeHandle, buffer, pipeSize, &dwReturn, NULL); //读取文件
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
void PipeReceiver::close() {
    CloseHandle(pipeHandle); //关闭句柄
}

wstring PipeReceiver::getName() {
    return pipeName;
}
int PipeReceiver::getSize() {
    return pipeSize;
}
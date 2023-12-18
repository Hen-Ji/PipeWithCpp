#include "PipeSender.h"

PipeSender::PipeSender() {
	pipeHandle = NULL;
	pipeSize = 4096;
}
HANDLE PipeSender::create(wstring name, int size, int timeout) {
	pipeName = name; //���ùܵ����ƺ�·��
	pipePath = L"\\\\.\\pipe\\" + name; //ò�Ʊ�����ǰ׺ "\\\\.\\pipe\\"
    pipeSize = size; //���ùܵ���С

    pipeHandle = CreateNamedPipe( //�����ܵ����ɹ��򷵻ط���˾����ʧ���򷵻�INVALID_HANDLE_VALUE
        pipePath.c_str(), //�ܵ����ƣ������ִ�Сд��
        PIPE_ACCESS_DUPLEX, //��ģʽ, PIPE_ACCESS_DUPLEX: ˫��ģ����ͻ��˺ͷ���˶����Խ��ж�д
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, //�ܵ�ģʽ
        PIPE_UNLIMITED_INSTANCES, //��Ϊ�˹ܵ����������ʵ������1 �� 255��PIPE_UNLIMITED_INSTANCES: 255
        size, //ҪΪ����������������ֽ���
        size, //ҪΪ���뻺�����������ֽ���
        timeout, //��ʱֵ��ֵΪ0��Ĭ�ϳ�ʱΪ 50 ����
        NULL); //ָ�� SECURITY_ATTRIBUTES �ṹ��ָ�룬NULL��ΪĬ�ϰ�ȫ������
    return pipeHandle;
}
bool PipeSender::connect() {
    return ConnectNamedPipe( //�ȴ����ӣ����������ͬ���ģ��ɹ��򷵻ط�0ֵ��ʧ���򷵻�0
        pipeHandle, //����˾��
        NULL); //ָ�� OVERLAPPED �ṹ��ָ�룬û��ʹ��FILE_FLAG_OVERLAPPED����дnull
}
bool PipeSender::write(string str) {
    DWORD dwReturn = 0;
    char *buffer = new char[pipeSize](); //��������ſ���������ռ�ʱ�ͽ�����ڴ��ʼ��(����char��int�������ʼ��Ϊ0)

    stringstream ss(str); // д�뻺��
    ss >> buffer;

    BOOL write = WriteFile( //д���ļ������������ͬ���ģ��ɹ��򷵻ط�0ֵ��ʧ���򷵻�0
        pipeHandle, //�ļ����
        buffer, //ָ�򻺳�����ָ��
        strlen(buffer), //Ҫд����ֽ���
        &dwReturn, //һ��ָ�룬����д����ֽ���
        NULL); //ָ�� OVERLAPPED �ṹ��ָ�룬û��ʹ��FILE_FLAG_OVERLAPPED����дnull

    delete[] buffer; //�ͷ��ڴ�
    buffer = nullptr;
    return write;
}
string PipeSender::read() {
    DWORD dwReturn = 0;
    char* buffer = new char[pipeSize]();

    BOOL read = ReadFile( //��ȡ�ļ�
        pipeHandle, //�ļ����
        buffer, //ָ�򻺳�����ָ��
        pipeSize, //Ҫ��ȡ������ֽ���
        &dwReturn, //һ��ָ�룬���ն�ȡ���ֽ���
        NULL); //ָ�� OVERLAPPED �ṹ��ָ�룬û��ʹ��FILE_FLAG_OVERLAPPED����дnull
    if (!read) { //��ȡʧ��, ���ͷ��ڴ棬���ؿ�
        delete[] buffer;
        buffer = nullptr;
        return "";
    }

    string res;
    stringstream ss(buffer); // �������
    ss >> res;

    delete[] buffer; //�ͷ��ڴ�
    buffer = nullptr;
    return res;
}
void PipeSender::disconnect() {
    DisconnectNamedPipe(pipeHandle); //�Ͽ�����
}
void PipeSender::close() {
    DisconnectNamedPipe(pipeHandle); //�Ͽ�����
    CloseHandle(pipeHandle); //�رմ򿪵Ķ�����
}

wstring PipeSender::getName() {
    return pipeName;
}

int PipeSender::getSize() {
    return pipeSize;
}

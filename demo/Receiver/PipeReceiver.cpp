#include "PipeReceiver.h"

PipeReceiver::PipeReceiver() {
	pipeHandle = NULL;
	pipeSize = 4096;
}
HANDLE PipeReceiver::open(wstring name, int size) {
    pipeName = name; //���ùܵ�����,·��,��С
    pipePath = L"\\\\.\\pipe\\" + name;
    pipeSize = size;

    BOOL accessible = WaitNamedPipe( //�ȴ�Ŀ��ܵ����ڳ�ʱǰ�ܵ������򷵻ط�0ֵ�����򷵻�0
        pipePath.c_str(), //�ܵ�����
        NMPWAIT_USE_DEFAULT_WAIT); //�ȴ��ܵ����õĺ�������NMPWAIT_USE_DEFAULT_WAITΪCreateNamedPipe���趨�ĳ�ʱ����NMPWAIT_WAIT_FOREVERΪһֱ�ȴ�
    if (!accessible)
    {
        return NULL;
    }

    // �򿪿��õ������ܵ� 
    pipeHandle = CreateFile( //��������ļ�(�ܵ�)���ɹ��򷵻��ļ�(�ܵ�)���, ʧ���򷵻� INVALID_HANDLE_VALUE
        pipePath.c_str(), //�ļ�(�ܵ�)����
        GENERIC_READ | GENERIC_WRITE, //����Ȩ��, GENERIC_READ: �ɶ�, GENERIC_WRITE: ��д
        FILE_SHARE_READ | FILE_SHARE_WRITE, //����ģʽ����������ôд
        NULL, //ָ�� SECURITY_ATTRIBUTES �ṹ��ָ�룬Ϊ NULL ��ΪĬ�ϵİ�ȫ������
        OPEN_EXISTING, //ĳ�������Ҫִ�еĲ�����OPEN_EXISTING�������ļ����豸����ʱ�Ŵ���
        0, //�ļ����豸���Ժͱ�־��������д0
        NULL); //���� GENERIC_READ ����Ȩ�޵�ģ���ļ�����Ч������������ļ�ʱ�����Դ˲�����дnull����
    return pipeHandle;
}
bool PipeReceiver::write(string str) {
    DWORD dwReturn = 0;
    char* buffer = new char[pipeSize](); //��������ſ���������ռ�ʱ�ͽ�����ڴ��ʼ��(����char��int�������ʼ��Ϊ0)

    stringstream ss(str); // д�뻺��
    ss >> buffer;

    BOOL write = WriteFile(pipeHandle, buffer, strlen(buffer), &dwReturn, NULL); //д���ļ�

    delete[] buffer; //�ͷ��ڴ�
    buffer = nullptr;
    return write;
}
string PipeReceiver::read() {
    DWORD dwReturn = 0;
    char* buffer = new char[pipeSize]();

    BOOL read = ReadFile(pipeHandle, buffer, pipeSize, &dwReturn, NULL); //��ȡ�ļ�
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
void PipeReceiver::close() {
    CloseHandle(pipeHandle); //�رվ��
}

wstring PipeReceiver::getName() {
    return pipeName;
}
int PipeReceiver::getSize() {
    return pipeSize;
}
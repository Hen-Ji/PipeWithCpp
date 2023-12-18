//�ܵ���һ��������ļ������ļ��������ڴ���
//����Ϊ˫��ܵ�

#pragma once
#include <windows.h>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

class PipeSender
{
private:
	HANDLE pipeHandle;
	wstring pipeName;
	wstring pipePath;
	int pipeSize;

public:
	PipeSender();
	HANDLE create(wstring name, int size = 4096, int timeout = 0);
	bool connect();
	bool write(string str);
	string read();
	void disconnect();
	void close();

	wstring getName();
	int getSize();
};


//管道是一种特殊的文件，但文件内容在内存中
//此类为双向管道

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


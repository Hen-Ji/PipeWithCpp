#pragma once
#include <windows.h>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

class PipeReceiver
{
private:
	HANDLE pipeHandle;
	wstring pipeName;
	wstring pipePath;
	int pipeSize;

public:
	PipeReceiver();
	HANDLE open(wstring name, int size = 4096);
	bool write(string str);
	string read();
	void close();

	wstring getName();
	int getSize();
};


#pragma once
#include "qobject.h"
#include <winsock.h>
#include <iphlpapi.h>
#include <iostream>
#include <Windows.h>
#include <string.h>

class GetLocalPCInfo :
	public QObject
{
	Q_OBJECT

public:
	GetLocalPCInfo();

private:
	DWORD dwIn = 0;//下载速度
	DWORD dwLastIn = 0;//下载最后字节
	DWORD dword = 0;

	ULARGE_INTEGER nFreeBytesAvailable;
	ULARGE_INTEGER nTotalNumberOfBytes;
	ULARGE_INTEGER nTotalNumberOfFreeBytes;
	double myfreespace;
	std::string Ethernet_Port_Number;

public slots:
	void GetCurrentInfo();

signals:
	void sendCurrentInfo(double netInfo, double freespace);
	void Restart(bool status);
};
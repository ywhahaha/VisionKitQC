#include "GetLocalPCInfo.h"
#include "INI_Config.h"
clock_t oldtime = 0;


GetLocalPCInfo::GetLocalPCInfo()
{
	INI_Config *ini = new INI_Config();
	ini->ReadINI("config.ini");//读.ini配置文件
	Ethernet_Port_Number = ini->GetValue("Ethernet", "Port_Number");
	replace(Ethernet_Port_Number.begin(), Ethernet_Port_Number.end(), '_', ' ');
	ini->Clear();
}

void GetLocalPCInfo::GetCurrentInfo()
{ 
	Sleep(5000);

	PMIB_IFTABLE pTable = nullptr;

	/*获取以太网的带宽*/
	ULONG retCode = GetIfTable(pTable, &dword, true);
	if (retCode == ERROR_NOT_SUPPORTED)
		return;
	if (retCode == ERROR_INSUFFICIENT_BUFFER)
		pTable = (PMIB_IFTABLE)new BYTE[65535];

	GetIfTable(pTable, &dword, true);
	DWORD dwInOc = 0;
	char DeviceName[256];
	int i;
	for (i = 0; i < Ethernet_Port_Number.length(); i++) {
		DeviceName[i] = Ethernet_Port_Number[i];
	}
	DeviceName[i] = '\0';

	for (int i = 0; i < pTable->dwNumEntries; i++)
	{
		MIB_IFROW row = pTable->table[i];
		if(strcmp((char*)row.bDescr, DeviceName) == 0)
		{
			dwInOc = row.dwInOctets;
			break;
		}
	}
	double interval = (double)(clock() - oldtime) / CLOCKS_PER_SEC;
	oldtime = clock();
	dwIn = dwInOc - dwLastIn;

	if (dwLastIn <= 0)
		dwIn = 0;
	else
		dwIn = dwIn / 1024 / 1024 * 8 / interval;

	dwLastIn = dwInOc;



	/*获取D盘空间*/
	if (GetDiskFreeSpaceEx(L"D:",
		&nFreeBytesAvailable,
		&nTotalNumberOfBytes,
		&nTotalNumberOfFreeBytes))//利用GetDiskFreeSpaceEx函数获得指定磁盘信息
	{
		myfreespace = nTotalNumberOfFreeBytes.QuadPart / 1024 / 1024 / 1024;//得到磁盘剩余空间，单位为G
	}

	double Bandwidth = dwIn;
	emit sendCurrentInfo(Bandwidth, myfreespace);
	emit Restart(true);

	delete[] pTable;
}
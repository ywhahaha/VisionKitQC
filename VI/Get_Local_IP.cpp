#include "Get_Local_IP.h"
#include <windows.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <iostream>
#include "INI_Config.h"
#include <algorithm>
#pragma comment(lib, "iphlpapi.lib")
INI_Config *ini = new INI_Config();

std::string getEthernetIp()
{
	INI_Config *ini = new INI_Config();
	ini->ReadINI("config.ini");//¶Á.iniÅäÖÃÎÄ¼þ
	std::string Ethernet_Port_Number = ini->GetValue("Ethernet", "Port_Number");
	replace(Ethernet_Port_Number.begin(), Ethernet_Port_Number.end(), '_', ' ');
	ini->Clear();

    PIP_ADAPTER_INFO pAdapterInfo;
    pAdapterInfo = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
    ULONG buflen = sizeof(IP_ADAPTER_INFO);

    if (GetAdaptersInfo(pAdapterInfo, &buflen) == ERROR_BUFFER_OVERFLOW) {
        free(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO*)malloc(buflen);
    }

    if (GetAdaptersInfo(pAdapterInfo, &buflen) == NO_ERROR) {
        PIP_ADAPTER_INFO pAdapter = pAdapterInfo;
		char EthernetDeviceName[256] = "Realtek PCIe GBE Family Controller";
		int i;
		for (i = 0; i < Ethernet_Port_Number.length(); i++) {
			EthernetDeviceName[i] = Ethernet_Port_Number[i];
		}
		EthernetDeviceName[i] = '\0';

		while (pAdapter->Next != NULL)
		{
			if (strcmp(pAdapter->Description, EthernetDeviceName) == 0)break;
			pAdapter = pAdapter->Next;
		}
        return pAdapter->IpAddressList.IpAddress.String;
    }
    else {
        printf("Call to GetAdaptersInfo failed.\n");
    }

	return "192.168.1.88";
}
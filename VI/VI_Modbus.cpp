#pragma once
#include "VI_Modbus.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "StateCode.h"
#include "Get_Local_IP.h"
#include <Windows.h>
#pragma comment(lib,"ws2_32.lib")
#ifndef _MSC_VER
#include <unistd.h>
#endif

#include "modbus.h"
#include "modbus-private.h"

extern bool DebugFlag;
using namespace std;


VI_Modbus::VI_Modbus()
{

}

VI_Modbus::~VI_Modbus()
{
	
}
int VI_Modbus::build_comm_connect()
{
	int ret = -1;

	/*申请内存用于存储收发的PLC数据，失败返回1*/
	Memory_Request();

	/*载入PLC初始属性*/
	Load_PLC_Parameters();
	
	/*创建一个TCP类型的变量*/
	ctx = modbus_new_tcp(PLC_IP.c_str(), atoi(PLC_Port_Number.c_str()));
	if (NULL == ctx)
	{
		return VI_E_PLC_TCP;
	}

	/*设置Debug模式*/
	ret = modbus_set_debug(ctx, TRUE);
	if (-1 == ret)
	{
		modbus_free(ctx);
		return VI_E_PLC_DEBUG;
	}

	/*连接Server*/
	ret = modbus_connect(ctx);
	if (-1 == ret)
	{
		modbus_free(ctx);
		ctx = NULL;
		return VI_E_PLC_BUILD;
	}

	return VI_PLC_ON;
}

int VI_Modbus::close_comm_connect()
{
	/*释放内存*/
	free(send_data_registers);
	free(rcv_data_registers);

	/*断开连接*/
	modbus_close(ctx);
	modbus_free(ctx);

	return VI_PLC_OFF;
}


/*读取PLC命令*/
int VI_Modbus::Read_PLC_Register(int camera_type)
{
	int ret = -1;

	while (1)
	{
		mtx.lock();
		ret = modbus_read_registers(ctx, PLC_Register_addr + camera_type - 1, 1, rcv_data_registers + (camera_type - 1));
		mtx.unlock();
		
		if (-1 == ret)
		{
			cout << "读寄存器失败：" << camera_type << endl;
			return VI_E_PLC_READ;
		}
		else if (rcv_data_registers[camera_type - 1] == TakePhotos)
		{
			return VI_SIGNAL_TOPHOTO;
		}
		else if (rcv_data_registers[camera_type - 1] == ProcessImage)
		{
			return VI_SIGNAL_TOPROCESS;
		}
		else if(rcv_data_registers[camera_type - 1] >= 0 || rcv_data_registers[camera_type - 1] <= 3 || rcv_data_registers[camera_type - 1] == 901)
		{
			/*忽略PLC的其他信号*/
			return -1;
		}
	}
	return VI_E_PLC_UNKNOW;
}


/*往PLC寄存器写值*/
int VI_Modbus::Write_PLC_Register(int camera_type, int data)
{
	int ret = -1;

	send_data_registers[camera_type - 1] = (uint16_t)data;
	mtx.lock();
	ret = modbus_write_registers(ctx, PLC_Register_addr + (camera_type - 1), 1, send_data_registers + (camera_type - 1));
	mtx.unlock();
	if (-1 == ret)
	{
		cout << "写寄存器失败："<<camera_type << endl;
		return VI_E_PLC_WRITE;
	}
	return -1;
}

/*往特定寄存器内写特定值*/
int VI_Modbus::WritePLC(int PLCAddr, int value)
{
	int ret = -1;
	mtx.lock();
	ret = modbus_write_registers(ctx, PLCAddr, 1, (uint16_t*)&value);
	mtx.unlock();
	if (-1 == ret)
	{
		return VI_E_PLC_WRITE;
	}
	return -1;
}


/*申请内存用于存储收发的PLC数据*/
int VI_Modbus::Memory_Request()
{
	rcv_data_registers = (uint16_t*)malloc(3 * sizeof(uint16_t));
	if (NULL == rcv_data_registers)
	{
		modbus_free(ctx);
		return VI_E_PLC_MALLOC;
	}
	else
	{
		memset(rcv_data_registers, 0, sizeof(uint16_t));
	}

	send_data_registers = (uint16_t*)malloc(3 * sizeof(uint16_t));
	if (NULL == send_data_registers)
	{
		modbus_free(ctx);
		return VI_E_PLC_MALLOC;
	}
	else
	{
		memset(send_data_registers, 0, sizeof(uint16_t));
	}

	
	DataPacketRegisters = (uint16_t*)malloc(3 * sizeof(uint16_t));
	if (NULL == DataPacketRegisters)
	{
		modbus_free(ctx);
		return VI_E_PLC_MALLOC;
	}
	else
	{
		memset(DataPacketRegisters, 0, sizeof(uint16_t));
	}
	return 0;
}

void VI_Modbus::Load_PLC_Parameters()
{
	ini->ReadINI("config.ini");

	PLC_IP = ini->GetValue("PLC", "IP");
	PLC_Port_Number = ini->GetValue("PLC", "Port_Number");
	if (DebugFlag)
	{
		PLC_IP = ini->GetValue("DeveloperOptions", "IP");
		PLC_Port_Number = ini->GetValue("DeveloperOptions", "Port_Number");
	}
	ini->Clear();
}


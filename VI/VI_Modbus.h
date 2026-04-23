#pragma once
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\types_c.h>
#include <opencv2/core/utils/logger.hpp>
#include "modbus.h"
#include "show_camera_config_window.h"
#include <mutex>
#ifndef TRUE
#define TRUE 1
#endif


#define TakePhotos 801
#define ProcessImage 802
#define PhotoIsOver 901

class VI_Modbus
{
public:
	VI_Modbus();
	~VI_Modbus();
	int build_comm_connect();
	int close_comm_connect();
	int Read_PLC_Register(int camera_type);
	int Write_PLC_Register(int camera_type, int data);
	int WritePLC(int PLCAddr, int value);
	void Load_PLC_Parameters();
	int Memory_Request();

private:
	int PLC_Register_addr = 901;
	int PLCPacketNumAdd = 910;
	modbus_t* ctx = NULL;
	uint16_t* rcv_data_registers = NULL;//存放接收数据
	uint16_t* send_data_registers = NULL;//存放待发送数据
	uint16_t* DataPacketRegisters = NULL;//存放PLC数据包个数

	INI_Config *ini = new INI_Config();

	std::string PLC_IP;
	std::string PLC_Port_Number;

	std::mutex mtx;
};
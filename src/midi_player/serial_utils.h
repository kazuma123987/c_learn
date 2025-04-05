#ifndef __SERIAL_UTILS_H__
#define __SERIAL_UTILS_H__
#include <vector>
#include <string>
#include <windows.h>
#include <iostream>

class SerialUtils
{
public:
    static std::string m_port;
    static int m_baudRate;
    static int m_dataBits;
    static int m_stopBits;
    static int m_parity;
    static std::vector<std::string> getSerialPorts();
    // 打开串口
    static HANDLE openSerialPort(std::string portname);
    // 关闭串口
    static void closeSerialPort(HANDLE hSerial);
    // 读取串口数据
    static std::string readSerialPort(HANDLE hSerial);
    // 写入串口数据
    static void writeSerialPort(HANDLE hSerial, const std::string &data);
};

#endif

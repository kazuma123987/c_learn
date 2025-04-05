#include <windows.h>
#include "serial_utils.h"
// const std::string &portName, int baudRate = CBR_9600, int byteSize = 8, int stopBits = ONESTOPBIT, int parity = NOPARITY

std::string SerialUtils::m_port = "";
int SerialUtils::m_baudRate = CBR_9600;
int SerialUtils::m_dataBits = 8;
int SerialUtils::m_stopBits = 1;
int SerialUtils::m_parity = NOPARITY;

std::vector<std::string> SerialUtils::getSerialPorts()
{
    std::vector<std::string> ports;
    char lpTargetPath[500]; // buffer to store the path of the COM ports
    for (int i = 0; i < 255; i++)
    {
        std::string str = "COM" + std::to_string(i);
        DWORD test = QueryDosDevice(str.c_str(), lpTargetPath, 500);
        if (test != 0)
        {
            ports.push_back(str);
        }
    }
    return ports;
}

HANDLE SerialUtils::openSerialPort(std::string portname)
{
    m_port = portname;
    HANDLE hSerial = CreateFile(m_port.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (hSerial == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Failed to open serial port" << std::endl;
        return INVALID_HANDLE_VALUE;
    }

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams))
    {
        std::cerr << "Failed to get comm state" << std::endl;
        return INVALID_HANDLE_VALUE;
    }

    dcbSerialParams.BaudRate = m_baudRate;
    dcbSerialParams.ByteSize = m_dataBits;
    dcbSerialParams.StopBits = m_stopBits;
    dcbSerialParams.Parity = m_parity;
    if (!SetCommState(hSerial, &dcbSerialParams))
    {
        std::cerr << "Failed to set comm state" << std::endl;
        return INVALID_HANDLE_VALUE;
    }

    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    if (!SetCommTimeouts(hSerial, &timeouts))
    {
        std::cerr << "Failed to set comm timeouts" << std::endl;
        return INVALID_HANDLE_VALUE;
    }

    return hSerial;
}

void SerialUtils::closeSerialPort(HANDLE hSerial)
{
    CloseHandle(hSerial);
}

std::string SerialUtils::readSerialPort(HANDLE hSerial)
{
    DWORD bytesRead;
    char buffer[256];
    if (!ReadFile(hSerial, buffer, 256, &bytesRead, NULL))
    {
        std::cerr << "Failed to read from serial port" << std::endl;
        return "";
    }
    buffer[bytesRead] = 0;
    return std::string(buffer);
}

void SerialUtils::writeSerialPort(HANDLE hSerial, const std::string &data)
{
    DWORD bytesSend;
    if (!WriteFile(hSerial, data.c_str(), data.size(), &bytesSend, 0))
    {
        std::cerr << "Failed to write to serial port" << std::endl;
    }
}

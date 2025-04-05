#include "main.h"
#include "views.h"
#include "user_views.h"
#include "midi_play.h"
#include "midi_extract.h"
#include "serial_utils.h"
#include <thread>
#include <atomic>

// static inline int max(int a, int b)
// {
//     return a > b ? a : b;
// }

// static inline int min(int a, int b)
// {
//     return a < b ? a : b;
// }

// 全局变量
int windowWidth = 800;
int windowHeight = 600;
int screenWidth = 0;
int screenHeight = 0;
Button buttonNotefile({0, 0}, {100, 50}, NULL);
Editor editorNoteFile({110, 0}, {600, 50}, NULL);
Button buttonMidiFile({0, 60}, {100, 50}, NULL);
Editor editorMidiFile({110, 60}, {600, 50}, NULL);
Button buttonPlayMidi({0, 120}, {100, 50}, NULL);
Button buttonPlayNote({110, 120}, {100, 50}, NULL);
Button buttonExtractNotes({220, 120}, {100, 50}, NULL);
ComboBox serialPort({0, 180}, {100, 300}, NULL);
Button buttonProperity({110, 180}, {100, 25}, NULL);
Button buttonConnect({220, 180}, {100, 25}, NULL);
HANDLE hSerial = INVALID_HANDLE_VALUE;
Editor editorSerial({0, 210}, {600, 30}, NULL);
Button buttonSend({610, 210}, {60, 30}, NULL);
Editor editorReceive({0, 250}, {600, 150}, NULL);
std::vector<char> serialData;

SettingsWindow settingsWnd({300, 300}, "Settings");

// 退出线程原子量
std::atomic<bool> exitThread(false);

// 函数声明
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // 窗口过程函数
std::string OpenFile(HWND parent_hwnd, LPCSTR filter);                           // 打开文件对话框
void CreateUI(HWND hwnd, HINSTANCE hInstance);                                   // 创建UI控件
void SetCallbacks(void);                                                         // 设置回调函数

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef __TEST__
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file.mid>" << std::endl;
        return 1;
    }
    // 如果第二个参数为txt文件，则播放音符，否则播放midi文件
    if (strstr(argv[1], ".txt"))
    {
        test_play_notes(argc, argv);
    }
    else
    {
        test_play_midi_file(argc, argv);
    }
    return 0;
#endif
    // 初始化变量
    initializeNoteToPitch();
    initializePitchToNote();
    // 注册窗口类
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "MIDI Player";
    RegisterClass(&wc);

    // 获取当前显示器的分辨率(忽略设置中的缩放比例)
    HDC screen = GetDC(0);
    screenWidth = GetDeviceCaps(screen, HORZRES);
    screenHeight = GetDeviceCaps(screen, VERTRES);
    windowWidth = screenWidth / 2;
    windowHeight = screenHeight / 2;

    // 创建窗口
    HWND hwnd = CreateWindow(wc.lpszClassName, "MIDI Player", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                             windowWidth, windowHeight, NULL, NULL, hInstance, NULL);
    if (!hwnd)
    {
        std::cerr << "Failed to create window" << std::endl;
        return 1;
    }

    CreateUI(hwnd, hInstance); // 创建UI控件
    SetCallbacks();            // 设置回调函数

    // 显示窗口
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // 消息循环
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

// 窗口过程函数
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_SIZE:
    {
        // 处理窗口大小变化
        windowWidth = LOWORD(lParam);
        windowHeight = HIWORD(lParam);
        std::cout << "Window resized: width=" << windowWidth << ", height=" << windowHeight << std::endl;
        return 0;
    }
    case WM_PAINT:
    {
        // 处理窗口绘制
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        // 设置背景颜色为白色
        HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
        FillRect(hdc, &ps.rcPaint, brush);
        DeleteObject(brush);
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_COMMAND:
    {
        // 处理按钮点击事件或菜单选择事件
        if (HIWORD(wParam) == BN_CLICKED)
        {
            HWND hwndButton = (HWND)lParam;
            void (*onClick)(HWND hwnd) = (void (*)(HWND hwnd))GetWindowLongPtr(hwndButton, GWLP_USERDATA);
            if (onClick)
            {
                onClick(hwndButton);
            }
        }
        else if (HIWORD(wParam) == CBN_SELCHANGE)
        {
            HWND hwndComboBox = (HWND)lParam;
            void (*onSelect)(HWND hwnd, int index) = (void (*)(HWND hwnd, int index))GetWindowLongPtr(hwndComboBox, GWLP_USERDATA);
            if (onSelect)
            {
                int index = SendMessage(hwndComboBox, CB_GETCURSEL, 0, 0);
                onSelect(hwndComboBox, index);
            }
        }
        return 0;
    }
    case WM_DESTROY:
        // 处理窗口销毁
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

std::string OpenFile(HWND parent_hwnd, LPCSTR filter)
{
    OPENFILENAME ofn;
    char szFile[260];
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = parent_hwnd; // 设置父窗口句柄，防止用户与父窗口交互创建新窗口等
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    GetOpenFileName(&ofn);
    return ofn.lpstrFile;
}

void CreateUI(HWND hwnd, HINSTANCE hInstance)
{
    // 创建控件
    buttonNotefile.create(hwnd, hInstance, "Open Note File");
    buttonMidiFile.create(hwnd, hInstance, "Open MIDI File");
    editorMidiFile.create(hwnd, hInstance, "MIDI File Path");
    editorNoteFile.create(hwnd, hInstance, "Note File Path");
    buttonPlayMidi.create(hwnd, hInstance, "Play MIDI");
    buttonPlayNote.create(hwnd, hInstance, "Play Note");
    buttonExtractNotes.create(hwnd, hInstance, "Extract Notes");
    serialPort.create(hwnd, hInstance, "Serial Port");
    buttonConnect.create(hwnd, hInstance, "Connect");
    editorSerial.create(hwnd, hInstance, "Serial Data");
    buttonProperity.create(hwnd, hInstance, "settings");
    buttonSend.create(hwnd, hInstance, "Send");
    editorReceive.create(hwnd, hInstance, "", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | ES_MULTILINE);
    // 添加串口列表
    serialPort.addItem("Refresh");
    std::vector<std::string> ports = SerialUtils::getSerialPorts();
    for (size_t i = 0; i < ports.size(); i++)
    {
        serialPort.addItem(ports[i]);
    }
}
void SetCallbacks(void)
{
    // 设置按钮点击事件
    buttonNotefile.setOnClick([](HWND hwnd)
                              {
        std::string filePath = OpenFile(hwnd, "Text Files\0*.txt\0");
        SetWindowText(editorNoteFile.m_hwnd, filePath.c_str()); });
    buttonMidiFile.setOnClick([](HWND hwnd)
                              {
        std::string filePath = OpenFile(hwnd, "MIDI Files\0*.mid\0");
        SetWindowText(editorMidiFile.m_hwnd, filePath.c_str()); });
    buttonPlayMidi.setOnClick([](HWND hwnd)
                              {
        char filePath[256];
        GetWindowText(editorMidiFile.m_hwnd, filePath, 256);
        if(strstr(filePath, ".mid")){
            // 创建线程播放midi文件
            std::thread(playMidiFile, std::string(filePath)).detach();
        } });
    buttonPlayNote.setOnClick([](HWND hwnd)
                              {
        char filePath[256];
        GetWindowText(editorNoteFile.m_hwnd, filePath, 256);
        if(strstr(filePath, ".txt")){
            std::vector<NoteEvent> notes = readNotesFromFile(filePath);
            // 创建线程播放音符
            std::thread(playNotes, notes).detach();
        } });
    buttonExtractNotes.setOnClick([](HWND hwnd)
                                  {
        char filePath[256];
        GetWindowText(editorMidiFile.m_hwnd, filePath, 256);
        if(strstr(filePath, ".mid")){
            // 创建线程提取音符
            std::thread(extractNotes, std::string(filePath), "./notes.txt").detach();
        } });
    serialPort.setOnSelect([](HWND hwnd, int index)
                           {
        if(index == 0)
        {
            // 获取串口列表
            SendMessage(hwnd, CB_RESETCONTENT, 0, 0);
            serialPort.addItem("Refresh");
            std::vector<std::string> ports = SerialUtils::getSerialPorts();
            for (size_t i = 0; i < ports.size(); i++)
            {
                serialPort.addItem(ports[i]);
            }
        } 
        else
        {
            SendMessage(serialPort.m_hwnd, CB_SETCURSEL, (WPARAM)index, (LPARAM)0);
        } });
    buttonConnect.setOnClick([](HWND hwnd)
                             {
        char portName[256];
        GetWindowText(serialPort.m_hwnd, portName, 256);
        if(strstr(portName, "COM"))
        {
            // 如果串口已连接，则关闭串口
            if(strcmp(buttonConnect.getText().c_str(), "Disconnect") == 0)
            {
                // 把buttonConnect的文本设置为Connect，且背景色设置为默认
                SetWindowText(buttonConnect.m_hwnd, "Connect");
                SerialUtils::closeSerialPort(hSerial); // 关闭串口
                hSerial = INVALID_HANDLE_VALUE;
                // 退出读取串口数据线程
                exitThread = true;
                return;
            }
            else if(strcmp(buttonConnect.getText().c_str(), "Connect") == 0)
            {
                // 创建线程连接串口
                std::thread([](std::string portName)
                {
                    hSerial = SerialUtils::openSerialPort(portName);
                    if(hSerial != INVALID_HANDLE_VALUE)
                    {
                        // 把buttonConnect的文本设置为Disconnect
                        SetWindowText(buttonConnect.m_hwnd, "Disconnect");
                    }
                }, std::string(portName)).detach();

                exitThread = false;
                // 创建线程读取串口数据
                std::thread([](HWND hwnd)
                {
                    std::cout << "Create read serial port thread" << std::endl;
                    while(!exitThread)
                    {
                        if(hSerial != INVALID_HANDLE_VALUE)
                        {
                            std::string data = SerialUtils::readSerialPort(hSerial);
                            if(data.empty())
                            {
                                continue;
                            }
                            GetWindowText(editorReceive.m_hwnd, serialData.data(), serialData.size());
                            // 获取当前时间字符串
                            time_t now = time(0);
                            tm *ltm = localtime(&now);
                            char timeStr[256];
                            sprintf(timeStr, "[%d-%02d-%02d %02d:%02d:%02d] ", 1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday, ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
                            // 拼接新数据到后面
                            data = std::string(timeStr) + data;
                            data += "\r\n";
                            if(serialData.size() > 0)
                            {
                                serialData.resize(serialData.size() - 1);
                            }
                            serialData.insert(serialData.end(), data.begin(), data.end());
                            serialData.push_back(0);
                            // 设置新数据
                            SetWindowText(editorReceive.m_hwnd, serialData.data());
                        }
                    }
                    std::cout << "Exit read serial port thread" << std::endl;
                }, hwnd).detach();
            }
            else
            {
                return;
            }
        } });
    buttonSend.setOnClick([](HWND hwnd)
                          {
        char data[256];
        GetWindowText(editorSerial.m_hwnd, data, 256);
        if(hSerial != INVALID_HANDLE_VALUE)
        {
            SerialUtils::writeSerialPort(hSerial, std::string(data));
        } });

    buttonProperity.setOnClick([](HWND hwnd)
                               {
        HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE); 
        // 显示窗口
        settingsWnd.create(hwnd , hInstance); 
        // 添加文字
        settingsWnd.AddTextView({20, 25}, {60, 40},"Port");
        settingsWnd.AddComboBox({130, 25}, {120, 100}, NULL, SerialUtils::getSerialPorts());
        settingsWnd.AddTextView({20, 50}, {60, 40},"BaudRate");
        settingsWnd.AddComboBox({130, 50}, {120, 100}, NULL, 
                                                            {
                                                                "115200", // 现代设备最高标准速率
                                                                "230400", // 扩展高速1
                                                                "460800", // 扩展高速2
                                                                "921600",  // 超高速模式
                                                                "110",    // 老式电传打印机
                                                                "300",    // 早期调制解调器
                                                                "600",    // 工业控制设备
                                                                "1200",   // 基础串行通信
                                                                "2400",   // 基本串口速率
                                                                "4800",   // 常见低速设备
                                                                "9600",   // 最常用默认速率
                                                                "14400",  // 中速设备
                                                                "19200",  // 中高速设备
                                                                "38400",  // 高速设备
                                                                "57600",  // USB转串口常用
                                                            });
        settingsWnd.AddTextView({20, 75}, {60, 40},"Data Bits");
        settingsWnd.AddComboBox({130, 75}, {120, 100}, NULL,{"8","7","6","5"});
        settingsWnd.AddTextView({20, 100}, {60, 40},"Stop Bits");
        settingsWnd.AddComboBox({130, 100}, {120, 100}, NULL, {"1","1.5","2"});
        settingsWnd.AddTextView({20, 125}, {60, 40},"Parity");
        settingsWnd.AddComboBox({130, 125}, {120, 100}, NULL, {"None","Odd","Even","Mark","Space"});
        settingsWnd.AddButton({50, 200}, {60, 30}, "OK" ,[](HWND hwnd)
        {
            SerialUtils::m_port = settingsWnd.m_combos[0].GetComboSelectedText();
            SerialUtils::m_baudRate = atoi(settingsWnd.m_combos[1].GetComboSelectedText().c_str());
            SerialUtils::m_dataBits = atoi(settingsWnd.m_combos[2].GetComboSelectedText().c_str());
            const char * temp_Ptr = settingsWnd.m_combos[3].GetComboSelectedText().c_str();
            if(strcmp(temp_Ptr,"1")==0)
            {
                SerialUtils::m_stopBits = ONESTOPBIT;
            }
            else if(strcmp(temp_Ptr,"1.5")==0)
            {
                SerialUtils::m_stopBits = ONE5STOPBITS;
            }
            else if(strcmp(temp_Ptr,"2")==0)
            {
                SerialUtils::m_stopBits = TWOSTOPBITS;
            }
            temp_Ptr = settingsWnd.m_combos[4].GetComboSelectedText().c_str();
            if(strcmp(temp_Ptr,"None")==0)
            {
                SerialUtils::m_parity = NOPARITY;
            }
            else if(strcmp(temp_Ptr,"Odd")==0)
            {
                SerialUtils::m_parity = ODDPARITY;
            }
            else if(strcmp(temp_Ptr,"Even")==0)
            {
                SerialUtils::m_parity = EVENPARITY;
            }
            else if(strcmp(temp_Ptr,"Mark")==0)
            {
                SerialUtils::m_parity = MARKPARITY;
            }
            else if(strcmp(temp_Ptr,"Space")==0)
            {
                SerialUtils::m_parity = SPACEPARITY;
            }
            CloseWindow(settingsWnd.m_hwnd);
        });
        settingsWnd.AddButton({150, 200}, {60, 30}, "Cancel" ,[](HWND hwnd)
        {
            CloseWindow(settingsWnd.m_hwnd);
        }); });
}
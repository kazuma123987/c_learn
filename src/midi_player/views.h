#ifndef __VIEWS_H__
#define __VIEWS_H__
#include <stdint.h>
#include <windows.h>
#include <winuser.h>
#include <string>
#include <vector>

typedef struct __VEC2
{
    uint32_t x;
    uint32_t y;
} VEC2;

class BasicView
{
public:
    HWND m_hwnd;
    HMENU id = NULL;
    VEC2 bkPos = {0, 0};
    VEC2 bkSize = {0, 0};
    BasicView(VEC2 pos, VEC2 size, HMENU id = NULL);
    void setbkPos(VEC2 bkPos);
    void setbkSize(VEC2 bkSize);
    virtual void create(HWND parent_hwnd, HINSTANCE hinstance, std::string text = "", DWORD style = WS_CHILD) = 0;
};

class Button : public BasicView
{
public:
    Button(VEC2 pos, VEC2 size, HMENU id = NULL);
    ~Button();
    void create(HWND parent_hwnd, HINSTANCE hinstance, std::string text = "", DWORD style = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON);
    void setOnClick(void (*onClick)(HWND hwnd));
    std::string getText();
};

class Editor : public BasicView
{
public:
    Editor(VEC2 pos, VEC2 size, HMENU id = NULL);
    ~Editor();
    void create(HWND parent_hwnd, HINSTANCE hinstance, std::string text = "", DWORD style = WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL);
};

// 下拉框
class ComboBox : public BasicView
{
public:
    std::vector<std::string> items;
    ComboBox(VEC2 pos, VEC2 size, HMENU id = NULL);
    ~ComboBox();
    void create(HWND parent_hwnd, HINSTANCE hinstance, std::string text = "", DWORD style = WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | CBS_HASSTRINGS);
    void addItem(std::string item);                           // 添加选项
    void setOnSelect(void (*onSelect)(HWND hwnd, int index)); // 选中索引回调
    std::string GetComboSelectedText();
};

// 滑动条
class ScrollBar : public BasicView
{
public:
    ScrollBar(VEC2 pos, VEC2 size, HMENU id = NULL);
    ~ScrollBar();
    void create(HWND parent_hwnd, HINSTANCE hinstance, std::string text = "", DWORD style = WS_CHILD | WS_VISIBLE | SBS_VERT);
    void setOnScroll(void (*onScroll)(HWND hwnd, int pos));
};

// 正方形组区域
class GroupBox : public BasicView
{
public:
    GroupBox(VEC2 pos, VEC2 size, std::string text);
    void create(HWND parent_hwnd, HINSTANCE hinstance, std::string text = "settings", DWORD style = WS_CHILD | WS_VISIBLE | BS_GROUPBOX);
};

// 弹出窗口
class PopupWindow
{
public:
    HWND m_hwnd;
    HINSTANCE m_hInstance;
    VEC2 m_size;
    std::string m_title;
    PopupWindow(VEC2 size, std::string title);
    virtual void create(HWND parent, HINSTANCE hInstance) = 0;
};

// 静态文本
class TextView : public BasicView
{
public:
    TextView(VEC2 pos, VEC2 size, std::string text = "", int fontSize = 10, HMENU id = NULL);
    ~TextView();
    void create(HWND parent_hwnd, HINSTANCE hinstance, std::string text = "", DWORD style = WS_CHILD | WS_VISIBLE | SS_LEFT);
    void setFontSize(int size);

private:
    std::string m_text;
    HFONT m_font = NULL;
    int m_fontSize = 8;
};
#endif
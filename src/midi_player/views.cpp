#include "views.h"

static std::string WStringToString(const std::wstring &wstr, UINT codePage = CP_UTF8)
{
    if (wstr.empty())
        return std::string();

    // 计算需要的缓冲区大小
    int bufferSize = WideCharToMultiByte(
        codePage,     // 目标编码 (CP_UTF8、CP_ACP 等)
        0,            // 转换标志
        wstr.c_str(), // 输入宽字符串
        -1,           // 自动计算长度
        nullptr,      // 不预接收输出
        0,            // 仅计算所需缓冲区大小
        nullptr,      // 默认字符（未映射时使用）
        nullptr       // 是否使用默认字符
    );

    if (bufferSize == 0)
    {
        return ""; // 转换失败
    }

    // 分配缓冲区
    std::string str(bufferSize, 0);

    // 实际转换
    WideCharToMultiByte(
        codePage,
        0,
        wstr.c_str(),
        -1,
        &str[0],
        bufferSize,
        nullptr,
        nullptr);

    // 去除末尾的 '\0'（如果存在）
    if (!str.empty() && str.back() == '\0')
    {
        str.pop_back();
    }

    return str;
}

BasicView::BasicView(VEC2 pos, VEC2 size, HMENU id)
{
    this->bkPos = pos;
    this->bkSize = size;
    this->id = id;
}

void BasicView::setbkPos(VEC2 bkPos)
{
    this->bkPos = bkPos;
}
void BasicView::setbkSize(VEC2 bkSize)
{
    this->bkSize = bkSize;
}

Button::Button(VEC2 pos, VEC2 size, HMENU id) : BasicView(pos, size, id)
{
}

Button::~Button()
{
}

void Button::create(HWND parent_hwnd, HINSTANCE hinstance, std::string text, DWORD style)
{
    m_hwnd = CreateWindow("BUTTON", text.c_str(), style, bkPos.x, bkPos.y, bkSize.x, bkSize.y, parent_hwnd, id, hinstance, NULL);
}

void Button::setOnClick(void (*onClick)(HWND hwnd))
{
    SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)onClick);
}

std::string Button::getText()
{
    char text[256];
    GetWindowText(m_hwnd, text, 256);
    return std::string(text);
}

Editor::Editor(VEC2 pos, VEC2 size, HMENU id) : BasicView(pos, size, id)
{
}

Editor::~Editor()
{
}

void Editor::create(HWND parent_hwnd, HINSTANCE hinstance, std::string text, DWORD style)
{
    m_hwnd = CreateWindow("EDIT", text.c_str(), style, bkPos.x, bkPos.y, bkSize.x, bkSize.y, parent_hwnd, id, hinstance, NULL);
}

ComboBox::ComboBox(VEC2 pos, VEC2 size, HMENU id) : BasicView(pos, size, id)
{
}

ComboBox::~ComboBox()
{
}

void ComboBox::create(HWND parent_hwnd, HINSTANCE hinstance, std::string text, DWORD style)
{
    m_hwnd = CreateWindow("COMBOBOX", text.c_str(), style, bkPos.x, bkPos.y, bkSize.x, bkSize.y, parent_hwnd, id, hinstance, NULL);
}

void ComboBox::addItem(std::string item)
{
    int result = SendMessage(m_hwnd, CB_ADDSTRING, 0, (LPARAM)item.c_str());
    if (result == CB_ERR)
    {
        MessageBox(NULL, "Failed to add item to ComboBox", "Error", MB_OK | MB_ICONERROR);
    }
    else if (result == CB_ERRSPACE)
    {
        MessageBox(NULL, "Failed to add item to ComboBox: insufficient space", "Error", MB_OK | MB_ICONERROR);
    }
    else
    {
        items.push_back(item);
    }
}

void ComboBox::setOnSelect(void (*onSelect)(HWND hwnd, int index))
{
    SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)onSelect);
}

std::string ComboBox::GetComboSelectedText()
{
    // 获取当前选中项的索引
    int index = SendMessage(m_hwnd, CB_GETCURSEL, 0, 0);
    if (index == CB_ERR)
        return ""; // 没有选中项
    if(this->items.empty())
    {
        return "";
    }
    return this->items[index];
}

ScrollBar::ScrollBar(VEC2 pos, VEC2 size, HMENU id) : BasicView(pos, size, id)
{
}

ScrollBar::~ScrollBar()
{
}

void ScrollBar::create(HWND parent_hwnd, HINSTANCE hinstance, std::string text, DWORD style)
{
    m_hwnd = CreateWindow("SCROLLBAR", text.c_str(), style, bkPos.x, bkPos.y, bkSize.x, bkSize.y, parent_hwnd, id, hinstance, NULL);
}

void ScrollBar::setOnScroll(void (*onScroll)(HWND hwnd, int pos))
{
    SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)onScroll);
}

PopupWindow::PopupWindow(VEC2 size, std::string title)
    : m_size(size), m_title(title) {}

GroupBox::GroupBox(VEC2 pos, VEC2 size, std::string text)
    : BasicView(pos, size, NULL)
{
    // 继承text参数需要扩展BasicView
}

void GroupBox::create(HWND parent, HINSTANCE hInst, std::string text, DWORD style)
{
    (void)text;
    (void)style;
    m_hwnd = CreateWindow(
        "BUTTON",
        text.c_str(),
        style,
        bkPos.x, bkPos.y,
        bkSize.x, bkSize.y,
        parent,
        id,
        hInst,
        NULL);
}

TextView::TextView(VEC2 pos, VEC2 size, std::string text, int fontSize, HMENU id) : BasicView(pos, size, id)
{
    m_text = text;
    m_fontSize = fontSize;
}

TextView::~TextView()
{
}

void TextView::create(HWND parent_hwnd, HINSTANCE hinstance, std::string text, DWORD style)
{
    (void)text;
    m_hwnd = CreateWindow("STATIC", m_text.c_str(), style, bkPos.x, bkPos.y, bkSize.x, bkSize.y, parent_hwnd, NULL, hinstance, NULL);
    setFontSize(m_fontSize);
}

void TextView::setFontSize(int size)
{
    if (m_font)
        DeleteObject(m_font);

    LOGFONT lf;
    SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(lf), &lf, 0);
    lf.lfHeight = -MulDiv(size, GetDeviceCaps(GetDC(NULL), LOGPIXELSY), 72);

    m_font = CreateFontIndirect(&lf);
    SendMessage(m_hwnd, WM_SETFONT, (WPARAM)m_font, TRUE);
}
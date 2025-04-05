#include "user_views.h"

SettingsWindow::SettingsWindow(VEC2 size, std::string title) : PopupWindow(size, title),
                                                               m_group({0, 0}, {0, 0}, "") {}
SettingsWindow::~SettingsWindow() {
};

void SettingsWindow::create(HWND parent, HINSTANCE hInstance)
{
    const char *className = "PopupWindowClass";
    m_hInstance = hInstance;

    // 注册窗口类
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = m_hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = className;

    RegisterClassEx(&wc);

    // 创建窗口
    m_hwnd = CreateWindowEx(
        0,
        className,
        m_title.c_str(),
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        m_size.x, m_size.y,
        parent,
        NULL,
        m_hInstance,
        this);

    // 创建分组框
    m_group.bkPos = {10, 10};
    m_group.bkSize = {m_size.x - 40, m_size.y - 130};
    m_group.create(m_hwnd, m_hInstance);
}

void SettingsWindow::AddComboBox(VEC2 pos, VEC2 size, HMENU id, const std::vector<std::string> &items)
{
    ComboBox combo(pos, size, id);
    combo.bkPos = pos;
    combo.bkSize = size;

    if (m_hwnd)
    {
        combo.create(m_hwnd, m_hInstance, "", WS_CHILD | WS_VISIBLE | CBS_DROPDOWN);
        for (const auto &item : items)
        {
            combo.addItem(item);
        }
        SendMessage(combo.m_hwnd, CB_SETCURSEL, 0, 0); // 默认选第一个
        this->m_combos.push_back(combo);
    }
    else
    {
        this->m_combos.push_back(combo);
    }
}

void SettingsWindow::AddTextView(VEC2 pos, VEC2 size, const std::string &text)
{
    TextView textview(pos, size, text);
    this->m_textviews.push_back(textview);

    if (m_hwnd)
    {
        textview.create(m_hwnd, m_hInstance);
    }
}

void SettingsWindow::AddButton(VEC2 pos, VEC2 size, const std::string &text, void (*onClick)(HWND hwnd))
{
    Button btn(pos, size);
    this->m_buttons.push_back(btn);

    if (m_hwnd)
    {
        btn.create(m_hwnd, m_hInstance, text);
        btn.setOnClick(onClick);
    }
}

LRESULT CALLBACK SettingsWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    SettingsWindow *pThis = nullptr;
    if (msg == WM_NCCREATE)
    {
        CREATESTRUCT *cs = (CREATESTRUCT *)lParam;
        pThis = static_cast<SettingsWindow *>(cs->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
        pThis->m_hwnd = hwnd;
    }
    else
    {
        pThis = reinterpret_cast<SettingsWindow *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (pThis)
        return pThis->HandleMessage(msg, wParam, lParam);
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT SettingsWindow::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        return 0;
    case WM_DESTROY:
        return 0;
    // 在父窗口的窗口过程中处理消息
    case WM_CTLCOLORSTATIC:
    {
        HDC hdc = (HDC)wParam;
        SetBkMode(hdc, TRANSPARENT);                  // 关键设置
        SetTextColor(hdc, RGB(0, 0, 0));              // 设置文字颜色
        return (LRESULT)GetStockObject(HOLLOW_BRUSH); // 返回空画刷
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
        return 0;
    }
    default:
        return DefWindowProc(m_hwnd, msg, wParam, lParam);
    }
}
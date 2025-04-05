#ifndef __USER_VIEWS_H
#define __USER_VIEWS_H

#include "views.h"

class SettingsWindow : public PopupWindow
{
public:
    std::vector<ComboBox> m_combos;
    std::vector<TextView> m_textviews;
    std::vector<Button> m_buttons;
    void create(HWND parent, HINSTANCE hInstance);
    void AddComboBox(VEC2 pos, VEC2 size, HMENU id, const std::vector<std::string> &items);
    void AddTextView(VEC2 pos, VEC2 size, const std::string &text);
    void AddButton(VEC2 pos, VEC2 size, const std::string &text, void (*onClick)(HWND hwnd));
    SettingsWindow(VEC2 size, std::string title);
    ~SettingsWindow();

private:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
    GroupBox m_group;
};

#endif
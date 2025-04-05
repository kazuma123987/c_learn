#include <stdint.h>

typedef enum PAGE_STATE
{
    PAGE_STATE_MAIN,
    PAGE_STATE_INFO,
    PAGE_STATE_SETTINGS
} PAGE_STATE;

PAGE_STATE page_state = PAGE_STATE_MAIN;
uint8_t page_position = 0;
#define KEY_UP 1
#define KEY_DOWN 2
#define KEY_ENTER 3
void logic(void)
{
}
uint8_t getkey()
{
}
void cursor_move(uint8_t direction)
{
}

int main(void)
{
    init();
    while (1)
    {
        logic();
        uint8_t key = getkey();
        switch (page_state)
        {
        case PAGE_STATE_MAIN:
            if (key == KEY_UP)
            {
                cursor_move(KEY_UP);
            }
            else if (key == KEY_DOWN)
            {
                cursor_move(KEY_DOWN);
            }
            else if (key == KEY_ENTER)
            {
                if (page_position == 0)
                    page_state = PAGE_STATE_INFO;
                else if (page_position == 1)
                    page_state = PAGE_STATE_SETTINGS;
            }
            break;

        case PAGE_STATE_INFO:
            /* code */
            break;

        case PAGE_STATE_SETTINGS:

            break;
        }
        OLED_Update();
    }
}
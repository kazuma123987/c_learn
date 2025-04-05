#include <stdio.h>
#include <string.h>
#include <math.h>
#include <locale.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t ERR_8_T;

ERR_8_T convert_worktime(const char* worktime_string, int* hour_ptr, int* minute_ptr);

int main(int argc, char** argv)
{
	int a = 0;
	int hour = 0;
	int minute = 0;
	ERR_8_T err_flag = 0;
	setlocale(LC_CTYPE, "");
	if (argc == 1)
	{
		wprintf(L"本软件旨在方便大家计算下班时间和含优质餐补的下班时间\n----------------------------------\n(Created by ADS R&D2 Ma,YuanXin)\n");
		wprintf(L"请输入上班时间：\n（请输入小数，例如8点30分上班请输入8.30或者8.3，8点3分上班请输入8.03，然后请按回车键）\n");
	}
	char temp[128];
	volatile uint8_t run_flag = 0;
	do
	{
		memset(temp, 0, sizeof(temp));
		if (argc > 1 && (strcmp(argv[1], "-t") == 0 || strcmp(argv[1], "--t") == 0 || strcmp(argv[1], "-time") == 0 || strcmp(argv[1], "--time") == 0))
		{
			err_flag = convert_worktime(argv[2], &hour, &minute);
			run_flag = 1;
		}
		else
		{
			scanf_s("%s", temp, sizeof(temp));
			err_flag = convert_worktime(temp, &hour, &minute);
		}
		if (err_flag != 0)
			wprintf(L"请输入合法的上班时间,err_flag=%d\n", err_flag);
		else
		{
			int start = minute + hour * 60;
			int end = start + 8 * 60 + 45;
			wprintf(L"-----------------------\n"
				"您的上班时间为 %02d:%02d\n"
				"正常下班时间为 %02d:%02d\n"
				"享受优质餐补加班时间为:\n"
				" 就餐加班   直落加班\n"
				" %02d:%02d       %02d:%02d\n"
				"-----------------------\n",
				hour, minute, end / 60, end % 60, (end + 210) / 60, (end + 210) % 60, (end + 180) / 60, (end + 180) % 60);
		}
	} while (run_flag == 0);
	return 0;
}

ERR_8_T convert_worktime(const char* worktime_string, int* hour_ptr, int* minute_ptr)
{
	ERR_8_T err_flag = 0;
	int length = strlen(worktime_string);
	if (length > 4)
	{
		err_flag = 1;
	}
	else
	{
		for (int i = 0; i < length; i++)
		{
			if ((worktime_string[i] < '0' || worktime_string[i] > '9'))
			{
				if (worktime_string[i] == '.' && i != 1)
				{
					err_flag = 2;
					break;
				}
			}
		}
		if (err_flag != 2)
		{
			int minutes = 0;
			switch (length)
			{
			case 1:
			case 2:
				if (worktime_string[0] < '7' || worktime_string[0] > '9')
				{
					err_flag = 3;
				}
				else
				{
					hour_ptr[0] = worktime_string[0] - '0';
					minute_ptr[0] = 0;
				}
				break;
			case 3:
			case 4:
				minutes = (worktime_string[0] - '0') * 60 + atoi(&worktime_string[2]);
				if (worktime_string[2] >= '6' || minutes > (9 * 60 + 30) || minutes < (7 * 60 + 30))
				{
					err_flag = 3;
				}
				else
				{
					hour_ptr[0] = worktime_string[0] - '0';
					minute_ptr[0] = atoi(&worktime_string[2]);
				}
				break;

			default:
				break;
			}
		}
	}
	return err_flag;
}
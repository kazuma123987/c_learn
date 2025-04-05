#include <stdio.h>
#include <wchar.h>
#include <locale.h>
extern int s1;
extern int _text_start; // 存放函数段
extern int _text_end;
extern int _custom_data_start; // 自定义段
extern int _custom_data_end;
extern int _custom_func_start; // 自定义段
extern int _custom_func_end;
extern int _custom_start; // 自定义段
extern int _custom_end;
extern int _data_load_start; // 数据段加载地址
extern int __data_start__;	 // 存放已初始化非0的static变量和global全局变量,否则存放在全局区(bss段)
extern int __rdata_start;	 // 存放常量(只读)
extern int __rt_psrelocs_start;
extern int __rt_psrelocs_end;
extern int __pdata_start; // pdata
extern int __pdata_end;
extern int __xdata_start; // xdata
extern int __xdata_end;
extern int __bss_start__; // 存放未初始化或初始化为0的static变量和全局变量
extern int __bss_end__;
// 定义段(section)属性
#define _AT(addr) __attribute__((at(addr)))
#define CUSTOM_OTHER_DATA __attribute__((section(".custom_data")))
#define CUSTOM_OTHER_FUNC __attribute__((section(".custom_func")))
#define CUSTOM_DATA __attribute__((section(".custom.data")))
#define CUSTOM_FUNC __attribute__((section(".custom.func")))
#define CUSTOM_API __attribute__((__stdcall__))
#define PACKED __attribute__((__packed__))
#define ALIGNED __attribute__((aligned))
#define ALIGN(x) __attribute__((aligned(x))) // x的有效参数为2的n次幂,即1,2,4,8;该属性会与系统默认对其字节进行比较,并取较大者
// 64位系统默认为8字节对齐,32位为4字节对齐,64位系统下,n<8时,取8,n>8时,取n
CUSTOM_OTHER_DATA int c[5];
CUSTOM_DATA int array;									// 把array定义到.custom_data段
CUSTOM_FUNC CUSTOM_API void linker_swap(int *a, int *b) // 把linker_swap函数定义到.custom_func段
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}
// 位域存储规则:若相邻元素类型相同且未超过类型大小(int是4字节),则紧凑排列,这里abc紧凑排序,d单独排序,共占用4+1=5个字节(packed属性)
typedef struct PACKED Node
{
	int a : 1;
	int b : 1;
	int c : 1;
	char d : 1;
} Node;
typedef struct PACKED xNode
{
	char a : 1;
	char b : 1;
	unsigned char c : 1; // 只要占用字节数相同,都可以定义
	unsigned char : 4;	 // 未定义,占4个位,这里一定要定义为占一个字节的类型,否则占用字节数会变成1+4+1=6个
	char d : 1;
} xNode;

int global_init_zero = 0;				   // bss
int global_no_init;						   // bss
int global_init_nozero = 10;			   // data
static int static_global_init_zero = 0;	   // bss
static int static_global_no_init;		   // bss
static int static_global_init_nozero = 10; // data
int program()
{
	setlocale(LC_CTYPE, "");
	int stack_no_init;						  // stack
	int stack_init_zero = 0;				  // stack
	char *stack_init_with_const = "123456";	  // rodata
	static int static_stack_init_zero = 0;	  // bss
	static int static_stack_no_init;		  // bss
	static int static_stack_init_nozero = 10; // data
	wprintf(L"段的地址:\n"
			"s1:0x%x\n"
			"text_start:0x%x\n"
			"text_end:0x%x\n"
			"custom_data_start:0x%x\n"
			"custom_data_end:0x%x\n"
			"custom_func_start:0x%x\n"
			"custom_end_start:0x%x\n"
			"custom_start:0x%x\n"
			"custom_end:0x%x\n"
			"data_load_start:0x%x\n"
			"data_start:0x%x\n"
			"rdata_start:0x%x\n"
			"rdata_end:0x%x\n"
			"pdata_start:0x%x\n"
			"pdata_end:0x%x\n"
			"xdata_start:0x%x\n"
			"xdata_end:0x%x\n"
			"bss_start:0x%x\n"
			"bss_end:0x%x\n"
			"自定义数据和函数的地址:\n"
			"array:0x%x\n"
			"swap:0x%x\n"
			"c:0x%x\n"
			"位域的大小计算:\n"
			"sizeof(Node):%ld\n"
			"sizeof(xNode):%ld\n"
			"全局变量的地址:\n"
			"global_no_init:0x%x\n"
			"global_init_zero:0x%x\n"
			"global_init_nozero :0x%x\n"
			"static全局变量的地址:\n"
			"static_global_init_zero :0x%x\n"
			"static_global_no_init:0x%x\n"
			"static_global_init_nozero :0x%x\n"
			"局部变量(栈变量)的地址:\n"
			"stack_no_init:0x%x\n"
			"stack_init_zero:0x%x\n"
			"stack_init_with_const:0x%x\n"
			"static局部变量的地址:\n"
			"static_stack_init_zero :0x%x\n"
			"static_stack_no_init:0x%x\n"
			"static_stack_init_nozero :0x%x\n",
			&s1, &_text_start, &_text_end, &_custom_data_start, &_custom_data_end, &_custom_func_start, &_custom_func_end,
			&_custom_start, &_custom_end, &_data_load_start, &__data_start__, &__rdata_start, &__rt_psrelocs_end,
			&__pdata_start, &__pdata_end, &__xdata_start, &__xdata_end, &__bss_start__, &__bss_end__,
			&array, &linker_swap, &c, sizeof(Node), sizeof(xNode),
			&global_no_init, &global_init_zero, &global_init_nozero, &static_global_init_zero, &static_global_no_init,
			&static_global_init_nozero, &stack_no_init, &stack_init_zero, stack_init_with_const, &static_stack_init_zero,
			&static_stack_no_init, &static_stack_init_nozero);
	int a = 2, b = 4;
	linker_swap(&a, &b);
	printf("a=%d,b=%d\n", a, b);
	_wsystem(L"pause");
	return 0;
}
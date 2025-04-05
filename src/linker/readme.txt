gcc_default.ld是gcc默认使用的链接器脚本,test.ld是在gcc_default.ld的基础上自定义的链接脚本
此外linker.c中的入口程序在test.ld中被定义为了program,相当于gcc -e program,但还需要显示的指出不要使用任何启动文件
即gcc -nostartfiles
注意:
a.在ld文件中,'.'表示当前地址,且只能在SECTIONS里面使用
b.为ld文件的变量赋值时,要注意"左值[空格]=[空格]右值;"这种赋值方式,如s1 = .; 少了一个空格都会报错
c.KEEP()可以让指定的数据不被编译器优化
d.LOADADDR()可以返回指定段(section)的加载地址(LMA),而不是虚拟地址(VMA)
使用方法:
gcc linker.c -T ./test.ld -nostartfiles -o linker.exe
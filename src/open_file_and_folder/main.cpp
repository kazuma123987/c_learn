#include <iostream>
#include <filesystem>
#include <string>
#include <windows.h> //使用其他windows函数前要添加windows.h
#include <commdlg.h> //打开文件的头文件
#include <shlobj.h>  //打开文件夹的头文件

int main(int argc, char *argv[])
{
    // 打开文件(open file name a)
    OPENFILENAMEA ofn = {0};
    char fileName[MAX_PATH] = {0};
    ofn.lStructSize = sizeof(OPENFILENAMEA);
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = sizeof(fileName);
    ofn.lpstrFilter = "C/C++ Files\0*.c;*.h\0JPEG Files\0*.jpg\0All Files\0*.*\0";
    ofn.Flags = OFN_PATHMUSTEXIST;
    if (GetOpenFileNameA(&ofn))
        std::cout << ofn.lpstrFile << std::endl;

    // 打开文件夹(browse info a)
    BROWSEINFOA browseInfo = {0};
    char folderPath[MAX_PATH] = {0};
    browseInfo.hwndOwner = NULL;
    browseInfo.lpszTitle = "选择文件夹";
    browseInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS;
    LPITEMIDLIST folder = SHBrowseForFolderA(&browseInfo);
    if (folder != NULL)
    {
        SHGetPathFromIDListA(folder, folderPath);
        CoTaskMemFree(folder);
        std::cout << "选择的文件夹路径：" << folderPath << std::endl;
    }

    // 获取当前目录
    char filePath[FILENAME_MAX] = {0};
    GetCurrentDirectoryA(FILENAME_MAX, filePath); // 替换为你的目录路径
    std::string directoryPath(filePath);
    // 循环遍历查找.c文件并输出
    for (const auto &entry : std::filesystem::directory_iterator(directoryPath))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".c")
        {
            std::cout << entry.path().filename().string() << std::endl;
        }
    }
}
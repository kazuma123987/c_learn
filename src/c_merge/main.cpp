#include <string>
#include <vector>
#include <windows.h> //使用其他windows函数前要添加windows.h
#include <shlobj.h>  //打开文件夹的头文件
#include <commdlg.h> //打开文件的头文件
#include <dirent.h>
#include <stdio.h>

std::vector<std::string> get_files_from_extension(const char *path, const char *extension);
size_t find_char_index(const char *string, char ch);

int main(int argc, char *argv[])
{
    // 打开文件夹
    BROWSEINFOA browseInfo = {0};
    char folderPath[MAX_PATH] = {0};
    browseInfo.hwndOwner = NULL;
    browseInfo.lpszTitle = "select your folder with *.c";
    browseInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS;
    LPITEMIDLIST folder = SHBrowseForFolderA(&browseInfo);
    // 如果文件夹不是空的
    if (folder)
    {
        SHGetPathFromIDListA(folder, folderPath);
        printf("open folder:%s\n", folderPath);
        CoTaskMemFree(folder);
    }
    else
    {
        printf("ERROR::Open File Failed\n");
        return 1;
    }
    // 根据文件夹路径获取所有.c文件
    std::vector<std::string> c_file_paths = get_files_from_extension(folderPath, ".c");

    // 保存(打开文件)
    OPENFILENAMEA ofn = {0};
    char fileName[MAX_PATH] = {0};
    ofn.lStructSize = sizeof(OPENFILENAMEA);
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = sizeof(fileName);
    ofn.lpstrFilter = "C Files\0*.c;*.h\0";
    ofn.Flags = OFN_PATHMUSTEXIST;

    FILE *merge_file = NULL;
    FILE *fp = NULL;
    size_t index = 0;
    // 如果保存了文件
    if (GetSaveFileNameA(&ofn))
    {
        printf("save path:%s\n", ofn.lpstrFile);
        fopen_s(&merge_file, ofn.lpstrFile, "w");
        if (merge_file)
        {
            fprintf(merge_file, "const unsigned char g_array[]");
            fopen_s(&fp, c_file_paths[0].c_str(), "r");
            if (fp)
            {
                fseek(fp, 0, SEEK_END);
                size_t file_length = ftell(fp);
                char *tmp = (char *)malloc(file_length + 1);
                char *save_tmp = tmp;
                rewind(fp);
                fread(tmp, sizeof(char), file_length, fp);
                tmp = strchr(tmp, '[');
                index = find_char_index(tmp, '=');
                fwrite(tmp, sizeof(char), index + 1, merge_file);
                fputs(" {\n", merge_file);
                free(save_tmp);
            }
            else
            {
                printf("please find a directory with c files\n");
                return 1;
            }
            fclose(fp);
        }
        else
        {
            printf("ERROR:failed to open the path:%s\n", ofn.lpstrFile);
            return 1;
        }
    }
    else
    {
        printf("ERROR::Save File Failed\n");
        return 1;
    }

    // 循环遍历当前目录下的文件
    for (size_t i = 0; i < c_file_paths.size(); i++)
    {
        printf("%llu:%s\n", i, c_file_paths[i].c_str());
        fopen_s(&fp, c_file_paths[i].c_str(), "r");
        if (fp)
        {
            // 获取文件长度
            fseek(fp, 0, SEEK_END);
            size_t file_length = ftell(fp);
            // 分配内存
            char *tmp = (char *)malloc(file_length + 1);
            char *save_tmp = tmp;
            // 返回文件头并把文件所有数据写入tmp
            rewind(fp);
            fread(tmp, sizeof(char), file_length, fp);
            if (strstr(tmp, "const unsigned char") == NULL) // 查找开头是否正确
                continue;
            tmp[file_length] = '\0';
            // 写入数据到merge_file的文件
            fputs("{ /* ", merge_file);                  // 先写入'{ /*'
            tmp = strstr(tmp, "_");                      // 找到'_'
            tmp++;                                       // 跳过'_'
            index = find_char_index(tmp, '[');           // 找到[索引,目的是写注释时区分名字(gImage_WIFI[128]可提取‘WIFI’)
            tmp[index] = '\0';                           // 字符串结束
            fprintf(merge_file, "%s %llu */\n", tmp, i); // 写注释结束,准备写数据
            tmp += (index + 1);                          // 跳过'\0'
            tmp += (find_char_index(tmp, '\n') + 1);     // 跳过第一行
            while (tmp)
            {
                index = find_char_index(tmp, '\n'); // index也是'\n'之前的字符数量
                fwrite(tmp, sizeof(char), tmp[index - 1] == '\r' ? index - 1 : index, merge_file);
                if (tmp[index + 1] == '}')
                {
                    fseek(merge_file, -1, SEEK_CUR);
                    break;
                }
                tmp += (index + 1);
                fputc('\n', merge_file);
            }
            fputc('}', merge_file); // 写入'}'
            if (i < c_file_paths.size() - 1)
                fputs(",\n", merge_file);
            free(save_tmp);
        }
        else
            continue;
        fclose(fp);
    }
    fputs("};", merge_file); // 最后补上括号

    fclose(merge_file);
    printf("success!\n");
    return 0;
}

std::vector<std::string> get_files_from_extension(const char *path, const char *extension)
{
    std::vector<std::string> c_files_vector;
    DIR *directory = opendir(path);
    if (directory == NULL)
    {
        printf("无法打开目录: %s\n", path);
        return c_files_vector;
    }
    std::string fullDir = path;
    fullDir += "\\";
    struct dirent *entry; // dirent结构体:char d_name[260]存放文件路径名,d_namelen存放路径长度
    // 每次调用readdir会把指针指向下一个文件
    while ((entry = readdir(directory)) != NULL)
    {
        // 找到文件拓展 ".xx"
        char *tmp = entry->d_name, *ex_pointer = NULL;
        do
        {
            tmp = strchr(tmp, '.');
            if (tmp)
            {
                ex_pointer = tmp;
                tmp++;
            }
        } while (tmp);
        // 判断后缀是否正确
        if (ex_pointer != NULL && strcmp(ex_pointer, extension) == 0)
        {
            std::string fullPath = fullDir + entry->d_name;
            c_files_vector.emplace_back(fullPath);
        }
    }

    closedir(directory);
    return c_files_vector;
}

size_t find_char_index(const char *string, char ch)
{
    size_t i = 0;
    while (string != NULL)
    {
        if (*string == ch)
            return i;
        i++;
        string++;
    }
    return SIZE_MAX;
}
#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

int main()
{
    SYSTEM_INFO info;
    HANDLE handleFisFile, handleMappingFile;
    handleFisFile = CreateFileA("D://NIR_dir/os/os/4_3/testfile.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (handleFisFile == INVALID_HANDLE_VALUE)
        printf("handleFisFile  ERROR !!%x \n", GetLastError());
    else
        cout << "Fisfile created" << endl;


    handleMappingFile = CreateFileMappingA(handleFisFile, NULL, PAGE_READWRITE,0,512,"Local\\TEST" );
    if (handleMappingFile == INVALID_HANDLE_VALUE)
        printf("handleMappingFile ERROR !! %x \n", GetLastError());
    else
        cout << "Mapping file created" << endl;
    GetSystemInfo(&info);
    char* a =(char*)  MapViewOfFile(handleMappingFile, FILE_MAP_ALL_ACCESS,0, 0, 512);
    cout << "\n" << "Adress memory: 0x" << hex << (unsigned long)a << dec << endl;
    string str;

    cout << "Press string: " << endl;
    getline(cin, str);
    CopyMemory(a, str.c_str(), str.length());

    system("pause");
    UnmapViewOfFile(a);
}
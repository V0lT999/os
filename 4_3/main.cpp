#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

int main()
{
    SYSTEM_INFO info;
    HANDLE handleMappingFile;

    handleMappingFile = OpenFileMappingA(FILE_MAP_ALL_ACCESS, false, "Local\\TEST");
    if (handleMappingFile == INVALID_HANDLE_VALUE)
        printf("handleMappingFile ERROR !! %x \n", GetLastError());
    else
        cout << "file created" << endl;
    GetSystemInfo(&info);
    char* a = (char*)MapViewOfFile(handleMappingFile, FILE_MAP_ALL_ACCESS, 0, 0, 512);
    cout << "\n" << "Adress memory: 0x" << hex << (unsigned long)a << dec << endl;

    cout << "Information of memory: ";
    for (int i = 0; i < 50; i++)
        cout << a[i];
    cout << "\n\n";

    system("pause");
    UnmapViewOfFile(a);
}

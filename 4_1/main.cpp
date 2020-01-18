
#include <windows.h>
#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

void show(char* a) {
    cout << "|";
    for (int i = 0; i < 10; i++)
        cout << (char)a[i];

    cout << "|\n ";
    for (int i = 0; i < 10; i++)
        cout << i;
    cout << endl;
}


void showGetSystemInf() {
    int n;
    char dd[4];

    SYSTEM_INFO info;

    cout << "GetSystemInfo()" << endl;

    GetSystemInfo(&info);



    cout << "Page size = " << (unsigned long)info.dwPageSize << endl;
    cout << "Minimum Application Address = " << hex << (unsigned long*)info.lpMinimumApplicationAddress << endl;
    cout << "Maximum Application Address = " << hex << info.lpMaximumApplicationAddress << endl;

    cout << "Allocation Granularity = " << hex << info.dwAllocationGranularity << dec << endl;
    cout << "Number Of Processors = " << (unsigned long)info.dwNumberOfProcessors << endl;

    /*cout << "wProcessorLevel = " << info.wProcessorLevel << endl;
    cout << "dwActiveProcessorMask = " << (unsigned long) info.dwActiveProcessorMask << endl;
    cout << "wProcessorArchitecture = " << (unsigned long) info.wProcessorArchitecture << endl;
    */


}

void showGlobalMemoryStatus() {
    int n;
    char dd[4];
    MEMORYSTATUSEX mem;
    int x = 1 << 25;
    cout << "GlobalMemoryStatus()" << endl;
    mem.dwLength = sizeof(mem);
    GlobalMemoryStatusEx(&mem);
    //printf("ERROR %d \n", GetLastError());
    cout << "Page size = " << (unsigned long)mem.dwMemoryLoad << endl;
    cout << "Total Memory = " << (unsigned long)mem.ullTotalPhys / 1024 / 1024 << " MB" << endl;
    cout << "Available Memory = " << (unsigned long)mem.ullAvailPhys / 1024 / 1024 << " MB" << endl;
    cout << "Total Virtual Memory = " << (unsigned long)mem.ullTotalVirtual / 1024 / 1024 << " MB" << endl;
    cout << "Available Virtual Memory = " << (unsigned long)mem.ullAvailVirtual / 1024 / 1024 << " MB" << endl;

}


void showVirtualQuery(void* adress) {
    int n;
    char dd[4];
    MEMORY_BASIC_INFORMATION mem;
    void* adressMemory = adress;
    cout << "VirtualQuery()" << endl;

    cout << "Input Adress = 0x" << hex << adressMemory << endl;
    VirtualQuery(adressMemory, &mem, sizeof(MEMORY_BASIC_INFORMATION));


    cout << "BaseAddress or near adress  = " << "0x" << setw(10) << setfill('0') << fixed << (unsigned long long)mem.BaseAddress << endl;
    cout << dec << "Region Size = " << (unsigned long)mem.RegionSize << endl;
    cout << "State Memory = ";
    switch (mem.State) {
        case 0x1000:
            cout << "MEM_COMMIT" << endl;
            break;
        case 0x10000:
            cout << "MEM_FREE" << endl;
            break;
        case 0x2000:
            cout << "MEM_RESERVE" << endl;
            break;

    }
    cout << "Protect:" << hex << (unsigned long)mem.Protect << dec << endl;
    switch (mem.Protect) {
        case PAGE_READONLY:
            cout << "PAGE_READONLY" << endl;
            break;
        case PAGE_READWRITE:
            cout << "PAGE_READWRITE" << endl;
            break;
        case PAGE_NOACCESS:
            cout << "PAGE_NOACCESS" << endl;
            break;
        case PAGE_WRITECOPY:
            cout << "PAGE_WRITECOPY" << endl;
            break;
        case PAGE_EXECUTE:
            cout << "PAGE_EXECUTE" << endl;
            break;
        case PAGE_EXECUTE_READ:
            cout << "PAGE_EXECUTE_READ" << endl;
            break;
        case PAGE_EXECUTE_READWRITE:
            cout << "PAGE_EXECUTE_READWRITE" << endl;
            break;
        case PAGE_EXECUTE_WRITECOPY:
            cout << "PAGE_EXECUTE_WRITECOPY" << endl;
            break;
        case PAGE_GUARD:
            cout << "PAGE_GUARD" << endl;
            break;
        case PAGE_NOCACHE:
            cout << "PAGE_NOCACHE" << endl;
            break;

    }
    cout << endl;
    //cout << "Type = " << (unsigned long)mem.Type << endl<<endl;

    //printf("ERROR %d \n\n", GetLastError());

}


void showTwoVirtualAlloc(void* adress) {
    int n;
    char dd[4];
    int* a = NULL;
    MEMORYSTATUS mem;
    MEMORY_BASIC_INFORMATION  memo;
    int x = 1 << 25;
    cout << "Different VirtualAlloc()" << endl;
    cout << "Virtual MEM_RESERVE" << endl;

    a = (int*)VirtualAlloc(adress, 10 * sizeof(int), MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (a == NULL) {
        cout << "Error virtual alloc";
        return;
    }
    cout << "Adress Memory RESERVE =" << hex << a << dec << endl << endl;


    showVirtualQuery((void*)a);
    cout << "Virtual MEM_COMMIT" << endl;

    a = (int*)VirtualAlloc(a, 10 * sizeof(int), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    cout << "Adress Memory COMMIT =" << hex << a << dec << endl << endl;

    showVirtualQuery(a);
    //printf("ERROR %d \n", GetLastError());

    //cout << "VirtualFree()" << endl;
    //VirtualFree(a, 0, MEM_RELEASE);
    //showVirtualQuery(a);
    //cout << " Memory cleared" << endl;
}

void showOneVirtualAlloc(void* adress) {
    int n;
    char dd[4];
    int* a = NULL;
    MEMORYSTATUS mem;
    int x = 1 << 25;
    cout << "One VirtualAlloc()" << endl;

    a = (int*)VirtualAlloc(adress, 20 * sizeof(int), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (a == NULL) {
        cout << "Error virtual alloc";
        return;
    }
    showVirtualQuery(a);

    //printf("ERROR %d \n", GetLastError());

    /*cout << "VirtualFree()" << endl<<endl;
    VirtualFree(a, 0, MEM_RELEASE);

    cout << " Memory cleared" << endl;
    cout << endl;
    showVirtualQuery(a);*/
}



void showVirtualProtect(void *adress) {
    int n;
    unsigned long fdwNewProtect, fdwOldProtect;
    string s;
    MEMORYSTATUS mem;
    int x = 1 << 25;
    void* a = adress;
    boolean f;
    //char* a = (char*)VirtualAlloc(a, 20 * sizeof(int), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (a == NULL) {
        cout << "Error virtual alloc";
    }
    else {

        cout << "VirtualProtect()" << endl;
        cout << "Memory commited" << endl;
        showVirtualQuery(a);
        cout << "Press new protect: ";
        cin >> fdwNewProtect;
        cout << endl;
        printf("\nNew Protect = %x\n\n", fdwNewProtect);
        f = VirtualProtect(a, 20 * sizeof(int), (DWORD)fdwNewProtect, &fdwOldProtect);
        cout << "Operation is " << (f?"Success" : "Wrong");
        if (f) {
            //printf("ERROR %d \n", GetLastError());
            printf("\nOld Protect = %x\n\n", fdwOldProtect);


            showVirtualQuery(a);
        }
        /*
        VirtualFree(a, 0, MEM_RELEASE);


        //printf("ERROR %d \n", GetLastError());

        cout << " Memory cleared" << endl;*/
    }



}






int main()
{
    int x = -1;
    int z = -2;
    void* adr;
    char y = 'a';
    string s;
    while (x != 0) {
        cout << "1 showGetSystemInf" << endl;
        cout << "2 showGlobalMemoryStatus " << endl;
        cout << "3 showVirtualQuery" << endl;
        cout << "4 showTwoVirtualAlloc" << endl;
        cout << "5 showOneVirtualAlloc" << endl;
        cout << "6 showVirtualProtect" << endl;
        cout << "7 showWtiteMemory" << endl;
        cout << "8 VirtualFree" << endl;
        cout << "0 Exit" << endl;
        cout << "Press number: ";
        cin >> x;

        cout << endl;
        switch (x) {
            case 1:
                showGetSystemInf();

                break;
            case 2:
                showGlobalMemoryStatus();
                break;

            case 3:
                cout << "Press adress: ";
                cin >> adr;
                cout << "\n\n";
                showVirtualQuery(adr);
                break;
            case 4:
                cout << "Press adress: ";
                cin >> adr;
                cout << "\n\n";
                showTwoVirtualAlloc(adr);
                break;
            case 5:
                cout << "Press adress: ";
                cin >> adr;
                cout << "\n\n";
                showOneVirtualAlloc(adr);
                break;
            case 6:
                cout << "Press adress: ";
                cin >> adr;
                cout << "\n\n";
                showVirtualProtect(adr);
                break;

            case 7: {
                cout << "Press adress: ";
                cin >> adr;
                cout << "\n\n";
//			char* a = (char*)VirtualAlloc(adr, 20 * sizeof(char), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
                char* a = (char*)adr;
                cout << "Press string: ";
                cin >> s;
                cout << "\n" << "Adress memory: 0x" << hex << (unsigned long)a << dec << endl;

                try{
                    CopyMemory(a, s.c_str(), sizeof(char) * s.length());

                cout << "Information of memory: ";
                for (int i = 0; i < 20; i++)
                    cout << a[i] ;
                cout << "\n\n";

                //VirtualFree(a, 0, MEM_RELEASE);
                break;
                }
                catch(int e){
                    cout << "no success";
                }
            }
            case 8:
                cout << "Press adress: ";
                cin >> adr;
                if(VirtualFree(adr, 0, MEM_RELEASE)){
                    cout << "Success\n";
                }
        }

        cout << endl << endl;

    }
}
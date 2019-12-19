#include <windows.h>
#include <iostream>
#include <string>


using namespace std;

void showLogicalDrives() {
    int n;
    char dd[4];
    DWORD dr = GetLogicalDrives();
    int x = 1 << 25;
    cout << "GetLogicalDrives()" << endl;
    for (int i = 25; i >= 0; i--)
        cout << char(65 + i);

    cout << endl<<endl;
    for (int i = 0; i < 26; i++, x >>= 1) {
        cout << ((x & dr )?1:0);
    }

    cout << endl<< endl;
    for (int i = 0; i < 26; i++)
    {
        n = ((dr >> i) & 0x00000001);
        if (n == 1)
        {
            dd[0] = char(65 + i); dd[1] = ':'; dd[2] = '\\'; dd[3] = 0;
            cout << "Available disk drives : " << dd << endl;
        }
    }


    cout << "GetLogicalDriveStrings()" << endl;
    DWORD length = GetLogicalDriveStrings(0, NULL);
    TCHAR a[512];

    GetLogicalDriveStrings(length, a);
    for (int i = 0; i < length; i++) {
        wprintf(L"%c", a[i]);
    }
}



//-------------------------------------//-------------------------------------//-------------------------------------//-------------------------------------

void showDriveType() {
    int d;
    cout << "Press name drive: ";
    string str;
    cin >> str;
    str = str + ":\\";
    cout << endl;
    d = GetDriveTypeA(str.c_str());

    cout << "GetDriveType()" << endl;
    cout << str << " drive type: ";
    printf("%d", d);
    if (d == DRIVE_UNKNOWN) cout << " DRIVE_UNKNOWN" << endl;
    if (d == DRIVE_NO_ROOT_DIR) cout << " DRIVE NO ROOT DIR" << endl;
    if (d == DRIVE_REMOVABLE) cout << " DRIVE_REMOVABLE" << endl;
    if (d == DRIVE_FIXED) cout << +" DRIVE_FIXED" << endl;
    if (d == DRIVE_REMOTE) cout << " DRIVE_REMOTE" << endl;
    if (d == DRIVE_CDROM) cout << " DRIVE_CDROM" << endl;
    if (d == DRIVE_RAMDISK) cout << " DRIVE_RAMDISK" << endl;
}

void showVolumeInformation() {
    char VolumeNameBuffer[100];
    char FileSystemNameBuffer[100];
    unsigned long VolumeSerialNumber;
    DWORD FileSystemFlags;
    cout << "Press name drive: ";
    string str;
    cin >> str;
    str = str + ":\\";
    cout << endl;
    cout << "GetVolumeInformation()" << endl;
    cout << str << " Volume information: ";
    BOOL GetVolumeInformationFlag = GetVolumeInformationA(
            "C:\\",
            VolumeNameBuffer,
            100,
            &VolumeSerialNumber,
            NULL, //&MaximumComponentLength,
            &FileSystemFlags, //&FileSystemFlags,
            FileSystemNameBuffer,
            100
    );

    if (GetVolumeInformationFlag != 0)
    {
        cout << "Volume Name is " << VolumeNameBuffer << endl;
        cout << "Volume Serial Number is " << VolumeSerialNumber << endl;
        cout << "File System is " << FileSystemNameBuffer << endl;

        cout << "System Flags:";
        if (FileSystemFlags & FILE_CASE_PRESERVED_NAMES) cout << "FILE_CASE_PRESERVED_NAMES" << endl;
        if (FileSystemFlags & FILE_CASE_SENSITIVE_SEARCH) cout << "FILE_CASE_SENSITIVE_SEARCH" << endl;
        if (FileSystemFlags & 0x20000000) cout << "FILE_DAX_VOLUME" << endl;
        if (FileSystemFlags & FILE_FILE_COMPRESSION) cout << +"FILE_FILE_COMPRESSION" << endl;
        if (FileSystemFlags & FILE_NAMED_STREAMS) cout << "FILE_NAMED_STREAMS" << endl;
        if (FileSystemFlags & FILE_PERSISTENT_ACLS) cout << "FILE_PERSISTENT_ACLS" << endl;
        if (FileSystemFlags & FILE_READ_ONLY_VOLUME) cout << "FILE_READ_ONLY_VOLUME" << endl;
        if (FileSystemFlags & FILE_SEQUENTIAL_WRITE_ONCE) cout << "FILE_SEQUENTIAL_WRITE_ONCE" << endl;
        if (FileSystemFlags & FILE_SUPPORTS_ENCRYPTION) cout << "FILE_SUPPORTS_ENCRYPTION" << endl;
        if (FileSystemFlags & FILE_SUPPORTS_HARD_LINKS) cout << "FILE_SUPPORTS_HARD_LINKS" << endl;
        if (FileSystemFlags & FILE_SUPPORTS_EXTENDED_ATTRIBUTES) cout << "FILE_SUPPORTS_EXTENDED_ATTRIBUTES" << endl;
        if (FileSystemFlags & FILE_SUPPORTS_OBJECT_IDS) cout << "FILE_SUPPORTS_OBJECT_IDS" << endl;
        if (FileSystemFlags & FILE_SUPPORTS_OPEN_BY_FILE_ID) cout << "FILE_SUPPORTS_OPEN_BY_FILE_ID" << endl;
        if (FileSystemFlags & FILE_SUPPORTS_REPARSE_POINTS) cout << "FILE_SUPPORTS_REPARSE_POINTS" << endl;
        if (FileSystemFlags & FILE_SUPPORTS_SPARSE_FILES) cout << "FILE_SUPPORTS_SPARSE_FILES" << endl;
        if (FileSystemFlags & FILE_SUPPORTS_TRANSACTIONS) cout << "FILE_SUPPORTS_TRANSACTIONS" << endl;
        if (FileSystemFlags & FILE_SUPPORTS_USN_JOURNAL) cout << "FILE_SUPPORTS_USN_JOURNAL" << endl;
        if (FileSystemFlags & FILE_UNICODE_ON_DISK) cout << "FILE_UNICODE_ON_DISK" << endl;
        if (FileSystemFlags & FILE_VOLUME_IS_COMPRESSED) cout << "FILE_VOLUME_IS_COMPRESSED" << endl;
        if (FileSystemFlags & FILE_VOLUME_QUOTAS) cout << "FILE_VOLUME_QUOTAS" << endl;
    }
    else cout << "Not Present (GetVolumeInformation)" << endl;
}

void showDiskFreeSpace() {
    /*lpSectorsPerCluster,
  lpBytesPerSector,
   lpNumberOfFreeClusters,
   lpTotalNumberOfClusters*/
    DWORD lpSectorsPerCluster,
            lpBytesPerSector,
            lpNumberOfFreeClusters,
            lpTotalNumberOfClusters;

    cout << "Press name drive: ";
    string str;
    cin >> str;
    cout << endl;
    cout << "GetDiskFreeSpace()"<<endl;
    BOOL GetDiskFreeSpaceFlag = GetDiskFreeSpaceA(
            (str + ":\\").c_str(),					  // directory name
            &lpSectorsPerCluster,
            &lpBytesPerSector,
            &lpNumberOfFreeClusters,
            &lpTotalNumberOfClusters
    );

    if (GetDiskFreeSpaceFlag != 0)
    {
        cout << "Bytes in sector = " << (unsigned long)lpBytesPerSector << endl;
        cout << "Sectors per cluster = " << (unsigned long)lpSectorsPerCluster << endl;
        cout << "Bytes in cluster = " << (unsigned long)lpSectorsPerCluster * lpBytesPerSector << endl;
        double lpn = double((unsigned long)(lpNumberOfFreeClusters)/1024/1024);
        cout << "Total Number Of Free GB / Total Number Of GB = " << lpn * lpBytesPerSector * lpSectorsPerCluster / 1024 << " GB" << " / "
             << (double) (lpTotalNumberOfClusters /1024 / 1024)*lpBytesPerSector * lpSectorsPerCluster / 1024
             << " GB" << endl;
    }
    else	cout << "	Not Present (GetDiskFreeSpace)" << endl;
}

void menuDrive() {
    int x;

    cout << "1 GetDriveType" << endl;
    cout << "2 GetVolumeInformation" << endl;
    cout << "3 GetDiskFreeSpace" << endl;
    cin >> x;
    switch (x) {
        case 1:
            showDriveType();
            break;
        case 2:
            showVolumeInformation();
            break;
        case 3:
            showDiskFreeSpace();
            break;
        default:
            cout << "NOTHING!!!";
    }
}



//-------------------------------------//-------------------------------------//-------------------------------------//-------------------------------------

void menuDirectory() {
    int x;


    string str;
    cout << "1 CreateDirectory" << endl;
    cout << "2 RemoveDirectory" << endl;
    cin >> x;
    switch (x) {
        case 1:
            cout << "Press name new directory: ";
            cin >> str;
            cout << endl;
            cout << "CreateDirectory()"<< endl;
            if (CreateDirectoryA(str.c_str(), NULL))
                cout << "directory created" << endl;
            else
                cout << "error create directory" << endl;
            break;
        case 2:
            cout << "Press name directory: ";
            cin >> str;
            cout << endl;
            cout << "RemoveDirectory()" << endl;;
            if (RemoveDirectoryA(str.c_str()))
                cout << "directory removed" << endl;
            else
                cout << "error remove directory" << endl;
            break;

        default:
            cout << "NOTHING!!!";
    }
}

//-------------------------------------//-------------------------------------//-------------------------------------//-------------------------------------




/*
CreateFile(
	LPCTSTR  lpFileName,                         // имя файла
	DWORD  dwDesiredAccess,                      // режим доступа
	DWORD  dwShareMode,                          // совместный доступ
	LPSECURITY_ATTRIBUTES  lpSecurityAttributes, // SD (дескр. защиты)
	DWORD  dwCreationDisposition,                // как действовать
	DWORD  dwFlagsAndAttributes,                 // атрибуты файла
	HANDLE  hTemplateFile                        // дескр.шаблона файла
);*/
void menuFile() {
    int x;
    string str, toStr;
    wstring q;
    HANDLE handle;
    cout << "1 CreateFile " << endl;
    cout << "2 CopyFile" << endl;
    cout << "3 MoveFile" << endl;
    cout << "4 MoveFileEx" << endl;
    cin >> x;
    switch (x) {
        case 1:

            cout << "Press name file: ";
            cin >> str;
            cout << endl;
            cout << "CreateFile()" << endl;
            handle = CreateFileA(str.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
            if (handle == INVALID_HANDLE_VALUE) {
                printf("ERROR !!%x \n", GetLastError());

            }
            else
                cout << "file created" << endl;
            CloseHandle(handle);
            break;
        case 2:
            cout << "Press name copies file: ";
            cin >> str;
            cout << "\nPress name new file: ";
            cin >> toStr;
            cout << endl;
            cout << "CopyFile()" << endl;
            if (CopyFileA(str.c_str(), toStr.c_str(), FALSE))
                cout << "file copied" << endl;
            else
                cout << "error file copy" << endl;
            break;
        case 3:
            cout << "Press name moving file: ";
            cin >> str;
            cout << "\nPress name new file: ";
            cin >> toStr;
            cout << endl;
            cout << "MoveFile()" << endl;
            if (MoveFileA(str.c_str(), toStr.c_str()))
                cout << "file moved" << endl;
            else
                cout << "error file move" << endl;
            break;
        case 4:
            cout << "Press name moving file: ";
            cin >> str;
            cout << "\nPress name new file: ";
            cin >> toStr;
            cout << endl;
            cout << "MoveFileEx()" << endl;
            if (MoveFileExA(str.c_str(), toStr.c_str(), MOVEFILE_REPLACE_EXISTING))
                cout << "file movedEx" << endl;
            else
                cout << "error file moveEx" << endl;
            break;
        default:
            cout << "NOTHING!!!";
    }
}


//-------------------------------------//-------------------------------------//-------------------------------------//-------------------------------------

void menuAtribute() {
    HANDLE handle;
    int x;
    DWORD atribute;
    string str;
    cout << "1 GetFileAttributes " << endl;
    cout << "2 GetFileInformationByHandle" << endl;
    cout << "3 GetFileTime" << endl;
    cout << "4 SetFileAttributes" << endl;
    cout << "5 SetFileTime" << endl;
    cin >> x;
    switch (x) {
        case 1:
            cout << "Press name File: ";
            cin >> str;
            cout << endl;
            cout << "GetFileAttributes()" << endl;

            atribute = (GetFileAttributesA(str.c_str()));

            if (atribute != INVALID_FILE_ATTRIBUTES) {

                cout << "File Atribute: " << endl;
                if (atribute & FILE_ATTRIBUTE_ARCHIVE) cout << "FILE_ATTRIBUTE_ARCHIVE" << endl;
                if (atribute & FILE_ATTRIBUTE_COMPRESSED) cout << "FILE_ATTRIBUTE_COMPRESSED" << endl;
                if (atribute & FILE_ATTRIBUTE_DEVICE) cout << "FILE_ATTRIBUTE_DEVICE" << endl;
                if (atribute & FILE_ATTRIBUTE_DIRECTORY) cout << +"FILE_ATTRIBUTE_DIRECTORY" << endl;
                if (atribute & FILE_ATTRIBUTE_ENCRYPTED) cout << "FILE_ATTRIBUTE_ENCRYPTED" << endl;
                if (atribute & FILE_ATTRIBUTE_HIDDEN) cout << "FILE_ATTRIBUTE_HIDDEN" << endl;
                if (atribute & 0x8000) cout << "FILE_ATTRIBUTE_INTEGRITY_STREAM" << endl;
                if (atribute & FILE_ATTRIBUTE_NORMAL) cout << "FILE_ATTRIBUTE_NORMAL" << endl;
                if (atribute & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED) cout << "FILE_ATTRIBUTE_NOT_CONTENT_INDEXED" << endl;
                if (atribute & 0x20000) cout << "FILE_ATTRIBUTE_NO_SCRUB_DATA" << endl;
                if (atribute & FILE_ATTRIBUTE_OFFLINE) cout << "FILE_ATTRIBUTE_OFFLINE" << endl;
                if (atribute & FILE_ATTRIBUTE_READONLY) cout << "FILE_ATTRIBUTE_READONLY" << endl;
                if (atribute & 0x400000) cout << "FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS" << endl;
                if (atribute & 0x40000) cout << "FILE_ATTRIBUTE_RECALL_ON_OPEN" << endl;
                if (atribute & FILE_ATTRIBUTE_REPARSE_POINT) cout << "FILE_ATTRIBUTE_REPARSE_POINT" << endl;
                if (atribute & FILE_ATTRIBUTE_SPARSE_FILE) cout << "FILE_ATTRIBUTE_SPARSE_FILE" << endl;
                if (atribute & FILE_ATTRIBUTE_SYSTEM) cout << "FILE_ATTRIBUTE_SYSTEM" << endl;
                if (atribute & FILE_ATTRIBUTE_TEMPORARY) cout << "FILE_ATTRIBUTE_TEMPORARY" << endl;
                if (atribute & FILE_ATTRIBUTE_VIRTUAL) cout << "FILE_ATTRIBUTE_VIRTUAL" << endl;
            }
            else {
                cout << "Wrong path or file for a network share" << endl;
            }
            break;
        case 2:
        {
            cout << "Press name File: ";
            cin >> str;
            cout << endl;
            cout << "GetFileInformationByHandle()" << endl;
            handle = CreateFileA(str.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            if (handle != INVALID_HANDLE_VALUE) {
                BY_HANDLE_FILE_INFORMATION fileInfo;
                boolean f = GetFileInformationByHandle(handle, &fileInfo);
                if (f) {

                    SYSTEMTIME stime;
                    cout << "Volume serial number: " << (unsigned long)fileInfo.dwVolumeSerialNumber << endl;
                    cout << "Links number: " << (unsigned long)fileInfo.nNumberOfLinks << endl;
                    FileTimeToSystemTime(&(fileInfo.ftCreationTime), &stime);
                    printf("Create time: %d.%d.%d %d:%d\n", stime.wDay, stime.wMonth, stime.wYear, stime.wHour, stime.wMinute);
                    FileTimeToSystemTime(&(fileInfo.ftLastAccessTime), &stime);
                    printf("Access time: %d.%d.%d %d:%d\n", stime.wDay, stime.wMonth, stime.wYear, stime.wHour, stime.wMinute);
                    FileTimeToSystemTime(&(fileInfo.ftLastWriteTime), &stime);
                    printf("Write time: %d.%d.%d %d:%d\n", stime.wDay, stime.wMonth, stime.wYear, stime.wHour, stime.wMinute);

                }
                else
                    cout << "error GetFileInformationByHandle" << endl;
            }
            else
                cout << "INVALID_HANDLE_VALUE";


            CloseHandle(handle); }
            break;

        case 3: {
            cout << "Press name File: ";
            cin >> str;
            cout << endl;
            cout << "GetFileTime()" << endl;
            handle = CreateFileA(str.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            if (handle != INVALID_HANDLE_VALUE) {
                SYSTEMTIME stime;
                FILETIME crtime, actime, wrtime;
                system("pause");
                if (GetFileTime(handle, &crtime, &actime, &wrtime)) {
                    FileTimeToSystemTime(&crtime, &stime);
                    printf("Create time: %d.%d.%d %d:%d\n", stime.wDay, stime.wMonth, stime.wYear, stime.wHour, stime.wMinute);
                    FileTimeToSystemTime(&actime, &stime);
                    printf("Access time: %d.%d.%d %d:%d\n", stime.wDay, stime.wMonth, stime.wYear, stime.wHour, stime.wMinute);
                    FileTimeToSystemTime(&wrtime, &stime);
                    printf("Write time: %d.%d.%d %d:%d\n", stime.wDay, stime.wMonth, stime.wYear, stime.wHour, stime.wMinute);
                    CloseHandle(handle);
                }
                else {
                    cout << "Error GetFileTime";
                }
            }
            else
                cout << "INVALID_HANDLE_VALUE";
        }
            break;
        case 4:
            cout << "Press name File: ";
            cin >> str;
            cout << "\n4 SetFileAttributes()" << endl;
            int newatribute;
            cout << "Press new atr: ";
            cin >>hex>> newatribute;
            cin >> dec;
            cout << endl;
            if (SetFileAttributesA(str.c_str(), newatribute))
                cout << "Success";
            else
                cout << "Error SetFileAttribute";
            break;
        case 5: {
            cout << "Press name File: ";
            cin >> str;
            cout << endl;
            cout << "SetFileTime()" << endl;
            handle = CreateFileA(str.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

            if (handle != INVALID_HANDLE_VALUE) {
                SYSTEMTIME stime;
                FILETIME  wrtime;
                GetSystemTime(&stime);
                SystemTimeToFileTime(&stime, &wrtime);
                if (SetFileTime(handle, (LPFILETIME)NULL, (LPFILETIME)NULL, &wrtime))
                    cout << "Success";
                else
                    cout << "Error SetFileTime";
                CloseHandle(handle);
            }
            else
                cout << "INVALID_HANDLE_VALUE";
            break;
        }
        default:
            printf("ERROR %x \n", GetLastError());
            cout << "NOTHING!!!";
    }
}


//-------------------------------------//-------------------------------------//-------------------------------------//-------------------------------------

int main()
{

    int x = -1;

    while (x != 0) {
        cout << "1 showLogicalDrives " << endl;
        cout << "2 menuDrive" << endl;
        cout << "3 menuDirectory" << endl;
        cout << "4 menuFile" << endl;
        cout << "5 menuAtribute" << endl;
        cout << "0 Exit" << endl;
        cout << "Press number: ";
        cin >> x;

        cout << endl;
        switch (x) {
            case 1:
                showLogicalDrives();
                break;
            case 2:
                menuDrive();
                break;
            case 3:
                menuDirectory();
                break;
            case 4:
                menuFile();
                break;
            case 5:
                menuAtribute();
                break;
        }
        cout << endl << endl;
    }
}
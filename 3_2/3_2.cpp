
#include <windows.h>
#include <winbase.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>


using namespace std;

long long write_time = 0;
char** info;
DWORD global_i = 0;
int mul;
DWORD SizeCluster;
int operCount;

VOID WINAPI FileWrittenCallback(DWORD dwErrorCode, DWORD dwBytesTransferred, LPOVERLAPPED lpOverlapped) {
    if (dwErrorCode != 0) {
        cout << "Wrong write to file! Error:" << dwErrorCode << ", AddrOverlapped: " << lpOverlapped << endl;
    }
    else {
        //cout << "Good Writing: Transferred: " << dwBytesTransferred<<" , AddrOverlapped: " <<lpOverlapped<< endl;
    }
    //cout << "Good Writing"<<endl;*/
    write_time++;
}

VOID WINAPI CopyFinal(DWORD dwErrorCode, DWORD dwBytesTransferred, LPOVERLAPPED lpOverlapped) {
    if (dwErrorCode != 0) {
        cout << "CompletionRoutine: Unable to write to file! Error:" << dwErrorCode << ", AddrOverlapped: " << lpOverlapped << endl;
    }
    else {

        OVERLAPPED overlapped;
        overlapped.Offset = lpOverlapped->Offset;
        overlapped.OffsetHigh = lpOverlapped->OffsetHigh;
        DWORD t = WriteFileEx(lpOverlapped->hEvent, info[global_i], SizeCluster, &overlapped,
                              (LPOVERLAPPED_COMPLETION_ROUTINE)FileWrittenCallback);

        global_i++;

    }
}


//-------------------------------------//-------------------------------------//-------------------------------------//-------------------------------------
int BlockCopying() {

    HANDLE handleRead = CreateFileA("testfile.txt", GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING,	FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED , nullptr);
    if (handleRead == INVALID_HANDLE_VALUE) {
        cout << "Invalid read file" << endl;
        return -1;
    }
    HANDLE handleWrite = CreateFileA("out.txt", GENERIC_WRITE, FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED , nullptr);

    if (handleWrite == INVALID_HANDLE_VALUE) {
        cout << "Invalid write file" << endl;
        return -1;
    }

    LARGE_INTEGER cur_pos;
    DWORD tOperCount = operCount - 1;
    int sync_times = 0;
    DWORD src_size = GetFileSize(handleRead, NULL);
    const DWORD ovrCount = src_size % SizeCluster == 0 ? src_size / SizeCluster : src_size / SizeCluster + 1;
    vector<OVERLAPPED> ovrVector(ovrCount);

    for (DWORD i = 0; i <ovrCount; i++) {
        ovrVector.at(i).hEvent = handleWrite;
    }

    cout << "Src size: " << src_size << " Overlapped_count: " << ovrCount << endl;
    info = new char* [ovrCount];

    for (size_t i = 0; i < ovrCount; i++) {
        info[i] = new char[SizeCluster];

    }

    auto start = chrono::high_resolution_clock::now();

    for (size_t i = 0; i < ovrCount; i++) {
        cur_pos.QuadPart = i * SizeCluster;
        ovrVector.at(i).Offset = cur_pos.LowPart;
        ovrVector.at(i).OffsetHigh = cur_pos.HighPart;
        DWORD t = ReadFileEx(handleRead, info[i], SizeCluster, &ovrVector.at(i), (LPOVERLAPPED_COMPLETION_ROUTINE)CopyFinal);


        if (i == tOperCount) {
            sync_times++;
            tOperCount += operCount;

            SleepEx(INFINITE, TRUE);
        }
    }
    if (sync_times * operCount < ovrCount && operCount != 0) {
        sync_times++;
        SleepEx(INFINITE, TRUE);
    }

    while (ovrCount != write_time) SleepEx(100, TRUE);
    auto end = chrono::high_resolution_clock::now();

    auto time = chrono::duration_cast<std::chrono::duration<double>>(end - start).count();
    LARGE_INTEGER src_size_quad;
    src_size_quad.QuadPart = src_size;
    BOOL t = SetFilePointer(handleWrite, src_size_quad.LowPart, &src_size_quad.HighPart, 0);
    if (!t) {
        cout << "SetFilePointer ERROR" << endl;
    }
    t = SetEndOfFile(handleWrite);
    if (!t) {
        cout << "SetEndOfFile ERROR" << endl;
    }

    cout << "read file size (kb): " << GetFileSize(handleRead, NULL) / 1024 << endl;
    cout << "write file size (kb): " << GetFileSize(handleWrite, NULL) / 1024 << endl;
    cout << "Time (in seconds): " << time << endl;


    CloseHandle(handleRead);
    CloseHandle(handleWrite);

    for (int i = 0; i < ovrCount; i++)
        delete [] info[i];
    delete []info;

}


int main()
{
    cout << "Press multiplier claster: ";
    cin >> mul;

    SizeCluster = 4096 * mul;
    //cout << "\nPress Number operations: ";
    //cin >> operCount;
    operCount = 8;
    cout << endl;

    BlockCopying();
    ifstream fin1("testfile.txt"), fin2("out.txt");
    bool f = true;
    char x, y;
    int i = 0;
    while (f && !fin1.eof() && !fin2.eof()) {

        fin1 >> x;
        fin2 >> y;
        if (x == y)
            i++;
        else
            f = false;

    }
    if (f)
        if (fin1.eof() && fin2.eof())
            cout << "File equalante";
        else
            cout << "Diffrent size";
    else
        cout << i << " " << x << " " << y;

    system("pause");
}


#include<stdio.h>
#include<sysinfoapi.h>
#include<stdlib.h>
#include<time.h>

using namespace std;


int random()
{
    DWORD currentTime = GetTickCount();
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);
    //当前时间-当前处于调用进程的虚拟地址空间的用户模式部分的未保留和未提交的内存量（以字节为单位）+自系统启动以来已用过的毫秒数*系统或当前进程的当前已提交内存限制（以字节为单位）
    srand((int)time(NULL)- statex.ullAvailVirtual+currentTime*statex.ullAvailPhys);
    return rand();
}

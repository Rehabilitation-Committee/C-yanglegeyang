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
    //��ǰʱ��-��ǰ���ڵ��ý��̵������ַ�ռ���û�ģʽ���ֵ�δ������δ�ύ���ڴ��������ֽ�Ϊ��λ��+��ϵͳ�����������ù��ĺ�����*ϵͳ��ǰ���̵ĵ�ǰ���ύ�ڴ����ƣ����ֽ�Ϊ��λ��
    srand((int)time(NULL)- statex.ullAvailVirtual+currentTime*statex.ullAvailPhys);
    return rand();
}

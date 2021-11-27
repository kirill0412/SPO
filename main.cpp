#define WINVER 0x0502
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <tchar.h>
#define SIZE 32767
#define BSIZE MAX_PATH
using namespace std;

// 2
void Freq_meas()
{
    double ans;
    LARGE_INTEGER fr,cr1,cr2;
    QueryPerformanceCounter(&cr1);
    QueryPerformanceFrequency(&fr);
    QueryPerformanceCounter(&cr2);
    printf ("\n2.1) Частота ЦП:     %lld  Гц\n", fr);
    ans = (1000000*(double(cr2.QuadPart) - double(cr1.QuadPart)) / double(fr.QuadPart));
    cout << "2.2) Измерено за:   " << ans  << "  мкс \n";
}

int main()
{
    setlocale(LC_ALL, "Russian");

    //1.1
    OSVERSIONINFO OS_ver;
    OS_ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&OS_ver);
    printf("1.1) Версия ОС: Windows %i.%i\n", OS_ver.dwMajorVersion, OS_ver.dwMinorVersion);

    //1.2
    TCHAR bf[SIZE];
    GetSystemDirectory(bf, SIZE);
    printf("\n1.2) Системная директория: %s \n", bf);

    //1.3
    DWORD sz = sizeof(bf);
    GetComputerName(bf,&sz);
    printf("\n1.3) Имя компьютера: %s \n", bf);
    GetUserName(bf,&sz);
    printf("     Имя пользователя: %s \n", bf);

    //1.4
    char T_name[MAX_PATH],T_label[MAX_PATH];
    ULARGE_INTEGER T_free,T_volume;
    int T_count = 0;
    HANDLE T_find = FindFirstVolume(T_name, BSIZE);
    printf("\n1.4) Характеристики логических томов:");
    do
    {
        T_count++;
        cout << "\n     Имя " << T_count << "-го тома: ";
        printf(T_name);
        GetVolumePathNamesForVolumeName(T_name, T_label, sz, &sz);
        printf("\n     Метка тома: ");
        printf("%s",T_label);
        if (GetDiskFreeSpaceEx(T_name, &T_free, &T_volume, NULL)!= 0)
        {
            cout << "\n     Общий объем: " << T_volume.QuadPart << "\n     Свободное пространство: " << T_free.QuadPart;
        }
        else
        {
            printf("\n     Нет информации об объёме.");
        }
    }
    while (FindNextVolume(T_find, T_name, BSIZE));
    FindVolumeClose(T_find);

    //1.5)
    HKEY as_find;
    TCHAR as_name[SIZE], as_path[SIZE];
    DWORD as_count = 0, as_val = sizeof(as_name);
    sz = SIZE;
    printf("\n\n1.5) Список программ, запускаемых при старте системы:\n");
    RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &as_find);
    while (RegEnumValue(as_find, as_count, as_name, &as_val, NULL, REG_NONE, NULL, NULL) == ERROR_SUCCESS)
    {
        as_val = sizeof(as_name);
        RegQueryValueEx(as_find, as_name, NULL, REG_NONE, (LPBYTE)as_path, &sz);
        printf("     %i) %s: %s\n", as_count + 1, as_name, as_path);
        as_count++;
    }
    RegCloseKey(as_find);

    //2)
    Freq_meas();
    return 0;
}

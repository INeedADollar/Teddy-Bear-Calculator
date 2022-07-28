#include "processthread.h"
#include <QApplication>

ProcessThread::ProcessThread(DWORD pId, QWidget *parent) : QThread(parent)
{
    pID = pId;
}

BOOL ProcessThread::IsProcessRunning(DWORD pid)
{
    HANDLE process = OpenProcess(SYNCHRONIZE, FALSE, pid);
    DWORD ret = WaitForSingleObject(process, INFINITE);
    CloseHandle(process);
    return ret == WAIT_OBJECT_0;
}

void ProcessThread::run(){
    if(IsProcessRunning(pID))
        QApplication::exit();

}

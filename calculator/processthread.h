#ifndef PROCESSTHREAD_H
#define PROCESSTHREAD_H

#include <QThread>
#include <windows.h>
#include <QWidget>

class ProcessThread : public QThread
{
    Q_OBJECT

public:
    ProcessThread(DWORD, QWidget *parent = nullptr);

protected:
    void run() override;

private:
    BOOL IsProcessRunning(DWORD);

    DWORD pID;
};

#endif // PROCESSTHREAD_H

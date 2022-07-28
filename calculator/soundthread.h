#ifndef SOUNDTHREAD_H
#define SOUNDTHREAD_H

#include <QThread>
#include <windows.h>

class SoundThread : public QThread
{
public:
    SoundThread();
    void setFileName(LPCTSTR);

protected:
    void run() override;

private:
    LPCTSTR fileName;
};

#endif // SOUNDTHREAD_H

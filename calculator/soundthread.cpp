#include "soundthread.h"

SoundThread::SoundThread()
{

}

void SoundThread::run(){
    PlaySound(fileName, nullptr, SND_NODEFAULT | SND_SYNC);
}

void SoundThread::setFileName(LPCTSTR file){
    fileName = file;
}

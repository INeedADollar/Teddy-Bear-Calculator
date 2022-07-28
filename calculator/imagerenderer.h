#ifndef IMAGERENDERER_H
#define IMAGERENDERER_H

#include <QImage>
#include "soundthread.h"

class ImageRenderer : public QObject
{
    Q_OBJECT
public:
    ImageRenderer();
    QImage renderImage(QImage);

private:
    void drawBubbles(QImage &);

    int w = 0;
    int h = 0;
    int tw = 0;
    int th = 0;
    int nr = 1;
    int  tempWidth = 0;
    int tempHeight = 0;
    int tempWid = 0;
    QSize size;
    QPoint tempPos;
    QPoint pos = QPoint(0, 0);
    SoundThread *thread;
};

#endif // IMAGERENDERER_H

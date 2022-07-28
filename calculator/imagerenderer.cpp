#include "imagerenderer.h"
#include <QPainter>
#include <QRandomGenerator>

ImageRenderer::ImageRenderer()
{
    thread = new SoundThread;
}

QImage ImageRenderer::renderImage(QImage image){
    while(h <= image.width() - 1 && w <= image.width() - 1){
        if(h != -1 && w != -1 && image.valid(w, h))
            image.setPixelColor(w, h, Qt::transparent);

        if(h == 0 && th != image.width() - 2){
            w = 0;
            h = th + 1;
        }
        else{
            if(w < image.width() - 1 ){
                h--;
                w++;
            }
            else{
                h = th;
                tw++;
                w = tw;
            }
        }

        if(th < h && th != image.width() - 1)
            th = h;
        else
            if(h == image.width() - 1){
                tw++;
            }
        nr++;
        if(nr % 2500 == 0) break;
    }

    drawBubbles(image);
    return image;
}

void ImageRenderer::drawBubbles(QImage &image){
    if(nr == 2500){
        thread->setFileName(TEXT("sounds\\bula-inceput.wav"));
        thread->start();
    }

    if(nr%15000 == 0){
        if(pos.x() < image.width() && pos.y() < image.height()) {

        QPainter painter(&image);
        int rand = QRandomGenerator::global()->bounded(10);

        int width = QRandomGenerator::global()->bounded(30, 100);
        size = QSize(width, width);

        switch(rand) {
            case 0:{
                QImage img = QImage(":/icons/bula-mare-albastra.png").scaled(width, width);
                painter.drawImage(QRect(pos, size), img, QRect(QPoint(), size));
                break;
            }
            case 1:{
                QImage img = QImage(":/icons/bula-mica-albastra.png").scaled(width, width);
                painter.drawImage(QRect(pos, size), img, QRect(QPoint(), size));
                break;
            }
            case 2:{
                QImage img = QImage(":/icons/bula-mare-galbena.png").scaled(width, width);
                painter.drawImage(QRect(pos, size), img, QRect(QPoint(), size));
                break;
            }
            case 3:{
                QImage img = QImage(":/icons/bula-mica-galbena.png").scaled(width, width);
                painter.drawImage(QRect(pos, size), img, QRect(QPoint(), size));
                break;
            }
            case 4:{
                QImage img = QImage(":/icons/bula-mare-mov.png").scaled(width, width);
                painter.drawImage(QRect(pos, size), img, QRect(QPoint(), size));
                break;
            }
            case 5:{
                QImage img = QImage(":/icons/bula-mica-mov.png").scaled(width, width);
                painter.drawImage(QRect(pos, size), img, QRect(QPoint(), size));
                break;
            }
            case 6:{
                QImage img = QImage(":/icons/bula-mare-rosie.png").scaled(width, width);
                painter.drawImage(QRect(pos, size), img, QRect(QPoint(), size));
                break;
            }
            case 7:{
                QImage img = QImage(":/icons/bula-mica-rosie.png").scaled(width, width);
                painter.drawImage(QRect(pos, size), img, QRect(QPoint(), size));
                break;
            }
            case 8:{
                QImage img = QImage(":/icons/bula-mare-verde.png").scaled(width, width);
                painter.drawImage(QRect(pos, size), img, QRect(QPoint(), size));
                break;
            }
            case 9:{
                QImage img = QImage(":/icons/bula-mica-verde.png").scaled(width, width);
                painter.drawImage(QRect(pos, size), img, QRect(QPoint(), size));
                break;
            }
        }


        if(pos.y() == 0 && pos.x() + 100 <= image.height() - 1){
            pos.setY(pos.x() + 100);
            pos.setX(0);
        }
        else{
            if(tempWidth > 0)
                if(pos.x() + 100 <= image.width() - 1){
                    pos.setY(pos.y() - 100);
                    pos.setX(pos.x() + 100);
                }
                else{
                    tempWidth += 100;
                    pos.setX(tempWidth);
                    pos.setY(400);
                }
            else{
                pos.setY(pos.y() - 100);
                pos.setX(pos.x() + 100);
            }
        }


        if(pos.y() == 400){
            tempWidth = pos.x();
            tempWid = pos.x();
        }

        if(nr % 10000 == 0){
            for(int i = tempPos.x(); i <=  tempPos.x() + 100; i++){
                for(int j = tempPos.y(); j <=  tempPos.y() + 100; j++)
                    image.setPixelColor(i, j, Qt::transparent);
            }

            thread->setFileName(TEXT("sounds\\bula-sparta.wav"));
            thread->start();

            if(tempPos.y() == 0 && tempPos.x() + 100 <= image.height()){
                tempPos.setY(tempPos.x() + 100);
                tempPos.setX(0);
            }
            else{
                    tempPos.setY(tempPos.y() - 100);
                    tempPos.setX(tempPos.x() + 100);
                }
            }
        }
   }
}



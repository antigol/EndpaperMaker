#include "endpaperthread.h"
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QStringList>

qreal random(qreal min, qreal max)
{
    qreal ret = (qreal)rand() / (qreal)RAND_MAX;

    return ret * (max - min) + min;
}

EndpaperThread::EndpaperThread(QObject *parent) :
    QThread(parent)
{
}

void EndpaperThread::run()
{
    QRectF pageRect = _printer->pageRect();
    QGraphicsScene scene(pageRect);
    pageRect.setTop(pageRect.top() - pageRect.height() * 0.2);
    pageRect.setLeft(pageRect.left() - pageRect.width() * 0.2);

    QStringList textList = _text.split(',');
    for (int i = 0; i < textList.size(); ++i) {
        textList[i] = textList[i].trimmed();
    }


    QGraphicsSimpleTextItem *item;

    QList<const QGraphicsItem *> itemList;
//    QList<QPainterPath> shapeList;

    int value = 0;
    int stop = 0;
    bool ok;


    while (stop < 400) {
        if (_stop)
            break;

        // création du texte (taille et font)
        qreal textSize = random(_textSize1, _textSize2);
        _font.setPointSizeF(textSize);

        item = scene.addSimpleText(textList.at(rand() % textList.size()), _font);
        item->setFlag(QGraphicsItem::ItemClipsToShape);
        item->setBrush(QBrush(_color));

        for (int i = 0; i < 200; ++i) {
            // positionnement du texte (position et rotation)
            item->setPos(random(pageRect.left(), pageRect.right()),
                       random(pageRect.top(), pageRect.bottom()));


            item->setRotation(random(_textAngle1, _textAngle2));

            // verification de collision

            // crée le shape de l'objet actuel
//            QPainterPath shape = item->shape();

            ok = true;

            // cherche une intersection
            for (int i = 0; i < itemList.size(); ++i) {


                if (item->collidesWithItem(itemList[i])) {

//                if (item->collidesWithPath(item->mapFromItem(itemList[i], shapeList[i]))) {
                    ok = false;
                    break;
                }
            }

            if (ok) {

                // s'il n'y a pas d'intersection, on ajoute le shape dans la liste et on quitte la boucle
                itemList.append(item);
//                shapeList.append(item->shape());

                emit progress(++value);

                stop = 0;

                break;
            }
        } // for (int i = 0; i < 100; ++i)

        if (!ok) {
            // si on a quitté la boucle acause de la fin de la boucle for, on supprime l'objet et on r'essaye avec une autre taille

            scene.removeItem(item);

            stop++;
        }

    } // while (stop < 300)


    _painter->begin(_printer);

    scene.render(_painter);

    _painter->end();
}

void EndpaperThread::start(QPrinter *printer, QPainter *painter, const QString &text, const QFont &font, const QColor &color, qreal textSize1, qreal textSize2, qreal textAngle1, qreal textAngle2)
{
    _printer = printer;
    _painter = painter;
    _text = text;
    _font = font;
    _color = color;
    _textSize1 = textSize1;
    _textSize2 = textSize2;
    _textAngle1 = textAngle1;
    _textAngle2 = textAngle2;

    _stop = false;
    QThread::start();
}

void EndpaperThread::stop()
{
    _stop = true;
}

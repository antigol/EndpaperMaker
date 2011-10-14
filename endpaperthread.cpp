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
    // zone de la page
    QRectF pageRect = _printer->pageRect();

    QGraphicsScene scene(pageRect);

    // dépassement de la page à gauche et en haut, pour laisser des mots dépasser
    pageRect.setTop(pageRect.top() - pageRect.height() * 0.2);
    pageRect.setLeft(pageRect.left() - pageRect.width() * 0.2);

    // découpage de le liste des mots par les virgules
    QStringList textList = _text.split(',');

    for (int i = 0; i < textList.size(); ++i) {
        textList[i] = textList[i].trimmed();
    }

    // pointeur de l'objet courrant
    QGraphicsSimpleTextItem *item;

    // liste des objet rajoutés
    QList<const QGraphicsItem *> itemList;

    int wordsAmount = 0;
    int tryCounter = 0;
    bool ok;


    while (tryCounter < 300) {
        if (_stop)
            break;

        // création du texte
        _font.setPointSizeF(random(_textSize1, _textSize2));

        item = scene.addSimpleText(textList.at(rand() % textList.size()), _font);

        //item->setFlag(QGraphicsItem::ItemClipsToShape);
        item->setBrush(QBrush(_color));
        item->setRotation(random(_textAngle1, _textAngle2));


        // boucle de positionnement du texte
        for (int i = 0; i < 500; ++i) {
            // positionnement du texte
            item->setPos(random(pageRect.left(), pageRect.right()),
                         random(pageRect.top(), pageRect.bottom()));

            // verification de collision
            ok = true;

            // cherche une intersection
            for (int i = 0; i < itemList.size(); ++i) {
                if (item->collidesWithItem(itemList[i])) {
                    ok = false;
                    break;
                }
            }

            if (ok) {
                // s'il n'y a pas d'intersection, on ajoute l'objet dans la liste et on quitte la boucle
                itemList.append(item);

                emit progress(++wordsAmount);
                tryCounter = 0;

                break;
            }
        } // fin de la boucle de positionnement du texte


        if (!ok) {
            // si on a quitté la boucle acause de la fin de la boucle for, on supprime l'objet et on r'essaye avec une autre taille

            scene.removeItem(item);

            tryCounter++;
        }

    } // while (tryCounter < 300)


    _painter->begin(_printer);

    scene.render(_painter);

    _painter->end();
}

void EndpaperThread::start(QPrinter *printer, QPainter *painter, const QString &text, const QFont &font, const QColor &color, qreal textSize1, qreal textSize2, qreal textAngle1, qreal textAngle2)
{
    if (isRunning())
        return;

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

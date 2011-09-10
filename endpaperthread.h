#ifndef ENDPAPERTHREAD_H
#define ENDPAPERTHREAD_H

#include <QThread>
#include <QPrinter>
#include <QPainter>

class EndpaperThread : public QThread
{
    Q_OBJECT
public:
    explicit EndpaperThread(QObject *parent = 0);

signals:
    void progress(int value);

public slots:
    void start(QPrinter *printer, QPainter *painter, QString text, QFont font, qreal textSize1, qreal textSize2, qreal textAngle1, qreal textAngle2);
    void stop();

private:
    void run();

    QPrinter *_printer;
    QPainter *_painter;
    QString _text;
    QFont _font;
    qreal _textSize1;
    qreal _textSize2;
    qreal _textAngle1;
    qreal _textAngle2;

    bool _stop;
};

#endif // ENDPAPERTHREAD_H

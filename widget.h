#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QProgressDialog>
#include "endpaperthread.h"

namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void print();
    void cancel();

private slots:
    void threadprogress(int);
    void finish();

    void colorClicked();

private:
    void connexions();
    void loaddef();
    void savedef();

    Ui::Widget *ui;

    QProgressDialog *_progressDialog;
    EndpaperThread *_thread;

    QPrinter _printer;
    QPainter _painter;
};

#endif // WIDGET_H

#include "widget.h"
#include "ui_widget.h"

#include <QPrintDialog>
#include <QPrinter>
#include <QPainter>
#include <QFont>

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    connexions();

    loaddef();

    srand(time(0));
}

Widget::~Widget()
{
    savedef();

    delete ui;
}

void Widget::print()
{
    QPrinter printer;
    QPainter painter;

    QPrintDialog dialog(&printer, this);
    if (dialog.exec() == QDialog::Accepted) {
        setDisabled(true);


        QRectF pageRect = printer.pageRect();
        QGraphicsScene scene(pageRect);

        QString text(ui->textLineEdit->text());
        QFont font(ui->fontComboBox->font());

        QGraphicsTextItem *item;

        int stop = 0;
        bool ok;


        while (stop < 300) {
            // création du texte (taille et font)
            qreal textSize = random(ui->sizeMinDoubleSpinBox->value(), ui->sizeMaxDoubleSpinBox->value());
            font.setPointSizeF(textSize);

            item = scene.addText(text, font);

            for (int i = 0; i < 100; ++i) {
                // positionnement du texte (position et rotation)
                item->setPos(random(pageRect.left(), pageRect.right()),
                           random(pageRect.top(), pageRect.bottom()));


                item->setRotation(random(ui->angleMinDoubleSpinBox->value(), ui->angleMaxDoubleSpinBox->value()));

                // verification de collision

                ok = item->collidingItems().isEmpty();

                if (ok) {

                    stop = 0;

                    break;
                }
            }

            if (!ok) {
                scene.removeItem(item);

                stop++;
            }


        }


        painter.begin(&printer);

        scene.render(&painter);

        painter.end();

        QMessageBox::information(this, "Finish", "Finish");
        setEnabled(true);
    }
}

void Widget::connexions()
{
    connect(ui->printPushButton, SIGNAL(clicked()), this, SLOT(print()));
}

qreal Widget::random(qreal min, qreal max)
{
    qreal ret = (qreal)rand() / (qreal)RAND_MAX;

    return ret * (max - min) + min;
}

void Widget::loaddef()
{
    QSettings set;

    setGeometry(set.value("geo").toRect());

    ui->textLineEdit->setText(set.value("text", "Title").toString());
    ui->fontComboBox->setFont(QFont(set.value("font").toString()));
    ui->sizeMinDoubleSpinBox->setValue(set.value("sizemin", 10).toDouble());
    ui->sizeMaxDoubleSpinBox->setValue(set.value("sizemax", 60).toDouble());
    ui->angleMinDoubleSpinBox->setValue(set.value("anglemin", -45).toDouble());
    ui->angleMaxDoubleSpinBox->setValue(set.value("anglemax", 45).toDouble());
}

void Widget::savedef()
{
    QSettings set;

    set.setValue("geo", geometry());

    set.setValue("text", ui->textLineEdit->text());
    set.setValue("font", ui->fontComboBox->font().toString());
    set.setValue("sizemin", ui->sizeMinDoubleSpinBox->value());
    set.setValue("sizemax", ui->sizeMaxDoubleSpinBox->value());
    set.setValue("anglemin", ui->angleMinDoubleSpinBox->value());
    set.setValue("anglemax", ui->angleMaxDoubleSpinBox->value());
}

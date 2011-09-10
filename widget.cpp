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
#include <QColorDialog>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    _progressDialog = new QProgressDialog("Operation in progress.", "Print now!", 0, 0, this);
    _thread = new EndpaperThread(this);

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
    QPrintDialog dialog(&_printer, this);
    if (dialog.exec() == QDialog::Accepted) {
        _progressDialog->setLabelText("Operation in progress.");
        _progressDialog->show();

        // start thread...
        _thread->start(&_printer, &_painter, ui->textLineEdit->text(), ui->fontComboBox->font(), ui->colorPushButton->text(),
                       ui->sizeMinDoubleSpinBox->value(), ui->sizeMaxDoubleSpinBox->value(),
                       ui->angleMinDoubleSpinBox->value(), ui->angleMaxDoubleSpinBox->value());
    }
}

void Widget::cancel()
{
    _thread->stop();
}

void Widget::threadprogress(int value)
{
    _progressDialog->setLabelText(QString("Operation in progress: %1 words").arg(value));
}

void Widget::finish()
{
    _progressDialog->hide();
    QMessageBox::information(this, "Finish", "Finish");
}

void Widget::connexions()
{
    connect(ui->colorPushButton, SIGNAL(clicked()), this, SLOT(colorClicked()));
    connect(ui->printPushButton, SIGNAL(clicked()), this, SLOT(print()));

    connect(_thread, SIGNAL(finished()), this, SLOT(finish()));
    connect(_thread, SIGNAL(progress(int)), this, SLOT(threadprogress(int)));
    connect(_progressDialog, SIGNAL(canceled()), this, SLOT(cancel()));
}

void Widget::loaddef()
{
    QSettings set;

    setGeometry(set.value("geo").toRect());

    ui->textLineEdit->setText(set.value("text", "Title").toString());
    ui->fontComboBox->setFont(QFont(set.value("font").toString()));
    ui->colorPushButton->setText(set.value("color", "#000000").toString());
    ui->sizeMinDoubleSpinBox->setValue(set.value("sizemin", 10).toDouble());
    ui->sizeMaxDoubleSpinBox->setValue(set.value("sizemax", 50).toDouble());
    ui->angleMinDoubleSpinBox->setValue(set.value("anglemin", -70).toDouble());
    ui->angleMaxDoubleSpinBox->setValue(set.value("anglemax", 70).toDouble());
}

void Widget::savedef()
{
    QSettings set;

    set.setValue("geo", geometry());

    set.setValue("text", ui->textLineEdit->text());
    set.setValue("font", ui->fontComboBox->font().toString());
    set.setValue("color", ui->colorPushButton->text());
    set.setValue("sizemin", ui->sizeMinDoubleSpinBox->value());
    set.setValue("sizemax", ui->sizeMaxDoubleSpinBox->value());
    set.setValue("anglemin", ui->angleMinDoubleSpinBox->value());
    set.setValue("anglemax", ui->angleMaxDoubleSpinBox->value());
}

void Widget::colorClicked()
{
    QColorDialog cd(ui->colorPushButton->text(), this);

    if (cd.exec() == QDialog::Accepted) {
        ui->colorPushButton->setText(cd.currentColor().name());
    }
}

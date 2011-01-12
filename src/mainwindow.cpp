#include <QDebug>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->btnConnect, SIGNAL(clicked()), this, SLOT(openSerialPort()));
}

MainWindow::~MainWindow()
{
    if (serial_ctrl->isOpen()) {
        serial_ctrl->close();
    }

    if (timer_checkReadyState->isActive()) {
        timer_checkReadyState->stop();
    }

    delete serial_ctrl;
    delete timer_checkReadyState;
    delete ui;
}

void MainWindow::openSerialPort()
{
    serial_ctrl = new QExtSerialPort();
    serial_ctrl->setPortName("/dev/ttyUSB0");
    serial_ctrl->setBaudRate(BAUD38400);
    serial_ctrl->setParity(PAR_NONE);
    serial_ctrl->setDataBits(DATA_8);
    serial_ctrl->setStopBits(STOP_1);
    serial_ctrl->setFlowControl(FLOW_OFF);
    serial_ctrl->setQueryMode(QExtSerialPort::EventDriven);

    if (!serial_ctrl->open(QIODevice::ReadWrite|QIODevice::Unbuffered)) {
        QMessageBox::critical(this,
                              "Connection Error",
                              QString("Could not open /dev/ttyUSB0: %1").arg(serial_ctrl->errorString()));
    }
    else {
        serial_ctrl->flush();
        serial_ctrl->flush();

        timer_checkReadyState = new QTimer(this);
        connect(timer_checkReadyState, SIGNAL(timeout()), this, SLOT(checkReadyState()));
        timer_checkReadyState->start(500);
        statusBar()->showMessage("Waiting for controller...");
    }
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::checkReadyState()
{
    char buffer[256];
    int received = 0;

    bool check_1 = false;
    bool check_2 = false;
    bool check_3 = false;
    bool check_4 = false;

    timer_checkReadyState->stop();

    received = serial_ctrl->read(buffer, 255);
    if (received > 0) {
        buffer[received] = '\0';

        QString line = QString::fromLocal8Bit(buffer);
        QStringList parts = line.split(';');

        qDebug() << "Received serial line:" << line;

        if (parts.length() < 4) {
            QMessageBox::critical(this,
                                  "Protocol Error",
                                  "Protocol error: Invalid response from controller. Missing required data in response.");
            serial_ctrl->close();
            return;
        }
        for (int i = 0; i < 4; i++) {
            QStringList item_parts = parts.at(i).split('=');
            if (item_parts.length() != 2) {
                QMessageBox::critical(this,
                                      "Protocol Error",
                                      "Protocol error: Invalid response from controller. Invalid name/value pair format.");
                serial_ctrl->close();
                return;
            }
            else {
                QString key = item_parts.at(0);
                QString value = item_parts.at(1);

                if (key == QString("CMD")) {
                    if (value == QString("PWRON")) {
                        check_1 = true;
                    }
                    else {
                        QMessageBox::critical(this,
                                              "Protocol Error",
                                              "Protocol error: Invalid response from controller. Unexpected command identifier.");
                        serial_ctrl->close();
                        return;
                    }
                }

                if (key == QString("MODEL")) {
                    ui->lblModelName->setText(value);
                    check_2 = true;
                }

                if (key == QString("VERSION")) {
                    ui->lblVersion->setText(value);
                    check_3 = true;
                }

                if (key == QString("STATUS")) {
                    if (value == QString("READY")) {
                        check_4 = true;
                    }
                    else {
                        QMessageBox::critical(this,
                                              "Device Error",
                                              QString("Device error: Unexpected status %1 returned.").arg(value));
                        serial_ctrl->close();
                        return;
                    }
                }
            }
        }

        if (false == (check_1 && check_2 && check_3 && check_4)) {
            QMessageBox::critical(this,
                                  "Protocol Error",
                                  "Protocol error: Invalid response from controller. Missing required data in response.");
            serial_ctrl->close();
            return;
        }
        else {
            return;
        }
    }

    timer_checkReadyState->start(500);
}

void MainWindow::on_sliderChannel1_sliderMoved(int position)
{
    qDebug("Channel 1 slider moved to %d", position);

    ui->dialChannel1->setValue(position);
    ui->lneChannel1Value->setText(QString("%1").arg(position));

    if (serial_ctrl->isOpen()) {
        char pos_lsb;
        char pos_msb;

        pos_lsb = position;
        pos_msb = position >> 8;

        //serial_ctrl->write(command, 3);
        serial_ctrl->putChar('S');
        serial_ctrl->putChar('A');
        serial_ctrl->putChar(pos_lsb);
        serial_ctrl->putChar(pos_msb);
    }
}


void MainWindow::on_sliderChannel1_valueChanged(int value)
{
    on_sliderChannel1_sliderMoved(value);
}

void MainWindow::on_sliderChannel2_sliderMoved(int position)
{
    qDebug("Channel 2 slider moved to %d", position);

    ui->dialChannel2->setValue(position);
    ui->lneChannel2Value->setText(QString("%1").arg(position));

    if (serial_ctrl->isOpen()) {
        char pos_lsb;
        char pos_msb;

        pos_lsb = position;
        pos_msb = position >> 8;

        serial_ctrl->putChar('S');
        serial_ctrl->putChar('B');
        serial_ctrl->putChar(pos_lsb);
        serial_ctrl->putChar(pos_msb);
    }
}

void MainWindow::on_sliderChannel2_valueChanged(int value)
{
    on_sliderChannel2_sliderMoved(value);
}

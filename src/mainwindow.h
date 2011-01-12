#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "qextserialport.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void checkReadyState();
    void openSerialPort();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    QExtSerialPort *serial_ctrl;
    QTimer *timer_checkReadyState;

private slots:
    void on_sliderChannel2_valueChanged(int value);
    void on_sliderChannel2_sliderMoved(int position);
    void on_sliderChannel1_valueChanged(int value);
    void on_sliderChannel1_sliderMoved(int position);
};

#endif // MAINWINDOW_H

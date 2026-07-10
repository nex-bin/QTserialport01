#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "QTimer"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
    QSerialPort *serialPort;//定义串口指针

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;
    void init_app();
    void readSerialData();
    int baudPortIndex = 0;
    int parityIndex = 0;
    int stopIndex = 0;
    int dataBitIndex = 3;
    int flowControlIndex = 0;
    bool HexWriteFlag = 1;
    bool HexReadFlag = 1;
    bool isConnected = 0;
    bool enableReadSerialPortData = 1;

    void fleshSerialPort();

    QTimer *m_timer;
    QByteArray readDataBuffer;



private slots:
    void on_comboBox_activated(int index);
    void on_comboBox_2_activated(int index);
    void on_comboBox_3_activated(int index);
    void on_comboBox_4_activated(int index);
    void on_comboBox_5_activated(int index);
    void on_comboBox_6_activated(int index);
    void on_pushButton_clicked();    //刷新按钮
    void on_pushButton_2_clicked();  //打开串口按钮
    void on_pushButton_3_clicked();  //写入按钮
    void on_pushButton_4_clicked();  //读取功能开关
    void on_checkBox_clicked();
    void on_checkBox_2_clicked();
    void on_checkBox_3_clicked();
    void on_checkBox_4_clicked();
    void on_timerOut();


private:
    Ui::Widget *ui;
};
#endif // WIDGET_H

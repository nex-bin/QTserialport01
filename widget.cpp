#include "widget.h"
#include "ui_widget.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include "QDebug"
#include "QMessagebox.h"
#include "QTextEdit"
#include "QTime"
#include "QRegularExpression"
#include "QButtonGroup"
#include "QTimer"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{

    ui->setupUi(this);
    init_app();

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_comboBox_activated(int index)
{
    qDebug()<<"串口："<<index;

}
void Widget::on_comboBox_2_activated(int index)
{
    qDebug()<<"波特率："<<index;
    baudPortIndex = index;
}
void Widget::on_checkBox_clicked()
{
    HexWriteFlag = 0;
    qDebug()<<"HexWriteFlag: "<<HexWriteFlag;

}
void Widget::on_checkBox_2_clicked()
{
    HexWriteFlag = 1;
    qDebug()<<"HexWriteFlag: "<<HexWriteFlag;
}
void Widget::on_checkBox_3_clicked(){
    HexReadFlag = 0;
    qDebug()<<"HexReadFlag: "<<HexReadFlag;

}
void Widget::on_checkBox_4_clicked(){
    HexReadFlag = 1;
    qDebug()<<"HexReadFlag: "<<HexReadFlag;

}
void Widget::on_comboBox_3_activated(int index)
{
    qDebug()<<"奇偶校验："<<index;
    parityIndex = index;
}
void Widget::on_comboBox_4_activated(int index)
{
    qDebug()<<"停止位："<<index;
    stopIndex = index;
}
void Widget::on_comboBox_5_activated(int index)
{
    qDebug()<<"数据位："<<index;
    ui->comboBox_5->setCurrentIndex(index);
    dataBitIndex = index;
}void Widget::on_comboBox_6_activated(int index)
{
    qDebug()<<"数据流："<<index;
    ui->comboBox_6->setCurrentIndex(index);
    flowControlIndex = index;

}

void Widget::on_pushButton_clicked()
{
    ui->comboBox->clear();
    fleshSerialPort();

    Widget::on_comboBox_activated(0);
    Widget::on_comboBox_2_activated(baudPortIndex);
    Widget::on_comboBox_3_activated(parityIndex);
    Widget::on_comboBox_4_activated(stopIndex);
    Widget::on_comboBox_5_activated(dataBitIndex);
    Widget::on_comboBox_6_activated(flowControlIndex);

}
void Widget::on_pushButton_2_clicked(){

    if(isConnected){
        //之前串口是连接的，再点击就该断开了。
        if (serialPort) {
            serialPort->close();
            delete serialPort;
            serialPort = nullptr;
        }
        isConnected = false;
        ui->comboBox->setEnabled(true);
        ui->comboBox_2->setEnabled(true);
        ui->comboBox_3->setEnabled(true);
        ui->comboBox_4->setEnabled(true);
        ui->comboBox_5->setEnabled(true);
        ui->comboBox_6->setEnabled(true);
        ui->pushButton->setEnabled(true);
        qDebug()<<"串口已经关闭";
        ui->pushButton_2->setText("打开串口");
        return;
    }

    //检查串口
    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    if(serialPortInfos.isEmpty()){
        qDebug()<<"warning1: QSerialPort is disabled ";
        return;
    }

    int a = 0;
    a = ui->comboBox->currentIndex();
    QString portName = serialPortInfos[a].portName();
    qDebug()<<"***********串口信息****************";
    qDebug()<<"name:  "<<portName;

    //之前串口是断开的，再点击就该连接了
    serialPort = new QSerialPort(this);
    serialPort->setPortName(portName);
    // serialPort->setBaudRate(QSerialPort::Baud9600);
    switch(baudPortIndex){
    case 0:
        serialPort->setBaudRate(QSerialPort::Baud9600);
        break;
    case 1:
        serialPort->setBaudRate(QSerialPort::Baud19200);
        break;
    case 2:
        serialPort->setBaudRate(QSerialPort::Baud38400);
        break;
    case 3:
        serialPort->setBaudRate(QSerialPort::Baud57600);
        break;
    case 4:
        serialPort->setBaudRate(QSerialPort::Baud115200);
        break;
    }
    qDebug()<<"baudport: "<<serialPort->baudRate();

    switch(parityIndex){
    case 0:
        serialPort->setParity(QSerialPort::NoParity);
        break;
    case 1:
        serialPort->setParity(QSerialPort::OddParity);
        break;
    case 2:
        serialPort->setParity(QSerialPort::EvenParity);
        break;
    case 3:
        serialPort->setParity(QSerialPort::MarkParity);
        break;
    case 4:
        serialPort->setParity(QSerialPort::SpaceParity);
        break;

    }
    qDebug()<<"parity: "<<serialPort->parity();

    switch(stopIndex){
    case 0:
        serialPort->setStopBits(QSerialPort::OneStop);
        break;
    case 1:
        serialPort->setStopBits(QSerialPort::OneAndHalfStop);
        break;
    case 2:
        serialPort->setStopBits(QSerialPort::TwoStop);
        break;
    }
    qDebug()<<"stopBits: "<<serialPort->stopBits();

    switch(dataBitIndex){
    case 0:
        serialPort->setDataBits(QSerialPort::Data5);
        break;
    case 1:
        serialPort->setDataBits(QSerialPort::Data6);
        break;
    case 2:
        serialPort->setDataBits(QSerialPort::Data7);
        break;
    case 3:
        serialPort->setDataBits(QSerialPort::Data8);
        break;
    }
    qDebug()<<"dataBitIndex: "<<serialPort->dataBits();

    switch(flowControlIndex){
    case 0:
        serialPort->setFlowControl(QSerialPort::NoFlowControl);
        break;
    case 1:
        serialPort->setFlowControl(QSerialPort::HardwareControl);
        break;
    case 2:
        serialPort->setFlowControl(QSerialPort::SoftwareControl);
        break;
    }
    qDebug()<<"flowControl: "<<serialPort->flowControl();

    if(serialPort->open(QIODevice::ReadWrite)){
        isConnected = true;
        qDebug()<<"port is ok";
        ui->pushButton_2->setText("关闭串口");
        ui->comboBox->setEnabled(false);
        ui->comboBox_2->setEnabled(false);
        ui->comboBox_3->setEnabled(false);
        ui->comboBox_4->setEnabled(false);
        ui->comboBox_5->setEnabled(false);
        ui->comboBox_6->setEnabled(false);
        ui->pushButton->setEnabled(false);

        connect(serialPort, &QSerialPort::readyRead, this, &Widget::readSerialData);
    }
    else{
        QMessageBox::warning(this, "wrong2", serialPort->errorString());
    }


}
void Widget::init_app(){
    //初始化参数
    enableReadSerialPortData = 1;
    isConnected = false;
    m_timer = new QTimer;
    m_timer->setSingleShot(true);
    m_timer->setInterval(20);

    //初始化串口
    fleshSerialPort();
    //初始化波特率
    ui->comboBox_2->addItems({"9600", "19200", "38400", "57600", "115200"});
    //初始奇偶校验
    ui->comboBox_3->addItems({"NONE", "ODD", "EVEN", "MARK", "SPACE"});
    //初始停止位
    ui->comboBox_4->addItems({"1", "1.5", "2"});
    //初始数据位
    ui->comboBox_5->addItems({"5", "6", "7", "8"});
    //初始数据流
    ui->comboBox_6->addItems({"NoFlowContrel", "HardwareControl", "SoftwareControl"});

    //单选框设置为互斥
    QButtonGroup *WriteMode = new QButtonGroup(this);
    WriteMode->addButton(ui->checkBox);
    WriteMode->addButton(ui->checkBox_2);
    WriteMode->setExclusive(true);
    ui->checkBox_2->setChecked(true);

    QButtonGroup *ReadMode = new QButtonGroup(this);
    ReadMode->addButton(ui->checkBox_3);
    ReadMode->addButton(ui->checkBox_4);
    ReadMode->setExclusive(true);
    ui->checkBox_4->setChecked(true);

}
void Widget::readSerialData(){
    qDebug()<<"串口可读取：";

    readDataBuffer.append(serialPort->readAll());
    qDebug()<<"timer start";

    m_timer->start();

    connect(m_timer,&QTimer::timeout,this,&Widget::on_timerOut);
    // on_timerOut();



    // mainLayout->addWidget(scrollArea);
    // 2. 解析协议
    // if (data.size() >= 4) {
    //     quint8 header = static_cast<quint8>(data[0]);  // 第一个字节
    //     quint8 command = static_cast<quint8>(data[1]); // 第二个字节
    //     quint8 length = static_cast<quint8>(data[2]);  // 第三个字节

    //     qDebug() << QString("帧头: 0x%1").arg(header, 2, 16, QChar('0'));
    //     qDebug() << QString("命令: 0x%1").arg(command, 2, 16, QChar('0'));
    //     qDebug() << QString("长度: %1").arg(length);
    // }

}
void Widget::on_timerOut(){
    // ui->scrollAreaWidgetContents->();
    qDebug()<<"timer out";
    if (readDataBuffer.isEmpty()) {
        qDebug()<<"data is empty";
        return;
    }
    qDebug()<<readDataBuffer;
    qDebug() << "原始十六进制:" << readDataBuffer.toHex(' ').toUpper();
    QString time = "[" + QTime::currentTime().toString() + "] ";
    qDebug()<<enableReadSerialPortData;
    if(enableReadSerialPortData){
        if(HexReadFlag){
            ui->textBrowser->append(time + readDataBuffer.toHex(' ').toUpper());

        }else{
            ui->textBrowser->append(time + QString::fromUtf8(readDataBuffer));

        }
    }
    readDataBuffer.clear();
}

void fun (){
    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    if (serialPortInfos.isEmpty()) {
        qDebug() << "No serial ports found.";

    }
    // 遍历列表，打印每个串口的信息
    for (const QSerialPortInfo &portInfo : serialPortInfos) {
        qDebug() << "\n"
                 << "Port:" << portInfo.portName() << "\n"
                 << "Location:" << portInfo.systemLocation() << "\n"
                 << "Description:" << portInfo.description() << "\n"
                 << "Manufacturer:" << portInfo.manufacturer() << "\n"
                 << "Serial number:" << portInfo.serialNumber() << "\n"
                 << "Vendor Identifier:"
                 << (portInfo.hasVendorIdentifier()
                         ? QByteArray::number(portInfo.vendorIdentifier(), 16)
                         : QByteArray()) << "\n"
                 << "Product Identifier:"
                 << (portInfo.hasProductIdentifier()
                         ? QByteArray::number(portInfo.productIdentifier(), 16)
                         : QByteArray())
                 << "portInfo manufacturer()" << portInfo.manufacturer()<<"\n"
                 <<"manufecturer";
    }
}
void Widget::on_pushButton_4_clicked(){
    enableReadSerialPortData = !enableReadSerialPortData;
    qDebug()<<"enableReadSerialPortData is :"+ QString::number(enableReadSerialPortData);

    ui->checkBox_3->setDisabled(!enableReadSerialPortData);
    ui->checkBox_4->setDisabled(!enableReadSerialPortData);

}
void Widget::on_pushButton_3_clicked(){
    //检查串口
    if(QSerialPortInfo::availablePorts().isEmpty()){
        qDebug()<<"warning2: QSerialPort is disabled ";
        return;
    }
    //数据清洗
    QString text = ui->textEdit->toPlainText();
    text.remove(QRegularExpression("\\s+"));  //去除空格和换行
    //判断是否为空
    if(text.isEmpty()){
        qDebug()<<"text is empty.";
        return;
    }
    QRegularExpression hexPattern("^[0-9A-Fa-f]+$");
    if(!hexPattern.match(text).hasMatch()){
        qDebug()<<"text 中含有非法字符，请重新输入。";
        return;
    }
    qDebug()<<"text: "+ text ;
    qDebug()<<QString(typeid(text).name());
    qDebug()<<"text.toUtf8(): "+text.toUtf8();
    qDebug()<<QString(typeid(text.toUtf8()).name());
    QByteArray data = QByteArray::fromHex(text.toUtf8());
    int a = 0;
    //定义写模式
    if(HexWriteFlag){
        //Hex写
        a = serialPort->write(data);
    }else{
        //Ascll写
        a = serialPort->write(text.toUtf8());
    }
    // int a = serialPort->write(data);
    qDebug()<<"data: "+ data;
    qDebug()<<QString(typeid(data).name());
    if(a<0){
        QMessageBox::warning(this,"wrong","写错误");
    }

}
void Widget::fleshSerialPort(){
    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    if(serialPortInfos.isEmpty()){
        ui->comboBox->addItem("no port");
    }else{
        for (const QSerialPortInfo &portInfo : serialPortInfos) {
            ui->comboBox->addItem(portInfo.portName() );
            qDebug()<<"find port";
        }
    }

}



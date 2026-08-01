#include "widget.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include "QDebug"
#include "QMessagebox.h"
#include "QTextEdit"
#include "QTime"
#include "QRegularExpression"
#include "QButtonGroup"
#include "QTimer"
#include "ui_widget.h"
// #include <core5compat>
#include <QTextCodec>

Widget::Widget(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::Widget )
{

    ui->setupUi(this);
    init_app();
    // serialWrite();
    connect(m_timer,&QTimer::timeout,this,&Widget::on_timerOut);

}

void Widget::on_comboBox_activated(int index)
{
    qDebug()<<"串口："<<index;
    portIndex = index;

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
    appendColorText(ui->textBrowser,"文本写",INFO);

}
void Widget::on_checkBox_2_clicked()
{
    HexWriteFlag = 1;
    qDebug()<<"HexWriteFlag: "<<HexWriteFlag;
    appendColorText(ui->textBrowser,"Hex写",INFO);

}
void Widget::on_checkBox_3_clicked(){
    HexReadFlag = 0;
    qDebug()<<"HexReadFlag: "<<HexReadFlag;
    appendColorText(ui->textBrowser,"文本读取",INFO);


}
void Widget::on_checkBox_4_clicked(){
    HexReadFlag = 1;
    qDebug()<<"HexReadFlag: "<<HexReadFlag;
    appendColorText(ui->textBrowser,"Hex读取",INFO);


}
void Widget::on_comboBox_3_activated(int index)
{
    qDebug()<<"奇偶校验："<<index;
    parityIndex = index;
    appendColorText(ui->textBrowser,"奇偶校验变更："+ui->comboBox_3->currentText(),INFO);

}
void Widget::on_comboBox_4_activated(int index)
{
    qDebug()<<"停止位："<<index;
    stopIndex = index;
    appendColorText(ui->textBrowser,"停止位变更："+ui->comboBox_4->currentText(),INFO);

}
void Widget::on_comboBox_5_activated(int index)
{
    qDebug()<<"数据位："<<index;
    ui->comboBox_5->setCurrentIndex(index);
    dataBitIndex = index;
    appendColorText(ui->textBrowser,"数据位变更："+ui->comboBox_5->currentText(),INFO);

}void Widget::on_comboBox_6_activated(int index)
{
    qDebug()<<"数据流："<<index;
    ui->comboBox_6->setCurrentIndex(index);
    flowControlIndex = index;
    appendColorText(ui->textBrowser,"数据流变更："+ui->comboBox_6->currentText(),INFO);

}

void Widget::on_pushButton_clicked()
{
    ui->comboBox->clear();
    fleshSerialPort();

    Widget::on_comboBox_activated(portIndex);
    Widget::on_comboBox_2_activated(baudPortIndex);
    Widget::on_comboBox_3_activated(parityIndex);
    Widget::on_comboBox_4_activated(stopIndex);
    Widget::on_comboBox_5_activated(dataBitIndex);
    Widget::on_comboBox_6_activated(flowControlIndex);

}
void Widget::on_pushButton_2_clicked(){
//打开/关闭串口

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
        ui->pushButton_3->setEnabled(isConnected);
        ui->pushButton_4->setEnabled(isConnected);

        ui->checkBox->setEnabled(isConnected);
        ui->checkBox_2->setEnabled(isConnected);
        qDebug()<<"串口已经关闭";
        appendColorText(ui->textBrowser,"QSerialPort is closed.",ERROR);

        ui->pushButton_2->setText("打开串口");
        return;
    }

    //检查串口
    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    if(serialPortInfos.isEmpty()){
        qDebug()<<"warning1: QSerialPort is disabled ";
        appendColorText(ui->textBrowser,"QSerialPort is disabled.",ERROR);
        return;
    }

    // int a = 0;
    // a = ui->comboBox->currentIndex();
    QString portName = serialPortInfos[portIndex].portName();
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

        ui->pushButton_3->setEnabled(isConnected);
        ui->pushButton_4->setEnabled(isConnected);
        ui->checkBox->setEnabled(isConnected);
        ui->checkBox_2->setEnabled(isConnected);
        appendColorText(ui->textBrowser,"串口打开成功",SUCCESS);

        connect(serialPort, &QSerialPort::readyRead, this, &Widget::readSerialData);
    }
    else{
        appendColorText(ui->textBrowser,serialPort->errorString(),ERROR);
    }


}

void Widget::init_app(){

    //初始化参数
    enableReadSerialPortData = 1;
    isConnected = false;
    m_timer = new QTimer;
    m_timer->setSingleShot(true);
    m_timer->setInterval(20);

    portIndex = 0;
    baudPortIndex = 0;
    parityIndex = 0;
    stopIndex = 0;
    dataBitIndex = 3;
    flowControlIndex = 0;
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

    ui->pushButton->setEnabled(!isConnected);
    ui->pushButton_3->setEnabled(isConnected);
    ui->pushButton_4->setEnabled(isConnected);
    ui->checkBox->setEnabled(isConnected);
    ui->checkBox_2->setEnabled(isConnected);
    ui->comboBox->setEnabled(!isConnected);

    if(isConnected){
        ui->pushButton_2->setText("关闭串口");
    }else{
        ui->pushButton_2->setText("打开串口");
    }
    connect(ui->pushButton_5,&QPushButton::clicked,this,&Widget::cleanReadArea);
    connect(ui->pushButton_6,&QPushButton::clicked,this,&Widget::cleanWriteArea);

}
void Widget::readSerialData(){
    qDebug()<<"串口可读取：";

    readDataBuffer.append(serialPort->readAll());
    qDebug()<<"timer start";

    m_timer->start();

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
    qDebug()<<"timer out";
    if (readDataBuffer.isEmpty()) {
        qDebug()<<"data is empty";
        return;
    }
    qDebug()<<"接收到的数据"<<readDataBuffer;
    qDebug() << "原始十六进制:" <<typeid(readDataBuffer).name() << readDataBuffer.toHex(' ').toUpper()<<readDataBuffer;
    QString time = QString("[" + QTime::currentTime().toString() + "] ");
    if(enableReadSerialPortData){
        if(HexReadFlag){
            QString text = QString(readDataBuffer.toHex(' ').toUpper());
            appendColorText(ui->textBrowser,text,RECEIVE);

        }else{
            // 1. 创建 GB18030 解码器
            QTextCodec *codec = QTextCodec::codecForName("GB18030");
            if(!codec){
                qDebug()<<"不支持GB18030编码";
                qDebug() << "支持的编码：" << QStringDecoder::availableCodecs();
            }
            // 2. 执行解码，得到 QString
            QString text = codec->toUnicode(readDataBuffer);

            // // 1. 创建一个解码器，指定源数据的编码
            // auto decoder = QStringDecoder(QStringDecoder::System); // 或 QStringDecoder::Utf8
            // // 对于GBK，需要指定名称 "GBK"
            // auto gbkDecoder = QStringDecoder("GB18030");

            // 2. 执行解码，得到 QString
            // QString text = gbkDecoder(readDataBuffer);
            // if (gbkDecoder.hasError()) {
            //     // 处理解码错误
            // }
            qDebug()<<"text"<<text;
            appendColorText(ui->textBrowser,text,DEBUG);

            text = QString(QString::fromUtf8(readDataBuffer));
            appendColorText(ui->textBrowser,text,RECEIVE);

        }
    }
    m_timer->stop();
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
    //检查是否有串口
    if(QSerialPortInfo::availablePorts().isEmpty()){
        appendColorText(ui->textBrowser,"无串口可用，请检查连接。",ERROR);
        qDebug()<<"warning2: QSerialPort is disabled ";
        return;
    }
    //检查当前串口是否可使用
    bool EnableUsedFlag = 0;
    for(QSerialPortInfo ab :QSerialPortInfo::availablePorts()){
        if(ab.portName() == serialPort->portName() && serialPort->isReadable()){

            EnableUsedFlag = 1;;
        }
    }
    if(!EnableUsedFlag){
        appendColorText(ui->textBrowser,"使用的串口被拔出。请检查。",ERROR);
        isConnected = false;

        //串口指针 serialPort 已经不可以用了，要重置掉。
        if(m_timer->isActive()){
            m_timer->stop();
        }
        if(serialPort->isOpen()){
            serialPort->close();
            delete(serialPort);
            serialPort = nullptr;
        }
        readDataBuffer.clear();
        init_app();
        fleshSerialPort();
        return;
    }
    QString text = ui->textEdit->toPlainText();
    if(HexWriteFlag){
        serialWrite(serialPort,text,HEX);
    }
    else{
        serialWrite(serialPort,text,ASCLL);
    }

}
void Widget::fleshSerialPort(){
    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    ui->comboBox->clear();

    if(serialPortInfos.isEmpty()){
        ui->comboBox->addItem("no port");
    }else{
        for(const QSerialPortInfo &ab : QSerialPortInfo::availablePorts()){
            qDebug()<<typeid(serialPortInfos).name() << ab.portName();
        }
        for (const QSerialPortInfo &portInfo : serialPortInfos) {
            ui->comboBox->addItem(portInfo.portName() );
            qDebug()<<"find port"<<portInfo.portName();
        }
    }

}
void Widget::appendColorText(QTextBrowser * browser,const QString & text,enum LOGLEVEL level){
    QString time = QTime::currentTime().toString();
    QString color,st;
    switch(level){//INFO,DEBUG,WARNing,ERROR,SUCCESS,RECEIVE,SEND
    case INFO:
        color = "#4fc3f7";
        st = QString("<span style='color:black;'>[%1]</span>""<span style='color:[%2];'>[%3]</span>").arg(time).arg(color).arg(text);
        break;
    case DEBUG:
        color = "#b39ddb";
        st = QString("<span style='color:black;'>[%1]</span>""<span style='color:black;font-size: 10px;'> Debug：</span>""<span style='color:%2;'>[%3]</span>").arg(time).arg(color).arg(text);
        break;
    case WARNing:
        color = "pink";
        st = QString("<span style='color:black;'>[%1]</span>""<span style='color:yellow;font-size: 10px;'> Warning：</span>""<span style='color:%2;'>[%3]</span>").arg(time).arg(color).arg(text);
        break;
    case ERROR:
        color = "#ffb74d";
        st = QString("<span style='color:black;'>[%1]</span>""<span style='color:red;font-size: 15px;'> ERROR! </span>""<span style='color:%2;'>[%3]</span>").arg(time).arg(color).arg(text).arg(level);
        break;
    case SUCCESS:
        color = "#ef5350";
        st = QString("<span style='color:black;'>[%1]</span>""<span style='color:%2;'>[%3]</span>""<span style='color:green;'> SUCCESS </span>").arg(time).arg(color).arg(text).arg(level);
        break;
    case RECEIVE:
        color = "red";
        st = QString("<span style='color:black;'>[%1]</span>""<span style='color:black;font-size: 15px;'> 接收：</span>""<span style='color:%2;'>[%3]</span>").arg(time).arg(color).arg(text);
        break;
    case SEND:
        color = "brown";
        st = QString("<span style='color:black;'>[%1]</span>""<span style='color:black;font-size: 15px;'> 发送：</span>""<span style='color:%2;'>[%3]</span>").arg(time).arg(color).arg(text);
        break;
    }

    browser->append(st);

}
qint64 Widget::serialWrite(QSerialPort *& mySerialport,QString text,WRITETYPE writeType){
    qint64 successWriteFlag = -1;
    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    //判断串口可用
    bool EnablePort = 0;
    for(const QSerialPortInfo &info : serialPortInfos){
        if(info.portName() == mySerialport->portName()){
            EnablePort = 1;
        }
    }
    //串口不可用  返回
    if(!EnablePort){
        appendColorText(ui->textBrowser,"ci串口不可用。",ERROR);
        return successWriteFlag;
    }
    //串口可用
    text.remove(QRegularExpression("\\s+"));  //去除空格和换行
    QString text2 = text;
    qDebug()<<text<<"text:";
    //判断字符是否为空
    if(text.isEmpty()){
        appendColorText(ui->textBrowser,"输入内容为空，请输入内容。",ERROR);
        return successWriteFlag;
    }
    //判断字符是否非法

    //不为空  输出
    //ASCll输出
    if(writeType == ASCLL){
        qDebug()<<"text1 中含有非法字符，请重新输入。"<<text.toUtf8();

        successWriteFlag = mySerialport->write(text.toUtf8());
        if(successWriteFlag != -1){

            appendColorText(ui->textBrowser,"ASCLL: "+text,SEND);
        }
        return successWriteFlag;
    }
    //Hex输出
    if(writeType == HEX){
        //判断文本是否非法
        QRegularExpression hexPattern("^[0-9A-Fa-f]+$");
        if(!hexPattern.match(text2).hasMatch()){
            appendColorText(ui->textBrowser,"输入中含有非法字符，请重新输入。",ERROR);
            return successWriteFlag;
        }
    }
    qDebug()<<QByteArray::fromHex(text.toUtf8());
    successWriteFlag = mySerialport->write(QByteArray::fromHex(text.toUtf8()));
    appendColorText(ui->textBrowser,"HEX: "+text.toUtf8(),SEND);


    qDebug()<<typeid(mySerialport).name()<<"text.toUtf8(): "<<text.toUtf8();
    return successWriteFlag;
}
void Widget::cleanReadArea(){
    ui->textBrowser->clear();
}
void Widget::cleanWriteArea(){
    ui->textEdit->clear();
}
Widget::~Widget()
{
    delete ui;
}

#include "dabai.h"
#include "ui_dabai.h"
//save_data savedatas;

dabai::dabai(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::dabai)
{
    ui->setupUi(this);
    this->init();
    this->graph_init();
}

dabai::~dabai()
{
    delete ui;
}

crc8 mycrc;

void dabai::init()
{
    //QDirModel *model = new QDirModel;
   // ui->treeView->setModel(model);
   // ui->treeView->setRootIndex(model->index("D:/"));

    my_timer = new QTimer(this);

    G_App_file = new QFile(this);

    //myHelper::SetGB2312Code();
    //myHelper::SetUTF8Code();
    myHelper::SetsysCode();
    QStringList list_operat,list_device_type,list_fan_speed,list_priority,list_app_to_jump;
    list_operat<<"学习"<<"清除"<<"全清";
    list_device_type<<"开关"<<"人体"<<"窗磁";
    list_fan_speed<<"高"<<"中"<<"低"<<"自动"<<"关";
    list_priority<<""<<"开关"<<"时间表"<<"人体"<<"窗磁";
    list_app_to_jump<<"app_1"<<"app_2"<<"app_3"<<"app_base";
    ui->comboBox_operat->addItems(list_operat);
    ui->comboBox_device_type->addItems(list_device_type);
    ui->comboBox_parameter_3->addItems(list_fan_speed);
    ui->comboBox_parameter_4->addItems(list_fan_speed);
    ui->comboBox_app_to_jump->addItems(list_app_to_jump);
    ui->comboBox_priority_1->addItems(list_priority);
    ui->comboBox_priority_2->addItems(list_priority);
    ui->comboBox_priority_3->addItems(list_priority);
    ui->comboBox_priority_4->addItems(list_priority);
    ui->comboBox_priority_5->addItems(list_priority);
    ui->comboBox_priority_6->addItems(list_priority);
    ui->comboBox_priority_7->addItems(list_priority);
    ui->comboBox_priority_8->addItems(list_priority);

    QRegExp regx("[0-9]+$");
    QValidator *validator = new QRegExpValidator(regx);
    ui->parameter_1->setValidator(validator);
    ui->parameter_2->setValidator(validator);
    ui->data_refresh_rate->setValidator(validator);
    ui->parameter_1->setMaxLength(5);
    ui->parameter_2->setMaxLength(5);

    ui->data_refresh_rate->setMaxLength(5);

    ui->parameter_id->setMaxLength(8);



    ui->port_set->setText("3333");
    ui->hex_rx->setChecked(TRUE);
    ui->hex_tx->setChecked(TRUE);

    save_datas = new save_data;

    tcpClient = new QTcpSocket(this);
    tcpClient->abort();
    connect(tcpClient,SIGNAL(readyRead()),this,SLOT(ReadData()));
    connect(tcpClient,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(ReadError(QAbstractSocket::SocketError)));

    tcpServer = new myTcpServer(this);
    connect(tcpServer,SIGNAL(ClientConnect(int,QString,int)),this,SLOT(ClientConnect(int,QString,int)));
    connect(tcpServer,SIGNAL(ClientDisConnect(int,QString,int)),this,SLOT(ClientDisConnect(int,QString,int)));
    connect(tcpServer,SIGNAL(ClientReadData(int,QString,int,QByteArray)),this,SLOT(ClientReadData(int,QString,int,QByteArray)));


    ////timer
    connect(my_timer,SIGNAL(timeout()),this,SLOT(timer_updata()));
    my_timer->start(1000);
    ////
    for (int i=5;i<=100;i=i+5)
    {
        ui->connect_num->addItem(QString::number(i));
    }
    ui->connect_num->setCurrentIndex(5);

    ui->progressBar_download->setRange(0,49999);
    ui->progressBar_download->setValue(0);
    ui->progressBar_download->setVisible(false);
}

void dabai::deal_with_data(QByteArray in_put_data)
{
    QByteArray id;
    int recv_voc = 0,recv_hum = 0;
    int temp_tmp = 0;
    int recv_pm25 = 0;
    unsigned char recv_fan_speed = 0;
    char tmp;
    float recv_temp = 0.0;

    id.resize(4);
    if((in_put_data.length() == 27)&&(in_put_data.at(8)==5))//读取机器状态
    {
        for(int loop=0;loop<4;loop++)
        {
            //id.setNum(in_put_data.at(loop),16);
            id[loop] = in_put_data.at(loop+3);
        }
        recv_hum = int(in_put_data.at(15));
        //recv_temp = float(int(in_put_data.at(11))*255+int(in_put_data.at(12)))/10;
        temp_tmp |=(in_put_data[11] << 8)&0X0000FF00;
        temp_tmp |=in_put_data[12]&0X000000FF;
        recv_temp = (float)temp_tmp/10;
        recv_voc = int(in_put_data.at(22));
        //recv_pm25 = int(in_put_data.at(18))*255+int(in_put_data.at(19));
        //recv_pm25 = tmp_h.toInt(&ok,16)*255 + tmp_l.toInt(&ok,16);
        recv_pm25 |=(in_put_data[18] << 8)&0X0000FF00;
        recv_pm25 |=in_put_data[19]&0x000000FF;

        recv_fan_speed = (unsigned char)in_put_data.at(25);

        ui->label_tcm300ID->setText(tr("ID:%1").arg(myHelper::ByteArrayToHexStr(id)));
        ui->device_state->setText(tr("状态:设备 %1 在线").arg(myHelper::ByteArrayToHexStr(id)));
        if(in_put_data.at(10)== 0)
        {
            ui->label_tem->setText(tr("温度 : -%1℃").arg(recv_temp));
        }
        else
        {
            ui->label_tem->setText(tr("温度 : %1℃").arg(recv_temp));
        }
        ui->label_hum->setText(tr("湿度 : %1%").arg(recv_hum));
        ui->label_voc->setText(tr("VOC : %1").arg(recv_voc));
        ui->label_pm25->setText(tr("PM2.5 : %1").arg(recv_pm25));

        if(g_tem_data.size() > 100)
        {
            g_tem_data.pop_back();
        }
        //g_tem_data.append(recv_temp);
        g_tem_data.insert(0,recv_temp);

        if(g_hum_data.size() > 100)
        {
            g_hum_data.pop_back();
        }
        g_hum_data.insert(0,recv_hum);

        if(g_pm25_data.size() > 100)
        {
            g_pm25_data.pop_back();
        }
        g_pm25_data.insert(0,recv_pm25);
        graph_replot();

    switch(recv_fan_speed)
    {
        case 0XFF:
        ui->label_fan_speed->setText("当前风速 : 停");
        ui->switch_res->setText("当前风速 : 停");
        break;
        case 0X01:
        ui->label_fan_speed->setText("当前风速 : 低");
        ui->switch_res->setText("当前风速 : 低");
        break;
        case 0X02:
        ui->label_fan_speed->setText("当前风速 : 中");
        ui->switch_res->setText("当前风速 : 中");
        break;
        case 0X03:
        ui->label_fan_speed->setText("当前风速 : 高");
        ui->switch_res->setText("当前风速 : 高");
        break;
        case 0X00:
        ui->label_fan_speed->setText("当前风速 : 自动");
        ui->switch_res->setText("当前风速 : 自动");
        break;
    }
    }

    if((in_put_data.length() == 34)&&(in_put_data.at(29) == 14))//优先级数据
    {
        tmp = in_put_data.at(10);
        if((tmp>1)&&(tmp<6))
        {
            ui->comboBox_priority_1->setCurrentIndex(tmp-1);
        }
        tmp = in_put_data.at(11);
        if((tmp>1)&&(tmp<6))
        {
            ui->comboBox_priority_2->setCurrentIndex(tmp-1);
        }
        tmp = in_put_data.at(12);
        if((tmp>1)&&(tmp<6))
        {
            ui->comboBox_priority_3->setCurrentIndex(tmp-1);
        }
        tmp = in_put_data.at(13);
        if((tmp>1)&&(tmp<6))
        {
            ui->comboBox_priority_4->setCurrentIndex(tmp-1);
        }
        tmp = in_put_data.at(14);
        if((tmp>1)&&(tmp<6))
        {
            ui->comboBox_priority_5->setCurrentIndex(tmp-1);
        }
        tmp = in_put_data.at(15);
        if((tmp>1)&&(tmp<6))
        {
            ui->comboBox_priority_6->setCurrentIndex(tmp-1);
        }
        tmp = in_put_data.at(16);
        if((tmp>1)&&(tmp<6))
        {
            ui->comboBox_priority_7->setCurrentIndex(tmp-1);
        }
        tmp = in_put_data.at(17);
        if((tmp>1)&&(tmp<6))
        {
            ui->comboBox_priority_8->setCurrentIndex(tmp-1);
        }
        ui->button_set_priority_1->setEnabled(FALSE);
        ui->button_set_priority_2->setEnabled(FALSE);
        ui->button_set_priority_3->setEnabled(FALSE);
        ui->button_set_priority_4->setEnabled(FALSE);
        ui->button_set_priority_5->setEnabled(FALSE);
        ui->button_set_priority_6->setEnabled(FALSE);
        ui->button_set_priority_7->setEnabled(FALSE);
        ui->button_set_priority_8->setEnabled(FALSE);
    }

    if((in_put_data.length() == 34)&&(in_put_data.at(29)== 6))//机器上报滤网寿命
    {
        unsigned int life_paper = 0,life_ac = 0;
        life_paper |= in_put_data.at(13)&0X000000FF;
        life_paper |= (in_put_data.at(12)<<8)&0X0000FF00;
        life_paper |= (in_put_data.at(11)<<16)&0X00FF0000;
        life_paper |= (in_put_data.at(10)<<24)&0XFF000000;

        life_ac |= in_put_data.at(17)&0X000000FF;
        life_ac |= (in_put_data.at(16)<<8)&0X0000FF00;
        life_ac |= (in_put_data.at(15)<<16)&0X00FF0000;
        life_ac |= (in_put_data.at(14)<<24)&0XFF000000;

        ui->label_paper_life->setText(tr("纸质滤网:%1min").arg(life_paper));
        ui->label_c_life->setText(tr("活性炭滤网:%1min").arg(life_ac));

    }

    if((in_put_data.length() == 33)&&(in_put_data.at(28)== 4)&&(in_put_data.at(29)== 14))//机器上报时间
        {
        QString data;
        data = myHelper::ByteArrayToHexStr(in_put_data);
        QString week_day;
        switch(in_put_data.at(13))
        {
        case 0:
        week_day = "星期日";
        break;
        case 1:
        week_day = "星期一";
        break;
        case 2:
        week_day = "星期二";
        break;
        case 3:
        week_day = "星期三";
        break;
        case 4:
        week_day = "星期四";
        break;
        case 5:
        week_day = "星期五";
        break;
        case 6:
        week_day = "星期六";
        break;        
        }
        int year,mounth,day,hour,minue,second;
        //year = data.split(" ")[10].toInt() +2000;
       // mounth = data.split(" ")[11].toInt();
        //day = data.split(" ")[12].toInt();
        //hour = data.split(" ")[14].toInt();
        //minue = data.split(" ")[15].toInt();
        //second = data.split(" ")[16].toInt();
        year = in_put_data.at(10) + 2000;
        mounth = in_put_data.at(11);
        day = in_put_data.at(12);
        hour = in_put_data.at(14);
        minue = in_put_data.at(15);
        second = in_put_data.at(16);
        QString meachine_time = tr("机器时间 %1 %2 %3 %4 %5 %6 %7").arg(year).arg(mounth).arg(day).arg(week_day).arg(hour).arg(minue).arg(second);
        ui->label_meachine_time->setText(meachine_time);
        }

    if((in_put_data.length() == 13)&&(in_put_data.at(8)== 4)&&(in_put_data.at(9)== -16))//接受传输iap数据帧的请求
    {
        int page_num = 0;
        page_num |= in_put_data.at(10)<<8 & 0X0000FF00;
        page_num |= in_put_data.at(11) & 0X000000FF;
        //page_num++;
        ui->progressBar_download->setVisible(true);
        ui->progressBar_download->setValue(page_num);
        transmission_iap_data_by_wifi(G_App_file,page_num);

    }

    if((in_put_data.length() == 17)&&(in_put_data.at(8)== 8)&&(in_put_data.at(9)== -15))//解析iap等命令
    {
        switch(in_put_data.at(10))
            {
                case 2://收到设备上传上来的设备当前使用的固件信息
                device_iap_need_file(in_put_data.at(11));
                break;

                case 5:
                ui->read_data->append(tr("升级完成"));
                myHelper::ShowMessageBoxInfo("升级完成");
                G_App_file->close();
                ui->progressBar_download->setVisible(false);


                default:
                break;
            }
        //myHelper::Sleep(10);
       /* char buff[1];
        buff[0] = in_put_data.at(10);
        transmission_data_by_wifi(0xF1,buff,1);
        if(buff[0]<100)
        {
            ui->progressBar_download->setVisible(true);
            ui->progressBar_download->setValue(buff[0]);
        }*/
    }
}

void dabai::ReadData()
{
    QByteArray buffer = tcpClient->readAll();
    if(!buffer.isEmpty())
    {
        if(ui->hex_rx->isChecked())
        {
            ui->rx_buf->append(tr("time:%1 recv:%2").arg(QTime::currentTime().toString("hh:mm:ss")).arg(myHelper::ByteArrayToHexStr(buffer)));
        }
        else
        {
            ui->rx_buf->append(tr("time:%1 recv:%2").arg(QTime::currentTime().toString("hh:mm:ss")).arg(QString(buffer)));
        }
    }
}

void dabai::ReadError(QAbstractSocket::SocketError)
{
    tcpClient->disconnectFromHost();
    ui->listen->setText("try again");
    ui->rx_buf->append(tr("error: %1").arg(tcpClient->errorString()));
}

void dabai::ClientReadData(int clientID, QString IP, int Port, QByteArray data)
{
    if(!data.isEmpty())
    {
        if(ui->hex_rx->isChecked())
        {
            QString target_device = ui->device->currentText();
            if(target_device.split(":")[0].toInt() == clientID)
            {
                deal_with_data(data);
            }
            ui->rx_buf->append(tr("recv: %1  from: ip:%2:%3").arg(QString(myHelper::ByteArrayToHexStr(data))).arg(IP).arg(Port));
        }
        else
        {
            ui->rx_buf->append(tr("recv: %1  from: ip:%2:%3").arg(QString(data)).arg(IP).arg(Port));
        }
    }
}

void dabai::ClientConnect(int clientID, QString IP, int Port)
{
    ui->rx_buf->append(tr("client: %1 ip:%2 port:%3 connected!").arg(clientID).arg(IP).arg(Port));
    ui->client->addItem(tr("%1:%2:%3").arg(clientID).arg(IP).arg(Port));
    ui->device->addItem(tr("%1:%2:%3").arg(clientID).arg(IP).arg(Port));
    ui->comboBox_device_stat->addItem(tr("%1:%2:%3").arg(clientID).arg(IP).arg(Port));
}

void dabai::ClientDisConnect(int clientID, QString IP, int Port)
{
    ui->rx_buf->append(tr("client: %1 ip:%2 port:%3 disconnected!").arg(clientID).arg(IP).arg(Port));
    ui->client->removeItem(ui->client->findText(tr("%1:%2:%3").arg(clientID).arg(IP).arg(Port)));
    ui->device->removeItem(ui->device->findText(tr("%1:%2:%3").arg(clientID).arg(IP).arg(Port)));
    ui->comboBox_device_stat->removeItem(ui->comboBox_device_stat->findText(tr("%1:%2:%3").arg(clientID).arg(IP).arg(Port)));
}



void dabai::on_listen_clicked()
{
    if(ui->listen->text()=="监听")
    {
        bool ok=tcpServer->listen(QHostAddress::Any,ui->port_set->text().toInt());
        if(ok)
        {
            ui->listen->setText("停止");
            ui->rx_buf->append(tr("监听成功"));
        }
    }
    else
    {
        tcpServer->CloseAllClient();
        tcpServer->close();
        ui->listen->setText("监听");
        ui->rx_buf->append(tr("停止监听成功"));
    }
}

void dabai::on_clear_rx_buf_clicked()
{
    ui->rx_buf->clear();
}

void dabai::on_clear_tx_buf_clicked()
{
    ui->tx_buf->clear();
}

void dabai::on_send_clicked()
{
    QString txt = ui->tx_buf->text();
    if(txt == ""){return;}

    QString target = ui->client->currentText();
    int clientID = target.split(":")[0].toInt();
    QByteArray send_data;
    if(ui->hex_tx->isChecked())
    {
        send_data = myHelper::HexStrToByteArray(txt);
        tcpServer->SendData(clientID,send_data);
    }
    else
    {
        send_data = txt.toAscii();
        tcpServer->SendData(clientID,send_data);
    }


}


void dabai::on_connect_num_currentIndexChanged(const QString &arg1)
{
    tcpServer->setMaxPendingConnections(arg1.toInt());
}

void dabai::on_send_all_clicked()
{
    QString txt = ui->tx_buf->text();
    if(txt == ""){return;}

    QByteArray hex_data;
    QByteArray ascii_data;
    hex_data = myHelper::HexStrToByteArray(txt);
    ascii_data = txt.toAscii();

    for(int loop=0;loop<ui->client->count();loop++)
    {
        QString target = ui->client->itemText(loop);
        int clientID = target.split(":")[0].toInt();
        if(ui->hex_tx->isChecked())
        {
            tcpServer->SendData(clientID,hex_data);
        }
        else
        {
            tcpServer->SendData(clientID,ascii_data);
        }
    }
}

void dabai::get_meachine_state(int index)
{
    QByteArray send_data;
    QString target = ui->device->itemText(index);
    int clientID = target.split(":")[0].toInt();
    send_data.resize(19);
    send_data[0] = 0XAA;
    send_data[1] = 0X55;
    send_data[2] = 0X11;
    send_data[3] = 0XFF;
    send_data[4] = 0XFF;
    send_data[5] = 0XFF;
    send_data[6] = 0XFF;
    //send_data[3] = 0X01;
    //send_data[4] = 0X9f;
    //send_data[5] = 0X0b;
    //send_data[6] = 0XF5;
    send_data[7] = 0X01;
    send_data[8] = 0X0A;
    send_data[9] = 0XE2;
    send_data[10] = 0X11;
    send_data[11] = 0X11;
    send_data[12] = 0X11;
    send_data[13] = 0X11;
    send_data[14] = 0X11;
    send_data[15] = 0X11;
    send_data[16] = 0X11;
    send_data[17] = 0X11;
    send_data[18] = 0X11;
    tcpServer->SendData(clientID,send_data);
}
void dabai::set_fan_speed(unsigned char speed)
{
    if(ui->label_tcm300ID->text()== "ID:")
    {
        myHelper::ShowMessageBoxErrorX("未选择设备!或者设备不在线!请检查或者点击刷新后重试!");
        return;
    }
    QByteArray send_data;
    QByteArray ID;
    ID.resize(4);
    QString target = ui->device->currentText();
    int clientID = target.split(":")[0].toInt();
    QString TCM300_id = (ui->label_tcm300ID->text()).split(":")[1];
    ID = myHelper::HexStrToByteArray(TCM300_id);
    send_data.resize(12);
    send_data[0] = 0XAA;
    send_data[1] = 0X55;
    send_data[2] = 0X0A;
    send_data[3] = ID.at(0);
    send_data[4] = ID.at(1);
    send_data[5] = ID.at(2);
    send_data[6] = ID.at(3);
    send_data[7] = 0X01;
    send_data[8] = 0X03;
    send_data[9] = 0X10;
    send_data[10] = speed;
    //send_data[11] = 0X11;
    mycrc.SetCRC8Sub(send_data.data(),11);
    tcpServer->SendData(clientID,send_data);
}

void dabai::on_device_currentIndexChanged(int index)
{

    ui->label_tcm300ID->setText("ID:");
    ui->label_hum->setText("湿度 :");
    ui->label_tem->setText("温度 :");
    ui->label_voc->setText("VOC :");
    ui->label_pm25->setText("PM2.5 :");
    ui->label_fan_speed->setText("当前风速 :");
    ui->switch_res->setText("当前风速 :");
    ui->device_state->setText("状态:无设备");
    ui->comboBox_device_stat->setCurrentIndex(index);
    ui->client->setCurrentIndex(index);
    ui->comboBox_priority_1->setCurrentIndex(0);
    ui->comboBox_priority_2->setCurrentIndex(0);
    ui->comboBox_priority_3->setCurrentIndex(0);
    ui->comboBox_priority_4->setCurrentIndex(0);
    ui->comboBox_priority_5->setCurrentIndex(0);
    ui->comboBox_priority_6->setCurrentIndex(0);
    ui->comboBox_priority_7->setCurrentIndex(0);
    ui->comboBox_priority_8->setCurrentIndex(0);
    get_meachine_state(index);
}

void dabai::on_state_refresh_clicked()
{
    ui->label_tcm300ID->setText("ID:");
    ui->label_hum->setText("湿度 :");
    ui->label_tem->setText("温度 :");
    ui->label_voc->setText("VOC :");
    ui->label_pm25->setText("PM2.5 :");
    ui->label_fan_speed->setText("当前风速 :");
    ui->switch_res->setText("当前风速 :");
    ui->device_state->setText("状态:无设备");
    int target = ui->device->currentIndex();
    get_meachine_state(target);
}

void dabai::on_switch_power_clicked()
{
    set_fan_speed(0XFF);
    //SleeperThread::msleep(1000);
    //int target = ui->device->currentIndex();
    //get_meachine_state(target);
}

void dabai::on_switch_low_clicked()
{
    set_fan_speed(0X01);
    //SleeperThread::msleep(1000);
    //int target = ui->device->currentIndex();
    //get_meachine_state(target);
}

void dabai::on_switch_mid_clicked()
{
    set_fan_speed(0X02);
    //SleeperThread::msleep(1000);
    //int target = ui->device->currentIndex();
    //get_meachine_state(target);
}

void dabai::on_switch_high_clicked()
{
    set_fan_speed(0X03);
    //SleeperThread::msleep(1000);
    //int target = ui->device->currentIndex();
    //get_meachine_state(target);
}

void dabai::on_switch_auto_clicked()
{
    set_fan_speed(0X00);
    //SleeperThread::msleep(1000);
    //int target = ui->device->currentIndex();
    //get_meachine_state(target);
}



void dabai::on_comboBox_operat_currentIndexChanged(int index)
{
    switch(index)
    {
    case 0://学习

        break;

    case 1://清除

        break;

    case 2://全清
        break;

    }
}

void dabai::on_comboBox_device_type_currentIndexChanged(int index)
{
    switch(index)
    {
    case 0://开关
        ui->label_parameter_1->setText("开关优先时间(min):");
        ui->label_parameter_2->setText("参数无效");
        ui->label_parameter_3->setText("参数无效");
        ui->label_parameter_4->setText("参数无效");
        ui->parameter_1->setMaxLength(5);
        ui->parameter_2->setEnabled(FALSE);
        ui->comboBox_parameter_3->setEnabled(FALSE);
        ui->comboBox_parameter_4->setEnabled(FALSE);
        break;

    case 1://人体
        ui->label_parameter_1->setText("有人时风速保持时间(min)");
        ui->label_parameter_2->setText("无人时风速保持时间(min)");
        ui->label_parameter_3->setText("有人时风速大小");
        ui->label_parameter_4->setText("无人时风速大小");
        ui->parameter_2->setEnabled(TRUE);
        ui->comboBox_parameter_3->setEnabled(TRUE);
        ui->comboBox_parameter_4->setEnabled(TRUE);
        ui->parameter_1->setMaxLength(3);
        ui->parameter_2->setMaxLength(3);
        break;

    case 2://窗磁
        ui->label_parameter_1->setText("关窗时风速保持时间(min)");
        ui->label_parameter_2->setText("开窗时风速保持时间(min)");
        ui->label_parameter_3->setText("关窗时风速大小");
        ui->label_parameter_4->setText("开窗时风速大小");
        ui->parameter_2->setEnabled(TRUE);
        ui->comboBox_parameter_3->setEnabled(TRUE);
        ui->comboBox_parameter_4->setEnabled(TRUE);
        ui->parameter_1->setMaxLength(3);
        ui->parameter_2->setMaxLength(3);
        break;

    default:
        break;
    }
}

void dabai::switch_operat(unsigned char operat_value)
{
    QString TCM300_id;
    QByteArray send_data;
    send_data.resize(19);
    QByteArray ID;
    QByteArray send_time;
    QByteArray device_ID;
    ID.resize(4);
    send_time.resize(2);
    device_ID.resize(4);

    QString target = ui->device->currentText();
    int clientID = target.split(":")[0].toInt();
    TCM300_id = ui->label_tcm300ID->text();
    if(TCM300_id == "ID:")
    {
        TCM300_id="00000000";
    }
    else
    {
        TCM300_id = (ui->label_tcm300ID->text()).split(":")[1];
    }
    QString device_id_value = ui->parameter_id->text();
    if(device_id_value.length()<8)
    {
        myHelper::ShowMessageBoxErrorX("设备ID 错误!");
        return ;
    }

    ID = myHelper::HexStrToByteArray(TCM300_id);
    device_ID = myHelper::HexStrToByteArray(device_id_value);

    int time_value = ui->parameter_1->text().toInt();
    if(time_value ==0)
    {
        myHelper::ShowMessageBoxErrorX("开关优先时间不能为0,已设置为默认时间20分钟!");
        ui->parameter_1->setText("20");
        time_value = 20;
    }
    if(time_value >65535)
    {
        myHelper::ShowMessageBoxErrorX("开关优先时间超过范围,已设置为最大时间65535分钟!");
        ui->parameter_1->setText("65535");
        time_value = 65535;
    }
    send_time[1] = time_value&0X000000FF;
    send_time[0] = (time_value >> 8)&0X000000FF;
    /*send_time[1] = time_value%254;
    if(time_value >=255)
    {
        send_time[0] = time_value/254;
        if(time_value > 65535)
        {
            send_time[0] = 0XFF;
            send_time[1] = 0XFF;
        }
    }
    else
    {
        send_time[0] = 0;
    }*/

    send_data[0] = 0XAA;
    send_data[1] = 0X55;
    send_data[2] = 0X11;
    send_data[3] = ID.at(0);
    send_data[4] = ID.at(1);
    send_data[5] = ID.at(2);
    send_data[6] = ID.at(3);
    send_data[7] = 0X01;
    send_data[8] = 0X0A;
    send_data[9] = operat_value;
    send_data[10] = device_ID[0];
    send_data[11] = device_ID[1];
    send_data[12] = device_ID[2];
    send_data[13] = device_ID[3];
    send_data[14] = send_time[0];
    send_data[15] = send_time[1];
    send_data[16] = 0XFF;
    send_data[17] = 0XFF;
    //send_data[18] = 0X11;
    mycrc.SetCRC8Sub(send_data.data(),18);
    tcpServer->SendData(clientID,send_data);
}

void dabai::sensor_operat(unsigned char operat_value)
{
    QString TCM300_id;
    QByteArray send_data;
    send_data.resize(19);
    QByteArray ID;
    QByteArray send_parameter;
    QByteArray device_ID;
    ID.resize(4);
    send_parameter.resize(4);
    device_ID.resize(4);

    QString target = ui->device->currentText();
    int clientID = target.split(":")[0].toInt();
    TCM300_id = ui->label_tcm300ID->text();
    if(TCM300_id == "ID:")
    {
        TCM300_id="00000000";
    }
    else
    {
        TCM300_id = (ui->label_tcm300ID->text()).split(":")[1];
    }
    QString device_id_value = ui->parameter_id->text();
    if(device_id_value.length()<8)
    {
        myHelper::ShowMessageBoxErrorX("设备ID 错误!");
        return ;
    }

    ID = myHelper::HexStrToByteArray(TCM300_id);
    device_ID = myHelper::HexStrToByteArray(device_id_value);

    send_parameter[0]= ui->parameter_1->text().toShort();
    if(ui->parameter_1->text().toShort()>255)
    {
        myHelper::ShowMessageBoxErrorX("延时时间最大为255分钟,超过范围!已经设置为255");
        ui->parameter_1->setText("255");
        send_parameter[0] = 255;
    }
    if(ui->parameter_1->text().toShort()== 0)
    {
        myHelper::ShowMessageBoxErrorX("延时时间不能为0,已经设置为20");
        ui->parameter_1->setText("20");
        send_parameter[0] = 20;
    }

    send_parameter[1]= ui->parameter_2->text().toShort();
    if(ui->parameter_2->text().toShort()>255)
    {
        myHelper::ShowMessageBoxErrorX("延时时间最大为255分钟,超过范围!已经设置为255");
        ui->parameter_2->setText("255");
        send_parameter[1] = 255;
    }
    if(ui->parameter_2->text().toShort()== 0)
    {
        myHelper::ShowMessageBoxErrorX("延时时间不能为0,已经设置为20");
        ui->parameter_2->setText("20");
        send_parameter[1] = 20;
    }
    switch(ui->comboBox_parameter_3->currentIndex())
    {
        case 0://高
        send_parameter[2] = 0X03;
        break;
        case 1://中
        send_parameter[2] = 0X02;
        break;
        case 2://低
        send_parameter[2] = 0X01;
        break;
        case 3://自动
        send_parameter[2] = 0X00;
        break;
        case 4://关
        send_parameter[2] = 0XFF;
        break;
        default:
        break;
    }
    switch(ui->comboBox_parameter_4->currentIndex())
    {
        case 0://高
        send_parameter[3] = 0X03;
        break;
        case 1://中
        send_parameter[3] = 0X02;
        break;
        case 2://低
        send_parameter[3] = 0X01;
        break;
        case 3://自动
        send_parameter[3] = 0X00;
        break;
        case 4://关
        send_parameter[3] = 0XFF;
        break;
        default:
        break;
    }

    send_data[0] = 0XAA;
    send_data[1] = 0X55;
    send_data[2] = 0X11;
    send_data[3] = ID.at(0);
    send_data[4] = ID.at(1);
    send_data[5] = ID.at(2);
    send_data[6] = ID.at(3);
    send_data[7] = 0X01;
    send_data[8] = 0X0A;
    send_data[9] = operat_value;
    send_data[10] = device_ID[0];
    send_data[11] = device_ID[1];
    send_data[12] = device_ID[2];
    send_data[13] = device_ID[3];
    send_data[14] = send_parameter[0];
    send_data[15] = send_parameter[1];
    send_data[16] = send_parameter[2];
    send_data[17] = send_parameter[3];
    send_data[18] = 0X11;
    tcpServer->SendData(clientID,send_data);
}


void dabai::switch_learn()
{
    switch_operat(0XB0);
}

void dabai::switch_clear()
{
    switch_operat(0XC0);
}

void dabai::switch_all_clear()
{
    switch_operat(0XD0);
}

void dabai::windows_sensor_learn()
{
    sensor_operat(0XB1);
}
void dabai::windows_sensor_clear()
{
    sensor_operat(0XC1);
}
void dabai::windows_sensor_all_clear()
{
    sensor_operat(0XD1);
}

void dabai::body_sensor_learn()
{
    sensor_operat(0XB2);
}
void dabai::body_sensor_clear()
{
    sensor_operat(0XC2);
}
void dabai::body_sensor_all_clear()
{
    sensor_operat(0XD2);
}

void dabai::on_button_execute_clicked()
{
   /* if((ui->device_state->text() == "状态:")||(ui->device_state->text() == "状态:无设备"))
    {
        myHelper::ShowMessageBoxErrorX("设备不在线!请检查或者点击'刷新'后重试");
        return;
    }*/
    switch(ui->comboBox_device_type->currentIndex())
    {
    case 0://开关
        switch(ui->comboBox_operat->currentIndex())
        {
            case 0://学习
            switch_learn();
            break;
            case 1://清除
            switch_clear();
            break;
            case 2://全清
            switch_all_clear();
            break;
        default:
            break;
        }
        break;

    case 1://人体
        switch(ui->comboBox_operat->currentIndex())
        {
            case 0://学习
            body_sensor_learn();
            break;
            case 1://清除
            body_sensor_clear();
            break;
            case 2://全清
            body_sensor_all_clear();
            break;
        default:
            break;
        }
        break;
        break;

    case 2://窗磁
        switch(ui->comboBox_operat->currentIndex())
        {
            case 0://学习
            windows_sensor_learn();
            break;
            case 1://清除
            windows_sensor_clear();
            break;
            case 2://全清
            windows_sensor_all_clear();
            break;
        default:
            break;
        }
        break;
        break;

    default:
        break;
    }
}

void dabai::on_button_refresh_clicked()
{
    QByteArray send_data;
    send_data.resize(14);
    QByteArray ID;
    QByteArray send_time;
    ID.resize(4);
    send_time.resize(2);

    QString target = ui->device->currentText();
    int clientID = target.split(":")[0].toInt();
    QString TCM300_id = (ui->label_tcm300ID->text()).split(":")[1];

    ID = myHelper::HexStrToByteArray(TCM300_id);

    int time_value = ui->data_refresh_rate->text().toInt();

    if(time_value ==0)
    {
        myHelper::ShowMessageBoxErrorX("开关优先时间不能为0,已设置为默认时间20秒!");
        ui->data_refresh_rate->setText("20");
        time_value = 20;
    }
    if(time_value >65535)
    {
        myHelper::ShowMessageBoxErrorX("开关优先时间超过范围,已设置为最大时间65535秒!");
        ui->data_refresh_rate->setText("65535");
        time_value = 65535;
    }
    g_time_value = time_value;
    graph_set_key_range(g_time_value);
    send_time[1] = time_value&0X000000FF;
    send_time[0] = (time_value >> 8)&0X000000FF;
    /*send_time[1] = time_value%255;
    if(time_value >=255)
    {
        send_time[0] = time_value/255;
        if(time_value > 65535)
        {
            send_time[0] = 0XFF;
            send_time[1] = 0XFF;
        }
    }
    else
    {
        send_time[0] = 0;
    }*/

    send_data[0] = 0XAA;
    send_data[1] = 0X55;
    send_data[2] = 0X0C;
    send_data[3] = ID.at(0);
    send_data[4] = ID.at(1);
    send_data[5] = ID.at(2);
    send_data[6] = ID.at(3);
    send_data[7] = 0X01;
    send_data[8] = 0X05;
    send_data[9] = 0XEE;
    send_data[10] = 0X0F;
    send_data[11] = send_time[0];
    send_data[12] = send_time[1];
    //send_data[13] = 11;
    mycrc.SetCRC8Sub(send_data.data(),13);
    tcpServer->SendData(clientID,send_data);
}

void dabai::on_comboBox_device_stat_currentIndexChanged(int index)
{
    ui->label_tcm300ID->setText("ID:");
    ui->label_hum->setText("湿度 :");
    ui->label_tem->setText("温度 :");
    ui->label_voc->setText("VOC :");
    ui->label_pm25->setText("PM2.5 :");
    ui->label_fan_speed->setText("当前风速 :");
    ui->switch_res->setText("当前风速 :");
    ui->device_state->setText("状态:无设备");
    ui->device->setCurrentIndex(index);
    ui->client->setCurrentIndex(index);
    //get_meachine_state(index);
}

void dabai::on_state_refresh_2_clicked()
{
    ui->label_tcm300ID->setText("ID:");
    ui->label_hum->setText("湿度 :");
    ui->label_tem->setText("温度 :");
    ui->label_voc->setText("VOC :");
    ui->label_pm25->setText("PM2.5 :");
    ui->label_fan_speed->setText("当前风速 :");
    ui->switch_res->setText("当前风速 :");
    ui->device_state->setText("状态:无设备");
    int target = ui->comboBox_device_stat->currentIndex();
    get_meachine_state(target);
}

void dabai::on_client_currentIndexChanged(int index)
{
    ui->comboBox_device_stat->setCurrentIndex(index);
    ui->device->setCurrentIndex(index);
}

void dabai::get_priority()
{
    QByteArray send_data;
    send_data.resize(12);
    QByteArray ID;
    ID.resize(4);
    QString target = ui->device->currentText();
    int clientID = target.split(":")[0].toInt();
    QString TCM300_id = (ui->label_tcm300ID->text()).split(":")[1];

    ID = myHelper::HexStrToByteArray(TCM300_id);
    send_data[0] = 0XAA;
    send_data[1] = 0X55;
    send_data[2] = 0X0A;
    send_data[3] = ID.at(0);
    send_data[4] = ID.at(1);
    send_data[5] = ID.at(2);
    send_data[6] = ID.at(3);
    send_data[7] = 0X01;
    send_data[8] = 0X03;
    send_data[9] = 0XEE;
    send_data[10] = 0X0e;
    //send_data[11] = 11;
    mycrc.SetCRC8Sub(send_data.data(),11);
    tcpServer->SendData(clientID,send_data);
}

void dabai::set_priority(char priority_type, char priority_num)
{
    QByteArray send_data;
    send_data.resize(14);
    QByteArray ID;
    ID.resize(4);

    int tmp;

    QString target = ui->device->currentText();
    int clientID = target.split(":")[0].toInt();
    QString TCM300_id = (ui->label_tcm300ID->text()).split(":")[1];

    ID = myHelper::HexStrToByteArray(TCM300_id);
    send_data[0] = 0XAA;
    send_data[1] = 0X55;
    send_data[2] = 0X0C;
    send_data[3] = ID.at(0);
    send_data[4] = ID.at(1);
    send_data[5] = ID.at(2);
    send_data[6] = ID.at(3);
    send_data[7] = 0X01;
    send_data[8] = 0X05;
    send_data[9] = 0XEE;
    send_data[10] = 0X0D;
    send_data[11] = priority_type;
    send_data[12] = priority_num;
    send_data[13] = 11;
    tcpServer->SendData(clientID,send_data);
    /*tmp=ui->comboBox_priority_1->currentIndex();
    //if(tmp>0)
    {
    send_data[11] = tmp+1;
    send_data[12] = 0;
    tcpServer->SendData(clientID,send_data);
    SleeperThread::msleep(500);
    }

    tmp=ui->comboBox_priority_2->currentIndex();
    //if(tmp>0)
    {
    send_data[11] = tmp+1;
    send_data[12] = 1;
    tcpServer->SendData(clientID,send_data);
    SleeperThread::msleep(500);
    }

    tmp=ui->comboBox_priority_3->currentIndex();
    //if(tmp>0)
    {
    send_data[11] = tmp+1;
    send_data[12] = 2;
    tcpServer->SendData(clientID,send_data);
    SleeperThread::msleep(500);
    }

    tmp=ui->comboBox_priority_4->currentIndex();
    //if(tmp>0)
    {
    send_data[11] = tmp+1;
    send_data[12] = 3;
    tcpServer->SendData(clientID,send_data);
    SleeperThread::msleep(500);
    }

    tmp=ui->comboBox_priority_5->currentIndex();
    //if(tmp>0)
    {
    send_data[11] = tmp+1;
    send_data[12] = 4;
    tcpServer->SendData(clientID,send_data);
    SleeperThread::msleep(500);
    }

    tmp=ui->comboBox_priority_6->currentIndex();
    //if(tmp>0)
    {
    send_data[11] = tmp+1;
    send_data[12] = 5;
    tcpServer->SendData(clientID,send_data);
    SleeperThread::msleep(500);
    }

    tmp=ui->comboBox_priority_7->currentIndex();
    //if(tmp>0)
    {
    send_data[11] = tmp+1;
    send_data[12] = 6;
    tcpServer->SendData(clientID,send_data);
    SleeperThread::msleep(500);
    }

    tmp=ui->comboBox_priority_8->currentIndex();
    //if(tmp>0)
    {
    send_data[11] = tmp+1;
    send_data[12] = 7;
    tcpServer->SendData(clientID,send_data);
    SleeperThread::msleep(500);
    }*/
}
void dabai::on_button_get_priority_clicked()
{
    if((ui->device_state->text() == "状态:")||(ui->device_state->text() == "状态:无设备"))
    {
        myHelper::ShowMessageBoxErrorX("设备不在线!请检查或者点击'刷新'后重试");
        return;
    }
    ui->comboBox_priority_1->setCurrentIndex(0);
    ui->comboBox_priority_2->setCurrentIndex(0);
    ui->comboBox_priority_3->setCurrentIndex(0);
    ui->comboBox_priority_4->setCurrentIndex(0);
    ui->comboBox_priority_5->setCurrentIndex(0);
    ui->comboBox_priority_6->setCurrentIndex(0);
    ui->comboBox_priority_7->setCurrentIndex(0);
    ui->comboBox_priority_8->setCurrentIndex(0);

    get_priority();
}


void dabai::on_button_set_priority_1_clicked()
{
    char tmp = ui->comboBox_priority_1->currentIndex();
    if(tmp)
    {tmp++;}
    set_priority(tmp,0);
}

void dabai::on_button_set_priority_2_clicked()
{
    char tmp = ui->comboBox_priority_2->currentIndex();
    if(tmp)
    {tmp++;}
    set_priority(tmp,1);
}

void dabai::on_button_set_priority_3_clicked()
{
    char tmp = ui->comboBox_priority_3->currentIndex();
    if(tmp)
    {tmp++;}
    set_priority(tmp,2);
}

void dabai::on_button_set_priority_4_clicked()
{
    char tmp = ui->comboBox_priority_4->currentIndex();
    if(tmp)
    {tmp++;}
    set_priority(tmp,3);
}

void dabai::on_button_set_priority_5_clicked()
{
    char tmp = ui->comboBox_priority_5->currentIndex();
    if(tmp)
    {tmp++;}
    set_priority(tmp,4);
}

void dabai::on_button_set_priority_6_clicked()
{
    char tmp = ui->comboBox_priority_6->currentIndex();
    if(tmp)
    {tmp++;}
    set_priority(tmp,5);
}

void dabai::on_button_set_priority_7_clicked()
{
    char tmp = ui->comboBox_priority_7->currentIndex();
    if(tmp)
    {tmp++;}
    set_priority(tmp,6);
}

void dabai::on_button_set_priority_8_clicked()
{
    char tmp = ui->comboBox_priority_8->currentIndex();
    if(tmp)
    {tmp++;}
    set_priority(tmp,7);
}

void dabai::on_comboBox_priority_1_currentIndexChanged(int index)
{
    index++;
    ui->button_set_priority_1->setEnabled(TRUE);
}

void dabai::on_comboBox_priority_2_currentIndexChanged(int index)
{
    index++;
    ui->button_set_priority_2->setEnabled(TRUE);
}

void dabai::on_comboBox_priority_3_currentIndexChanged(int index)
{
    index++;
    ui->button_set_priority_3->setEnabled(TRUE);
}

void dabai::on_comboBox_priority_4_currentIndexChanged(int index)
{
    index++;
    ui->button_set_priority_4->setEnabled(TRUE);
}

void dabai::on_comboBox_priority_5_currentIndexChanged(int index)
{
    index++;
    ui->button_set_priority_5->setEnabled(TRUE);
}

void dabai::on_comboBox_priority_6_currentIndexChanged(int index)
{
    index++;
    ui->button_set_priority_6->setEnabled(TRUE);
}

void dabai::on_comboBox_priority_7_currentIndexChanged(int index)
{
    index++;
    ui->button_set_priority_7->setEnabled(TRUE);
}

void dabai::on_comboBox_priority_8_currentIndexChanged(int index)
{
    index++;
    ui->button_set_priority_8->setEnabled(TRUE);
}


void dabai::get_life()
{
    if(ui->label_tcm300ID->text()== "ID:")
    {
        myHelper::ShowMessageBoxErrorX("未选择设备!或者设备不在线!请检查或者点击刷新后重试!");
        return;
    }
    QByteArray send_data;
    QByteArray ID;
    ID.resize(4);
    QString target = ui->device->currentText();
    int clientID = target.split(":")[0].toInt();
    QString TCM300_id = (ui->label_tcm300ID->text()).split(":")[1];
    ID = myHelper::HexStrToByteArray(TCM300_id);
    send_data.resize(12);
    send_data[0] = 0XAA;
    send_data[1] = 0X55;
    send_data[2] = 0X0A;
    send_data[3] = ID.at(0);
    send_data[4] = ID.at(1);
    send_data[5] = ID.at(2);
    send_data[6] = ID.at(3);
    send_data[7] = 0X01;
    send_data[8] = 0X03;
    send_data[9] = 0XEE;
    send_data[10] = 0X06;
    //send_data[11] = 0X11;
    mycrc.SetCRC8Sub(send_data.data(),11);
    tcpServer->SendData(clientID,send_data);
}

void dabai::on_button_life_clicked()
{
    get_life();
}

void dabai::timer_updata()
{
   ui->label_time->setText(tr("当前时间 %1").arg(QDateTime::currentDateTime().toString("yyyy MM dd dddd hh mm ss")));
   //ui->label_meachine_time->setText(tr("当前时间 %1").arg(QDateTime::currentDateTime().toString("yyyy MM dd dddd hh mm ss")));
}

void dabai::set_meachine_time()
{
    QString TCM300_id;
    QByteArray send_data;
    send_data.resize(19);
    QByteArray ID;
    QByteArray send_time;
    ID.resize(4);
    send_time.resize(7);

    QString target = ui->device->currentText();
    int clientID = target.split(":")[0].toInt();
    TCM300_id = ui->label_tcm300ID->text();
    if(TCM300_id == "ID:")
    {
        TCM300_id="00000000";
    }
    else
    {
        TCM300_id = (ui->label_tcm300ID->text()).split(":")[1];
    }
    ID = myHelper::HexStrToByteArray(TCM300_id);

    QString string_time = ui->label_time->text();
    send_time[0] = string_time.split(" ")[1].toInt() - 2000;
    send_time[1] = string_time.split(" ")[2].toInt();
    send_time[2] = string_time.split(" ")[3].toInt();
    if(string_time.split(" ")[4] == "星期一")
        {
            send_time[3] = 1;
        }
    if(string_time.split(" ")[4] == "星期二")
        {
            send_time[3] = 2;
        }
    if(string_time.split(" ")[4] == "星期三")
        {
            send_time[3] = 3;
        }
    if(string_time.split(" ")[4] == "星期四")
        {
            send_time[3] = 4;
        }
    if(string_time.split(" ")[4] == "星期五")
        {
            send_time[3] = 5;
        }
    if(string_time.split(" ")[4] == "星期六")
        {
            send_time[3] = 6;
        }
    if(string_time.split(" ")[4] == "星期日")
        {
            send_time[3] = 0;
        }
    send_time[4] = string_time.split(" ")[5].toInt();
    send_time[5] = string_time.split(" ")[6].toInt();
    send_time[6] = string_time.split(" ")[7].toInt();

    send_data[0] = 0XAA;
    send_data[1] = 0X55;
    send_data[2] = 0X11;
    send_data[3] = ID.at(0);
    send_data[4] = ID.at(1);
    send_data[5] = ID.at(2);
    send_data[6] = ID.at(3);
    send_data[7] = 0X01;
    send_data[8] = 0X0A;
    send_data[9] = 0XE0;
    send_data[10] = send_time[0];
    send_data[11] = send_time[1];
    send_data[12] = send_time[2];
    send_data[13] = send_time[3];
    send_data[14] = send_time[4];
    send_data[15] = send_time[5];
    send_data[16] = send_time[6];
    send_data[17] = 0XFF;
    //send_data[18] = 0X11;
    mycrc.SetCRC8Sub(send_data.data(),18);
    tcpServer->SendData(clientID,send_data);
}

void dabai::get_meachine_time()
{
    if(ui->label_tcm300ID->text()== "ID:")
    {
        myHelper::ShowMessageBoxErrorX("未选择设备!或者设备不在线!请检查或者点击刷新后重试!");
        return;
    }
    QByteArray send_data;
    QByteArray ID;
    ID.resize(4);
    QString target = ui->device->currentText();
    int clientID = target.split(":")[0].toInt();
    QString TCM300_id = (ui->label_tcm300ID->text()).split(":")[1];
    ID = myHelper::HexStrToByteArray(TCM300_id);
    send_data.resize(13);
    send_data[0] = 0XAA;
    send_data[1] = 0X55;
    send_data[2] = 0X0B;
    send_data[3] = ID.at(0);
    send_data[4] = ID.at(1);
    send_data[5] = ID.at(2);
    send_data[6] = ID.at(3);
    send_data[7] = 0X01;
    send_data[8] = 0X04;
    send_data[9] = 0XEE;
    send_data[10] = 0X04;
    send_data[11] = 0X0E;
    //send_data[12] = 0X11;
    mycrc.SetCRC8Sub(send_data.data(),12);
    tcpServer->SendData(clientID,send_data);
}
void dabai::on_pushButton_clicked()
{
    get_meachine_time();
}

void dabai::on_pushButton_2_clicked()
{
    set_meachine_time();
}


void dabai::read_bin_file()
{
    /*char buff[16];
    ui->progressBar_download->setVisible(true);
    for(int i=0;i<5000;i++)
       {
           ui->progressBar_download->setValue(i);
       }
    QFile file("./iap.bin");
    //qDebug()<<"current applicationDirPath: "<<QCoreApplication::applicationDirPath();
    //qDebug()<<"current currentPath: "<<QDir::currentPath();
    if(!file.open(QIODevice::ReadOnly))
    {
        ui->read_data->append(tr("open file error!"));
    }
    while(!file.atEnd())
    {
        qint64 length;
        length = file.read(buff,16);
        //QByteArray line = file.readLine();
        QByteArray str(buff,length);
        ui->read_data->append(tr("%1").arg(myHelper::ByteArrayToHexStr(str)));
    }*/
}

int dabai::read_iap_page_data_by_wifi(QFile* &app, int num,char* buff ,int &read_num)
{
    int offset;
    if(num > 303)
    {
        offset = num *128;
    }
    offset = num *128;
    if(offset > app->size())
    {
        return 0;
    }
    app->seek(offset);
    read_num = app->read(buff,128);
    return 1;
}

void dabai::transmission_iap_data_by_wifi(QFile* &app,int num)
{
    int read_num;
    char iap_buf[200] = {0};
    iap_buf[1] = num & 0X000000FF;
    iap_buf[0] = (num>>8) & 0X000000FF;
    if(read_iap_page_data_by_wifi(app,num,(iap_buf+2),read_num))
    {
        transmission_data_by_wifi(0XF0,iap_buf,read_num+2);
    }
}

void dabai::transmission_data_by_wifi(int command, char *data, int length)
{
    QByteArray ID;
    ID.resize(4);
    QString target = ui->device->currentText();
    int clientID = target.split(":")[0].toInt();
    QString TCM300_id = (ui->label_tcm300ID->text()).split(":")[1];
    ID = myHelper::HexStrToByteArray(TCM300_id);

    char* send_data = (char*)malloc(11+length);
    send_data[0] = 0XAA;
    send_data[1] = 0X55;
    send_data[2] = length + 9;
    send_data[3] = ID[0];
    send_data[4] = ID[1];
    send_data[5] = ID[2];
    send_data[6] = ID[3];
    send_data[7] = 0X01;
    send_data[8] = length + 2;
    send_data[9] = command;
    for(int loop = 0; loop < length ; loop++)
    {
        send_data[10+loop] = *(data+loop);
    }
    //send_data[10+length] = 0X11;
    mycrc.SetCRC8Sub(send_data,10+length);

    QByteArray send_buff(send_data,(11+length));
    tcpServer->SendData(clientID,send_buff);
    free((void*)(send_data));
}

int dabai::check_file_then_open(QFile* &file)
{
    //if(file->open(QIODevice::ReadOnly|QIODevice::Text))//bug 不会发送0d
    if(file->open(QIODevice::ReadOnly))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void dabai::check_file()
{
    QFile file_1("./app_1.bin");
    int res;
    if(file_1.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        if(-1 == ui->comboBox_applist->findText("app_1"))
        {
            ui->comboBox_applist->addItem("app_1");
        }
        file_1.close();
    }
    else
    {
        res = ui->comboBox_applist->findText("app_1");
        if(-1 != res)
        {
            ui->comboBox_applist->removeItem(res);
        }
    }

    QFile file_2("./app_2.bin");
    if(file_2.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        if(-1 == ui->comboBox_applist->findText("app_2"))
        {
            ui->comboBox_applist->addItem("app_2");
        }
        file_2.close();
    }
    else
    {
        res = ui->comboBox_applist->findText("app_2");
        if(-1 != res)
        {
            ui->comboBox_applist->removeItem(res);
        }
    }

    QFile file_3("./app_3.bin");
    if(file_3.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        if(-1 == ui->comboBox_applist->findText("app_3"))
        {
            ui->comboBox_applist->addItem("app_3");
        }
        file_3.close();
    }
    else
    {
        res = ui->comboBox_applist->findText("app_3");
        if(-1 != res)
        {
            ui->comboBox_applist->removeItem(res);
        }
    }

    QFile file_base("./app_base.bin");
    if(file_base.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        if(-1 == ui->comboBox_applist->findText("app_base"))
        {
            ui->comboBox_applist->addItem("app_base");
        }
        file_base.close();
    }
    else
    {
        res = ui->comboBox_applist->findText("app_base");
        if(-1 != res)
        {
            ui->comboBox_applist->removeItem(res);
        }
    }
}

void dabai::on_button_check_file_clicked()
{
    myHelper::ShowMessageBoxInfoX("请将固件文件放到当前文件夹中!\r\n如app_1.bin app_base.bin");
    check_file();
    read_bin_file();
}

void dabai::on_button_iap_clicked()
{

    //QFile file("./iap.bin");
    //qDebug()<<"current applicationDirPath: "<<QCoreApplication::applicationDirPath();
    //qDebug()<<"current currentPath: "<<QDir::currentPath();
    //if(!file.open(QIODevice::ReadOnly))
    //{
       // ui->read_data->append(tr("open file error!"));
    //}
    ui->read_data->clear();
    QMessageBox msg(this);
    msg.setStyleSheet("font:10pt '宋体'");
    msg.setWindowTitle("准备升级");
    msg.setText("检测当前设备正在使用的固件!");
    msg.setIcon(QMessageBox::Warning);
    QAbstractButton *yes = msg.addButton("确定",QMessageBox::ActionRole);
    QAbstractButton *no = msg.addButton("取消",QMessageBox::RejectRole);
    msg.exec();
    if(msg.clickedButton() == yes)
    {
        ui->read_data->append(tr("检测设备固件,如果没反应,请检查设备是否在线!"));
        //transmission_iap_data_by_wifi(file,1);
        //char buff[1];
        //buff[0] = 0;
        //transmission_data_by_wifi(0XF1,buff,1);
        transmission_start_code_by_wifi();
    }
    if(msg.clickedButton() == no)
    {
        ui->read_data->append(tr("取消检测设备固件信息"));
        //transmission_iap_data_by_wifi(file,256);

    }
    //file.close();
}



void dabai::on_button_jump_f_clicked()
{
    ui->read_data->clear();
    QMessageBox msg(this);
    msg.setStyleSheet("font:10pt '宋体'");
    msg.setWindowTitle("固件跳转");
    msg.setText("请确认跳转固件可用!否则可能使设备死机!\n是否跳转");
    msg.setIcon(QMessageBox::Warning);
    QAbstractButton *yes = msg.addButton("跳转",QMessageBox::ActionRole);
    QAbstractButton *no = msg.addButton("取消",QMessageBox::RejectRole);
    msg.exec();
    if(msg.clickedButton() == yes)
    {
        ui->read_data->append(tr("开始跳转至新固件,如果设备跳转失败请断电重启!"));
    }
    if(msg.clickedButton() == no)
    {
        ui->read_data->append(tr("取消固件跳转"));
        //transmission_iap_data_by_wifi(file,256);
        return;

    }
    int app = ui->comboBox_app_to_jump->currentIndex();
    switch(app)
    {
        case 0:
        choose_app(0X22);
        break;
    case 1:
        choose_app(0X33);
        break;
    case 2:
        choose_app(0X44);
        break;
    case 3:
        choose_app(0X11);
        break;
    default:
        break;
    }
}

void dabai::on_button_jump_clicked()
{

}

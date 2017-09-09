#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

#define CONTAIN(v, flg) (((v) & (flg)) == (flg))

#define MAKE_DEFAULT_ADDRESS_ENTRY(name) \
    QNetworkAddressEntry name; \
    name.setBroadcast(QHostAddress("192.168.1.255")); \
    name.setNetmask(QHostAddress("255.255.255.0")); \
    name.setIp(QHostAddress("127.0.0.1"));

QNetworkAddressEntry getCurrentInterfaceAddress() {
    for (auto i : QNetworkInterface::allInterfaces()) {
        auto flg = i.flags();
        if (! CONTAIN(flg, QNetworkInterface::IsUp | QNetworkInterface::IsRunning | QNetworkInterface::CanBroadcast)) continue;
        if (CONTAIN(flg, QNetworkInterface::IsLoopBack) || CONTAIN(flg, QNetworkInterface::IsPointToPoint)) continue;

        if (i.humanReadableName().indexOf(QString("Virtual")) != -1) continue;

        for (auto a : i.addressEntries()) {
            if (a.ip().toString().length() >= 16) continue;
            if (a.prefixLength() < 0) continue;

            return a;
        }
    }

    MAKE_DEFAULT_ADDRESS_ENTRY(rv);
    return rv;
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    first_color = 0;
    connect(ui->draught_board,SIGNAL(moveFT(int,int,int,int)),this,SLOT(sendMove(int,int,int,int)));
    connect(ui->draught_board,SIGNAL(gameOver(int)),this,SLOT(sendGameOver(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_2_triggered()
{
    bool ok = false;
    int i = QInputDialog::getInt(
                this,tr("Create server"),tr("IP : ")+ getCurrentInterfaceAddress().ip().toString() +tr("\n Port : "), 1025, 1025, 65535, 1 &ok);
    this->startServer(i);
}


void MainWindow::on_action_triggered()
{
    bool ok = false;
    QString i = QInputDialog::getText(
                      this,tr("Connect"),tr("IP && Port(xxx.xxx.xxx.xxx:pppp) : "),QLineEdit::Normal,QString(),&ok);
    QStringList st = i.split(":");
    this->connectTo(QHostAddress(st.at(0)),st.at(1).toInt());
}

void MainWindow::startServer(quint16 port){
    server = new QTcpServer(this);
    server->listen(QHostAddress::Any,port);
    server->waitForNewConnection(-1);
    QMessageBox box(QMessageBox::NoIcon,tr("Server created."),tr("Client successfully connected!"),QMessageBox::Ok);
    box.exec();
    this->socket = server->nextPendingConnection();
    connect(socket,SIGNAL(readyRead()),this,SLOT(read()));
    ui->draught_board->my_color = 0;
    ui->draught_board->color = first_color ^ 1;
    ui->draught_board->exchangeTurn();
    QByteArray cmd("setBoard ");
    cmd.append(QByteArray::number(first_color));
    for(int i=0;i<Board::M;i++){
        cmd.append(" ");
        cmd.append(QByteArray::number(int(ui->draught_board->board[Board::pos(i)])));
    }
    write(cmd);
}

void MainWindow::connectTo(const QHostAddress &address, quint16 port){
    socket = new QTcpSocket(this);
    socket->connectToHost(address,port);
    socket->waitForConnected();
    connect(socket,SIGNAL(readyRead()),this,SLOT(read()));
    ui->draught_board->my_color = 1;
    ui->draught_board->color = first_color ^ 1;
    ui->draught_board->exchangeTurn();
}

void MainWindow::read(){
    static QByteArray buf;
    buf.append(socket->readAll());
    auto tmp = buf.split('$');
    buf = tmp.last();
    tmp.pop_back();
    for(auto current_cmd = tmp.begin(); current_cmd != tmp.end(); ++current_cmd){
        dealWithCommand(*current_cmd);
    }
}

void MainWindow::dealWithCommand(const QByteArray & cmd){
    auto tmp = cmd.split(' ');
    auto cc = tmp.at(0);
    tmp.pop_front();
    if(cc == "setBoard"){
        int count = 0;
        ui->draught_board->color = (first_color = tmp.at(0).toInt()) ^ 1;
        tmp.pop_front();
        for(auto p = tmp.begin();p!=tmp.end();p++,count++){
            ui->draught_board->setCell(count,p->toInt());
        }
        ui->draught_board->exchangeTurn();
    }
    else if(cc == "move"){
        ui->draught_board->makeMove(tmp.at(0).toInt(),tmp.at(1).toInt(),tmp.at(2).toInt(),tmp.at(3).toInt());
    }
    else if(cc == "gameover"){
        this->game_over(tmp.at(0).toInt());
    }
    else if(cc == "tie"){
        auto pp = tmp.at(0).toInt();
        if(pp==0){ // quest!
            QMessageBox box(QMessageBox::NoIcon,tr("Tie request."),tr(""),QMessageBox::Ok | QMessageBox::Cancel);
            int ret = box.exec();
            if(ret==QMessageBox::Ok){
                write("tie 1");
                this->game_tie();
            }
            else{
                write("tie -1");
            }
        }
        else if(pp==-1){ // refused!
            QMessageBox box(QMessageBox::NoIcon,tr("Tie request."),tr("Refused!"),QMessageBox::Ok);
            box.exec();
        }
        else if(pp==1){ // accepted!
            this->game_tie();
        }
    }
}

void MainWindow::write(const QByteArray &cmd){
    socket->write(cmd);
    socket->write("$");
}

void MainWindow::game_tie(){
    QMessageBox box(QMessageBox::NoIcon,tr("Game over!"),tr("Tied."),QMessageBox::Ok);
    box.exec();
    this->close();
}

void MainWindow::game_over(int color){
    QMessageBox box(QMessageBox::NoIcon,tr("Game over!"),tr(color == ui->draught_board->my_color ? "You won!" : "You lost!"), QMessageBox::Ok);
    box.exec();
    this->close();
}

void MainWindow::sendMove(int a,int b,int c,int d){
    write("move "+QByteArray::number(a)+" "+QByteArray::number(b)+" "+QByteArray::number(c)+" "+QByteArray::number(d));
}

void MainWindow::sendGameOver(int color){
    write("gameover "+QByteArray::number(color));
    game_over(color);
}

void MainWindow::on_pushButton_clicked()
{
    QMessageBox box(QMessageBox::NoIcon,tr(""),tr("Are you sure?"),QMessageBox::Ok | QMessageBox::Cancel);
    int ret = box.exec();
    if(ret == QMessageBox::Ok){
        write("gameover "+QByteArray::number(ui->draught_board->my_color ^ 1));
        this->game_over(ui->draught_board->my_color ^ 1);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QMessageBox box(QMessageBox::NoIcon,tr(""),tr("Are you sure?"),QMessageBox::Ok | QMessageBox::Cancel);
    int ret = box.exec();
    if(ret == QMessageBox::Ok){
        write("tie 0");
    }
}

void MainWindow::on_action_3_triggered()
{
    QString text = QInputDialog::getText(this,"","",QLineEdit::Normal,
                                         "1 4 4 4 4 4 4 0 4 2 2 4 4 4 4 0 4 0 4 4 4 4 2 4 4 4 4 0 4 4 4 4 4 0 4 4 4 0 4 4 4 4 4 2 4 4 4 4 4 4 4");
    auto tmp = text.split(" ");
    qDebug() << tmp ;
    int count = 0;
    ui->draught_board->color = (first_color = tmp.at(0).toInt()) ^ 1;
    tmp.pop_front();
    for(auto p = tmp.begin();p!=tmp.end();p++,count++){
        ui->draught_board->setCell(count,p->toInt());
    }
    ui->draught_board->exchangeTurn();
}

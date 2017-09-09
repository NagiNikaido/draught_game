#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QtNetwork>
#include <QNetworkInterface>
#include <QMessageBox>
#include <QInputDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    int first_color;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void startServer(quint16 );
    void connectTo(const QHostAddress &, quint16 );
    void write(const QByteArray &);
    void dealWithCommand(const QByteArray &);
    void game_tie();
    void game_over(int );
public slots:
    void read();
    void sendMove(int , int , int , int );
    void sendGameOver(int );
private slots:
    void on_action_2_triggered();
    void on_action_triggered();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_action_3_triggered();

private:
    Ui::MainWindow *ui;
    QTcpServer *server;
    QTcpSocket *socket;
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QSocketNotifier>
#include <QDebug>
#include <QFile>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_pushButton_STop_clicked();

    void on_pushButton_Stop_clicked();

    void on_pushButton_Quit_clicked();

    void readyRead();

private:
    Ui::MainWindow *ui;
    QTimer m_timerLow;
    QTimer m_timerHigh;
    QTimer m_timerReader;
    QSocketNotifier* m_socketNotifier;
    int m_fd;
    QFile m_file;
};

#endif // MAINWINDOW_H

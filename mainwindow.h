#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "connect/connectmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void playClicked();
    void connectClicked();
    void testClicked();

private:
    Ui::MainWindow *ui;

    ConnectManager* connectManager;
};
#endif // MAINWINDOW_H

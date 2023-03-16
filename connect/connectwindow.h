#ifndef CONNECTWINDOW_H
#define CONNECTWINDOW_H

#include <QtWidgets/qdialog.h>

QT_USE_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui {
    class ConnectWindow;
}
QT_END_NAMESPACE

class ConnectWindow : public QDialog
{
    Q_OBJECT

public:
    ConnectWindow(QWidget *parent = nullptr);
    ~ConnectWindow();

private:
    Ui::ConnectWindow *ui;
};

#endif // CONNECTWINDOW_H

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class SettingsWindow; }
QT_END_NAMESPACE

class Settings : public QMainWindow
{
    Q_OBJECT
public:
    explicit Settings(QWidget *parent = nullptr);
    void saveValues();

signals:

private:
    Ui::SettingsWindow *ui;

};

#endif // SETTINGS_H

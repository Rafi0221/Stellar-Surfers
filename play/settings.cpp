
#include "settings.h"
#include "ui_settingswindow.h"
#include "saves/settingssave.h"
#include "saves/settingssaver.h"

#include "../utils/consts.h"

Settings::Settings(QWidget *parent)
    : QMainWindow{parent}
    , ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/media/media/ic_launcher.png"));

    connect(ui->renderDistanceSlider, &QSlider::valueChanged, this, &Settings::saveValues);
    connect(ui->planetQualitySlider, &QSlider::valueChanged, this, &Settings::saveValues);

    ui->renderDistanceSlider->setSliderPosition(RENDER_DIST);
    ui->planetQualitySlider->setSliderPosition(MAX_LOD_DIFFERENCE);

    ui->renderDistanceValue->setText(QString::number(RENDER_DIST));
    ui->planetQualityValue->setText(QString::number(MAX_LOD_DIFFERENCE));

    connect(ui->renderDistanceSlider, &QSlider::valueChanged, this, [=] () {
        ui->renderDistanceValue->setText(QString::number(ui->renderDistanceSlider->value()));
    });
    connect(ui->planetQualitySlider, &QSlider::valueChanged, this, [=] () {
        ui->planetQualityValue->setText(QString::number(ui->planetQualitySlider->value()));
    });

    connect(ui->renderDistanceSlider, &QSlider::valueChanged, this, [=] () {
        ui->notifyLabel->show();
    });
    connect(ui->planetQualitySlider, &QSlider::valueChanged, this, [=] () {
        ui->notifyLabel->show();
    });

    ui->notifyLabel->hide();
}


void Settings::saveValues() {
    int newRenderDistance = ui->renderDistanceSlider->value();
    int newPlanetQuality = ui->planetQualitySlider->value();
    qDebug() << "changing to" << newRenderDistance << newPlanetQuality;
    SettingsSaver::write(SettingsSave(newRenderDistance, newPlanetQuality));
}


#include "settingssaver.h"

#include <QFile>
#include <fstream>


SettingsSaver::SettingsSaver()
{
}


void SettingsSaver::write(SettingsSave save) {
    std::ofstream f;

    f.open("../Stellar-Surfers/play/saves/renderdistance.dat");
    f << save.renderDistance;
    f.close();

    f.open("../Stellar-Surfers/play/saves/planetquality.dat");
    f << save.planetQuality;
    f.close();
}


#ifndef SETTINGSSAVER_H
#define SETTINGSSAVER_H


#include "settingssave.h"

class SettingsSaver
{
public:
    SettingsSaver();

    static void write(SettingsSave save);
};

#endif // SETTINGSSAVER_H

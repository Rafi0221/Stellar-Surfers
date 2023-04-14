
#ifndef PARSER_H
#define PARSER_H

#include "glupdater.h"
#include <QtCore/qobject.h>


class Parser : public QObject
{
    Q_OBJECT

public:
    Parser(GLUpdater *_glupdater);

public slots:
    void parse(QByteArray line);

private:
    GLUpdater *glupdater;
};

#endif // PARSER_H

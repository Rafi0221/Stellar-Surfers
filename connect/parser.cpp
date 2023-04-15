
#include "parser.h"

#include <qdebug.h>

Parser::Parser(GLUpdater *_glupdater)
{
    glupdater = _glupdater;
}

void Parser::parse(QByteArray line){
    if(line.size() >= 13){
        line = line.last(13);

        const float* ptrFloat = reinterpret_cast<const float*>(line.constData());

        glupdater->setCameraXYZ(*(ptrFloat+2), *ptrFloat, *(ptrFloat+1));

//        qDebug() << *ptrFloat << " " << *(ptrFloat+1) << " " << *(ptrFloat+2);
    }

}

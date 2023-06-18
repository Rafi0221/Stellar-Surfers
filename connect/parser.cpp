
#include "parser.h"
#include "client.h"
#include "../utils/consts.h"

#include <qdebug.h>

Parser::Parser(GLUpdater *glupdater)
{
    this->glupdater = glupdater;
}

/*
 * data format:
 * 3 floats - rotation angles
 * 1 float - acceleration
 *
 * messages must be of constant size specified in Client::messageSize
 */

void Parser::parse(QByteArray line){
    //qDebug() << line.size() << " " << QString::fromUtf8(line.constData(), line.length());
    const int* msg_type = reinterpret_cast<const int*>(line.constData());
    //qDebug() << "msgType = " << *msg_type;
    if(line.size() == Client::messageSize){
        if (*msg_type == MSG_INFO) {
            const float* ptrFloat = reinterpret_cast<const float*>(line.constData() + 4);

            glupdater->setCameraXYZ(*(ptrFloat+2), *ptrFloat, -(*(ptrFloat+1)));
            glupdater->setAcceleration(*(ptrFloat+3));
            //    qDebug() << *ptrFloat << " " << *(ptrFloat+1) << " " << *(ptrFloat+2) << " " << *(ptrFloat+3);
        } else {
            const float* ptrFloat = reinterpret_cast<const float*>(line.constData() + 4);
//            qDebug() << "Shoot " << *(ptrFloat) << " " << *(ptrFloat + 1);
            glupdater->notifyShoot(*(ptrFloat), *(ptrFloat + 1));
        }
    }
    else {
        qDebug() << "received message of size " << line.size() << ", expected " << Client::messageSize;
    }

}

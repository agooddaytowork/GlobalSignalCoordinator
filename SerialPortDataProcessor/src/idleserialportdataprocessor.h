#ifndef IDLESERIALPORTDATAPROCESSOR_H
#define IDLESERIALPORTDATAPROCESSOR_H

#include "serialportdataprocessorbasis.h"
#include <QTimer>

class idleSerialPortDataProcessor : public QState
{
public:
    idleSerialPortDataProcessor(SerialPortDataProcessorBasis *parentBasis, QState *parent = nullptr);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    SerialPortDataProcessorBasis *basisptr = nullptr;
    QTimer autoSignaller;
};

#endif // IDLESERIALPORTDATAPROCESSOR_H

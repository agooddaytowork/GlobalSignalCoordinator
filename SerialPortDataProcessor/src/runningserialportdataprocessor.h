#ifndef RUNNINGSERIALPORTDATAPROCESSOR_H
#define RUNNINGSERIALPORTDATAPROCESSOR_H

#include "serialportdataprocessorbasis.h"

class runningSerialPortDataProcessor : public QState
{
public:
    runningSerialPortDataProcessor(SerialPortDataProcessorBasis *parentBasis, QState *parent = nullptr);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    SerialPortDataProcessorBasis *basisptr = nullptr;
};

#endif // RUNNINGSERIALPORTDATAPROCESSOR_H

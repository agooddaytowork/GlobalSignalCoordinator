#ifndef UNINITIATEDSERIALPORTDATAPROCESSOR_H
#define UNINITIATEDSERIALPORTDATAPROCESSOR_H

#include "serialportdataprocessorbasis.h"

class uninitiatedSerialPortDataProcessor : public QState
{
public:
    uninitiatedSerialPortDataProcessor(SerialPortDataProcessorBasis *parentBasis, QState *parent = nullptr);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    SerialPortDataProcessorBasis * basisptr = nullptr;
};

#endif // UNINITIATEDSERIALPORTDATAPROCESSOR_H

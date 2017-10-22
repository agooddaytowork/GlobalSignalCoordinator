#ifndef ERRORSERIALPORTDATAPROCESSOR_H
#define ERRORSERIALPORTDATAPROCESSOR_H

#include "serialportdataprocessorbasis.h"

class errorSerialPortDataProcessor : public QState
{
public:
    errorSerialPortDataProcessor(SerialPortDataProcessorBasis *parentBasis, QState *parent = nullptr);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    SerialPortDataProcessorBasis *basisptr = nullptr;
};

#endif // ERRORSERIALPORTDATAPROCESSOR_H

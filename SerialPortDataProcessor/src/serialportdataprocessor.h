#ifndef SERIALPORTDATAPROCESSOR_H
#define SERIALPORTDATAPROCESSOR_H

#include "serialportdataprocessorbasis.h"
#include "shared/directtransition.h"
#include "uninitiatedserialportdataprocessor.h"
#include "idleserialportdataprocessor.h"
#include "runningserialportdataprocessor.h"
#include "errorserialportdataprocessor.h"

class SerialPortDataProcessor : public QStateMachine
{
    Q_OBJECT
public:
    explicit SerialPortDataProcessor(bool isUHV2, QObject *parent = nullptr);
signals:
    void Out(const GlobalSignal &);
public slots:
    void In(const GlobalSignal &aGlobalSignal);
private:
    SerialPortDataProcessorBasis *currentBasis = nullptr;
};

#endif // SERIALPORTDATAPROCESSOR_H

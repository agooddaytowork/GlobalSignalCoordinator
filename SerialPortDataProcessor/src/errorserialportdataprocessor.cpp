#include "errorserialportdataprocessor.h"

errorSerialPortDataProcessor::errorSerialPortDataProcessor(SerialPortDataProcessorBasis *parentBasis, QState *parent) :
    QState(parent),
    basisptr(parentBasis)
{
    setObjectName(QStringLiteral("errorSerialPortDataProcessor"));
    anIf(SerialPortDataProcessorBasisDbgEn, anAck("errorSerialPortDataProcessor Constructed"));
}

void errorSerialPortDataProcessor::onEntry(QEvent *)
{
    anIf(SerialPortDataProcessorBasisDbgEn, anTrk("errorSerialPortDataProcessor Entered"));
    basisptr->currentStateName = objectName();
    qApp->processEvents();
    basisptr->errorSerialPortDataProcessorOnEntry();
}

void errorSerialPortDataProcessor::onExit(QEvent *)
{
    anIf(SerialPortDataProcessorBasisDbgEn, anTrk("Leave errorSerialPortDataProcessor"));
    basisptr->clearError();
    basisptr->previousStateName = objectName();
}

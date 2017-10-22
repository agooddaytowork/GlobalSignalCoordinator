#include "uninitiatedserialportdataprocessor.h"

uninitiatedSerialPortDataProcessor::uninitiatedSerialPortDataProcessor(SerialPortDataProcessorBasis *parentBasis, QState *parent) :
    QState(parent),
    basisptr(parentBasis)
{
    setObjectName(QStringLiteral("uninitiatedSerialPortDataProcessor"));
    anIf(SerialPortDataProcessorBasisDbgEn, anAck("uninitiatedSerialPortDataProcessor Constructed"));
}

void uninitiatedSerialPortDataProcessor::onEntry(QEvent *)
{
    anIf(SerialPortDataProcessorBasisDbgEn, anTrk("uninitiatedSerialPortDataProcessor Entered"));
    basisptr->currentStateName = objectName();
    qApp->processEvents();
    basisptr->uninitiatedSerialPortDataProcessorOnEntry();
}

void uninitiatedSerialPortDataProcessor::onExit(QEvent *)
{
    anIf(SerialPortDataProcessorBasisDbgEn, anTrk("Leave uninitiatedSerialPortDataProcessor"));
    basisptr->previousStateName = objectName();
}

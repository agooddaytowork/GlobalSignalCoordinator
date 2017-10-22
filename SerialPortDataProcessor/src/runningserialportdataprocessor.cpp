#include "runningserialportdataprocessor.h"

runningSerialPortDataProcessor::runningSerialPortDataProcessor(SerialPortDataProcessorBasis *parentBasis, QState *parent) :
    QState(parent),
    basisptr(parentBasis)
{
    setObjectName(QStringLiteral("runningSerialPortDataProcessor"));
    anIf(SerialPortDataProcessorBasisDbgEn, anAck("runningSerialPortDataProcessor Constructed"));
}

void runningSerialPortDataProcessor::onEntry(QEvent *)
{
    anIf(SerialPortDataProcessorBasisDbgEn, anTrk("runningSerialPortDataProcessor Entered"));
    basisptr->currentStateName = objectName();
    qApp->processEvents();
    basisptr->runningSerialPortDataProcessorOnEntry();
}

void runningSerialPortDataProcessor::onExit(QEvent *)
{
    anIf(SerialPortDataProcessorBasisDbgEn, anTrk("Leave runningSerialPortDataProcessor"));
    basisptr->previousStateName = objectName();
}

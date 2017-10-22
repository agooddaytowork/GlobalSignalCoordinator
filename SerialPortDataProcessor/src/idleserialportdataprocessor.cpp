#include "idleserialportdataprocessor.h"

idleSerialPortDataProcessor::idleSerialPortDataProcessor(SerialPortDataProcessorBasis *parentBasis, QState *parent) :
    QState(parent),
    basisptr(parentBasis)
{
    setObjectName(QStringLiteral("idleSerialPortDataProcessor"));
    autoSignaller.setParent(this);
    autoSignaller.setInterval(5000);
    QObject::connect(&autoSignaller, &QTimer::timeout, basisptr, &SerialPortDataProcessorBasis::autoSignaller);
    anIf(SerialPortDataProcessorBasisDbgEn, anAck("idleSerialPortDataProcessor Constructed"));
}

void idleSerialPortDataProcessor::onEntry(QEvent *)
{
    anIf(SerialPortDataProcessorBasisDbgEn, anTrk("idleSerialPortDataProcessor Entered"));
    basisptr->currentStateName = objectName();
    qApp->processEvents();
    basisptr->idleSerialPortDataProcessorOnEntry();
    if (basisptr->isAutoSignallerEnabled)
        autoSignaller.start();
}

void idleSerialPortDataProcessor::onExit(QEvent *)
{
    anIf(SerialPortDataProcessorBasisDbgEn, anTrk("Leave idleSerialPortDataProcessor"));
    autoSignaller.stop();
    basisptr->previousStateName = objectName();
}

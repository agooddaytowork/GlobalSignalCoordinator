#include "idlecandataprocessor.h"

idleCanDataProcessor::idleCanDataProcessor(CanDataProcessorBasis *parentBasis, QState *parent) :
    QState(parent),
    basisptr(parentBasis)
{
    setObjectName(QStringLiteral("idleCanDataProcessor"));
    autoSignaller.setParent(this);
    autoSignaller.setInterval(30000);
    QObject::connect(&autoSignaller, &QTimer::timeout, basisptr, &CanDataProcessorBasis::autoSignaller);
    anIf(CanDataProcessorBasisDbgEn, anAck("idleCanDataProcessor Constructed"));
}

void idleCanDataProcessor::onEntry(QEvent *)
{
    anIf(CanDataProcessorBasisDbgEn, anTrk("idleCanDataProcessor Entered"));
    basisptr->currentStateName = objectName();
    qApp->processEvents();
    basisptr->idleCanDataProcessorOnEntry();
    if (basisptr->isAutoSignallerEnabled)
        autoSignaller.start();
}

void idleCanDataProcessor::onExit(QEvent *)
{
    anIf(CanDataProcessorBasisDbgEn, anTrk("Leave idleCanDataProcessor"));
    autoSignaller.stop();
    basisptr->previousStateName = objectName();
}

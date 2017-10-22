#include "errorcandataprocessor.h"

errorCanDataProcessor::errorCanDataProcessor(CanDataProcessorBasis *parentBasis, QState *parent) :
    QState(parent),
    basisptr(parentBasis)
{
    setObjectName(QStringLiteral("errorCanDataProcessor"));
    anIf(CanDataProcessorBasisDbgEn, anAck("errorCanDataProcessor Constructed"));
}

void errorCanDataProcessor::onEntry(QEvent *)
{
    anIf(CanDataProcessorBasisDbgEn, anTrk("errorCanDataProcessor Entered"));
    basisptr->currentStateName = objectName();
    qApp->processEvents();
    basisptr->errorCanDataProcessorOnEntry();
}

void errorCanDataProcessor::onExit(QEvent *)
{
    anIf(CanDataProcessorBasisDbgEn, anTrk("Leave errorCanDataProcessor"));
    basisptr->clearError();
    basisptr->previousStateName = objectName();
}

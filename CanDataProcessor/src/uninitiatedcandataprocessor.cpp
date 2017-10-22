#include "uninitiatedcandataprocessor.h"

uninitiatedCanDataProcessor::uninitiatedCanDataProcessor(CanDataProcessorBasis *parentBasis, QState *parent) :
    QState(parent),
    basisptr(parentBasis)
{
    setObjectName(QStringLiteral("uninitiatedCanDataProcessor"));
    anIf(CanDataProcessorBasisDbgEn, anAck("uninitiatedCanDataProcessor Constructed"));
}

void uninitiatedCanDataProcessor::onEntry(QEvent *)
{
    anIf(CanDataProcessorBasisDbgEn, anTrk("uninitiatedCanDataProcessor Entered"));
    basisptr->currentStateName = objectName();
    qApp->processEvents();
    basisptr->uninitiatedCanDataProcessorOnEntry();
}

void uninitiatedCanDataProcessor::onExit(QEvent *)
{
    anIf(CanDataProcessorBasisDbgEn, anTrk("Leave uninitiatedCanDataProcessor"));
    basisptr->previousStateName = objectName();
}

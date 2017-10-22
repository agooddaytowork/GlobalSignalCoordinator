#include "runningcandataprocessor.h"

runningCanDataProcessor::runningCanDataProcessor(CanDataProcessorBasis *parentBasis, QState *parent) :
    QState(parent),
    basisptr(parentBasis)
{
    setObjectName(QStringLiteral("runningCanDataProcessor"));
    anIf(CanDataProcessorBasisDbgEn, anAck("runningCanDataProcessor Constructed"));
}

void runningCanDataProcessor::onEntry(QEvent *)
{
    anIf(CanDataProcessorBasisDbgEn, anTrk("runningCanDataProcessor Entered"));
    basisptr->currentStateName = objectName();
    qApp->processEvents();
    basisptr->runningCanDataProcessorOnEntry();
}

void runningCanDataProcessor::onExit(QEvent *)
{
    anIf(CanDataProcessorBasisDbgEn, anTrk("Leave runningCanDataProcessor"));
    basisptr->previousStateName = objectName();
}

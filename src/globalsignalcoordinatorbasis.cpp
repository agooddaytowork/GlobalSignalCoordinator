#include "globalsignalcoordinatorbasis.h"

GlobalSignalCoordinatorBasis::GlobalSignalCoordinatorBasis(QObject *parent) : AbstractStateMachineBasis(parent)
{
    registerGlobalSignal;
    anIf(GlobalSignalCoordinatorBasisDbgEn, anAck("GlobalSignalCoordinatorBasis Constructed"));
}

GlobalSignalCoordinatorBasis::~GlobalSignalCoordinatorBasis()
{

}

void GlobalSignalCoordinatorBasis::initiate()
{

}

const QMetaEnum GlobalSignalCoordinatorBasis::DataMetaEnum = QMetaEnum::fromType<GlobalSignalCoordinatorBasis::Data>();
const QMetaEnum GlobalSignalCoordinatorBasis::ErrorMetaEnum = QMetaEnum::fromType<GlobalSignalCoordinatorBasis::Error>();
const QMetaEnum GlobalSignalCoordinatorBasis::WarningMetaEnum = QMetaEnum::fromType<GlobalSignalCoordinatorBasis::Warning>();
const QMetaEnum GlobalSignalCoordinatorBasis::NotificationMetaEnum = QMetaEnum::fromType<GlobalSignalCoordinatorBasis::Notification>();

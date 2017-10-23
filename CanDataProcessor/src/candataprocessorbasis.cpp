#include "candataprocessorbasis.h"

CanDataProcessorBasis::CanDataProcessorBasis(QObject *parent) : AbstractStateMachineBasis(parent)
{
    registerGlobalSignal;
    anIf(CanDataProcessorBasisDbgEn, anAck("CanDataProcessorBasis Constructed"));
}

CanDataProcessorBasis::~CanDataProcessorBasis()
{
    anIf(CanDataProcessorBasisDbgEn, anWarn("Destroy CanDataProcessorBasis"));
    dispose();
}

void CanDataProcessorBasis::uninitiatedCanDataProcessorOnEntry()
{
    if (!isInitiated)
    {
        initiate();
    }
}

void CanDataProcessorBasis::idleCanDataProcessorOnEntry()
{
    if (previousStateName == QStringLiteral("uninitiatedCanDataProcessor"))
    {
        GlobalSignal iamReady;
        iamReady.Type = QVariant::fromValue(readyToWork);
        iamReady.Data = QVariant::fromValue(parent()->objectName());
        iamReady.TimeStamp = NOW2String;
        iamReady.DstStrs.append(GlobalSignalCoordinatorObjName);
        iamReady.SignalPriority = 200;
        pushAGlobalSignalIntoPrioritizedBuffer(iamReady);
        emit GlobalSignalExecutionRequested();
    }
}

void CanDataProcessorBasis::runningCanDataProcessorOnEntry()
{
    clearCache();
    if (prioritizedBuffer.size())
    {
        currentGlobalSignal = prioritizedBuffer.last().takeFirst();
        deleteEmptyListsFromPrioritizedBuffer();
        QString currentGlobalSignalTypeTypeName = currentGlobalSignal.Type.typeName();
        if (currentGlobalSignalTypeTypeName == QStringLiteral("CanDataProcessorBasis::Data"))
        {
            switch (currentGlobalSignal.Type.toInt()) {
            case disableAutoSignaller:
            {
                isAutoSignallerEnabled = false;
                break;
            }
            case enableAutoSignaller:
            {
                isAutoSignallerEnabled = true;
                break;
            }
            default:
                break;
            }
        }
        else if (currentGlobalSignalTypeTypeName == QStringLiteral("CanDataProcessorBasis::Notification"))
        {
            switch (currentGlobalSignal.Type.toInt()) {
            case readyToWork:
            {
                anIf(CanDataProcessorBasisDbgEn, anAck("readyToWork"));
                emit Out(GlobalSignal(currentGlobalSignal));
                break;
            }
            default:
                break;
            }
        }
        else if (currentGlobalSignalTypeTypeName == QStringLiteral("CanBusWorkerBasis::Data"))
        {
            switch (currentGlobalSignal.Type.toInt()) {
            case CanBusWorkerBasis::replyFrameWithTimeStamp:
            {
                CanProtocol reply = CanProtocol(currentGlobalSignal.Data.value<QCanBusFrame>());
                if (reply.getMsgMean().contains(QStringLiteral("Data response"),Qt::CaseInsensitive))
                {
                    QString anRFID(reply.getRFID());
                    QSqlQuery queryGlobalID(localDb);
                    if (execSQL(&queryGlobalID,
                                "SELECT GlobalID,RFID FROM stations WHERE sdcsAddr=" + QString::number(reply.getSdcsId()) +
                                " AND sdcsCH=" + QString::number(reply.getChId())))
                    {
                        if (queryGlobalID.value("RFID").toString().contains(anRFID,Qt::CaseInsensitive))
                        {
                            piLocalDBWorkerBasis::RFIDData newRFIDData;
                            newRFIDData.GlobalID = queryGlobalID.value("GlobalID").toInt();
                            newRFIDData.srcRFID = anRFID;
                            anIf(CanDataProcessorBasisDbgEn,
                                    anAck("Emit newRFIDData To Local Database !");
                                    anVar(newRFIDData.GlobalID);
                                    anVar(newRFIDData.srcRFID));
                            GlobalSignal updateRFIDData;
                            updateRFIDData.Type = QVariant::fromValue(piLocalDBWorkerBasis::updateRFIDData);
                            updateRFIDData.Data = QVariant::fromValue(newRFIDData);
                            updateRFIDData.TimeStamp = NOW2String;
                            updateRFIDData.DstStrs.append(piLocalDBWorkerObjName);
                            emit Out(updateRFIDData);
                        }
                    }
                    else
                    {
                        anIf(CanDataProcessorBasisDbgEn, anWarn("Can Info Not Found In Database"));
                    }
                }
            }
            default:
                break;
            }
        }
    }
    if (prioritizedBuffer.isEmpty())
    {
        emit goIdle();
    }
    else
    {
        emit GlobalSignalExecutionRequested();
    }
}

void CanDataProcessorBasis::errorCanDataProcessorOnEntry()
{
    anIf(CanDataProcessorBasisDbgEn,
         anError("Emit CanDataProcessorBasis::Error");
         anInfo("ErrorType: " + QString(ErrorMetaEnum.valueToKey(static_cast<int>(ErrorType))));
         anInfo("ErrorInfo: " + ErrorInfo);
    );
    GlobalSignal errorGlobalSignal;
    errorGlobalSignal.Type = QVariant::fromValue(ErrorType);
    errorGlobalSignal.Data = QVariant::fromValue(ErrorInfo);
    errorGlobalSignal.Priority = 200;
    errorGlobalSignal.SignalPriority = 200;
    errorGlobalSignal.DstStrs.append(GlobalSignalCoordinatorObjName);
    emit Out(errorGlobalSignal);
}

void CanDataProcessorBasis::In(const GlobalSignal &aGlobalSignal)
{
    pushAGlobalSignalIntoPrioritizedBuffer(aGlobalSignal);
    if (currentStateName == QStringLiteral("idleCanDataProcessor"))
    {
        emit GlobalSignalExecutionRequested();
    }
}

void CanDataProcessorBasis::autoSignaller()
{
    if (goToNextGlobalID())
    {
        emit Out(GlobalSignal(autoGlobalSignalToCanBusWorker));
    }
}

void CanDataProcessorBasis::initiate()
{
    dispose();
    autoGlobalSignalToCanBusWorker.Type = QVariant::fromValue(CanBusWorkerBasis::requestFrameTransmission);
    autoGlobalSignalToCanBusWorker.DstStrs.append(CanBusWorkerObjName);
    autoGlobalSignalToCanBusWorker.DstStrs.append(parent()->objectName());
    if (openLocalDatabaseConnection())
    {
        isInitiated = true;
        emit goIdle();
    }
    anIf(CanDataProcessorBasisDbgEn && isInitiated, anWarn("CanDataProcessorBasis Initialized"));
}

void CanDataProcessorBasis::dispose()
{
    anIf(CanDataProcessorBasisDbgEn && isInitiated, anWarn("Clean CanDataProcessorBasis"));
    closeLocalDatabaseConnection();
    clearPrioritizedBuffer();
    previousStateName.clear();
    currentStateName.clear();
    clearCache();
    isInitiated = false;
}

void CanDataProcessorBasis::setError(const CanDataProcessorBasis::Error &anErrorType, const QString &anErrorInfo)
{
    if (anErrorType != NoError)
    {
        anIf(CanDataProcessorBasisDbgEn, anError("Error Occurred !"));
        ErrorType = anErrorType;
        ErrorInfo = anErrorInfo;
        emit ErrorOccurred();
    }
}

void CanDataProcessorBasis::clearError()
{
    anIf(CanDataProcessorBasisDbgEn && (ErrorType!=NoError), anInfo("Clear Error !"));
    ErrorType = NoError;
    ErrorInfo.clear();
    lastQSqlError = QSqlError();
}

bool CanDataProcessorBasis::openLocalDatabaseConnection()
{
    localDb = QSqlDatabase::cloneDatabase(localQSqlDatabase, parent()->objectName());
    if (localDb.open())
    {
        anIf(CanDataProcessorBasisDbgEn, anAck("OK Local Database Connected !"));
        return true;
    }
    else
    {
        lastQSqlError = localDb.lastError();
        anIf(CanDataProcessorBasisDbgEn,
             anError("Failed To Connect Local Database !");
             anInfo(lastQSqlError.text()));
        setError(QSqlErrorOccured,lastQSqlError.text());
    }
    return false;
}

void CanDataProcessorBasis::closeLocalDatabaseConnection()
{
    QString currentConnection = localDb.connectionName();
    localDb.close();
    localDb = QSqlDatabase();
    localDb.removeDatabase(currentConnection);
    anIf(CanDataProcessorBasisDbgEn && currentConnection.size(), anWarn("Local Database Connection Closed !"));
}

void CanDataProcessorBasis::clearCache()
{
    currentGlobalSignal = GlobalSignal();
}

bool CanDataProcessorBasis::goToNextGlobalID()
{
    QSqlQuery queryNextGlobalID(localDb);
    if (!execSQL(&queryNextGlobalID,
                "SELECT GlobalID,sdcsAddr FROM stations WHERE GlobalID>"
                    + QString::number(currentGlobalID)))
    {
        currentGlobalID = 0;
        if (!execSQL(&queryNextGlobalID,
                    "SELECT GlobalID,sdcsAddr FROM stations WHERE GlobalID>"
                        + QString::number(currentGlobalID)))
        {
            return false;
        }
    }
    currentGlobalID = queryNextGlobalID.value("GlobalID").toInt();
    autoGlobalSignalToCanBusWorker.Data = QVariant::fromValue(CanProtocol::DataRequest(queryNextGlobalID.value("sdcsAddr").toInt()).getMsg());
    anIf(CanDataProcessorBasisDbgEn, anAck("Move To GlobalID = " << currentGlobalID));
    return true;
}

bool CanDataProcessorBasis::execSQL(QSqlQuery *aQuery, const QString &aSQL, bool navigateFirstRecordForSELECT)
{
    anIf(CanDataProcessorBasisDbgEn, anInfo("SQL : " << aSQL));
    if (aQuery->exec(aSQL))
    {
        anIf(CanDataProcessorBasisDbgEn, anInfo("OK SQL"));
        if (navigateFirstRecordForSELECT && aSQL.trimmed().left(6).contains(QStringLiteral("SELECT"),Qt::CaseInsensitive))
        {
            bool tmpReturn = aQuery->next();
            anIf(CanDataProcessorBasisDbgEn, anInfo("Is Any Info Selected? " << (tmpReturn?"YES":"NO")));
            return tmpReturn;
        }
        return true;
    }
    lastQSqlError = aQuery->lastError();
    anIf(CanDataProcessorBasisDbgEn, anError("ERROR SQL: " << lastQSqlError.text()));
    setError(QSqlErrorOccured,lastQSqlError.text());
    return false;
}

const QMetaEnum CanDataProcessorBasis::DataMetaEnum = QMetaEnum::fromType<CanDataProcessorBasis::Data>();
const QMetaEnum CanDataProcessorBasis::ErrorMetaEnum = QMetaEnum::fromType<CanDataProcessorBasis::Error>();
const QMetaEnum CanDataProcessorBasis::WarningMetaEnum = QMetaEnum::fromType<CanDataProcessorBasis::Warning>();
const QMetaEnum CanDataProcessorBasis::NotificationMetaEnum = QMetaEnum::fromType<CanDataProcessorBasis::Notification>();

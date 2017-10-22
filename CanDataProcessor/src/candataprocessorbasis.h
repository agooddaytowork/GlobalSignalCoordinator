#ifndef CANDATAPROCESSORBASIS_H
#define CANDATAPROCESSORBASIS_H

#define CanDataProcessorBasisDbgEn 1

#include "shared/abstractstatemachinebasis.h"
#include "CanBusWorker/src/canbusworkerbasis.h"
#include "piLocalDBWorker/src/pilocaldbworkerbasis.h"
#include "CanProtocol/src/canprotocol.h"

class CanDataProcessorBasis : public AbstractStateMachineBasis
{
    Q_OBJECT
public:
    explicit CanDataProcessorBasis(QObject *parent = nullptr);
    ~CanDataProcessorBasis();

    enum Data {
        NoData = 0,
        disableAutoSignaller,
        enableAutoSignaller
    };
    Q_ENUM(Data)

    enum Error
    {
        NoError = 0,
        QSqlErrorOccured
    };
    Q_ENUM(Error)

    enum Warning
    {
        NoWarning = 0
    };
    Q_ENUM(Warning)

    enum Notification
    {
        NoNotification = 0,
        readyToWork
    };
    Q_ENUM(Notification)

    static const QMetaEnum DataMetaEnum;
    static const QMetaEnum ErrorMetaEnum;
    static const QMetaEnum WarningMetaEnum;
    static const QMetaEnum NotificationMetaEnum;

    bool isAutoSignallerEnabled = false;

    void uninitiatedCanDataProcessorOnEntry();
    void idleCanDataProcessorOnEntry();
    void runningCanDataProcessorOnEntry();
    void errorCanDataProcessorOnEntry();
    void clearError();

signals:

public slots:
    void In(const GlobalSignal &aGlobalSignal);
    void autoSignaller();
private:

    QSqlDatabase localDb;
    Error ErrorType = NoError;
    QString ErrorInfo;
    QSqlError lastQSqlError;

    quint32 currentGlobalID = 0;
    GlobalSignal autoGlobalSignalToCanBusWorker;

    void initiate();
    void dispose();
    void setError(const Error & anErrorType, const QString & anErrorInfo);

    bool openLocalDatabaseConnection();
    void closeLocalDatabaseConnection();
    void clearCache();
    bool goToNextGlobalID();
    bool execSQL(QSqlQuery *aQuery,const QString &aSQL,bool navigateFirstRecordForSELECT = true);
};

#endif // CANDATAPROCESSORBASIS_H

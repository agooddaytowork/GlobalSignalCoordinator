#ifndef SERIALPORTDATAPROCESSORBASIS_H
#define SERIALPORTDATAPROCESSORBASIS_H

#define SerialPortDataProcessorBasisDbgEn 1

#include "shared/abstractstatemachinebasis.h"
#include "piLocalDBWorker/src/pilocaldbworkerbasis.h"
#include "SerialPortWorker/src/serialportworkerbasis.h"
#include "BinaryProtocol/src/binaryprotocol.h"
#include "WindowProtocol/src/windowprotocol.h"

class SerialPortDataProcessorBasis : public AbstractStateMachineBasis
{
    Q_OBJECT
public:
    explicit SerialPortDataProcessorBasis(bool isAnUHV2, QObject *parent = nullptr);
    ~SerialPortDataProcessorBasis();

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

    void uninitiatedSerialPortDataProcessorOnEntry();
    void idleSerialPortDataProcessorOnEntry();    
    void runningSerialPortDataProcessorOnEntry();
    void errorSerialPortDataProcessorOnEntry();
    void clearError();
signals:

public slots:
    void In(const GlobalSignal &aGlobalSignal);
    void autoSignaller();
private:
    const bool isUHV2;

    QSqlDatabase localDb;
    Error ErrorType = NoError;
    QString ErrorInfo;
    QSqlError lastQSqlError;

    quint32 currentGlobalID = 0;
    quint8 currentPumpAddress = 0;
    quint8 currentPumpChannel = 0;
    //AutoSignaller Cache
    bool isPressureMeasured = false;
    bool isVoltageMeasured = false;
    bool isCurrentMeasured = false;
    piLocalDBWorkerBasis::PVIData currentPVIData;
    QByteArray QBAReadP;
    QByteArray QBAReadV;
    QByteArray QBAReadI;
    GlobalSignal autoGlobalSignalToUHV;

    void initiate();
    void dispose();
    void setError(const Error & anErrorType, const QString & anErrorInfo);
    bool openLocalDatabaseConnection();
    void closeLocalDatabaseConnection();
    void clearCache();
    bool arePVIDataReady();
    void clearAutoSignallerCache();
    bool goToNextGlobalID();
    void emitCurrentPVIDataToLocalDatabase();
    bool execSQL(QSqlQuery *aQuery,const QString &aSQL,bool navigateFirstRecordForSELECT = true);
};

#endif // SERIALPORTDATAPROCESSORBASIS_H

#ifndef GLOBALSIGNALCOORDINATORBASIS_H
#define GLOBALSIGNALCOORDINATORBASIS_H

#define GlobalSignalCoordinatorBasisDbgEn 1

#include "shared/abstractstatemachinebasis.h"
#include "SerialPortWorker/src/serialportworkerbasis.h"
#include "CanBusWorker/src/canbusworkerbasis.h"
#include "piLocalDBWorker/src/pilocaldbworkerbasis.h"
#include "SerialPortDataProcessor/src/serialportdataprocessorbasis.h"
#include "CanDataProcessor/src/candataprocessorbasis.h"

class GlobalSignalCoordinatorBasis : public AbstractStateMachineBasis
{
    Q_OBJECT
public:
    explicit GlobalSignalCoordinatorBasis(QObject *parent = nullptr);
    ~GlobalSignalCoordinatorBasis();

    enum Data
    {
        NoData = 0,
        enableAllAutoSignallers,
        enableAutoSignallerForUHV2SerialPort,
        enableAutoSignallerForUHV4SerialPort,
        enableAutoSignallerForCanBus,
        disableAllAutoSignallers,
        disableAutoSignallerForUHV2SerialPort,
        disableAutoSignallerForUHV4SerialPort,
        disableAutoSignallerForCanBus
    };
    Q_ENUM(Data)

    enum Error
    {
        NoError = 0
    };
    Q_ENUM(Error)

    enum Warning
    {
        NoWarning = 0
    };
    Q_ENUM(Warning)

    enum Notification
    {
        NoNotification = 0
    };
    Q_ENUM(Notification)

    static const QMetaEnum DataMetaEnum;
    static const QMetaEnum ErrorMetaEnum;
    static const QMetaEnum WarningMetaEnum;
    static const QMetaEnum NotificationMetaEnum;

    void uninitiatedGlobalSignalCoordinatorOnEntry();
    void runningGlobalSignalCoordinatorOnEntry();
    void errorGlobalSignalCoordinatorOnEntry();

    void clearError();

signals:
    void ToPiLocalDBWorker(const GlobalSignal &);
    void ToUHV2SerialPortWorker(const GlobalSignal &);
    void ToUHV4SerialPortWorker(const GlobalSignal &);
    void ToUHV2SerialPortDataProcessor(const GlobalSignal &);
    void ToUHV4SerialPortDataProcessor(const GlobalSignal &);
    void ToCanBusWorker(const GlobalSignal &);
    void ToCanDataProcessor(const GlobalSignal &);
public slots:
    void In(const GlobalSignal &aGlobalSignal);
private:
    Error ErrorType = NoError;
    QString ErrorInfo;

    bool isPiLocalDBWorkerReady = false;
    bool isUHV2SerialPortWorkerReady = false;
    bool isUHV4SerialPortWorkerReady  =false;
    bool isUHV2SerialPortDataProcessorReady = false;
    bool isUHV4SerialPortDataProcessorReady = false;
    bool isCanBusWorkerReady = false;
    bool isCanDataProcessorReady =false;

    void initiate();
    void dispose();
    void setError(const Error & anErrorType, const QString & anErrorInfo);
    void clearCache();
    bool areWorkersReady();
};

#endif // GLOBALSIGNALCOORDINATORBASIS_H

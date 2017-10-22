#ifndef PILOCALDBWORKER_H
#define PILOCALDBWORKER_H

#define piLocalDBWorkerDbgEn 1

#include <QStateMachine>
#include "pilocaldbworkerbasis.h"
#include "shared/directtransition.h"
#include "errorpilocaldbworker.h"
#include "idlepilocaldbworker.h"
#include "runningpilocaldbworker.h"
#include "sendjsonpilocaldbworker.h"
#include "setissentpilocaldbworker.h"
#include "uninitiatedpilocaldbworker.h"

class piLocalDBWorker : public QStateMachine
{
    Q_OBJECT
public:
    explicit piLocalDBWorker(QObject *parent = nullptr);
signals:
    void Out(const GlobalSignal &);
public slots:
    void In(const GlobalSignal &aGlobalSignal);
private:
    piLocalDBWorkerBasis * currentBasis = nullptr;

};

#endif // PILOCALDBWORKER_H

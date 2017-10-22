#ifndef CANDATAPROCESSOR_H
#define CANDATAPROCESSOR_H

#include "candataprocessorbasis.h"
#include "shared/directtransition.h"
#include "uninitiatedcandataprocessor.h"
#include "idlecandataprocessor.h"
#include "runningcandataprocessor.h"
#include "errorcandataprocessor.h"

class CanDataProcessor : public QStateMachine
{
    Q_OBJECT
public:
    explicit CanDataProcessor(QObject *parent = nullptr);
signals:
    void Out(const GlobalSignal &);
public slots:
    void In(const GlobalSignal &aGlobalSignal);
private:
    CanDataProcessorBasis *currentBasis = nullptr;
};

#endif // CANDATAPROCESSOR_H

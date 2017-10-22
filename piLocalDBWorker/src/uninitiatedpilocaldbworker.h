#ifndef UNINITIATEDPILOCALDBWORKER_H
#define UNINITIATEDPILOCALDBWORKER_H

#include "pilocaldbworkerbasis.h"

class uninitiatedPiLocalDBWorker : public QState
{
public:
    uninitiatedPiLocalDBWorker(piLocalDBWorkerBasis *parentBasis, QState *parent = nullptr);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    piLocalDBWorkerBasis *basisptr = nullptr;
};

#endif // UNINITIATEDPILOCALDBWORKER_H

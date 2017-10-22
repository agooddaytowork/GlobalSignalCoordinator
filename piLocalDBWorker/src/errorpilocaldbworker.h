#ifndef ERRORPILOCALDBWORKER_H
#define ERRORPILOCALDBWORKER_H

#include "pilocaldbworkerbasis.h"

class errorPiLocalDBWorker : public QState
{
public:
    errorPiLocalDBWorker(piLocalDBWorkerBasis *parentBasis,QState *parent = nullptr);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    piLocalDBWorkerBasis *basisptr = nullptr;
};

#endif // ERRORPILOCALDBWORKER_H

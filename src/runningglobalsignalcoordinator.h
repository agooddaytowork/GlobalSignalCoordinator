#ifndef RUNNINGGLOBALSIGNALCOORDINATOR_H
#define RUNNINGGLOBALSIGNALCOORDINATOR_H

#include "globalsignalcoordinatorbasis.h"

class runningGlobalSignalCoordinator : public QState
{
public:
    runningGlobalSignalCoordinator(GlobalSignalCoordinatorBasis *parentBasis, QState *parent = nullptr);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    GlobalSignalCoordinatorBasis *basisptr = nullptr;
};

#endif // RUNNINGGLOBALSIGNALCOORDINATOR_H

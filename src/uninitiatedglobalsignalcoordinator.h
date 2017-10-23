#ifndef UNINITIATEDGLOBALSIGNALCOORDINATOR_H
#define UNINITIATEDGLOBALSIGNALCOORDINATOR_H

#include "globalsignalcoordinatorbasis.h"

class uninitiatedGlobalSignalCoordinator : public QState
{
public:
    uninitiatedGlobalSignalCoordinator(GlobalSignalCoordinatorBasis *parentBasis, QState *parent = nullptr);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    GlobalSignalCoordinatorBasis *basisptr = nullptr;
};

#endif // UNINITIATEDGLOBALSIGNALCOORDINATOR_H

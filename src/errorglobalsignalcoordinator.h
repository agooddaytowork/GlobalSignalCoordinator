#ifndef ERRORGLOBALSIGNALCOORDINATOR_H
#define ERRORGLOBALSIGNALCOORDINATOR_H

#include "globalsignalcoordinatorbasis.h"

class errorGlobalSignalCoordinator : public QState
{
public:
    errorGlobalSignalCoordinator(GlobalSignalCoordinatorBasis *parentBasis, QState *parent = nullptr);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    GlobalSignalCoordinatorBasis *basisptr = nullptr;
};

#endif // ERRORGLOBALSIGNALCOORDINATOR_H

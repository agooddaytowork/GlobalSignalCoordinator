#ifndef IDLEGLOBALSIGNALCOORDINATOR_H
#define IDLEGLOBALSIGNALCOORDINATOR_H

#include "globalsignalcoordinatorbasis.h"
#include <QTimer>

class idleGlobalSignalCoordinator : public QState
{
public:
    idleGlobalSignalCoordinator(GlobalSignalCoordinatorBasis *parentBasis, QState *parent = nullptr);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    GlobalSignalCoordinatorBasis *basisptr = nullptr;
    QTimer timer;
};

#endif // IDLEGLOBALSIGNALCOORDINATOR_H

#ifndef GLOBALSIGNALCOORDINATOR_H
#define GLOBALSIGNALCOORDINATOR_H


class GlobalSignalCoordinator : public QStateMachine
{
    Q_OBJECT
public:
    explicit GlobalSignalCoordinator(QObject *parent = nullptr);

signals:

public slots:
};

#endif // GLOBALSIGNALCOORDINATOR_H

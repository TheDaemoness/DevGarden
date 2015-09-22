#ifndef BUILDCONTROLLER_H
#define BUILDCONTROLLER_H

#include <QObject>

class DGProjectLoader;

class BuildController : public QObject {
	Q_OBJECT
public:
	explicit BuildController(DGProjectLoader& pl, QObject *parent = 0);

signals:

public slots:
	void currentBuild();
	void currentRebuild();
private:
	DGProjectLoader* pl;
};

#endif // BUILDCONTROLLER_H

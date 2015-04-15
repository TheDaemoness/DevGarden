#ifndef DGCONTROLLER_H
#define DGCONTROLLER_H

#include <QObject>

class DGController : public QObject {
	Q_OBJECT
public:
	explicit DGController(QObject *parent = 0);

signals:

public slots:
	void openFolder();
	void openFiles();
	void saveFileAs();
};

#endif // DGCONTROLLER_H

#pragma once

#include <QObject>

class TileFactory : public QObject
{
	Q_OBJECT

public:
	TileFactory(QString desFolder);
	~TileFactory();

protected slots:
	void UpdateTileChanged(const QString& strDirectory);

private:
	QString m_desDirectory;
};

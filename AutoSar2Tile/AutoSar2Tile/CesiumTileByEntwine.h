#ifndef CESIUMTILEBYENTWINE_H
#define CESIUMTILEBYENTWINE_H

#include <QObject>

class CesiumTileByEntwine : public QObject
{
	Q_OBJECT

public:
	CesiumTileByEntwine(QString desFolder);
	~CesiumTileByEntwine();

protected slots:
	void UpdateTileChanged(const QString& strDirectory);

private:
	QString m_desDirectory;
	
};

#endif // CESIUMTILEBYENTWINE_H

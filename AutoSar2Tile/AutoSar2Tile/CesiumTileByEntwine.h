#ifndef CESIUMTILEBYENTWINE_H
#define CESIUMTILEBYENTWINE_H

#include <QObject>

class PointCloud;
class CesiumTileByEntwine : public QObject
{
	Q_OBJECT

public:
	CesiumTileByEntwine(QString desFolder);
	~CesiumTileByEntwine();

	QString TransformTxt2las(QString fileName);
	bool SaveToFile(PointCloud* pointcloud, QString filename);

	void UpdateTileChanged(const QString& strDirectory);

private:
	QString m_desDirectory;

	//! 需要切片的文件名称全路径
	QString m_inputFilenameForTile;
	
	bool isDataTransformSucess;
};

#endif // CESIUMTILEBYENTWINE_H

#pragma once

#include <QObject>

class OGCImageTileBygdal2tilesDotpy : public QObject
{
	Q_OBJECT

public:
	OGCImageTileBygdal2tilesDotpy(QString desFolder);
	~OGCImageTileBygdal2tilesDotpy();

	void UpdateTileChanged(const QString& strDirectory);

private:
	QString m_desDirectory;

	//! 需要切片的文件名称全路径
	QString m_inputFilenameForTile;
};

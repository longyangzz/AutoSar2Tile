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

	//! ��Ҫ��Ƭ���ļ�����ȫ·��
	QString m_inputFilenameForTile;
};

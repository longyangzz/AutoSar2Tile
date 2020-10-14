#include "TileFactory.h"

#include "QFileInfo"
#include "QDirIterator"
#include "PointCloud.h"
#include "QCoreApplication"
#include "Logger.h"

#include "QProcess"
#include "QTextStream"

#include "CesiumTileByEntwine.h"
#include "OGCImageTileBygdal2tilesDotpy.h"
#include "XMLSettingValueManager.h"
TileFactory::TileFactory(QString desFolder)
{
	m_desDirectory = desFolder;
}

TileFactory::~TileFactory()
{
}

void TileFactory::UpdateTileChanged(const QString& fileNmaeNew)
{
	Logger::Message(QStringLiteral("�ۺ���������������%1").arg(fileNmaeNew));
	//! �жϴ�����ļ���չ�� text�Ļ���ʹ��ָ���ֶ�תΪ��ɫֵ��תΪlas temp�ļ�������������Ƶ��ļ����ƣ����ݵ�bat��ִ��
	QString fileName = fileNmaeNew;
	QString extension = QFileInfo(fileName).suffix();

	while (!QFile(fileNmaeNew).exists())
	{
		Sleep(1000 * 10);
	}
	Sleep(1000 * 10);

	Logger::Message(QStringLiteral("�ȴ���ɣ���ʼ����"));
	//txtתlas
	if (extension == "txt" || extension.toUpper() == "CSV")
	{
		//! ִ��ת��Ϊlas��д�������ݸ�
		//! ���ݴ��������
		Logger::Message(QStringLiteral("������Ƭ��ʼ"));
		CesiumTileByEntwine cesiumTileByEntwine(m_desDirectory);
		cesiumTileByEntwine.UpdateTileChanged(fileNmaeNew);
	}
	else if (extension.toUpper() == "TIF" || extension.toUpper() == "TIFF") {
		Logger::Message(QStringLiteral("xyzӰ����Ƭ��ʼ"));
		OGCImageTileBygdal2tilesDotpy ogcImageTileBygdal2tilesDotpy(m_desDirectory);
		ogcImageTileBygdal2tilesDotpy.UpdateTileChanged(fileNmaeNew);
	}
}

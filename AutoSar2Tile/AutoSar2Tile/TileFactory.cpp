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
	//! 判断传入的文件扩展名 text的话，使用指定字段转为颜色值，转为las temp文件，更新输入点云的文件名称，传递到bat中执行
	QString fileName = fileNmaeNew;
	QString extension = QFileInfo(fileName).suffix();

	while (!QFile(fileNmaeNew).exists())
	{
		Sleep(1000 * 10);
	}
	Sleep(1000 * 10);
	//txt转las
	if (extension == "txt" || extension.toUpper() == "CSV")
	{
		//! 执行转换为las，写出并传递给
		//! 数据处理管理类
		CesiumTileByEntwine cesiumTileByEntwine(m_desDirectory);
		cesiumTileByEntwine.UpdateTileChanged(fileNmaeNew);
	}
	else if (extension.toUpper() == "TIF" || extension.toUpper() == "TIFF") {
		OGCImageTileBygdal2tilesDotpy ogcImageTileBygdal2tilesDotpy(m_desDirectory);
		ogcImageTileBygdal2tilesDotpy.UpdateTileChanged(fileNmaeNew);
	}
}

#include "OGCImageTileBygdal2tilesDotpy.h"

#include "ogr_spatialref.h"
#include "gdal.h"
#include <gdal_priv.h>
#include <cpl_conv.h>

#include "QFileInfo"
#include "QDirIterator"
#include "PointCloud.h"
#include "QCoreApplication"
#include "Logger.h"

#include "QProcess"
#include "QTextStream"
#include "QTime"

#include "XMLSettingValueManager.h"

OGCImageTileBygdal2tilesDotpy::OGCImageTileBygdal2tilesDotpy(QString desFolder)
{
	m_desDirectory = desFolder;
}

OGCImageTileBygdal2tilesDotpy::~OGCImageTileBygdal2tilesDotpy()
{
}

void OGCImageTileBygdal2tilesDotpy::UpdateTileChanged(const QString& fileNmaeNew)
{
	Sleep(1000 * 10);

	//! 1、启动gdal2tiles.py，传入文件参数
	QString baseDir = QDir::toNativeSeparators(QCoreApplication::applicationDirPath()).append("\\scripts");
	QString proPath = QDir::toNativeSeparators(QCoreApplication::applicationDirPath()).append("\\scripts\\gdal2tiles.py");
	QString runbatFile = QDir::toNativeSeparators(QCoreApplication::applicationDirPath()).append("\\scripts\\run.bat");

	QString pythonexepath = getGlobleSettingFieldValue("image", "python", "");
	QString zoom = getGlobleSettingFieldValue("image", "zoom", "2-10");

	if (!QFile(pythonexepath).exists())
	{
		Logger::Error(QStringLiteral("%1下python不可用").arg(pythonexepath));
		return;
	}

	isDirExist(m_desDirectory);
	clearTempFiles(m_desDirectory);
	//! 当前第三方库目录下写出个run.bat文件，并执行
	QFile file(runbatFile);
	if (!file.open(QFile::WriteOnly | QFile::Truncate))
	{
		Logger::Error(QStringLiteral("%1文件打开失败").arg(runbatFile));
		return;
	}
	QTextStream stream(&file);

	QString line = "cd " + baseDir;
	stream << line << "\r\n";

	line.clear();
	line.append(QString("%1 %2 %3 %4 --zoom=%5 --xyz").arg("start python").arg(proPath).arg(fileNmaeNew).arg(m_desDirectory).arg(zoom));
	stream << line << "\r\n";
	file.close();
	//3-带空格，使用带參模式。能够启动
	QProcess* process = new QProcess(this);

	QStringList arg;
	arg.append(QString("%1").arg(proPath));
	arg.append(QString("%1").arg(fileNmaeNew));
	arg.append(QString("%1").arg(m_desDirectory));
	arg.append(QString("--zoom"));
	arg.append(QString("%1").arg(zoom));
	arg.append(QString("%1").arg("--xyz"));
	
	bool status = process->startDetached(pythonexepath, arg);
	if (status)
	{
		Logger::Message(QStringLiteral("影像切片调用成功"));
		Logger::Message(QStringLiteral("输出目录为%1").arg(m_desDirectory));
	}
	else {
		Logger::Error(QStringLiteral("请确认是否配置了python环境"));
		Logger::Error(process->errorString());
	}
}
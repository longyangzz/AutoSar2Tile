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

	//! 1������gdal2tiles.py�������ļ�����
	QString baseDir = QDir::toNativeSeparators(QCoreApplication::applicationDirPath()).append("\\scripts");
	QString proPath = QDir::toNativeSeparators(QCoreApplication::applicationDirPath()).append("\\scripts\\gdal2tiles.py");
	QString runbatFile = QDir::toNativeSeparators(QCoreApplication::applicationDirPath()).append("\\scripts\\run.bat");

	QString pythonexepath = getGlobleSettingFieldValue("image", "python", "");
	QString zoom = getGlobleSettingFieldValue("image", "zoom", "2-10");

	if (!QFile(pythonexepath).exists())
	{
		Logger::Error(QStringLiteral("%1��python������").arg(pythonexepath));
		return;
	}

	isDirExist(m_desDirectory);
	clearTempFiles(m_desDirectory);
	//! ��ǰ��������Ŀ¼��д����run.bat�ļ�����ִ��
	QFile file(runbatFile);
	if (!file.open(QFile::WriteOnly | QFile::Truncate))
	{
		Logger::Error(QStringLiteral("%1�ļ���ʧ��").arg(runbatFile));
		return;
	}
	QTextStream stream(&file);

	QString line = "cd " + baseDir;
	stream << line << "\r\n";

	line.clear();
	line.append(QString("%1 %2 %3 %4 --zoom=%5 --xyz").arg("start python").arg(proPath).arg(fileNmaeNew).arg(m_desDirectory).arg(zoom));
	stream << line << "\r\n";
	file.close();
	//3-���ո�ʹ�ô���ģʽ���ܹ�����
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
		Logger::Message(QStringLiteral("Ӱ����Ƭ���óɹ�"));
		Logger::Message(QStringLiteral("���Ŀ¼Ϊ%1").arg(m_desDirectory));
	}
	else {
		Logger::Error(QStringLiteral("��ȷ���Ƿ�������python����"));
		Logger::Error(process->errorString());
	}
}
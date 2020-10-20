
#include <QtCore/QCoreApplication>

#include "QFile"
#include "QFileInfo"
#include "QDomDocument"
#include "QDir"
#include "FileMoniter.h"
#include "TileFactory.h"
#include "Logger.h"
#include "QDebug"

#include "XMLSettingValueManager.h"

#include "ReadCsvData.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	//������xml�ļ�
	//! ����ȫ�������ļ��е�setting.xml�е�itemstyle����ֵ
	QString srcFolder = getGlobleSettingFieldValue("inputFolder", "path", "D:\input");
	QString desFolder = getGlobleSettingFieldValue("outputFolder", "path", "D:\output");
	Logger::CreateNewLogFile(QCoreApplication::applicationName());
	if (!QDir(srcFolder).exists() || !QDir(desFolder).exists()) {
		Logger::Message(QStringLiteral("��ǰ���ӵ�����Ŀ¼Ϊ%1�����Ŀ¼Ϊ%2").arg(srcFolder).arg(desFolder));
		Logger::Message(QStringLiteral("��ȷ��Ŀ¼��Ч����������޷��������"));
	}

	//�� �����ļ���Ŀ¼�ı任���������ָ��Ŀ¼�µ�����
	FileMoniter fileMoniter(srcFolder, desFolder);

	//! ������Ƭ�������ַ���
	TileFactory tileFactory(desFolder);

	//�� ���ļ������࣬�����źŸ����ݴ�������´���
	QObject::connect(&fileMoniter, SIGNAL(sigCommitReconRequest(const QString&)), &tileFactory, SLOT(UpdateTileChanged(const QString&)));

	
	//! ��ǰ����·��
	QString curDirectory = QDir::toNativeSeparators(QCoreApplication::applicationDirPath());
	Logger::Message(QStringLiteral("��Ȩ���У�������˹��"));
	Logger::Message(QStringLiteral("��ǰ����Ŀ¼Ϊ%1��ֻ����.txt | .csv�ļ���������Ŀ¼��").arg(curDirectory));
	Logger::Message(QStringLiteral("��ǰ���ӵ�����Ŀ¼Ϊ%1��ֻ����.txt�ļ�  | .csv�ļ���������Ŀ¼��").arg(srcFolder));
	Logger::Message(QStringLiteral("��ǰ�������Ŀ¼Ϊ%1��ֻ����.txt�ļ���������Ŀ¼��").arg(desFolder));

	//! ����ĳ������

	return a.exec();
}


#include <QtCore/QCoreApplication>

#include "QFile"
#include "QFileInfo"
#include "QDomDocument"
#include "QDir"
#include "FileMoniter.h"
#include "CesiumTileByEntwine.h"
#include "Logger.h"
#include "QDebug"

QString getGlobleSettingFieldValue(QString xmlkey, QString attrskey, QString attrsValueDefault)
{
	//! ����ȫ�������ļ��е�setting.xml�е�itemstyle����ֵ
	QString filePath = QString("%1/%2").arg(qApp->applicationDirPath()).arg("setting.xml");
	QString attsValue = attrsValueDefault;
	QFile xmlFile(filePath);
	if (!xmlFile.open(QIODevice::ReadOnly | QFile::Text))
	{
		attsValue = attrsValueDefault;
	}

	QDomDocument doc;
	if (!doc.setContent(&xmlFile))
	{
		xmlFile.close();
	}
	xmlFile.close();

	QDomElement root = doc.documentElement(); //���ظ��ڵ�
	QDomElement child = root.firstChildElement();
	QString aa = root.tagName();

	while (!child.isNull())
	{
		if (child.isElement())
		{
			QDomElement element = child.toElement();

			QString tagName = element.tagName();
			QString tempmenuitemStyle = element.attribute(attrskey);
			if (tagName == xmlkey && !tempmenuitemStyle.isEmpty())
			{
				attsValue = tempmenuitemStyle;
			}

			QDomNodeList nodeList = element.childNodes();
			for (int i = 0; i != nodeList.count(); ++i)
			{
				QDomElement ele = nodeList.at(i).toElement();
				QString tagName = ele.tagName();
				QString type = ele.attribute("type");
			}
		}

		child = child.nextSiblingElement();
	}

	return attsValue;
}

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

	//! ���ݴ��������
	CesiumTileByEntwine cesiumTileByEntwine(desFolder);

	//�� ���ļ������࣬�����źŸ����ݴ�������´���
	QObject::connect(&fileMoniter, SIGNAL(sigCommitReconRequest(const QString&)), &cesiumTileByEntwine, SLOT(UpdateTileChanged(const QString&)));

	
	//! ��ǰ����·��
	QString curDirectory = QDir::toNativeSeparators(QCoreApplication::applicationDirPath());
	Logger::Message(QStringLiteral("��ǰ���ӵ�����Ŀ¼Ϊ��ֻ����.txt�ļ���������Ŀ¼��") + curDirectory);
	return a.exec();
}

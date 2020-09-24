
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
	//! 解析全局配置文件中的setting.xml中的itemstyle配置值
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

	QDomElement root = doc.documentElement(); //返回根节点
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

	//！解析xml文件
	//! 解析全局配置文件中的setting.xml中的itemstyle配置值
	QString srcFolder = getGlobleSettingFieldValue("inputFolder", "path", "D:\input");
	QString desFolder = getGlobleSettingFieldValue("outputFolder", "path", "D:\output");
	Logger::CreateNewLogFile(QCoreApplication::applicationName());
	if (!QDir(srcFolder).exists() || !QDir(desFolder).exists()) {
		Logger::Message(QStringLiteral("当前监视的数据目录为%1，输出目录为%2").arg(srcFolder).arg(desFolder));
		Logger::Message(QStringLiteral("请确认目录有效，否则程序无法正常监控"));
	}

	//！ 监视文件夹目录的变换，则处理更新指定目录下的内容
	FileMoniter fileMoniter(srcFolder, desFolder);

	//! 数据处理管理类
	CesiumTileByEntwine cesiumTileByEntwine(desFolder);

	//！ 绑定文件监视类，发送信号给数据处理类更新处理
	QObject::connect(&fileMoniter, SIGNAL(sigCommitReconRequest(const QString&)), &cesiumTileByEntwine, SLOT(UpdateTileChanged(const QString&)));

	
	//! 当前工作路径
	QString curDirectory = QDir::toNativeSeparators(QCoreApplication::applicationDirPath());
	Logger::Message(QStringLiteral("当前监视的数据目录为，只监视.txt文件，不监视目录：") + curDirectory);
	return a.exec();
}

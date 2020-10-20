
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

	//! 数据切片处理管理分发类
	TileFactory tileFactory(desFolder);

	//！ 绑定文件监视类，发送信号给数据处理类更新处理
	QObject::connect(&fileMoniter, SIGNAL(sigCommitReconRequest(const QString&)), &tileFactory, SLOT(UpdateTileChanged(const QString&)));

	
	//! 当前工作路径
	QString curDirectory = QDir::toNativeSeparators(QCoreApplication::applicationDirPath());
	Logger::Message(QStringLiteral("版权所有：北京富斯德"));
	Logger::Message(QStringLiteral("当前工作目录为%1，只监视.txt | .csv文件，不监视目录：").arg(curDirectory));
	Logger::Message(QStringLiteral("当前监视的数据目录为%1，只监视.txt文件  | .csv文件，不监视目录：").arg(srcFolder));
	Logger::Message(QStringLiteral("当前数据输出目录为%1，只监视.txt文件，不监视目录：").arg(desFolder));

	//! 测试某个函数

	return a.exec();
}

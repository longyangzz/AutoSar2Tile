#include "XMLSettingValueManager.h"

#include "QFileInfo"
#include "QDirIterator"
#include "PointCloud.h"
#include "QCoreApplication"
#include "Logger.h"

#include "QProcess"
#include "QTextStream"
#include "QTime"

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

void Sleep(unsigned int msec)
{
	QTime reachTime = QTime::currentTime().addMSecs(msec);
	while (QTime::currentTime() < reachTime)
	{
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
	}

}

bool isDirExist(QString fullPath)
{
	QDir dir(fullPath);
	if (dir.exists())
	{
		return true;
	}
	else
	{
		bool ok = dir.mkdir(fullPath);//只创建一级子目录，即必须保证上级目录存在
		return ok;
	}
}

void clearTempFiles(const QString& temp_path)
{


	QDir Dir(temp_path);
	QStringList filters;
	filters << QString("*.xml") << QString("*.html");
	Dir.setFilter(QDir::Files | QDir::NoSymLinks); //设置类型过滤器，只为文件格式
	Dir.setNameFilters(filters);

	if (!Dir.exists())
	{
		Logger::Message(QStringLiteral("临时文件文件夹%1不存在").arg(temp_path));
		return;
	}

	// 第三个参数是QDir的过滤参数，这三个表示收集所有文件和目录，且不包含"."和".."目录。
	// 因为只需要遍历第一层即可，所以第四个参数填QDirIterator::NoIteratorFlags
	QDirIterator DirsIterator(temp_path, filters, QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
	while (DirsIterator.hasNext())
	{
		
		if (QFileInfo(DirsIterator.filePath()).suffix() == "config")
		{
			//DirsIterator.next();
			//qDebug() << "filePath00:" << DirsIterator.filePath();  //包含文件名的文件的全路径
		}
		
		if (!Dir.remove(DirsIterator.next())) // 删除文件操作如果返回否，那它就是目录
		{
			//qDebug() << "filePath:" << DirsIterator.filePath();  //包含文件名的文件的全路径
			QDir(DirsIterator.filePath()).removeRecursively(); // 删除目录本身以及它下属所有的文件及目录
		}
		
	}
}
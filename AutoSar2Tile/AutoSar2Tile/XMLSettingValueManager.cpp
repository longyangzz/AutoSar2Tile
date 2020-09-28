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
		bool ok = dir.mkdir(fullPath);//ֻ����һ����Ŀ¼�������뱣֤�ϼ�Ŀ¼����
		return ok;
	}
}

void clearTempFiles(const QString& temp_path)
{


	QDir Dir(temp_path);
	QStringList filters;
	filters << QString("*.xml") << QString("*.html");
	Dir.setFilter(QDir::Files | QDir::NoSymLinks); //�������͹�������ֻΪ�ļ���ʽ
	Dir.setNameFilters(filters);

	if (!Dir.exists())
	{
		Logger::Message(QStringLiteral("��ʱ�ļ��ļ���%1������").arg(temp_path));
		return;
	}

	// ������������QDir�Ĺ��˲�������������ʾ�ռ������ļ���Ŀ¼���Ҳ�����"."��".."Ŀ¼��
	// ��Ϊֻ��Ҫ������һ�㼴�ɣ����Ե��ĸ�������QDirIterator::NoIteratorFlags
	QDirIterator DirsIterator(temp_path, filters, QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
	while (DirsIterator.hasNext())
	{
		
		if (QFileInfo(DirsIterator.filePath()).suffix() == "config")
		{
			//DirsIterator.next();
			//qDebug() << "filePath00:" << DirsIterator.filePath();  //�����ļ������ļ���ȫ·��
		}
		
		if (!Dir.remove(DirsIterator.next())) // ɾ���ļ�����������ط���������Ŀ¼
		{
			//qDebug() << "filePath:" << DirsIterator.filePath();  //�����ļ������ļ���ȫ·��
			QDir(DirsIterator.filePath()).removeRecursively(); // ɾ��Ŀ¼�����Լ����������е��ļ���Ŀ¼
		}
		
	}
}
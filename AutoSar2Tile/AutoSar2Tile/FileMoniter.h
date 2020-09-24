#ifndef FILEMONITER_H
#define FILEMONITER_H

#include <QObject>
#include "QFile"
#include "QFileInfo"
#include "QStringList"

class QFileSystemWatcher;
class FileMoniter : public QObject
{
	Q_OBJECT
public:
	FileMoniter(const QString& strDirectory, QString desFolder);
	~FileMoniter();

signals:
	void sigCommitReconRequest(const QString& strFilePath);

protected slots:
	void slotDirectoryChanged(const QString& strDirectory);

private:
	QStringList GetFileNames(const QFileInfoList& fileInfoList);

private:
	QString m_strMonitorDirectory;
	QStringList m_strListFileNames;
	QFileSystemWatcher* m_pDirectoryWatcher;

	QString m_desDirectory;
private:
	
};

#endif // FILEMONITER_H

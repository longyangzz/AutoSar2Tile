#include "FileMoniter.h"
#include "QFileSystemWatcher"
#include "QDir"

FileMoniter::FileMoniter(const QString& strDirectory, QString desDirectory)
{
	m_strListFileNames.clear();
	m_strListFileNames = QDir(strDirectory).entryList();
	m_strMonitorDirectory = strDirectory;
	m_desDirectory = desDirectory;
	m_pDirectoryWatcher = new QFileSystemWatcher(this);
	m_pDirectoryWatcher->addPath(strDirectory);

	connect(m_pDirectoryWatcher, SIGNAL(directoryChanged(const QString&)), this, SLOT(slotDirectoryChanged(const QString&)));
}

FileMoniter::~FileMoniter()
{
	m_strListFileNames.clear();
}

void FileMoniter::slotDirectoryChanged(const QString&)
{
	QStringList strListFileNames; //To save new file names
	QFileInfoList fileInfoList = QDir(m_strMonitorDirectory).entryInfoList();

	for (int n = 0; n < fileInfoList.size(); n++)
	{
		QFileInfo fileInfo = fileInfoList[n];
		if (fileInfo.fileName().compare(".") == 0 || fileInfo.fileName().compare("..") == 0)
			continue;

		if (fileInfo.isDir())
			continue;

		QString strFileName = fileInfo.fileName();
		if (!m_strListFileNames.contains(strFileName))
			strListFileNames << strFileName;
	}

	if (strListFileNames.isEmpty())
	{
		m_strListFileNames.clear();
		m_strListFileNames = GetFileNames(fileInfoList);
		return;
	}

	for (int n = 0; n < strListFileNames.size(); n++)
	{
		//commit recon request
		QString strFilePath = m_strMonitorDirectory + "/" + strListFileNames[n];
		emit sigCommitReconRequest(strFilePath);
	}

	m_strListFileNames.clear();
	m_strListFileNames = GetFileNames(fileInfoList);
	return;
}

QStringList FileMoniter::GetFileNames(const QFileInfoList& fileInfoList)
{
	QStringList strLstNames;
	int nFileNum = fileInfoList.size();
	for (int n = 0; n < nFileNum; n++)
	{
		QFileInfo fileInfo = fileInfoList[n];
		strLstNames << fileInfo.fileName();
	}
	return strLstNames;
}

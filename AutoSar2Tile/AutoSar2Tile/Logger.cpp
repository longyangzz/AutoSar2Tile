#include "Logger.h"

//!qt
#include "QFile"
#include "QTextStream"
#include "QCoreApplication"
#include "QTime"
#include "QDebug"

//唯一的日志实体
static Logger* s_pInstance = nullptr;
static QTextEdit* s_UIConsole = nullptr;

QString Logger::logFileName = LOG_NAME;

Logger* Logger::Instance()
{
    return s_pInstance;
}

void Logger::Register(Logger* pInstance)
{
	s_pInstance = pInstance;
}

void Logger::Message(const QString& rsMsg)
{
	if (s_pInstance)
		s_pInstance->ShowMessage(rsMsg, eMessageLog);

	Writeln(QString("%1:%2").arg("Msg").arg(rsMsg));
}

void Logger::Warning(const QString& rsWarning)
{
	if (s_pInstance)
		s_pInstance->ShowMessage(rsWarning, eWarningLog);

	Writeln(QString("%1:%2").arg("Warning").arg(rsWarning));
}

void Logger::Error(const QString& rsError)
{
	if (s_pInstance)
		s_pInstance->ShowMessage(rsError, eErrorLog);

	Writeln(QString("%1:%2").arg("Error").arg(rsError));
}

void Logger::MessageDebug(const QString& rsMsg)
{
#ifdef _DEBUG
	if (s_pInstance)
		s_pInstance->ShowMessage(rsMsg, eMessageLog);
#endif

	Writeln(QString("%1:%2").arg("Msg").arg(rsMsg));
}

void Logger::WarningDebug(const QString& rsWarning)
{
#ifdef _DEBUG
	if (s_pInstance)
		s_pInstance->ShowMessage(rsWarning, eWarningLog);
#endif

	Writeln(QString("%1:%2").arg("Warning").arg(rsWarning));
}

void Logger::ErrorDebug(const QString& rsError)
{
#ifdef _DEBUG
	if (s_pInstance)
		s_pInstance->ShowMessage(rsError, eErrorLog);
#endif
	Writeln(QString("%1:%2").arg("Error").arg(rsError));
}

void Logger::CreateNewLogFile(const QString logFile /* = LOG_NAME */)
{
	//Open a new empty logfile:
	QString appDir = QCoreApplication::applicationDirPath() + "/";
	logFileName = QString("%1%2%3").arg(appDir).arg(logFile).arg(".log");
	
	QFile file(logFileName);

	QString version = QCoreApplication::applicationVersion();
	QString vstate = "Debug";
#ifdef _DEBUG
	vstate = "Debug";
#else
	vstate = "Release";
#endif
	if (file.open(QIODevice::WriteOnly | QIODevice::Append))
	{
		QTextStream out(&file);
		out << "*********************************************************************\n"
			<< QStringLiteral("**                鼎创科技 - Logfile               **") << endl
			<< QString("**                 %1                          ").arg(QDateTime::currentDateTime().toString()) << endl
			<< "*********************************************************************\n"
			<< "Version: " << version << " " << vstate << endl << endl;

		file.flush();
	}
	else
	{
		perror("Unable to open logfile!");
	}

	file.close();
}

void Logger::Writeln(const QString aValue)
{
	//! 打开写入
	QFile file(logFileName);
	if (!file.exists())
	{
		return;
	}
	
	if (file.open(QIODevice::WriteOnly | QIODevice::Append))
	{
		QTextStream out(&file);
		out << aValue << endl
			<< "--------------------------------------- "
			<< QTime::currentTime().toString() << endl;
		file.flush();
	}

	file.close();

	qDebug() << aValue;
}

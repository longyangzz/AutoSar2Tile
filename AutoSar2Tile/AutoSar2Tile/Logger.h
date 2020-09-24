#ifndef LOGGER_H
#define LOGGER_H


//Qt
#include "QString"

#define LOG_NAME "Log"

class QTextEdit;
//��־��¼��
class Logger
{
public:

	//��������
	virtual ~Logger() {};

	//����һ��Ψһ����־
	static Logger* Instance();

	//ע����־
	static void Register(Logger* pInstance);


	//���������Ϣ
	static void Message(const QString& rsMsg);
	static void MessageDebug(const QString& rsMsg);

	//���������Ϣ
	static void Warning(const QString& rsWarning);
	static void WarningDebug(const QString& rsWarning);

	//���������Ϣ
	static void Error(const QString& rsError);
	static void ErrorDebug(const QString& rsError);

	//! ��־�ļ����ļ�����
	static void CreateNewLogFile(const QString logFile = LOG_NAME);

	static void Writeln(const QString aValue);
protected:

	//��Ϣ����
	enum LogLevel
	{
		eMessageLog				=	0x00001000	,
		eWarningLog				=	0x00002000	,
		eErrorLog				=	0x00004000
	};

	//��ʾ��Ϣ
	void virtual ShowMessage(const QString& msg, LogLevel level = eMessageLog) = 0;

	static QString logFileName;
};

#endif
#ifndef FTPMANAGER_INCLUDE_H
#define FTPMANAGER_INCLUDE_H

#include <string>
#include <list>

#define FTP_API int

typedef void (*PROGRESSCALLBACK)(int nEvent, unsigned long nHasTransSzie, unsigned long nTotalSzie, const void* userdata);

typedef enum 
{
	type_invalid = 0,
	type_file = 0x01,
	type_dir  = 0x02,
	type_link = 0x04,
	type_pipe = 0x08,
}FTPFILEITEMTYPE;

typedef enum
{
	auth_none	= 0x00,	// ��Ȩ��
	auth_exe	= 0x01, // ��ִ��
	auth_write	= 0x02, // ��д
	auth_read	= 0x04, // �ɶ�
}FTPFILEITEM_AUTHFLAG;

typedef struct FTPFILEITEM
{
	FTPFILEITEM()
	{
		m_szItemName = 0;
		m_eType = type_invalid;
		m_eAuth = auth_none;
	}
	~FTPFILEITEM()
	{
		clear(); 
	}
	FTPFILEITEM(const FTPFILEITEM &tCopy)
	{
		m_szItemName = 0;
		m_eType = type_invalid;
		m_eAuth = auth_none;

		SetAuthFlag(tCopy.GetAuthFlag()); 
		SetType(tCopy.GetType()); 
		SetName(tCopy.GetName());
	}
	void clear()
	{
		m_eType = type_invalid;
		clearname();
	}
	void clearname()
	{
		if(m_szItemName && strlen(m_szItemName))
		{
			free(m_szItemName);
		}
		m_szItemName = 0;
	}
	void	SetType(const FTPFILEITEMTYPE eType)
	{
		m_eType = eType; 
	}
	const FTPFILEITEMTYPE	GetType() const
	{
		return m_eType; 
	}
	void	SetName(const char*	strName)
	{
		clearname();
		if(strName && strlen(strName))
		{
			m_szItemName = strdup(strName);
		}
	}
	const char* GetName() const
	{
		return m_szItemName; 
	}
	const FTPFILEITEM_AUTHFLAG	GetAuthFlag() const
	{
		return m_eAuth; 
	}
	void	SetAuthFlag(const FTPFILEITEM_AUTHFLAG eAuth)
	{
		m_eAuth = eAuth;
	}
	FTPFILEITEM& operator= (const FTPFILEITEM& tRight)
	{
		if(this == &tRight)
			return *this; 

		SetAuthFlag(tRight.GetAuthFlag()); 
		SetType(tRight.GetType()); 
		SetName(tRight.GetName());
		return *this; 
	}
protected:
	FTPFILEITEMTYPE			m_eType; 
	FTPFILEITEM_AUTHFLAG	m_eAuth;
	char*					m_szItemName;
}FTPFILEITEM,*LPFTPFILEITEM;

class CFTPManager
{
public:
	enum type
	{
		binary = 0x31,
		ascii,
	};

	CFTPManager();
	virtual ~CFTPManager();

#ifdef _WINDOWS_

#endif

	// ��½FTPServer
	FTP_API	login2Server(const char* strServerIP,const char* strUserName , const char* strPassword);
	// �˳���½
	FTP_API	quitServer();
	// ��ȡ��ǰĿ¼
	const char*	getCurrentPath();
	// ���ô����ʽ �����ƻ���asscii
	FTP_API	setTransferMode(type mode);
	// ���ñ���ģʽ
	const char*	Pasv(); 
	// ��ȡĿ¼���ļ������ļ����б�
	const char* Dir(const char* strPath);
	FTP_API	Dir(const char* strPath , std::list<FTPFILEITEM> &vDirs);
	// CD����
	FTP_API CD(const char* strPath);
	//����Զ���ļ���ֻ�Ǵ�������д������
	FTP_API CreatRemoteFile(const char* strRemoteFile);
	// ɾ���ļ�
	FTP_API	DeleteRemoteFile(const char* strRemoteFile);
	// ɾ��Ŀ¼
	FTP_API DeleteDirectory(const char* strRemoteDir); 
	// ����Ŀ¼
	FTP_API CreateRemoteDirectory(const char* strRemoteDir); 
	// ������
	FTP_API Rename(const char* strRemoteFile,const char* strNewFile); 
	// ��ȡ�ļ���С
	long getFileSize(const char* strRemoteFile); 
	// �ر�����
	void Close(); 
	// �����ļ�
	FTP_API Get(const char* strRemoteFile , const char* strLocalFile, PROGRESSCALLBACK pProgressCB = NULL, const void* pUserData = NULL); 
	// �ϴ��ļ�
	FTP_API Put(const char* strLocalFile, const char* strRemoteFile, PROGRESSCALLBACK pProgressCB = NULL, const void* pUserData = NULL); 

	const char*		GetResultString(FTP_API nRet); 
	//ֹͣ����
	void Stop();
	//���ó�ʱѡ��
	void SetTimeOut(int nTimeData);

private:
	FTP_API			Connect(int sockfd , const char* strServerIP , unsigned long dwPort); 
	const char*		parseCommand(const unsigned int command, const char* strParam);
	const char*		serverResponse(int sockfd);
	FTP_API			getData(int fd, char *strBuf, unsigned long length);
	FTP_API			Send(int fd , const char* strBuf , const unsigned long length);
	FTP_API			createDataLink(int data_fd);
	FTP_API			ParseString(std::list<std::string> strArray, unsigned long & nPort ,std::string & strServerIp);
	FILE*			createLocalFile(const char*	strLocalFile);
	FTP_API			downLoad(const char* strRemoteFile, const char* strLocalFile, PROGRESSCALLBACK pProgressCB = NULL, const void* pUserData = NULL, const int pos = 0 , const unsigned int length = 0 );
	FTP_API			parseResponse(const char* str);

private:
	int				m_cmdSocket;
	std::string		m_strUserName;
	std::string		m_strPassWord;
	std::string		m_strServerIP;
	unsigned long	m_nServerPort;
	std::string		m_strResponse;
	std::string		m_commandStr;
	unsigned long	m_nCurrentCommand;
	bool			m_bLogin;
	bool			m_bStop;
	int				m_nTimeOut;  //��ʱ�ȴ�
};


#endif // FTPMANAGER_INCLUDE_H

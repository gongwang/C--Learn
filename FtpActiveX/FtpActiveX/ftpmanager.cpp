#include "ftpmanager.h"


#ifdef WIN32
#include <WinSock2.h>
#include <fcntl.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <errno.h>  
#include <sys/stat.h>  
#include <assert.h>
//#include "logprintf/logprintf.h"

#define close(s) closesocket(s)
#define SHUT_RDWR SD_BOTH
#define ioctl(s,cmd,argp) ioctlsocket(s,cmd,argp)
#define bzero(data,size) ZeroMemory(data,size)

typedef int socklen_t; 

#else

#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>
#include <assert.h>

#endif

#include <map>

#ifndef INVALID_SOCKET
#define INVALID_SOCKET				-1
#endif

#define MAX_PATH					260
#define trace						printf

#define FTP_PARAM_BASE
#define FTP_DEFAULT_PORT			"21"						//FTP默认端口号
#define FTP_DEFAULT_BUFFER			1024*10						//FTP下载缓冲默认大小
#define FTP_DEFAULT_PATH			"/mnt/dvs/"					//FTP默认保存路径

#define FTP_COMMAND_BASE				1000
#define FTP_COMMAND_END					FTP_COMMAND_BASE + 30
#define FTP_COMMAND_USERNAME			FTP_COMMAND_BASE + 1	//用户名
#define FTP_COMMAND_PASSWORD			FTP_COMMAND_BASE + 2	//密码
#define FTP_COMMAND_QUIT				FTP_COMMAND_BASE + 3	//退出
#define FTP_COMMAND_CURRENT_PATH		FTP_COMMAND_BASE + 4	// 获取文件路径
#define FTP_COMMAND_TYPE_MODE			FTP_COMMAND_BASE + 5	// 改变传输模式
#define FTP_COMMAND_PSAV_MODE			FTP_COMMAND_BASE + 6	// 被动端口模式
#define FTP_COMMAND_DIR					FTP_COMMAND_BASE + 7	// 获取文件列表
#define FTP_COMMAND_CHANGE_DIRECTORY	FTP_COMMAND_BASE + 8	// 改变路径
#define FTP_COMMAND_DELETE_FILE			FTP_COMMAND_BASE + 9	// 删除文件
#define FTP_COMMAND_DELETE_DIRECTORY	FTP_COMMAND_BASE + 10	// 删除目录/文件夹
#define FTP_COMMAND_CREATE_DIRECTORY	FTP_COMMAND_BASE + 11	// 创建目录/文件夹
#define FTP_COMMAND_RENAME_BEGIN		FTP_COMMAND_BASE  +12	// 开始重命名
#define FTP_COMMAND_RENAME_END			FTP_COMMAND_BASE + 13	// 重命名结束
#define FTP_COMMAND_FILE_SIZE			FTP_COMMAND_BASE + 14	// 获取文件大小
#define FTP_COMMAND_DOWNLOAD_POS		FTP_COMMAND_BASE + 15	// 下载文件从指定位置开始
#define FTP_COMMAND_DOWNLOAD_FILE		FTP_COMMAND_BASE + 16	// 下载文件
#define FTP_COMMAND_UPLOAD_FILE			FTP_COMMAND_BASE + 17	// 上传文件
#define FTP_COMMAND_APPEND_FILE			FTP_COMMAND_BASE + 18	// 追加上载文件	

std::map<long , std::string>	mapFTPError;

class FTPError
{
public:
	FTPError()
	{
		mapFTPError[-1]	 = "SOCKET错误";
		mapFTPError[150] = "文件状态良好,打开数据连接";
		mapFTPError[200] = "命令成功";
		mapFTPError[202] = "命令未实现";
		mapFTPError[211] = "系统状态或系统帮助响应";
		mapFTPError[212] = "目录状态";
		mapFTPError[213] = "文件状态";
		mapFTPError[214] = "帮助信息,信息仅对人类用户有用";
		mapFTPError[215] = "名字系统类型";
		mapFTPError[220] = "对新用户服务准备好";
		mapFTPError[221] = "服务关闭控制连接,可以退出登录";
		mapFTPError[225] = "数据连接打开,无传输正在进行";
		mapFTPError[226] = "关闭数据连接,请求的文件操作成功";
		mapFTPError[227] = "进入被动模式";
		mapFTPError[230] = "用户登录成功";
		mapFTPError[250] = "请求的文件操作完成";
		mapFTPError[257] = "创建”PATHNAME”";
		mapFTPError[331] = "用户名正确,需要口令";
		mapFTPError[332] = "登录时需要帐户信息";
		mapFTPError[350] = "请求的文件操作需要进一步命令";
		mapFTPError[400] = "Bad request失败请求错误";
		mapFTPError[401] = "Unauthorized未经过认证的错误";
		mapFTPError[403] = "Forbidden禁止访问错误";
		mapFTPError[404] = "Not found没有找到错误";
		mapFTPError[421] = "连接用户过多";
		mapFTPError[425] = "不能打开数据连接";
		mapFTPError[426] = "关闭连接,中止传输";
		mapFTPError[450] = "请求的文件操作未执行";
		mapFTPError[451] = "中止请求的操作:有本地错误";
		mapFTPError[452] = "未执行请求的操作:系统存储空间不足";
		mapFTPError[500] = "格式错误,命令不可识别";
		mapFTPError[501] = "参数语法错误";
		mapFTPError[502] = "命令未实现";
		mapFTPError[503] = "命令顺序错误";
		mapFTPError[504] = "此参数下的命令功能未实现";
		mapFTPError[530] = "账号或密码错误";
		mapFTPError[532] = "存储文件需要帐户信息";
		mapFTPError[550] = "未执行请求的操作";
		mapFTPError[551] = "请求操作中止:页类型未知";
		mapFTPError[552] = "请求的文件操作中止,存储分配溢出";
		mapFTPError[553] = "未执行请求的操作:文件名不合法";
	}
	~FTPError(){}
};

FTPError cFptErr; 

void	PrintLog(char* szPrint)
{
#ifdef WIN32
	SYSTEMTIME	st; 
	GetSystemTime(&st); 
	char szDebug[1024] = {0};
	memset(szDebug , 0 , 1024);
	sprintf(szDebug , "[%02d:%02d:%02d:%03d] %s\n" , st.wHour , st.wMinute , st.wSecond , st.wMilliseconds , szPrint);
	OutputDebugStringA(szDebug);
#endif
}

static int SplitString( std::string strSrc, std::list<std::string> &strArray , std::string strFlag)
{
	int pos = 1; 

	while((pos = (int)strSrc.find_first_of(strFlag.c_str())) >= 0) 
	{
		strArray.insert(strArray.end(), strSrc.substr(0 , pos));
		strSrc = strSrc.substr(pos + 1, strSrc.length() - pos - 1); 
	}

	strArray.insert(strArray.end(), strSrc.substr(0, strSrc.length()));

	return 0; 
}

std::string ftp_trim(std::string &strSrc , std::string strFlag)
{
	if (!strSrc.empty()) 
	{
		strSrc.erase(0,strSrc.find_first_not_of(strFlag));
		strSrc.erase(strSrc.find_last_not_of(strFlag) + 1);
	}
	return strSrc; 
}

static int SplitString2( std::string strSrc, std::list<std::string> &strArray , std::string strFlag)
{
	int pos = 1; 

	while((pos = (int)strSrc.find_first_of(strFlag.c_str())) >= 0) 
	{
		strArray.insert(strArray.end(), strSrc.substr(0 , pos));
		strSrc = strSrc.substr(pos + 1, strSrc.length() - pos - 1); 

		ftp_trim(strSrc , " ");
	}

	strArray.insert(strArray.end(), strSrc.substr(0, strSrc.length()));

	return 0; 
}

CFTPManager::CFTPManager():
m_bLogin(false)
,m_cmdSocket(INVALID_SOCKET)
{
//	m_cmdSocket = socket(AF_INET, SOCK_STREAM, 0);
	m_nTimeOut = 100000;
}

CFTPManager::~CFTPManager()
{

}
FTP_API CFTPManager::Connect(int socketfd, const char* serverIP, unsigned long nPort)
{
	if (socketfd == INVALID_SOCKET)
	{
		return -1;
	}

	unsigned long argp = 1;
	int error = -1;
	int len = sizeof(int);
	struct sockaddr_in  addr;
	bool ret = false;
	timeval stime;
	fd_set  set;

	ioctl(socketfd, FIONBIO, &argp);  // 设置为非阻塞模式 

	memset(&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port	= htons(nPort);
	addr.sin_addr.s_addr = inet_addr(serverIP);
	bzero(&(addr.sin_zero), 8);

	trace("Address: %s %d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

	if (connect(socketfd, (struct sockaddr*)&addr, sizeof(struct sockaddr)) == -1)  //若直接返回 则说明正在进行TCP三次握手
	{
		stime.tv_sec = 20;	
		stime.tv_usec = 0;
		FD_ZERO(&set);
		FD_SET(socketfd, &set);

		if (select(socketfd + 1, NULL, &set, NULL, &stime) > 0)  
		{
			getsockopt(socketfd, SOL_SOCKET, SO_ERROR, (char*)&error, (socklen_t*)&len);
			if (error == 0)
			{
				ret = true;
			}
			else
			{
				ret = false;
			}
		}
	}
	else
	{	trace("Connect Immediately!!!\n");
	ret = true;
	}

	argp = 0;
	ioctl(socketfd, FIONBIO, &argp);

	if (!ret)
	{
		close(socketfd);
		fprintf(stderr, "cannot connect server!!\n");
		return -1;
	}

	//fprintf(stdout, "Connect!!!\n");

	return 0;
}

const char* CFTPManager::serverResponse(int sockfd)
{
	if (sockfd == INVALID_SOCKET)
	{
		return "";
	}

	int nRet = -1;
	char buf[MAX_PATH] = {0};

	m_strResponse.clear();

	while ((nRet = getData(sockfd, buf, MAX_PATH - 1)) > 0)
	{
		buf[MAX_PATH - 1] = '\0';
		m_strResponse += buf;
	}

	return m_strResponse.c_str();
}

FTP_API CFTPManager::parseResponse(const char* str)
{
	if(str == 0 || strlen(str) == 0)
		return 0;

	std::string strResponse = str; 

	std::string strData = strResponse.substr(0, 3);
	unsigned int val = atoi(strData.c_str());

	return val;
}

FTP_API CFTPManager::getData(int fd, char *strBuf, unsigned long length)
{
	assert(strBuf != NULL);

	if (fd == INVALID_SOCKET)
	{
		return -1;
	}

	memset(strBuf, 0, length);
	timeval stime;
	int nLen;

	stime.tv_sec = 0;
	stime.tv_usec = m_nTimeOut;

	fd_set	readfd;
	FD_ZERO( &readfd );
	FD_SET(fd, &readfd );

	PrintLog("Start Select");
	if (select(fd + 1, &readfd, 0, 0, &stime) > 0)
	{
		PrintLog("Start recv");
		if ((nLen = recv(fd, strBuf, length, 0)) > 0)
		{
			PrintLog("recv end");
			return nLen;
		}
		else
		{
			PrintLog("recv end");
			return -2;
		}
	}
	PrintLog("Select end");
	return 0;
}

FTP_API CFTPManager::Send(int fd, const char *cmd, const unsigned long len)
{
	if((FTP_COMMAND_USERNAME != m_nCurrentCommand) 
		&&(FTP_COMMAND_PASSWORD != m_nCurrentCommand)
		&&(!m_bLogin))
	{
		return -1;
	}

	timeval timeout;
	timeout.tv_sec  = 1;
	timeout.tv_usec = 0;

	fd_set  writefd;
	FD_ZERO(&writefd);  
	FD_SET(fd, &writefd);

	if(select(fd + 1, 0, &writefd , 0 , &timeout) > 0)
	{
		size_t nlen  = len; 
		int nSendLen = 0; 
		while (nlen >0) 
		{
			nSendLen = send(fd, cmd , (int)nlen , 0);

			if(nSendLen == -1) 
				return -2; 

			nlen = nlen - nSendLen;
			cmd +=  nSendLen;
		}
		return 0;
	}
	return -1;
}

const char* CFTPManager::parseCommand(const unsigned int command, const char* strParam)
{
	if (command < FTP_COMMAND_BASE || command > FTP_COMMAND_END)
	{
		return "";
	}

	std::string strCommandLine;

	m_nCurrentCommand = command;
	m_commandStr.clear();

	switch (command)
	{
	case FTP_COMMAND_USERNAME:
		strCommandLine = "USER ";
		break;
	case FTP_COMMAND_PASSWORD:
		strCommandLine = "PASS ";
		break;
	case FTP_COMMAND_QUIT:
		strCommandLine = "QUIT ";
		break;
	case FTP_COMMAND_CURRENT_PATH:
		strCommandLine = "PWD ";
		break;
	case FTP_COMMAND_TYPE_MODE:
		strCommandLine = "TYPE ";
		break;
	case FTP_COMMAND_PSAV_MODE:
		strCommandLine = "PASV ";
		break;
	case FTP_COMMAND_DIR:
		strCommandLine = "LIST ";
		break;
	case FTP_COMMAND_CHANGE_DIRECTORY:
		strCommandLine = "CWD ";
		break;
	case FTP_COMMAND_DELETE_FILE:
		strCommandLine = "DELE ";
		break;
	case FTP_COMMAND_DELETE_DIRECTORY:
		strCommandLine = "RMD ";
		break;
	case FTP_COMMAND_CREATE_DIRECTORY:
		strCommandLine = "MKD ";
		break;
	case FTP_COMMAND_RENAME_BEGIN:
		strCommandLine = "RNFR ";
		break;
	case FTP_COMMAND_RENAME_END:
		strCommandLine = "RNTO ";
		break;
	case FTP_COMMAND_FILE_SIZE:
		strCommandLine = "SIZE ";
		break;
	case FTP_COMMAND_DOWNLOAD_FILE:
		strCommandLine = "RETR ";
		break;
	case FTP_COMMAND_DOWNLOAD_POS:
		strCommandLine = "REST ";
		break;
	case FTP_COMMAND_UPLOAD_FILE:
		strCommandLine = "STOR ";
		break;
	case FTP_COMMAND_APPEND_FILE:
		strCommandLine = "APPE ";
		break;
	default :
		break;
	}

	strCommandLine += strParam;
	strCommandLine += "\r\n";

	m_commandStr = strCommandLine;
	trace("parseCommand: %s\n", m_commandStr.c_str());

	return m_commandStr.c_str();
}


FTP_API CFTPManager::login2Server( const char* strServerIP,const char* strUserName , const char* strPassword )
{
	std::string strPort;
	std::string serverIP = strServerIP; 
	int pos = serverIP.find_first_of(":");

	if (pos > 0)
	{
		strPort = serverIP.substr(pos + 1, serverIP.length() - pos);
	}
	else
	{
		pos = serverIP.length();
		strPort = FTP_DEFAULT_PORT;
	}

	m_strServerIP = serverIP.substr(0, pos);
	m_nServerPort = atol(strPort.c_str());

	trace("IP: %s port: %d\n", m_strServerIP.c_str(), m_nServerPort);

	PrintLog("Begin Connect");
	m_cmdSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (Connect(m_cmdSocket, m_strServerIP.c_str(), m_nServerPort) < 0)
	{
		return -1;
	}

	PrintLog("Wait Server response!");
	m_strResponse = serverResponse(m_cmdSocket);
	trace("@@@@Response: %s", m_strResponse.c_str());
	
	PrintLog("Start Parse Respone!");
	FTP_API nRet = parseResponse(m_strResponse.c_str());
	if(nRet < 200 || nRet >= 400)
		return nRet;

	if(strUserName == 0 || strlen(strUserName) == 0)
		return nRet;

	std::string strCommandLine = parseCommand(FTP_COMMAND_USERNAME, strUserName);
	m_strUserName = strUserName;

	PrintLog("Start Put username!");
	if (Send(m_cmdSocket, strCommandLine.c_str(),strCommandLine.size()) < 0)
	{
		return -1;
	}

	PrintLog("Wait username Response!");
	m_strResponse = serverResponse(m_cmdSocket);
	printf("Response: %s\n", m_strResponse.c_str());

	PrintLog("Start Parse Username Respone!");
	nRet = parseResponse(m_strResponse.c_str());

	if(nRet < 200 || nRet >= 400)
		return nRet; 

	if(strPassword == 0 || strlen(strPassword) == 0)
		return nRet;

	std::string strCmdLine = parseCommand(FTP_COMMAND_PASSWORD, strPassword);

	m_strPassWord = strPassword;

	PrintLog("Start Put Password!");

	if (Send(m_cmdSocket, strCmdLine.c_str() , strCmdLine.size()) < 0)
	{
		return -1;
	}

	PrintLog("Wait Password Response!");
	m_strResponse = serverResponse(m_cmdSocket);
	printf("Response: %s\n", m_strResponse.c_str());

	if(m_strResponse.size() == 0)
	{
		return 530; 
	}

	PrintLog("Start Parse Password Respone!");
	nRet = parseResponse(m_strResponse.c_str());

	if(nRet >= 200 && nRet < 300)
	{
		m_bLogin = true;
	}

	PrintLog("Login success!");
	return nRet ; 
}

FTP_API CFTPManager::quitServer(void)
{
	std::string strCmdLine = parseCommand(FTP_COMMAND_QUIT, "");
	if (Send(m_cmdSocket, strCmdLine.c_str() , strCmdLine.size()) < 0)
	{
		return -1;
	}
	else
	{
		m_strResponse = serverResponse(m_cmdSocket);
		printf("Response: %s\n", m_strResponse.c_str());

		return parseResponse(m_strResponse.c_str());
	}

}

const char* CFTPManager::getCurrentPath()
{
	std::string strCmdLine = parseCommand(FTP_COMMAND_CURRENT_PATH, "");

	if (Send(m_cmdSocket, strCmdLine.c_str(),strCmdLine.size()) < 0)
	{
		return "";
	}
	else
	{
		std::string strRet = serverResponse(m_cmdSocket);
		int nLeftPos = strRet.find('\"');
		int nRightPos = strRet.rfind('\"'); 
		m_strResponse = strRet;
		if(nLeftPos >= 0 && nRightPos > 0 && 
			nLeftPos != nRightPos)
		{
			std::string strDir = strRet.substr(nLeftPos + 1 , nRightPos - nLeftPos - 1);
			m_strResponse = strDir; 
		}
		return m_strResponse.c_str(); 
	}
}

FTP_API CFTPManager::setTransferMode(type mode)
{
	std::string strCmdLine;

	switch (mode)
	{
	case binary:
		strCmdLine = parseCommand(FTP_COMMAND_TYPE_MODE, "I");
		break;
	case ascii:
		strCmdLine = parseCommand(FTP_COMMAND_TYPE_MODE, "A");
		break;
	default:
		break;
	}

	if (Send(m_cmdSocket, strCmdLine.c_str(),strCmdLine.size()) < 0)
	{
		assert(false);
	}
	else
	{	
		m_strResponse  = serverResponse(m_cmdSocket);
		printf("@@@@Response: %s", m_strResponse.c_str());

		return parseResponse(m_strResponse.c_str());
	}
	return 0;
}

const char* CFTPManager::Pasv()
{
	std::string strCmdLine = parseCommand(FTP_COMMAND_PSAV_MODE, "");

	if (Send(m_cmdSocket, strCmdLine.c_str(),strCmdLine.size()) < 0)
	{
		return "";
	}
	else
	{
		m_strResponse = serverResponse(m_cmdSocket);

		return m_strResponse.c_str();
	}
}

const char* CFTPManager::Dir( const char* strPath )
{
	int dataSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (createDataLink(dataSocket) < 0)
	{
		return "";
	}
	// 
	std::string strCmdLine = parseCommand(FTP_COMMAND_DIR, strPath);

	if (Send(m_cmdSocket, strCmdLine.c_str() , strCmdLine.size()) < 0)
	{
		trace("@@@@Response: %s\n", serverResponse(m_cmdSocket));
		close(dataSocket);
		return "";
	}
	else
	{
		trace("@@@@Response: %s\n", serverResponse(m_cmdSocket));
		m_strResponse = serverResponse(dataSocket);

		trace("@@@@Response: \n%s\n", m_strResponse.c_str());
		close(dataSocket);

		return m_strResponse.c_str();
	}
}

FTP_API CFTPManager::Dir( const char* strPath , std::list<FTPFILEITEM> &vDirs )
{
	PrintLog("Start Create StreamSock!");
	int dataSocket = socket(AF_INET, SOCK_STREAM, 0);
	PrintLog("Start Create Data Link");
	FTP_API nRet = createDataLink(dataSocket);
	if (nRet < 0)
	{
		return nRet;
	}
	// 
	PrintLog("Start Parse Command");
	std::string strCmdLine = parseCommand(FTP_COMMAND_DIR, strPath);

	if (Send(m_cmdSocket, strCmdLine.c_str() , strCmdLine.size()) < 0)
	{
		trace("@@@@Response: %s\n", serverResponse(m_cmdSocket));
		close(dataSocket);
		return -1;
	}
	else
	{
		PrintLog("Server Response CmdSocket");
		trace("@@@@Response: %s\n", serverResponse(m_cmdSocket));

		PrintLog("Server Response DataSocket");
		m_strResponse = serverResponse(dataSocket);

		trace("@@@@Response: \n%s\n", m_strResponse.c_str());
		close(dataSocket);

		std::list<std::string> listDir; 
		SplitString(m_strResponse , listDir , "\n"); 

		std::list<std::string>::iterator itDir = listDir.begin(); 
		while(itDir != listDir.end())
		{
			std::list<std::string> listCmd; 
			SplitString2(*itDir , listCmd , " ");
			std::list<std::string>::iterator itCmd = listCmd.begin();
			std::string strAuth = *itCmd; 
			while(listCmd.size() >= 8 && 
				strAuth.size() >= 10)
			{
				FTPFILEITEM tFtpFile; 
				char cFileType = itCmd->at(0);
				if(cFileType == '-')
				{
					tFtpFile.SetType(type_file);
				}
				else if(cFileType == 'd')
				{
					tFtpFile.SetType(type_dir);
				}
				else if(cFileType == 'l')
				{
					tFtpFile.SetType(type_link); 
				}
				else if(cFileType == 'p')
				{
					tFtpFile.SetType(type_pipe);
				}
				else
				{
					break;
				}
				std::string strAuth = itCmd->substr(1 , 3);
				int eAuth = auth_none;
				if(strAuth.find('r') != -1)
				{
					eAuth |= auth_read;
				}
				if(strAuth.find('w') != -1)
				{
					eAuth |= auth_write;
				}
				if(strAuth.find('x') != -1)
				{
					eAuth |= auth_exe;
				}
				else if((strAuth.find('s') != -1) ||
					strAuth.find('t') != -1)
				{
					eAuth |= auth_exe; 
				}

				if(eAuth == auth_none)
					break;
				tFtpFile.SetAuthFlag((FTPFILEITEM_AUTHFLAG)eAuth);

				++itCmd; 
				if(itCmd == listCmd.end())
					break;
				std::string strLinkNum = *itCmd; 
				++itCmd;
				if(itCmd == listCmd.end())
					break;
				std::string strCurUserName = *itCmd; 
				++itCmd; 
				if(itCmd == listCmd.end())
					break;
				std::string strGroup = *itCmd; 
				++itCmd; 
				if(itCmd == listCmd.end())
					break;
				std::string strSize = *itCmd; 
				++itCmd; 
				if(itCmd == listCmd.end())
					break;
				std::string strMon = *itCmd; 
				++itCmd; 
				if(itCmd == listCmd.end())
					break;
				std::string strDay = *itCmd; 
				++itCmd; 
				if(itCmd == listCmd.end())
					break;
				std::string strYearOrTime = *itCmd; 
				++itCmd; 
				if(itCmd == listCmd.end())
					break;
				std::string strName = *itCmd; 

				ftp_trim(strName , "\r");
				if(strName.size())
				{
					tFtpFile.SetName(strName.c_str());
				}
				vDirs.push_back(tFtpFile);

				break;
			}

			++itDir; 
		}

		PrintLog("Server Dir Finished!");

		return 0;
	}
}

FTP_API CFTPManager::CD(const char* path)
{
	assert(m_cmdSocket != INVALID_SOCKET);

	std::string strCmdLine = parseCommand(FTP_COMMAND_CHANGE_DIRECTORY, path);

	if (Send(m_cmdSocket, strCmdLine.c_str() , strCmdLine.size()) < 0)
	{
		return -1;
	}

	m_strResponse = serverResponse(m_cmdSocket);

	trace("@@@@Response: %s\n", m_strResponse.c_str());
	return parseResponse(m_strResponse.c_str());
}

FTP_API CFTPManager::CreatRemoteFile( const char* strRemoteFile )
{
	assert(m_cmdSocket != INVALID_SOCKET);

	std::string strCmdLine;
	long nSize = 0;
	for ( int nIndex = 0; nIndex < 3; nIndex++ )
	{
		nSize = getFileSize(strRemoteFile);  //文件总大小
		if ( -1 != nSize )
		{
			break;
		}
	}

	if ( -1 != nSize )
	{
		return 0;
	}

	int data_fd = socket(AF_INET, SOCK_STREAM, 0);
	assert(data_fd != -1);

	if (createDataLink(data_fd) < 0)
	{
		return -1;
	}

	strCmdLine = parseCommand(FTP_COMMAND_UPLOAD_FILE, strRemoteFile);

	if (Send(m_cmdSocket, strCmdLine.c_str() , strCmdLine.size()) < 0)
	{
		close(data_fd);
		return -1;
	}

	close(data_fd);

	for ( int nIndex = 0; nIndex < 3; nIndex++ )
	{
		nSize = getFileSize(strRemoteFile);  //文件总大小
		if ( -1 != nSize )
		{
			break;
		}
	}

	if ( -1 != nSize )
	{
		return 0;
	}

	return -1;
}

FTP_API CFTPManager::DeleteRemoteFile(const char* strRemoteFile)
{
	assert(m_cmdSocket != INVALID_SOCKET);

	std::string strCmdLine = parseCommand(FTP_COMMAND_DELETE_FILE, strRemoteFile);

	if (Send(m_cmdSocket, strCmdLine.c_str(),strCmdLine.size()) < 0)
	{
		return -1;
	}

	m_strResponse = serverResponse(m_cmdSocket);
	printf("@@@@Response: %s\n", m_strResponse.c_str());
	return parseResponse(m_strResponse.c_str());
}

FTP_API CFTPManager::DeleteDirectory(const char* strRemoteDir)
{
	assert(m_cmdSocket != INVALID_SOCKET);

	std::string strCmdLine = parseCommand(FTP_COMMAND_DELETE_DIRECTORY, strRemoteDir);

	if (Send(m_cmdSocket, strCmdLine.c_str() , strCmdLine.size()) < 0)
	{
		return -1;
	}

	m_strResponse = serverResponse(m_cmdSocket);

	trace("@@@@Response: %s\n", m_strResponse.c_str());
	return parseResponse(m_strResponse.c_str());
}

FTP_API CFTPManager::CreateRemoteDirectory(const char* strRemoteDir)
{
	assert(m_cmdSocket != INVALID_SOCKET);

	std::string strCmdLine = parseCommand(FTP_COMMAND_CREATE_DIRECTORY, strRemoteDir);

	if (Send(m_cmdSocket, strCmdLine.c_str() , strCmdLine.size()) < 0)
	{
		return -1;
	}

	m_strResponse = serverResponse(m_cmdSocket);

	trace("@@@@Response: %s\n", m_strResponse.c_str());
	return parseResponse(m_strResponse.c_str());
}

FTP_API CFTPManager::Rename(const char* strRemoteFile, const char* strNewFile)
{
	assert(m_cmdSocket != INVALID_SOCKET);

	std::string strCmdLine = parseCommand(FTP_COMMAND_RENAME_BEGIN, strRemoteFile);
	Send(m_cmdSocket, strCmdLine.c_str() , strCmdLine.size());
	trace("@@@@Response: %s\n", serverResponse(m_cmdSocket));

	strCmdLine = parseCommand(FTP_COMMAND_RENAME_END, strNewFile);
	Send(m_cmdSocket, strCmdLine.c_str() , strCmdLine.size());

	m_strResponse = serverResponse(m_cmdSocket);
	trace("@@@@Response: %s\n", m_strResponse.c_str());
	return parseResponse(m_strResponse.c_str());
}

long CFTPManager::getFileSize(const char* strRemoteFile)
{
	assert(m_cmdSocket != INVALID_SOCKET);

	std::string strCmdLine = parseCommand(FTP_COMMAND_FILE_SIZE, strRemoteFile);

	if (Send(m_cmdSocket, strCmdLine.c_str() , strCmdLine.size()) < 0)
	{
		return -1;
	}

	m_strResponse = serverResponse(m_cmdSocket);

	trace("@@@@Response: %s\n", m_strResponse.c_str());

	std::string strData = m_strResponse.substr(0, 3);
	do 
	{
		unsigned long val = atol(strData.c_str());
		
		if (val == 213)
		{
			strData = m_strResponse.substr(4);
			trace("strData: %s\n", strData.c_str());
			val = atol(strData.c_str());

			return val;
		}
		else if(val == 226)
		{
			int nPos = m_strResponse.find('\r');
			strData = m_strResponse.substr(nPos + 1);
			m_strResponse = strData;
		}
		else
			break;
	} while (true);

	return -1;
}

FTP_API CFTPManager::createDataLink(int data_fd)
{
	assert(data_fd != INVALID_SOCKET);

	std::string strData;
	unsigned long nPort = 0 ;
	std::string strServerIp ; 
	std::list<std::string> strArray ;

	std::string parseStr = Pasv();

	if (parseStr.size() <= 0)
	{
		return -1;
	}

	//trace("parseInfo: %s\n", parseStr.c_str());

	/* 返回这样字符串，就会解析失败
	226-File successfully transferred
	226 1.414 seconds (measured here), 2.90 Mbytes per second
	227 Entering Passive Mode (172,16,231,30,163,185)
	*/

	std::list<std::string> listData; 
	int nPos = 0; 
	while(true)
	{
		size_t nBegin = parseStr.find("(", nPos);
		if(nBegin == -1)
			break;
		nPos = nBegin ;
		size_t nEnd = parseStr.find(")" , nPos);
		if(nEnd == -1)
			break;
		nPos = nEnd; 
		strData = parseStr.substr(nBegin + 1 , nEnd - nBegin - 1); 
		listData.push_back(strData); 
	}

	bool bParseSuccess = false; 
	std::list<std::string>::iterator itData = listData.begin(); 
	while(itData != listData.end())
	{
		strArray.clear();
		strData = *itData; 
		if(SplitString(strData , strArray , ",") >= 0 && 
			ParseString(strArray , nPort , strServerIp) >= 0)
		{
			bParseSuccess = true;
			break;
		}
		++itData;
	}
// 	size_t nBegin = parseStr.find_first_of("(");
// 	size_t nEnd	  = parseStr.find_first_of(")");
// 	strData		  = parseStr.substr(nBegin + 1, nEnd - nBegin - 1);
// 
// 	//trace("ParseAfter: %s\n", strData.c_str());
// 	if( SplitString( strData , strArray , "," ) <0)
// 		return -1;
// 
// 	if( ParseString( strArray , nPort , strServerIp) < 0)
// 		return -1;

	//trace("nPort: %ld IP: %s\n", nPort, strServerIp.c_str());
	if(bParseSuccess == false)
		return -1; 

	if (Connect(data_fd, strServerIp.c_str(), nPort) < 0)
	{
		return -1;
	}

	return 0;

}

FTP_API CFTPManager::ParseString(std::list<std::string> strArray, unsigned long & nPort ,std::string & strServerIp)
{
	if (strArray.size() < 6 )
		return -1 ;

	std::list<std::string>::iterator citor;
	citor = strArray.begin();
	strServerIp = *citor;
	strServerIp += ".";
	citor ++;
	strServerIp += *citor;
	strServerIp += ".";
	citor ++ ;
	strServerIp += *citor;
	strServerIp += ".";
	citor ++ ;
	strServerIp += *citor;
	citor = strArray.end();
	citor--;
	nPort = atol( (*citor).c_str());
	citor--;
	nPort += atol( (*(citor)).c_str()) * 256 ;
	return 0 ; 
}

void CFTPManager::Close()
{
	shutdown(m_cmdSocket, SHUT_RDWR);
	close(m_cmdSocket);
	m_cmdSocket = INVALID_SOCKET;
}

FTP_API CFTPManager::Get(const char* strRemoteFile, const char* strLocalFile, PROGRESSCALLBACK pProgressCB, const void* pUserData)
{
	return downLoad(strRemoteFile, strLocalFile, pProgressCB, pUserData);
}

FTP_API CFTPManager::Put(const char* strRemoteFile, const char* strLocalFile, PROGRESSCALLBACK pProgressCB, const void* pUserData)
{
	if( NULL == strLocalFile || NULL == strRemoteFile || 0 == strlen(strLocalFile) || 0 == strlen(strRemoteFile) )
	{
		if ( pProgressCB )
		{
			pProgressCB(-1, 0, 0, pUserData);
		}
		return -1; 
	}

	std::string strCmdLine;
	const unsigned long dataLen = FTP_DEFAULT_BUFFER;
	char strBuf[dataLen] = {0};
	unsigned long nSize = 0;
	for ( int nIndex = 0; nIndex < 3; nIndex++ )
	{
		nSize = getFileSize(strRemoteFile);  //文件总大小
		if ( -1 != nSize )
		{
			break;
		}
	}

	unsigned long nTotalSize = 0;
	unsigned long nLen = 0;

	struct stat sLocalFileInfo;
	stat(strLocalFile, &sLocalFileInfo);
	nTotalSize = sLocalFileInfo.st_size;
	trace("size: %d\n", sLocalFileInfo.st_size);

	FILE *pFile = fopen(strLocalFile, "rb");  // 以只读方式打开  且文件必须存在
	if ( NULL == pFile )
	{
		if ( pProgressCB )
		{
			pProgressCB(-1, 0, 0, pUserData);
		}
		return -2;
	}

	int data_fd = socket(AF_INET, SOCK_STREAM, 0);
	assert(data_fd != -1);

	if (createDataLink(data_fd) < 0)
	{
		if ( pProgressCB )
		{
			pProgressCB(-1, 0, 0, pUserData);
		}
		return -3;
	}

	if (nSize == -1)
	{
		strCmdLine = parseCommand(FTP_COMMAND_UPLOAD_FILE, strRemoteFile);
	}
	else
	{
		//strCmdLine = parseCommand(FTP_COMMAND_APPEND_FILE, strRemoteFile);
		DeleteRemoteFile(strRemoteFile); // 删除远端文件
		strCmdLine = parseCommand(FTP_COMMAND_UPLOAD_FILE, strRemoteFile);
	}

	if (Send(m_cmdSocket, strCmdLine.c_str() , strCmdLine.size()) < 0)
	{
		close(data_fd);
		if ( pProgressCB )
		{
			pProgressCB(-1, 0, 0, pUserData);  //上传失败
		}
		return -4;
	}

	trace("@@@@Response: %s\n", serverResponse(m_cmdSocket));

	if ( pProgressCB )
	{
		pProgressCB(1, 0, 0, pUserData);  //开始上传
	}

	SetTimeOut(1000000);
	unsigned long nSendLen = 0;
	fseek(pFile, 0, SEEK_SET);
	while (!feof(pFile))
	{
		nLen = fread(strBuf, 1, dataLen, pFile);
		if (nLen < 0)
		{
			break;
		}

		if (Send(data_fd, strBuf, nLen) < 0)
		{
			break;
		}

		nSendLen += nLen;

		if ( pProgressCB )
		{
			pProgressCB(2, nSendLen, nTotalSize, pUserData);  //上传进度
		}
	}

	trace("@@@@Response: %s\n", serverResponse(data_fd));

	close(data_fd);
	trace("@@@@Response: %s\n", serverResponse(m_cmdSocket));
	fclose(pFile);

	if ( nTotalSize != nSendLen )
	{
		if ( pProgressCB )
		{
			pProgressCB(-1, 0, 0, pUserData);  //上传失败
		}
	}

	if ( pProgressCB )
	{
		pProgressCB(3, 0, nTotalSize, pUserData);  //上传结束
	}

	return 0;
}

FILE *CFTPManager::createLocalFile(const char* strLocalFile)
{
	return fopen(strLocalFile, "w+b");
}

FTP_API CFTPManager::downLoad(const char* strRemoteFile, const char* strLocalFile, PROGRESSCALLBACK pProgressCB, const void* pUserData, const int pos, const unsigned int length)
{
	assert(length >= 0);
	if( NULL == strLocalFile || NULL == strRemoteFile || 0 == strlen(strLocalFile) || 0 == strlen(strRemoteFile) )
	{
		if ( pProgressCB )
		{
			pProgressCB(-1, 0, 0, pUserData);
		}
		return -1; 
	}

	FILE *file = NULL;

	file = createLocalFile(strLocalFile);
	if ( NULL == file )
	{
		if ( pProgressCB )
		{
			pProgressCB(3, 0, 0, pUserData);
		}
		return -1;
		//assert(file != NULL);
	}

// 	std::list<FTPFILEITEM> vDirs;
// 	std::string strDir = strRemoteFile; 
// 	int nPos = strDir.rfind('/');
// 	std::string strSub = strDir.substr(0 , nPos + 1); 
// 	Dir(strSub.c_str() , vDirs); 	

	unsigned long nTotalSize = 0;
	for ( int nIndex = 0; nIndex < 3; nIndex++ )
	{
		nTotalSize = getFileSize(strRemoteFile);  //文件总大小
		if ( -1 != nTotalSize )
		{
			break;
		}
	}

	if ( (unsigned long)-1 == nTotalSize || 0 == nTotalSize )
	{
		if ( pProgressCB )
		{
			pProgressCB(-1, 0, 0, pUserData);
		}
		return -1;
	}

	unsigned long nDataLen = FTP_DEFAULT_BUFFER;
	char strPos[MAX_PATH]  = {0};
	int data_fd = socket(AF_INET, SOCK_STREAM, 0);

	assert(data_fd != -1);

	if ((length != 0) && (length < nDataLen))
	{
		nDataLen = length;
	}
	char *dataBuf = new char[nDataLen];
	assert(dataBuf != NULL);

	sprintf(strPos, "%d", pos);

	if (createDataLink(data_fd) < 0)
	{
		trace("@@@@ Create Data Link error!!!\n");
		if ( pProgressCB )
		{
			pProgressCB(-1, 0, 0, pUserData);
		}
		return -1;
	}

	std::string strCmdLine = parseCommand(FTP_COMMAND_DOWNLOAD_POS, strPos);
	if (Send(m_cmdSocket, strCmdLine.c_str() , strCmdLine.size()) < 0)
	{
		if ( pProgressCB )
		{
			pProgressCB(-1, 0, 0, pUserData);
		}
		return -1;
	}
	trace("@@@@Response: %s\n", serverResponse(m_cmdSocket));

	strCmdLine = parseCommand(FTP_COMMAND_DOWNLOAD_FILE, strRemoteFile);

	if (Send(m_cmdSocket, strCmdLine.c_str() , strCmdLine.size()) < 0)
	{
		if ( pProgressCB )
		{
			pProgressCB(-1, 0, 0, pUserData);
		}
		return -1;
	}
	trace("@@@@Response: %s\n", serverResponse(m_cmdSocket));

	int len = 0;
	unsigned long nReceiveLen = 0;

	//开始下载
	if ( pProgressCB && -1 != nTotalSize )
	{
		pProgressCB(1, 0, nTotalSize, pUserData);
	}

	SetTimeOut(1000000);
	m_bStop = false;
	while ((len = getData(data_fd, dataBuf, nDataLen)) > 0)
	{
		if ( m_bStop )
		{
			break;
		}
		nReceiveLen += len;

		int num = fwrite(dataBuf, 1, len, file);
		memset(dataBuf, 0, sizeof(dataBuf));
		if ( pProgressCB && nReceiveLen != nTotalSize )
		{
			pProgressCB(1, nReceiveLen, nTotalSize, pUserData);
		}
		//trace("%s", dataBuf);
		trace("Num:%d\n", num);
		if (nReceiveLen == length && length != 0)
			break;

		if ((nReceiveLen + nDataLen) > length  && length != 0)
		{
			delete []dataBuf;
			nDataLen = length - nReceiveLen;
			dataBuf = new char[nDataLen];
		}
	}
	SetTimeOut(100000);

	close(data_fd);
	fclose(file);
	delete []dataBuf;

	if ( m_bStop && NULL != strLocalFile )
	{
		remove(strLocalFile);

		if ( pProgressCB )
		{
			pProgressCB(0, 0, 0, pUserData);
		}

		return 0;
	}

	if ( pProgressCB )
	{
		if ( nReceiveLen == nTotalSize )
		{
			pProgressCB(1, nReceiveLen, nTotalSize, pUserData);
		}
		else
		{
			remove(strLocalFile);
			pProgressCB(2, nReceiveLen, nTotalSize, pUserData);
		}
	}

	return 0;
}
const char* CFTPManager::GetResultString( FTP_API nRet )
{
	std::map<long , std::string>::iterator itErr = mapFTPError.find(nRet); 
	if(itErr != mapFTPError.end())
		return itErr->second.c_str(); 
	return "未知错误";
}

void CFTPManager::Stop()
{
	m_bStop = TRUE;
}

void CFTPManager::SetTimeOut( int nTimeData )
{
	m_nTimeOut = nTimeData;
}

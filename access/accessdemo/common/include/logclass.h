/*****************************************************************************
模块名      : logclass
文件名      : logclass.h
相关文件    : 
文件实现功能: 通用日志记录
作者        : 赵君祥
版本        : V1.0.0.0
-----------------------------------------------------------------------------
修改记录:
日  期            版本        修改人      修改内容
2019/10/16  1.0          赵君祥      创建
******************************************************************************/
#ifndef _LOGCLASS_H
#define _LOGCLASS_H

#include <list>
#include <time.h>
#include <io.h>
#include <direct.h> 
#include <process.h>
#include <Strsafe.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <WinBase.h>
#include <vector>
#include <string>
using namespace std;


/************************************************************************/
/*                              用法                                    */
/************************************************************************/
/*
#include "logclass.h"

#include "logclass.h"

void main()
{
    printf("hello world \n");
    
    // 创建文件日志LOG APP
    char szFilePath_coef[MAX_PATH + 1] = { 0 };
    GetModuleFileNameA(NULL, szFilePath_coef, MAX_PATH);
    char* pEnd = strrchr(szFilePath_coef, '\\');
    if(pEnd)
    {
        *pEnd = 0;
    }
    strcat(szFilePath_coef, "\\SIMS-Log\\");

    InitLog(szFilePath_coef, 1024, 10);

    for (int i = 0; i< 100; i++)
    {
        string temp = "hello www";

        LogPrintf("[id: %d]%s \n", i, temp.c_str());       // 写文件
        printf("[%d] :%s\n", i , "hello,ww");
        Sleep(200);
    }
    
    UnInitLog();

    system("pause");
}
*/


#ifdef WIN32
#define  PATH_FORMAT   '\\'
#endif

#ifdef _LINUX_
#define  PATH_FORMAT   '/'
#endif

const long   MAXLEN_LOGFILE_NAME = 255;          // 日志文件最大长度
const long   MAXNUM_LOG_NUM      = 100;          // 最多个数, svr_0.log svr_1.log.... svr_5.log, 默认2
const long   MINNUM_LOG_NUM      = 1;            // 日志文件最少个数
const long   MAXSIZE_LOG_FILE    = (10240 << 10);// 每个日志文件最大个数10M, 系统必须预留空间: MAXSIZE_LOG_FILE * MAXNUM_LOG_NUM
const long   MINSIZE_LOG_FILE    = (1024 << 10); // 每个日志文件最小个数2M==> 1M
const long   MAXLEN_LOGSTR       = 2*1024;       // 记录的最大日志长度
const long   MAXNUM_LOGSTR_LIST  = 1024;         // 日志列表最大长度

static void* gWriteLogToFile(void * pParam);
static unsigned int __stdcall gThreadGetLog(PVOID pM);
//默认锁
#define SVR_LOCK(strMutex) CAutoLock cLock(strMutex);

//结构体默认构造函数、拷贝构造函数和赋值构造函数定义
#define	STRUCT_CONSTRUCTORS_DEF(sname) \
    sname(){\
    Clear();\
}\
    sname(const sname& tRight){\
    *this = tRight;\
}\
    void Clear(){\
    memset(this, 0, sizeof(sname));\
}\
    sname& operator = (const sname& tRight){\
    if(this == &tRight){\
    return *this;\
    }\
    memcpy(this, &tRight, sizeof(sname));\
    return *this;\
}\

//char字符串字段的获取和设置方法
#define PROPERTY_DEF_char_ARRAY(Property) \
    void Set##Property(const char* lpsz##Property) {\
    if (NULL != lpsz##Property) \
    STRNCPY(m_ach##Property, sizeof(m_ach##Property), lpsz##Property, strlen(lpsz##Property));\
}\
    const char* Get##Property() const {\
    return m_ach##Property;\
}\

//赋值一个字符串
static BOOL STRNCPY(char* pchDst, size_t nDstLen, const char* pchSrc, size_t nSrcLen)
{
    if ( NULL == pchSrc || NULL == pchDst|| nDstLen < 1)
    {
        return FALSE;
    }

    if ( 0 == nSrcLen )
    {
        memset(pchDst, 0, nDstLen);
    }
    else
    {
        // 预留一个字节填写字符串结束符
        size_t nMinLen = ((nDstLen-1) > nSrcLen) ? nSrcLen : (nDstLen-1);
        strncpy(pchDst, pchSrc, nMinLen);
        pchDst[nMinLen] = '\0';
    }
    return TRUE;
}

// 日志信息
struct TLogStr
{
	char  m_achLogStr[MAXLEN_LOGSTR + 1];  //  日志描述信息
public:
    STRUCT_CONSTRUCTORS_DEF(TLogStr);
    PROPERTY_DEF_char_ARRAY(LogStr);
};

typedef std::list<TLogStr> ListLog;           // 日志列表

class CAutoLock
{
public:
    CAutoLock()
    {
    }
    CAutoLock(const char *muxtexName)
    {   

        HANDLE hMutex = CreateMutex(NULL, TRUE, muxtexName); //创建互斥量 

        m_strMuntexName = muxtexName;
        m_hMutex = OpenMutex(MUTEX_ALL_ACCESS, TRUE, muxtexName);
        if (hMutex == NULL)
        {
            printf("打开互斥量 : %s 失败\n", muxtexName);  
        }else
        {
            m_bTake = TRUE;
        }
    }
    ~CAutoLock(void)
    {
        if (m_bTake)
        {
            ReleaseMutex(m_hMutex);
            CloseHandle(m_hMutex);
            m_bTake = FALSE;
        }
    }
    
public:
    std::string m_strMuntexName;
    HANDLE m_hMutex;
    BOOL m_bTake;
};

class CLogClass
{
private:
	CLogClass()
	{
		m_listLog.clear();

		m_dwMaxSize = 0;
		m_dwCurFileSize = 0;
		m_dwMaxLogNum = 0;
		m_ptLogFd = NULL;
		m_bOpened = FALSE;
        m_strFilePath = "";

        m_cLogMutex = "Mutex_SycLog";
        m_cFileMutex = "Mutex_SycFile";

        memset(m_achLogFileName, 0, sizeof(m_achLogFileName) );  

        m_pcLogClass = NULL;
    }
	
public:
	static CLogClass *GetInstance()
	{
		if ( NULL == m_pcLogClass )
        {
            m_pcLogClass = new CLogClass();
        }		
		return m_pcLogClass;
	}

	~CLogClass()
	{
        if ( NULL != m_pcLogClass )
        {
            delete m_pcLogClass;
			m_pcLogClass = NULL;
        }
	}

	const char *GetLogFileName()
	{
		return m_achLogFileName;
	}
	void SetLogFileName(const char *pchFileName)
	{
		if ( NULL == pchFileName )
        {
            return;	
        }
		
		unsigned short wMinLen = strlen(pchFileName) > MAXLEN_LOGFILE_NAME ? MAXLEN_LOGFILE_NAME : strlen(pchFileName);
		strncpy(m_achLogFileName,pchFileName,wMinLen);
		m_achLogFileName[wMinLen] = '\0';
	}

    BOOL UnloadLogFile()
    {
        BOOL bRet = FALSE;
        if (m_bOpened && m_ptLogFd)
        {
            bRet = fclose(m_ptLogFd) == 0? TRUE: FALSE;
        }
        return bRet;
    }

	// 限制为一个文件,没有后缀的文件名,后缀默认为.log
	BOOL InitLogSlots(const char *pchLogName, long dwMaxSize, long dwMaxLogNum)
	{
		SVR_LOCK(m_cFileMutex);
		if ( NULL == pchLogName )
		{		
			return FALSE;
		}
		if ( m_bOpened && NULL != m_ptLogFd )
		{
			return FALSE;
		}
        
        m_strFilePath = pchLogName; //保存日志目录
         
        time_t dwCurTime = time(NULL);
        struct tm *tp;
        tp = localtime(&dwCurTime);

        unsigned short wYear = tp->tm_year + 1900;
        int  byMonth = tp->tm_mon + 1;
        int  byDay = tp->tm_mday;
        int  byHour = tp->tm_hour;
        int	byMinute = tp->tm_min;
        int	bySecond = tp->tm_sec;
        int nLen = 0;
        char achLog[256] ={0};

        nLen = sprintf(achLog, "SIMS-Log-%04d-%02d-%02d%02d%02d%02d.log",
            wYear, byMonth, byDay, byHour, byMinute, bySecond);

        string strFilename = achLog;
        string strFileDir = pchLogName;
        strFileDir.append(strFilename);

		// 设置日志名称, 不包括后缀
		SetLogFileName(strFileDir.c_str());
	
		m_dwMaxSize = dwMaxSize > MAXSIZE_LOG_FILE ? MAXSIZE_LOG_FILE : dwMaxSize;
	
		//  日志文件个数限制,至少2个,用于交替记录
		if( dwMaxLogNum < MINNUM_LOG_NUM )		
		{
			m_dwMaxLogNum = MINNUM_LOG_NUM;
		}
		else if ( dwMaxLogNum > MAXNUM_LOG_NUM )
		{
			m_dwMaxLogNum = MAXNUM_LOG_NUM;
		}
		else
		{
			m_dwMaxLogNum = dwMaxLogNum;
		}

		// 创建消息接收线程
        HANDLE hThreadGetLog = NULL;
        hThreadGetLog = (HANDLE)_beginthreadex(NULL, 0, gThreadGetLog, NULL, 0, NULL);
        if (!hThreadGetLog)
        {
            printf( "%s %d OspTaskCreate gLog2File failed !\n", __FUNCTION__, __LINE__);
            return FALSE;
        }
        CloseHandle(hThreadGetLog);
	
        /*	struct stat statbuf;  
        stat(pchLogName,&statbuf);  
        m_dwCurFileSize = statbuf.st_size; */
       
        struct _stati64 buf;
        memset(&buf, 0, sizeof(buf));
        m_dwCurFileSize = _stati64(strFileDir.c_str(), &buf);

        //检查磁盘空间
        CheckDiskMem(m_strFilePath.c_str(), 100);//100MB
        //检查文件个数，如果超过先删除
        FindFileAndDelete(m_strFilePath.c_str(), ".log", m_dwMaxLogNum, m_dwMaxLogNum/2);

        // 以追加的方式打开
		m_ptLogFd = fopen(strFileDir.c_str(), "ab+");

		if ( NULL == m_ptLogFd )
		{
			printf("Open file%s Error!\n", strFileDir.c_str());
			m_bOpened = FALSE;
			return FALSE;
		}

		m_bOpened = TRUE;
		
		return TRUE;
	}

    //检查磁盘空间，如果小于某阈值，则删除一半文件
    //参数1 路径；参数2 空间阈值
    void CheckDiskMem(const string Path, __int64 nFileSize)
    {
        __int64 p1, p2, p3;
        ::GetDiskFreeSpaceEx(Path.c_str(), (PULARGE_INTEGER)&p1, (PULARGE_INTEGER)&p2, (PULARGE_INTEGER)&p3);
        __int64 p = p3;

        printf("磁盘空间, 剩余[%lld MB] 阈值:%llu MB", p/(1024*1024), nFileSize);

        if (p/(1024*1024) < nFileSize)
        {
             FindFileAndDelete(Path.c_str(), ".log", m_dwMaxLogNum, m_dwMaxLogNum/2);
        }
    }

    //查找某种类型的文件，如果超过一定数量，则删除一定数量
    //param: 1.查找的目录 2.类型 3.个数上线 4.单次需要删除的文件个数
    void FindFileAndDelete(const string Path,const string extension, int Threshold,int delNum)
    {
        if ((Threshold < delNum) || Path == "" || extension == "")
        {
            printf("参数错误\n");
            return;
        }

        std::vector<string> statfileurllist;
        WIN32_FIND_DATAA wfd;  
        std::string PathExtension=Path;
        PathExtension+="\*"+extension;

        printf("path: %s\n",PathExtension.c_str());

        HANDLE hFile = FindFirstFile(PathExtension.c_str(),&wfd);  
        if(INVALID_HANDLE_VALUE == hFile)  
        {  
            printf("不合法!\n");  
            return;  
        }

        do{  
            statfileurllist.push_back(Path+"\\"+wfd.cFileName);  
            printf("%s\n",wfd.cFileName);  
        }while(FindNextFile(hFile,&wfd)); 


        //查找指定类型个数
        int dFileNum = statfileurllist.size();
        printf("类型：%s 个数：%d 阈值：%d 删除:%d\n", extension.c_str(),dFileNum, Threshold, delNum);

        if (delNum > Threshold) delNum = Threshold/2;//防止误删

        //删除
        if (dFileNum >= Threshold)
        {
            for (int i = 0; i< delNum; i++)
            {
                string strDelFilePath = statfileurllist[i];

                if (strDelFilePath != "")
                {
                    //删除
                    if(remove(strDelFilePath.c_str()) == NULL)
                    {
                        printf("删除成功：%s\n", strDelFilePath.c_str());
                    }
                    else
                    {
                        printf("删除失败：%s\n", strDelFilePath.c_str());
                    }
                }
            }
        }

    }


	// 往固定的一个Log文件写, 写完后判断是否超出,如果超出则按照以下操作
	// 如果超出，按照时间重新生成新的log，再次判断，如果超过N，则按时间排序，删除第一个文件
	// 其中N为最大文件个数
	BOOL WriteLog2File(TLogStr &tLogStr)
	{
		SVR_LOCK(m_cFileMutex);
		if ( !m_bOpened  || NULL == m_ptLogFd)
		{
			printf("Open File: %s, Opend:%d, LogFd:%d.\n", m_achLogFileName, m_bOpened, m_ptLogFd );
			return FALSE;
		}
		
		// 文件日志输出
		char *pchLog = (char*)tLogStr.GetLogStr();
        if ( NULL == pchLog )
        {
            return FALSE;
        }

		char achLog[MAXLEN_LOGSTR + 1] ={0};
        STRNCPY( achLog, sizeof(achLog), pchLog, strlen(pchLog) );
		
		int nLen = fwrite(achLog, sizeof(char), strlen(achLog), m_ptLogFd);

        //立即刷新否则一直显示为0，直到关闭文件
        fflush(m_ptLogFd);

		if ( nLen > 0 )
		{
			m_dwCurFileSize = m_dwCurFileSize + nLen;
		}
		
		if( m_dwCurFileSize > m_dwMaxSize )
		{
			fflush(m_ptLogFd);
			fclose(m_ptLogFd );

            char szFilePath_coef[MAX_PATH + 1] = { 0 };

            if (_access( m_strFilePath.c_str(), 0 ) == -1) //不存在则创建文件夹
            {
                _mkdir(m_strFilePath.c_str());
            }
            
            string strFilePath = m_strFilePath;

            time_t dwCurTime = time(NULL);
            struct tm *tp;
            tp = localtime(&dwCurTime);

            unsigned short wYear = tp->tm_year + 1900;
            int  byMonth = tp->tm_mon + 1;
            int  byDay = tp->tm_mday;
            int  byHour = tp->tm_hour;
            int	byMinute = tp->tm_min;
            int	bySecond = tp->tm_sec;
            int nLen = 0;
            char achLog[256] ={0};
           
            nLen = sprintf(achLog, "SIMS-Log-%04d-%02d-%02d%02d%02d%02d.log",
                wYear, byMonth, byDay, byHour, byMinute, bySecond);
            
            strFilePath.append(achLog);
            
			// 重新打开文件
			m_ptLogFd = fopen(strFilePath.c_str(), "ab+");


            //检查磁盘空间
            CheckDiskMem(m_strFilePath.c_str(), 100);//100MB

            // 判断文件个数，如果超过规定个数，删除第一个

            FindFileAndDelete(m_strFilePath.c_str(), ".log", m_dwMaxLogNum, m_dwMaxLogNum/2);

			m_dwCurFileSize = 0;
		}
		return TRUE;
	}
    // 打印日志 多一个是否写文件标识，不在依赖初始化模块参数
	inline void LogPrintInfo(std::string devName,char* pchLogFormat,...)
	{
		
        time_t dwCurTime = time(NULL);
        struct tm *tp;
        tp = localtime(&dwCurTime);

        unsigned short wYear = tp->tm_year + 1900;
        int  byMonth = tp->tm_mon + 1;
        int  byDay = tp->tm_mday;
        int  byHour = tp->tm_hour;
        int	byMinute = tp->tm_min;
        int	bySecond = tp->tm_sec;

		int nLen = 0;
        char achLog[MAXLEN_LOGSTR] ={0};

        nLen = sprintf(achLog, "[%04d-%02d-%02d %02d:%02d:%02d][%s]:",
            wYear, byMonth, byDay, byHour, byMinute, bySecond,devName.c_str());
	
		va_list pvlist;
		va_start(pvlist, pchLogFormat);
		int  nStrLen = vsnprintf((char *)achLog+nLen, MAXLEN_LOGSTR-nLen, pchLogFormat, pvlist);
		if ( nStrLen <= 0 || nStrLen >= MAXLEN_LOGSTR )
		{
			va_end(pvlist);
			return;
		}
		va_end(pvlist);

		TLogStr tLogStr;
		tLogStr.SetLogStr( achLog );

        // 写文件
        InsertLogStr(tLogStr);

	}

	// 获取一条日志记录
	BOOL GetLogStr(TLogStr &tLogStr)
	{
		SVR_LOCK(m_cLogMutex);

		BOOL bRet = FALSE;
		if ( 0 != m_listLog.size() )
		{
            bRet  = TRUE;
			tLogStr = m_listLog.front();
			m_listLog.pop_front();			
		}

		return bRet;
	}

	// 插入一条日志记录
	BOOL InsertLogStr(const TLogStr &tLogStr)
	{
		SVR_LOCK(m_cLogMutex);

		BOOL bRet = FALSE;
		if ( m_listLog.size() < MAXNUM_LOGSTR_LIST )
		{
            bRet = TRUE;
			m_listLog.push_back(tLogStr);			
		}
		else
		{
			printf("Log List full, discard it!\n");
		}

		return bRet;
	}


private:
	// 日志文件句柄
	FILE* m_ptLogFd;
	// 日志是否打开
	BOOL m_bOpened;
	// 文件最大长度,以Byte为单位
	long m_dwMaxSize;
	// 文件当前长度，以Byte为单位
	long m_dwCurFileSize;
	// 最大文件个数
	long m_dwMaxLogNum;
	// 文件名
	char m_achLogFileName[MAXLEN_LOGFILE_NAME + 1];

	// 日志列表
	ListLog m_listLog;			
    char *m_cLogMutex; 
    char* m_cFileMutex;

    //日志目录
    string m_strFilePath;
	// 单实例
	static CLogClass *m_pcLogClass;
};

//本成员变量由调用者负责初始化
//CLogClass *CLogClass::m_pcLogClass = NULL;

// 写日志文件操作线程

static unsigned int __stdcall gThreadGetLog( PVOID pM )
{
    while (TRUE)
    {
        TLogStr tLogStr;
        if ( CLogClass::GetInstance()->GetLogStr(tLogStr) )
        {	
            // 日志记录
            CLogClass::GetInstance()->WriteLog2File(tLogStr);
        }
        else
        {
            Sleep(100);
        }

    }
    return NULL;
}

/************************************************************************/
/*                              用法                                    */
/************************************************************************/
//CLogClass *CLogClass::m_pcLogClass = NULL;      // 当前log模块仅支持单实例, 只写一个日志文件

//初始化文件
//参数1. 文件夹路径 参数2. 文件最大 参数 3. 做多记录文件个数 
static  void InitLog(const char *pchLogDir, long dwMaxSize, long dwMaxLogNum)
{
    // 创建文件日志LOG APP

    if (pchLogDir == NULL || dwMaxSize == NULL || dwMaxLogNum == NULL)
    {
        printf("参数输入有误，请重新输入! \n");
    }

    char szFilePath_coef[MAX_PATH + 1] = { 0 };
    strcat(szFilePath_coef, pchLogDir);
  
    //修改成 多级创建目录
    char *tmp = (char *)pchLogDir;

    while (*tmp)
    {
        if (*tmp == '\\')
        {
            *tmp = '\0';
            if (_access(pchLogDir, 0))// 头文件io.h
            {
                if (_mkdir(pchLogDir))
                {
#ifdef _DEBUG
                    fprintf(stderr, "Failed to create directory %s:%s\n", 
                        path, strerror(errno));
#endif
                }
            }
            *tmp = '\\';
        }
        ++tmp;
    }

    CLogClass::GetInstance()->InitLogSlots(szFilePath_coef,dwMaxSize,dwMaxLogNum);     // 5 * 2M 

}
//关闭文件
static  void UnInitLog()
{
     if(CLogClass::GetInstance()->UnloadLogFile()) //手动关闭最后一个打开的文件
     {  
         printf("关闭文件 成功\n");
     }
     else
     {
          printf("关闭文件 失败\n");
     }
}

#define LogPrintf(strDevName,fmt,...)  {CLogClass::GetInstance()->LogPrintInfo(strDevName,fmt, __VA_ARGS__);printf(fmt, __VA_ARGS__);}

#endif
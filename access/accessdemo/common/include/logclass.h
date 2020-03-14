/*****************************************************************************
ģ����      : logclass
�ļ���      : logclass.h
����ļ�    : 
�ļ�ʵ�ֹ���: ͨ����־��¼
����        : �Ծ���
�汾        : V1.0.0.0
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��            �汾        �޸���      �޸�����
2019/10/16  1.0          �Ծ���      ����
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
/*                              �÷�                                    */
/************************************************************************/
/*
#include "logclass.h"

#include "logclass.h"

void main()
{
    printf("hello world \n");
    
    // �����ļ���־LOG APP
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

        LogPrintf("[id: %d]%s \n", i, temp.c_str());       // д�ļ�
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

const long   MAXLEN_LOGFILE_NAME = 255;          // ��־�ļ���󳤶�
const long   MAXNUM_LOG_NUM      = 100;          // ������, svr_0.log svr_1.log.... svr_5.log, Ĭ��2
const long   MINNUM_LOG_NUM      = 1;            // ��־�ļ����ٸ���
const long   MAXSIZE_LOG_FILE    = (10240 << 10);// ÿ����־�ļ�������10M, ϵͳ����Ԥ���ռ�: MAXSIZE_LOG_FILE * MAXNUM_LOG_NUM
const long   MINSIZE_LOG_FILE    = (1024 << 10); // ÿ����־�ļ���С����2M==> 1M
const long   MAXLEN_LOGSTR       = 2*1024;       // ��¼�������־����
const long   MAXNUM_LOGSTR_LIST  = 1024;         // ��־�б���󳤶�

static void* gWriteLogToFile(void * pParam);
static unsigned int __stdcall gThreadGetLog(PVOID pM);
//Ĭ����
#define SVR_LOCK(strMutex) CAutoLock cLock(strMutex);

//�ṹ��Ĭ�Ϲ��캯�����������캯���͸�ֵ���캯������
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

//char�ַ����ֶεĻ�ȡ�����÷���
#define PROPERTY_DEF_char_ARRAY(Property) \
    void Set##Property(const char* lpsz##Property) {\
    if (NULL != lpsz##Property) \
    STRNCPY(m_ach##Property, sizeof(m_ach##Property), lpsz##Property, strlen(lpsz##Property));\
}\
    const char* Get##Property() const {\
    return m_ach##Property;\
}\

//��ֵһ���ַ���
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
        // Ԥ��һ���ֽ���д�ַ���������
        size_t nMinLen = ((nDstLen-1) > nSrcLen) ? nSrcLen : (nDstLen-1);
        strncpy(pchDst, pchSrc, nMinLen);
        pchDst[nMinLen] = '\0';
    }
    return TRUE;
}

// ��־��Ϣ
struct TLogStr
{
	char  m_achLogStr[MAXLEN_LOGSTR + 1];  //  ��־������Ϣ
public:
    STRUCT_CONSTRUCTORS_DEF(TLogStr);
    PROPERTY_DEF_char_ARRAY(LogStr);
};

typedef std::list<TLogStr> ListLog;           // ��־�б�

class CAutoLock
{
public:
    CAutoLock()
    {
    }
    CAutoLock(const char *muxtexName)
    {   

        HANDLE hMutex = CreateMutex(NULL, TRUE, muxtexName); //���������� 

        m_strMuntexName = muxtexName;
        m_hMutex = OpenMutex(MUTEX_ALL_ACCESS, TRUE, muxtexName);
        if (hMutex == NULL)
        {
            printf("�򿪻����� : %s ʧ��\n", muxtexName);  
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

	// ����Ϊһ���ļ�,û�к�׺���ļ���,��׺Ĭ��Ϊ.log
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
        
        m_strFilePath = pchLogName; //������־Ŀ¼
         
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

		// ������־����, ��������׺
		SetLogFileName(strFileDir.c_str());
	
		m_dwMaxSize = dwMaxSize > MAXSIZE_LOG_FILE ? MAXSIZE_LOG_FILE : dwMaxSize;
	
		//  ��־�ļ���������,����2��,���ڽ����¼
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

		// ������Ϣ�����߳�
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

        //�����̿ռ�
        CheckDiskMem(m_strFilePath.c_str(), 100);//100MB
        //����ļ����������������ɾ��
        FindFileAndDelete(m_strFilePath.c_str(), ".log", m_dwMaxLogNum, m_dwMaxLogNum/2);

        // ��׷�ӵķ�ʽ��
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

    //�����̿ռ䣬���С��ĳ��ֵ����ɾ��һ���ļ�
    //����1 ·��������2 �ռ���ֵ
    void CheckDiskMem(const string Path, __int64 nFileSize)
    {
        __int64 p1, p2, p3;
        ::GetDiskFreeSpaceEx(Path.c_str(), (PULARGE_INTEGER)&p1, (PULARGE_INTEGER)&p2, (PULARGE_INTEGER)&p3);
        __int64 p = p3;

        printf("���̿ռ�, ʣ��[%lld MB] ��ֵ:%llu MB", p/(1024*1024), nFileSize);

        if (p/(1024*1024) < nFileSize)
        {
             FindFileAndDelete(Path.c_str(), ".log", m_dwMaxLogNum, m_dwMaxLogNum/2);
        }
    }

    //����ĳ�����͵��ļ����������һ����������ɾ��һ������
    //param: 1.���ҵ�Ŀ¼ 2.���� 3.�������� 4.������Ҫɾ�����ļ�����
    void FindFileAndDelete(const string Path,const string extension, int Threshold,int delNum)
    {
        if ((Threshold < delNum) || Path == "" || extension == "")
        {
            printf("��������\n");
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
            printf("���Ϸ�!\n");  
            return;  
        }

        do{  
            statfileurllist.push_back(Path+"\\"+wfd.cFileName);  
            printf("%s\n",wfd.cFileName);  
        }while(FindNextFile(hFile,&wfd)); 


        //����ָ�����͸���
        int dFileNum = statfileurllist.size();
        printf("���ͣ�%s ������%d ��ֵ��%d ɾ��:%d\n", extension.c_str(),dFileNum, Threshold, delNum);

        if (delNum > Threshold) delNum = Threshold/2;//��ֹ��ɾ

        //ɾ��
        if (dFileNum >= Threshold)
        {
            for (int i = 0; i< delNum; i++)
            {
                string strDelFilePath = statfileurllist[i];

                if (strDelFilePath != "")
                {
                    //ɾ��
                    if(remove(strDelFilePath.c_str()) == NULL)
                    {
                        printf("ɾ���ɹ���%s\n", strDelFilePath.c_str());
                    }
                    else
                    {
                        printf("ɾ��ʧ�ܣ�%s\n", strDelFilePath.c_str());
                    }
                }
            }
        }

    }


	// ���̶���һ��Log�ļ�д, д����ж��Ƿ񳬳�,��������������²���
	// �������������ʱ�����������µ�log���ٴ��жϣ��������N����ʱ������ɾ����һ���ļ�
	// ����NΪ����ļ�����
	BOOL WriteLog2File(TLogStr &tLogStr)
	{
		SVR_LOCK(m_cFileMutex);
		if ( !m_bOpened  || NULL == m_ptLogFd)
		{
			printf("Open File: %s, Opend:%d, LogFd:%d.\n", m_achLogFileName, m_bOpened, m_ptLogFd );
			return FALSE;
		}
		
		// �ļ���־���
		char *pchLog = (char*)tLogStr.GetLogStr();
        if ( NULL == pchLog )
        {
            return FALSE;
        }

		char achLog[MAXLEN_LOGSTR + 1] ={0};
        STRNCPY( achLog, sizeof(achLog), pchLog, strlen(pchLog) );
		
		int nLen = fwrite(achLog, sizeof(char), strlen(achLog), m_ptLogFd);

        //����ˢ�·���һֱ��ʾΪ0��ֱ���ر��ļ�
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

            if (_access( m_strFilePath.c_str(), 0 ) == -1) //�������򴴽��ļ���
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
            
			// ���´��ļ�
			m_ptLogFd = fopen(strFilePath.c_str(), "ab+");


            //�����̿ռ�
            CheckDiskMem(m_strFilePath.c_str(), 100);//100MB

            // �ж��ļ���������������涨������ɾ����һ��

            FindFileAndDelete(m_strFilePath.c_str(), ".log", m_dwMaxLogNum, m_dwMaxLogNum/2);

			m_dwCurFileSize = 0;
		}
		return TRUE;
	}
    // ��ӡ��־ ��һ���Ƿ�д�ļ���ʶ������������ʼ��ģ�����
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

        // д�ļ�
        InsertLogStr(tLogStr);

	}

	// ��ȡһ����־��¼
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

	// ����һ����־��¼
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
	// ��־�ļ����
	FILE* m_ptLogFd;
	// ��־�Ƿ��
	BOOL m_bOpened;
	// �ļ���󳤶�,��ByteΪ��λ
	long m_dwMaxSize;
	// �ļ���ǰ���ȣ���ByteΪ��λ
	long m_dwCurFileSize;
	// ����ļ�����
	long m_dwMaxLogNum;
	// �ļ���
	char m_achLogFileName[MAXLEN_LOGFILE_NAME + 1];

	// ��־�б�
	ListLog m_listLog;			
    char *m_cLogMutex; 
    char* m_cFileMutex;

    //��־Ŀ¼
    string m_strFilePath;
	// ��ʵ��
	static CLogClass *m_pcLogClass;
};

//����Ա�����ɵ����߸����ʼ��
//CLogClass *CLogClass::m_pcLogClass = NULL;

// д��־�ļ������߳�

static unsigned int __stdcall gThreadGetLog( PVOID pM )
{
    while (TRUE)
    {
        TLogStr tLogStr;
        if ( CLogClass::GetInstance()->GetLogStr(tLogStr) )
        {	
            // ��־��¼
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
/*                              �÷�                                    */
/************************************************************************/
//CLogClass *CLogClass::m_pcLogClass = NULL;      // ��ǰlogģ���֧�ֵ�ʵ��, ֻдһ����־�ļ�

//��ʼ���ļ�
//����1. �ļ���·�� ����2. �ļ���� ���� 3. �����¼�ļ����� 
static  void InitLog(const char *pchLogDir, long dwMaxSize, long dwMaxLogNum)
{
    // �����ļ���־LOG APP

    if (pchLogDir == NULL || dwMaxSize == NULL || dwMaxLogNum == NULL)
    {
        printf("����������������������! \n");
    }

    char szFilePath_coef[MAX_PATH + 1] = { 0 };
    strcat(szFilePath_coef, pchLogDir);
  
    //�޸ĳ� �༶����Ŀ¼
    char *tmp = (char *)pchLogDir;

    while (*tmp)
    {
        if (*tmp == '\\')
        {
            *tmp = '\0';
            if (_access(pchLogDir, 0))// ͷ�ļ�io.h
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
//�ر��ļ�
static  void UnInitLog()
{
     if(CLogClass::GetInstance()->UnloadLogFile()) //�ֶ��ر����һ���򿪵��ļ�
     {  
         printf("�ر��ļ� �ɹ�\n");
     }
     else
     {
          printf("�ر��ļ� ʧ��\n");
     }
}

#define LogPrintf(strDevName,fmt,...)  {CLogClass::GetInstance()->LogPrintInfo(strDevName,fmt, __VA_ARGS__);printf(fmt, __VA_ARGS__);}

#endif
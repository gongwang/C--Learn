#ifndef _TOPIC_MNG_DEF_H
#define _TOPIC_MNG_DEF_H
#include "moduleinterface.h"
#include "threadlock.h"
#include <map>
#include <list>

using std::map;
using std::list;
using std::string;

class CAppInstMsgNo
{
public:
	CAppInstMsgNo(){}
	CAppInstMsgNo(u32 dwAppInstID, u32 dwMsgNo)
	{
		appInstID = dwAppInstID;
		msgNo = dwMsgNo;
	}
	~CAppInstMsgNo(){}

	CAppInstMsgNo& operator= (const CAppInstMsgNo& obj)  
	{  
		appInstID = obj.appInstID;
		msgNo = obj.msgNo;
		return *this;  
	} 
	bool operator ==( const CAppInstMsgNo & obj)const
	{
		return ((appInstID == obj.appInstID) && ( msgNo == obj.msgNo ));
	}
	bool operator !=( const CAppInstMsgNo & obj)const
	{
		return ((appInstID != obj.appInstID) || ( msgNo != obj.msgNo ));
	}
	bool operator <(const CAppInstMsgNo & obj) const
	{
		if (appInstID < obj.appInstID) {return true;}
		
		if ((appInstID == obj.appInstID) && ( msgNo < obj.msgNo))
		{
			return true;
		}
		return false;
	}
	bool operator >(const CAppInstMsgNo & obj)const
	{
		if (appInstID > obj.appInstID) {return true;}

		if ((appInstID == obj.appInstID) && ( msgNo > obj.msgNo))
		{
			return true;
		}
		return false;
	}

	void SetMsgNo(u32 dwMsgNo){msgNo = dwMsgNo;}
	u32 GetMsgNo() {return msgNo;}

	void SetAppInstID(u32 dwAppInstID){appInstID = dwAppInstID;}
	u32 GetAppInstID() {return appInstID;}

private:
	u32 appInstID;                  //对应消息抛送的线程ID，订阅类型有效
	u32 msgNo;                       //对应的消息号
};

//对应topic的扩充信息
class CTopicExInfo
{
public:
	CTopicExInfo():appInstID(0),msgNo(0)
	{
		memset(devTypeName, 0, sizeof(devTypeName));
	}
	~CTopicExInfo(){}

	void SetMsgNo(u32 dwMsgNo){msgNo = dwMsgNo;}
	u32 GetMsgNo() {return msgNo;}

	void SetAppInstID(u32 dwAppInstID){appInstID = dwAppInstID;}
	u32 GetAppInstID() {return appInstID;}

	void SetDevTypeName(const s8* pszDevTypeName) {strncpy(devTypeName, pszDevTypeName, sizeof(devTypeName)-1);}
	const s8* GetDevTypeName() {return devTypeName;}
private:
	s8 devTypeName[MI_MAX_STR_LEN];  //设备类型参数
	u32 msgNo;                       //对应的消息号
	u32 appInstID;                   //对应消息抛送的线程ID，订阅类型有效
};

typedef list<CTopicExInfo> CTopicExInfoLst;               //设备类型扩充
typedef CTopicExInfoLst::iterator ITopicExInfoItr;
typedef CTopicExInfoLst::const_reverse_iterator CITopicExInfoItr;

//topic扩展信息管理类
class CTopicExInfoMng
{
public:
	CTopicExInfoMng(){};
	~CTopicExInfoMng(){};

	u32 GetSize()
	{
		CThreadAutoLock cLock(m_topicExInfoLock);
		return m_cTopicExInfoLst.size();
	}

	bool AddTopicEx(const s8* pszDevTypeName, u32 dwAppInstID, u32 dwMsgNo)
	{
		CThreadAutoLock cLock(m_topicExInfoLock);
		if (IsExistTopicEx(pszDevTypeName, dwAppInstID, dwMsgNo)){return false;}


		m_cTopicExInfoLst.push_back(CTopicExInfo());
		CITopicExInfoItr cItr = m_cTopicExInfoLst.rbegin();
		if(m_cTopicExInfoLst.rend() == cItr) {return false;}
		CTopicExInfo *pTopicExInfo = (CTopicExInfo*)&(*cItr);
		pTopicExInfo->SetDevTypeName(pszDevTypeName);
		pTopicExInfo->SetAppInstID(dwAppInstID);
		pTopicExInfo->SetMsgNo(dwMsgNo);
		return true;
	}

	void DelTopicExByAppInstID(u32 dwAppInstID)
	{
		CThreadAutoLock cLock(m_topicExInfoLock);
		ITopicExInfoItr itr = m_cTopicExInfoLst.begin();
		for (;itr != m_cTopicExInfoLst.end(); ++itr)
		{
			if(dwAppInstID == itr->GetAppInstID())
			{
				m_cTopicExInfoLst.erase(itr);
				break;
			}
		}
	}

	void DelTopicEx(u32 dwAppInstID, u32 dwMsgNo)
	{
		CThreadAutoLock cLock(m_topicExInfoLock);
		ITopicExInfoItr itr = m_cTopicExInfoLst.begin();
		for (;itr != m_cTopicExInfoLst.end(); ++itr)
		{
			if(dwAppInstID == itr->GetAppInstID() && dwMsgNo == itr->GetMsgNo())
			{
				m_cTopicExInfoLst.erase(itr);
				break;
			}
		}
	}

	bool FindTopicEx(const s8* pszDevTypeName, u32& dwAppInstID, u32& dwMsgNo)
	{
		CThreadAutoLock cLock(m_topicExInfoLock);
		ITopicExInfoItr itr = m_cTopicExInfoLst.begin();
		//当未传入设备类型名称时，以第一个设备类型为参考
		if (NULL == pszDevTypeName || 0 == strlen(pszDevTypeName))
		{
			if(itr != m_cTopicExInfoLst.end())
			{
				dwAppInstID = itr->GetAppInstID();
				dwMsgNo = itr->GetMsgNo();
				return true;
			}

			return false;
		}

		bool bRet= false;
		for (; itr != m_cTopicExInfoLst.end(); ++itr)
		{
			if (0 == strcmp(itr->GetDevTypeName(), pszDevTypeName))
			{
				dwAppInstID = itr->GetAppInstID();
				dwMsgNo = itr->GetMsgNo();
				bRet = true;
				break;
			}
		}
		return bRet;
	}

	CTopicExInfo* FindTopicExByAppInstID(u32 dwAppInstID)
	{
		CThreadAutoLock cLock(m_topicExInfoLock);
		CTopicExInfo* cRetTopicEx = NULL;
		ITopicExInfoItr itr = m_cTopicExInfoLst.begin();
		for (; itr != m_cTopicExInfoLst.end(); ++itr)
		{
			if (itr->GetAppInstID() == dwAppInstID)
			{
				cRetTopicEx = &(*itr);
				break;
			}
		}
		return cRetTopicEx;
	}

private:
	bool IsExistTopicEx(const s8* pszDevTypeName, u32 dwAppInstID, u32 dwMsgNo)
	{
		//内部操作不加锁，避免互锁
		bool bRet= false;
		if (NULL == pszDevTypeName){return false;}
		ITopicExInfoItr itr = m_cTopicExInfoLst.begin();
		for (;itr != m_cTopicExInfoLst.end(); ++itr)
		{
			if(0 == strcmp(itr->GetDevTypeName(), pszDevTypeName) && 
				dwAppInstID == itr->GetAppInstID() && 
				dwMsgNo == itr->GetMsgNo())
			{
				bRet = true;
				break;
			}
		}
		return bRet;
	}

private:
	CTopicExInfoLst m_cTopicExInfoLst;
	CThreadLockMutex m_topicExInfoLock;
};

//接收topic信息绑定的topic和msg
class CMIRecTopic2MsgNoInfo
{
public:
	CMIRecTopic2MsgNoInfo():m_nTopicHandle(0),timeoutMs(0)
	{
		memset(topic, 0, sizeof(topic));
		memset(groupId, 0, sizeof(groupId));
	}
	~CMIRecTopic2MsgNoInfo(){}

	void SetTopic2MsgNoInfo(CMITopic2MsgNoInfo& cTopicInfo)
	{
		SetTopic(cTopicInfo.GetTopic());
		SetGroupId(cTopicInfo.GetGroupId());
		SetTimeoutMs(cTopicInfo.GetTimeoutMs());
		m_cTopicExInfoMng.AddTopicEx(cTopicInfo.GetDevTypeName(), 
			cTopicInfo.GetAppInstID(), cTopicInfo.GetMsgNo());
	}

	void SetTopicHandle(int nTopicHandle)
	{
		m_nTopicHandle = nTopicHandle;
	}
	int GetTopicHandle(){return m_nTopicHandle;}

	void SetTopic(const s8* pszTopic) {strncpy(topic, pszTopic, sizeof(topic)-1);}
	const s8* GetTopic() {return topic;}

	void SetGroupId(const s8* pszGroupId) {strncpy(groupId, pszGroupId, sizeof(groupId)-1);}
	const s8* GetGroupId() {return groupId;}

	void SetTimeoutMs(u32 dwTimeoutMs){timeoutMs = dwTimeoutMs;}
	u32 GetTimeoutMs(){return timeoutMs;}

	CTopicExInfoMng& GetTopicExInfoMng(){return m_cTopicExInfoMng;}
private:
	int m_nTopicHandle;
	s8 topic[MI_MAX_TOPIC_LEN];      //topic名称
	s8 groupId[MI_MAX_STR_LEN];      //对应的groupId
	u32 timeoutMs;                   //超时时间（单位ms）
	CTopicExInfoMng m_cTopicExInfoMng;
};

typedef map<CAppInstMsgNo, CMIRecTopic2MsgNoInfo*> CRecAMKeyTopicMap;  //以APPID+消息号为key构建的消息号和topic绑定的接收map
typedef map<string, CMIRecTopic2MsgNoInfo*> CRecTKeyTopicMap;          //以topic为key构建的消息号和topic绑定的接收map
typedef CRecAMKeyTopicMap::iterator IRecAMKeyTopicIter;
typedef CRecTKeyTopicMap::iterator IRecTKeyTopicIter;

//发送topic绑定的topic和msg
class CMISendTopic2MsgNoInfo
{
public:
	CMISendTopic2MsgNoInfo():m_nTopicHandle(0),timeoutMs(0)
	{
		memset(topic, 0, sizeof(topic));
		memset(groupId, 0, sizeof(groupId));
	}
	~CMISendTopic2MsgNoInfo(){}

	void SetTopic2MsgNoInfo(CMITopic2MsgNoInfo& cTopicInfo)
	{
		SetTopic(cTopicInfo.GetTopic());
		SetGroupId(cTopicInfo.GetGroupId());
		SetTimeoutMs(cTopicInfo.GetTimeoutMs());
		m_cTopicExInfoMng.AddTopicEx(cTopicInfo.GetDevTypeName(), 
			cTopicInfo.GetAppInstID(), cTopicInfo.GetMsgNo());
		
	}

	void SetTopicHandle(int nTopicHandle)
	{
		m_nTopicHandle = nTopicHandle;
	}
	int GetTopicHandle(){return m_nTopicHandle;}

	void SetTopic(const s8* pszTopic) {strncpy(topic, pszTopic, sizeof(topic)-1);}
	const s8* GetTopic() {return topic;}

	void SetGroupId(const s8* pszGroupId) {strncpy(groupId, pszGroupId, sizeof(groupId)-1);}
	const s8* GetGroupId() {return groupId;}

	void SetTimeoutMs(u32 dwTimeoutMs){timeoutMs = dwTimeoutMs;}
	u32 GetTimeoutMs(){return timeoutMs;}

	CTopicExInfoMng& GetTopicExInfoMng(){return m_cTopicExInfoMng;}

private:
	int m_nTopicHandle;
	s8 topic[MI_MAX_TOPIC_LEN];      //topic名称
	s8 groupId[MI_MAX_STR_LEN];      //对应的groupId
	u32 timeoutMs;                   //超时时间（单位ms）
	CTopicExInfoMng m_cTopicExInfoMng;
};

typedef map<CAppInstMsgNo, CMISendTopic2MsgNoInfo*> CSendAMKeyTopicMap;  //以APPID+消息号为key构建的消息号和topic绑定的发送map
typedef map<string, CMISendTopic2MsgNoInfo*> CSendTKeyTopicMap;   //以topic为key构建的消息号和topic绑定的发送map
typedef CSendAMKeyTopicMap::iterator ISendAMKeyTopicIter;
typedef CSendTKeyTopicMap::iterator ISendTKeyTopicIter;

typedef list<unsigned int> TTopicHandleLst;
typedef TTopicHandleLst::iterator IHandelIter;

//topic管理类
class CMITopicMng
{
public:
	CMITopicMng(){}
	~CMITopicMng(){}

	//增加接收topic的信息
	CMIRecTopic2MsgNoInfo* AddRecTopicMsgNoInfo(CMITopic2MsgNoInfo& topic2MsgNoInfo)
	{
		CMIRecTopic2MsgNoInfo *pInfo = new CMIRecTopic2MsgNoInfo();
		if (!pInfo){return NULL;}

		pInfo->SetTopic2MsgNoInfo(topic2MsgNoInfo);
		string strTopic = pInfo->GetTopic();
		CAppInstMsgNo cAppInstMsgNo;
		cAppInstMsgNo.SetAppInstID(topic2MsgNoInfo.GetAppInstID());
		cAppInstMsgNo.SetMsgNo(topic2MsgNoInfo.GetMsgNo());

		{
			CThreadAutoLock cLock(m_recMsgNoLock);
			m_recMsgNoMap.insert(std::make_pair(cAppInstMsgNo, pInfo));
		}

		{
			CThreadAutoLock cLock(m_recTopicLock);
			m_recTopicMap.insert(std::make_pair(strTopic, pInfo));
		}
		
		return pInfo;
	}

	//累增接收信息的接口
	bool TiredRecTopicMsgNoInfo(CMIRecTopic2MsgNoInfo *pInfo, const s8* pszDevTypeName, 
		u32 dwAppInstID, u32 dwMsgNo)
	{
		if (NULL == pInfo) {return false;}
		pInfo->GetTopicExInfoMng().AddTopicEx(pszDevTypeName, dwAppInstID, dwMsgNo);
		CAppInstMsgNo cAppInstMsgNo;
		cAppInstMsgNo.SetAppInstID(dwAppInstID);
		cAppInstMsgNo.SetMsgNo(dwMsgNo);
		{
			CThreadAutoLock cLock(m_sendMsgNoLock);
			m_recMsgNoMap.insert(std::make_pair(cAppInstMsgNo, pInfo));
		}
		return true;
	}
	
	//增加发送topic的信息
	const CMISendTopic2MsgNoInfo* AddSendTopicMsgNoInfo(CMITopic2MsgNoInfo& topic2MsgNoInfo, s32 nSendHandle)
	{
		CMISendTopic2MsgNoInfo *pInfo = new CMISendTopic2MsgNoInfo();
		if (!pInfo){return NULL;}

		pInfo->SetTopic2MsgNoInfo(topic2MsgNoInfo);
		pInfo->SetTopicHandle(nSendHandle);
		string strTopic = pInfo->GetTopic();
		CAppInstMsgNo cAppInstMsgNo;
		cAppInstMsgNo.SetAppInstID(topic2MsgNoInfo.GetAppInstID());
		cAppInstMsgNo.SetMsgNo(topic2MsgNoInfo.GetMsgNo());

		{
			CThreadAutoLock cLock(m_sendMsgNoLock);
			m_sendMsgNoMap.insert(std::make_pair(cAppInstMsgNo, pInfo));
		}

		{
			CThreadAutoLock cLock(m_sendTopicLock);
			m_sendTopicMap.insert(std::make_pair(strTopic, pInfo));
		}

		return pInfo;
	}

	//累增接收信息的接口
	bool TiredSendTopicMsgNoInfo(CMISendTopic2MsgNoInfo *pInfo, const s8* pszDevTypeName, 
		u32 dwAppInstID, u32 dwMsgNo)
	{
		if (NULL == pInfo) {return false;}
		pInfo->GetTopicExInfoMng().AddTopicEx(pszDevTypeName, dwAppInstID, dwMsgNo);
		CAppInstMsgNo cAppInstMsgNo;
		cAppInstMsgNo.SetAppInstID(dwAppInstID);
		cAppInstMsgNo.SetMsgNo(dwMsgNo);
		{
			CThreadAutoLock cLock(m_sendMsgNoLock);
			m_sendMsgNoMap.insert(std::make_pair(cAppInstMsgNo, pInfo));
		}
		return true;
	}

	//删除接收topic的信息
	bool DelRecTopicMsgNoInfo(const string& strTopic, u32 dwAppInstID)
	{
		CAppInstMsgNo cAppInstMsgNo;
		CMIRecTopic2MsgNoInfo *pInfo = NULL;
		{
			CThreadAutoLock cLock(m_recTopicLock);
			IRecTKeyTopicIter itr = m_recTopicMap.find(strTopic);
			if (itr == m_recTopicMap.end())
			{
				return false;
			}

			CTopicExInfo* pTopicEx = itr->second->GetTopicExInfoMng().FindTopicExByAppInstID(dwAppInstID);
			if(NULL == pTopicEx) {return false;}


			cAppInstMsgNo.SetMsgNo(pTopicEx->GetMsgNo());
			cAppInstMsgNo.SetAppInstID(pTopicEx->GetAppInstID());
			pInfo = itr->second;
			pInfo->GetTopicExInfoMng().DelTopicExByAppInstID(dwAppInstID);
			if(0 == pInfo->GetTopicExInfoMng().GetSize())
			{
				m_recTopicMap.erase(itr);
			}
			else
			{
				//避免被删除
				pInfo = NULL;
			}
			
		}

		{
			CThreadAutoLock cLock(m_recMsgNoLock);
			IRecAMKeyTopicIter itr = m_recMsgNoMap.find(cAppInstMsgNo);
			if(itr != m_recMsgNoMap.end())
			{
				m_recMsgNoMap.erase(itr);
			}
		}

		if(NULL != pInfo) {delete pInfo;}
		return true;
	}

	//删除发送topic的信息
	bool DelSendTopicMsgNoInfo(const string& strTopic, u32 dwAppInstID)
	{
		CAppInstMsgNo cAppInstMsgNo;
		CMISendTopic2MsgNoInfo *pInfo = NULL;
		{
			CThreadAutoLock cLock(m_sendTopicLock);
			ISendTKeyTopicIter itr = m_sendTopicMap.find(strTopic);
			if (itr == m_sendTopicMap.end())
			{
				return false;
			}

			CTopicExInfo* pTopicEx = itr->second->GetTopicExInfoMng().FindTopicExByAppInstID(dwAppInstID);
			if(NULL == pTopicEx) {return false;}

			cAppInstMsgNo.SetMsgNo(pTopicEx->GetMsgNo());
			cAppInstMsgNo.SetAppInstID(pTopicEx->GetAppInstID());
			pInfo = itr->second;
			if(0 == pInfo->GetTopicExInfoMng().GetSize())
			{
				m_sendTopicMap.erase(itr);
			}
			else
			{
				//避免被删除
				pInfo = NULL;
			}
		}

		{
			CThreadAutoLock cLock(m_sendMsgNoLock);
			ISendAMKeyTopicIter itr = m_sendMsgNoMap.find(cAppInstMsgNo);
			if(itr != m_sendMsgNoMap.end())
			{
				m_sendMsgNoMap.erase(itr);
			}
		}

		if(NULL != pInfo) {delete pInfo;}
		return true;
	}

	//增加接收对象的句柄
	bool AddRecHandle(unsigned int dwRecHandle)
	{
		CThreadAutoLock cLock(m_RecHandleLock);
		m_recHandleLst.push_back(dwRecHandle);
		return true;
	}

	//增加发送对象的句柄
	void DelRecHandle(unsigned int dwRecHandle)
	{
		CThreadAutoLock cLock(m_RecHandleLock);
		IHandelIter itr = m_recHandleLst.begin();
		for(;itr != m_recHandleLst.end(); ++itr)
		{
			if(*itr == dwRecHandle)
			{
				m_recHandleLst.erase(itr);
				break;
			}
		}
	}
	
	//通过接收的消息号查找接收对应的信息
	CMIRecTopic2MsgNoInfo* FindRecMsgNo2TopicByAppInstMsgNo(u32 dwAppInstId, u32 dwMsgNo)
	{
		CAppInstMsgNo cAppInstMsgNo;
		cAppInstMsgNo.SetAppInstID(dwAppInstId);
		cAppInstMsgNo.SetMsgNo(dwMsgNo);
		CThreadAutoLock cLock(m_recMsgNoLock);
		IRecAMKeyTopicIter itr = m_recMsgNoMap.find(cAppInstMsgNo);
		if(itr == m_recMsgNoMap.end()) {return NULL;}
		return itr->second;
	}

	//通过接收的topic号查找接收对应的信息
	CMIRecTopic2MsgNoInfo* FindRecMsgNo2TopicByTopic(string& strTopic)
	{
		CThreadAutoLock cLock(m_recTopicLock);
		IRecTKeyTopicIter itr = m_recTopicMap.find(strTopic);
		if(itr == m_recTopicMap.end()) {return NULL;}
		return itr->second;
	}

	//通过发送的消息号查找发送对应的信息
	CMISendTopic2MsgNoInfo* FindSendMsgNo2TopicByAppInstMsgNo(u32 dwAppInstId, u32 dwMsgNo)
	{
		CAppInstMsgNo cAppInstMsgNo;
		cAppInstMsgNo.SetAppInstID(dwAppInstId);
		cAppInstMsgNo.SetMsgNo(dwMsgNo);
		CThreadAutoLock cLock(m_sendMsgNoLock);
		ISendAMKeyTopicIter itr = m_sendMsgNoMap.find(cAppInstMsgNo);
		if(itr == m_sendMsgNoMap.end()) {return NULL;}
		return itr->second;
	}

	//通过发送的topic号查找发送对应的信息
	CMISendTopic2MsgNoInfo* FindSendMsgNo2TopicByTopic(string& strTopic)
	{
		CThreadAutoLock cLock(m_sendTopicLock);
		ISendTKeyTopicIter itr = m_sendTopicMap.find(strTopic);
		if(itr == m_sendTopicMap.end()) {return NULL;}
		return itr->second;
	}

	//清理掉所有的接收信息
	void ClearAllRecMsgNo2Topic(void)
	{
		{
			CThreadAutoLock cLock(m_recMsgNoLock);
			m_recMsgNoMap.clear();
		}

		{
			CThreadAutoLock cLock(m_recTopicLock);
			IRecTKeyTopicIter curItr = m_recTopicMap.begin();
			for (; curItr != m_recTopicMap.end(); ++curItr)
			{
				if(NULL != curItr->second)
				{
					delete curItr->second;
				}
			}
			m_recTopicMap.clear();
		}
	}

	//清理掉所有的发送信息
	void ClearAllSendMsgNo2Topic(void)
	{
		{
			CThreadAutoLock cLock(m_sendMsgNoLock);
			m_sendMsgNoMap.clear();
		}

		{
			CThreadAutoLock cLock(m_sendTopicLock);
			ISendTKeyTopicIter curItr = m_sendTopicMap.begin();
			for (; curItr != m_sendTopicMap.end(); ++curItr)
			{
				if(NULL != curItr->second)
				{
					delete curItr->second;
				}
			}
			m_sendTopicMap.clear();
		}
	}

	TTopicHandleLst GetRecHandleLst(void)
	{
		CThreadAutoLock cLock(m_RecHandleLock);
		return m_recHandleLst;
	}

	TTopicHandleLst GetSendHandleLst(void)
	{
		CThreadAutoLock cLock(m_sendTopicLock);
		TTopicHandleLst sendHandleLst;
		ISendTKeyTopicIter curItr = m_sendTopicMap.begin();
		for (; curItr != m_sendTopicMap.end(); ++curItr)
		{
			sendHandleLst.push_back(curItr->second->GetTopicHandle());
		}
		
		return sendHandleLst;
	}



private:
	CRecAMKeyTopicMap m_recMsgNoMap;      //订阅topic，以msgno为key的map
	CRecTKeyTopicMap m_recTopicMap;      //订阅topic，以topic为key的map
	CSendAMKeyTopicMap m_sendMsgNoMap;    //发送的topic，以msgno为key的map
	CSendTKeyTopicMap m_sendTopicMap;    //发送的topic，以topic为key的map
	TTopicHandleLst m_recHandleLst;
	CThreadLockMutex m_recMsgNoLock;
	CThreadLockMutex m_recTopicLock;
	CThreadLockMutex m_sendMsgNoLock;
	CThreadLockMutex m_sendTopicLock;
	CThreadLockMutex m_RecHandleLock;
};

#endif //_TOPIC_MNG_DEF_H
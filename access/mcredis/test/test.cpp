// test.cpp : 定义控制台应用程序的入口点。
//

#include "mcinterface.h"
#include "windows.h"
#include <string>
#include <stdio.h>
void OnLogCb(eMCLoglev eLevel, const s8* pszLog, u32 nLogLen, void* pUser)
{
    printf("level:%d %s", eLevel, pszLog);
}
void OnDIsCb(void* pUser)
{
    printf("SERVER DOWN!\n");
}

int main(int argc, char* argv[])
{
    if(MC_ERR_NO !=MCInit())
    {
        printf("MCInit 初始化失败\n");
        return 0;
    }
    printf("MCInit 初始化over\n");
    IMCInterface *piMc = MCCreateObject();
    if (NULL != piMc)
    {
        piMc->Init(OnLogCb, OnDIsCb, NULL);
        vecMCServerInfo vServer;
        vServer.push_back(std::make_pair("172.16.64.12",6706));
        std::string strUser,strPwd;
        s32 nRet = 0;
        nRet = piMc->Login(vServer, strUser, strPwd);
        printf("5\n");
        printf("login result nret=%d\n", nRet);
        std::string strKey("testkey1");
        std::string strValue("teststrvalue");
        std::vector<std::string> vecKey,vecVal;
        vecKey.push_back(strKey);
        printf("getstate:%d\n", piMc->GetState());
        
        piMc->PrintServerData();
        nRet = piMc->Set( strKey, strValue);
        printf("Set result nret=%d\n", nRet);
        nRet = piMc->SetExpireTime(strKey, 1000);
        printf("SetExpireTime result nret=%d\n", nRet);
        nRet = piMc->GetString( strKey, strValue );
        printf("GetString:%s\n", strValue.c_str());
        strKey = "testkey2";
        strValue = "99999";
        printf("GetString result nret=%d\n", nRet);
        nRet = piMc->Set( strKey, strValue/*, MC_SET_EXIST, 1000*/);
        vecKey.push_back(strKey);
        printf("Set result nret=%d\n", nRet);
        nRet = piMc->MultiGetString( vecKey, vecVal);
        std::vector<std::string>::iterator iteVal = vecVal.begin();
        for (; iteVal != vecVal.end(); ++iteVal)
        {
            printf("GetmultiString:%s\n", iteVal->c_str());
        }
        int nval = 0;
        printf("MultiGetString result nret=%d\n", nRet);
        nRet = piMc->GetInt( strKey, nval );
        printf("GetInt:%d\n", nval);
        printf("GetInt result nret=%d\n", nRet);
        
        nRet = piMc->Incr( strKey,nval);
        printf("Incr:%d\n", nval);
        printf("Incr result nret=%d\n", nRet);
        nRet = piMc->Decr( strKey, nval );
        printf("Decr:%d\n", nval);
//         nRet = piMc->Append( strKey, strValue);
//         printf("Append result nret=%d\n", nRet);
        strKey = "testlist";
        printf("Decr result nret=%d\n", nRet);
        vecVal.clear();
        vecVal.push_back(std::string("listval1"));
        vecVal.push_back(std::string("listval2"));
        vecVal.push_back(std::string("listval3"));
        nRet = piMc->ListRPush( strKey, vecVal );
        strValue = "listvalue1";
        printf("ListRPush result nret=%d\n", nRet);
        nRet = piMc->ListLPush( strKey, vecVal );
        printf("ListLPush result nret=%d\n", nRet);
        nRet = piMc->ListGet( strKey, 1, strValue );
        strValue = "listvalue2";
        printf("ListGet result nret=%d\n", nRet);
        nRet = piMc->ListLPush( strKey, vecVal );
        printf("ListLPush result nret=%d\n", nRet);
        nRet = piMc->ListSet( strKey, -1, strValue );
        
        printf("ListSet result nret=%d\n", nRet);
        nRet = piMc->GetList( strKey, vecVal );
        iteVal = vecVal.begin();
        for (; iteVal != vecVal.end(); ++iteVal)
        {
            printf("GetList:%s\n", iteVal->c_str());
        }
        printf("GetList result nret=%d\n", nRet);
        nRet = piMc->ListLen( strKey, nval );
        printf("ListLen:%d\n", nval);
        
        printf("ListLen result nret=%d\n", nRet);
        nRet = piMc->ListLPop( strKey, strValue );
        printf("ListLPop:%s\n", strValue.c_str());
        printf("ListLPop result nret=%d\n", nRet);
        nRet = piMc->ListRPop( strKey, strValue );
        printf("ListRPop:%s\n", strValue.c_str());
        strKey = "testsets";
        printf("ListRPop result nret=%d\n", nRet);
        nRet = piMc->SetsAdd( strKey, vecVal);
        printf("SetsAdd result nret=%d\n", nRet);
        nRet = piMc->GetSets( strKey, vecVal );
        iteVal = vecVal.begin();
        for (; iteVal != vecVal.end(); ++iteVal)
        {
            printf("GetSets:%s\n", iteVal->c_str());
        }
        bool bexist = false;
        printf("GetSets result nret=%d\n", nRet);
        nRet = piMc->SetsExists( strKey, strValue, bexist );
        printf("setsexists:%d\n", bexist);
        vecVal.resize(1);
        printf("SetsExists result nret=%d\n", nRet);
        nRet = piMc->SetsRem( strKey, vecVal );
        strKey = "testhash";
        std::string strField = "hashkey3";
        printf("SetsRem result nret=%d\n", nRet);
        std::map<std::string,std::string> maphash;
        maphash.insert(std::make_pair(std::string("hashkey1"), std::string("hashval1")));
        maphash.insert(std::make_pair(std::string("hashkey2"), std::string("hashval2")));
        maphash.insert(std::make_pair(std::string("hashkey3"), std::string("hashval3")));
        nRet = piMc->HashSetAll( strKey, maphash);
        printf("HashSetAll result nret=%d\n", nRet);
        nRet = piMc->HashSet( strKey, strField, strValue );
        printf("HashSet result nret=%d\n", nRet);
        nRet = piMc->HashGet( strKey, strField, strValue );
        printf("HashGet result nret=%d\n", nRet);
        printf("HashGet result value=%s\n", strValue.c_str());
        nRet = piMc->GetHash( strKey, maphash );
        printf("GetHash result nret=%d\n", nRet);
        std::map<std::string,std::string>::iterator itehash = maphash.begin();
        for (; itehash != maphash.end(); ++itehash)
        {
            printf("Gethash:key=%s, val=%s\n", itehash->first.c_str(), itehash->second.c_str());
        }
        nRet = piMc->HashDel( strKey, strField );
        printf("HashDel result nret=%d\n", nRet);
        nRet = piMc->HashGetKeys( strKey, vecVal );
        iteVal = vecVal.begin();
        for (; iteVal != vecVal.end(); ++iteVal)
        {
            printf("HashGetKeys:%s\n", iteVal->c_str());
        }
        printf("HashGetKeys result nret=%d\n", nRet);
        nRet = piMc->HashExists( strKey, strField, bexist );
        printf("HashExists result:%d, nret=%d\n", bexist, nRet);
        strKey = "testsets";
        vecKey.clear();
        vecKey.push_back(std::string("testsets"));
        nRet = piMc->DelKeys( vecKey );

        printf("DelKeys result nret=%d\n", nRet);
        nRet = piMc->GetSets(strKey, vecVal);
        printf("Watch GetSets nret=%d\n", nRet);
        iteVal = vecVal.begin();
        for (; iteVal != vecVal.end(); ++iteVal)
        {
            printf("GetSets:%s\n", iteVal->c_str());
        }
        vecKey.clear();
        vecKey.push_back(std::string("testlist"));
        vecKey.push_back(std::string("testhash"));
        nRet = piMc->Watch( vecKey );
        printf("Watch result nret=%d\n", nRet);
        nRet = piMc->Multi();
        printf("Multi result nret=%d\n", nRet);
        

        printf("DelKeys result nret=%d\n", nRet);
        nRet = piMc->Exec();
        printf("Exec result nret=%d\n", nRet);
        nRet = piMc->Multi();
        printf("Multi result nret=%d\n", nRet);
        vecKey.clear();
        vecKey.push_back(std::string("testlist"));
        nRet = piMc->DelKeys( vecKey );
        nRet = piMc->Discard();
        printf("login result nret=%d\n", nRet);
    }
    Sleep(50000);
	return 0;
}


#include "commonsaproto.h"

const Json::Value TSANty::ToJsonWithNode()const
{
	Json::Value node; 
	JSonHelper::ToJson::AddChildNode(node, "msgNo", msgNo);
	JSonHelper::ToJson::AddChildNode(node, "sn", sn);
	return node;
}

const std::string TSANty::ToJson()const
{
	return ToJsonWithNode().toStyledString();
}

bool TSANty::FromJson(const Json::Value& jsonNode)
{
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"msgNo", msgNo);
	JSonHelper::ToStruct::ParseChildNode(jsonNode, "sn", sn);
	return true;
}
bool TSANty::FromJson(const std::string& json_string)
{
	Json::Reader reader;
	Json::Value jsonNode;

	if (!reader.parse(json_string, jsonNode))
	{
		return false;
	}

	return FromJson(jsonNode);
}
//////////////////////////////////////////////////////////////
const Json::Value TSANetAddr::ToJsonWithNode()const
{
	Json::Value node; 
	JSonHelper::ToJson::AddChildNode(node, "ip", ip);
	JSonHelper::ToJson::AddChildNode(node, "port", port);
	return node;
}

const std::string TSANetAddr::ToJson()const
{
	return ToJsonWithNode().toStyledString();
}

bool TSANetAddr::FromJson(const Json::Value& jsonNode)
{
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"ip", ip);
	JSonHelper::ToStruct::ParseChildNode(jsonNode, "port", port);

	return true;
}

bool TSANetAddr::FromJson(const std::string& json_string)
{
	Json::Reader reader;
	Json::Value jsonNode;

	if (!reader.parse(json_string, jsonNode))
	{
		return false;
	}

	return FromJson(jsonNode);
}

//////////////////////////////////////////////////////////////
const Json::Value TSAEventReq::ToJsonWithNode()const
{
	Json::Value node; 
	JSonHelper::ToJson::AddChildNode(node, "msgNo", msgNo);
	JSonHelper::ToJson::AddChildNode(node, "sn", sn);
	return node;
}

const std::string TSAEventReq::ToJson()const
{
	return ToJsonWithNode().toStyledString();
}


bool TSAEventReq::FromJson(const Json::Value& jsonNode)
{
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"msgNo", msgNo);
	JSonHelper::ToStruct::ParseChildNode(jsonNode, "sn", sn);

	return true;
}

bool TSAEventReq::FromJson(const std::string& json_string)
{
	Json::Reader reader;
	Json::Value jsonNode;

	if (!reader.parse(json_string, jsonNode))
	{
		return false;
	}

	return FromJson(jsonNode);
}

//////////////////////////////////////////////////////////////
const Json::Value TSAEventRsp::ToJsonWithNode()const
{
	Json::Value node; 
	JSonHelper::ToJson::AddChildNode(node, "msgNo", msgNo);
	JSonHelper::ToJson::AddChildNode(node, "sn", sn);
	JSonHelper::ToJson::AddChildNode(node,"errCode",errCode);
	JSonHelper::ToJson::AddChildNode(node,"errDesc",errDesc);
	return node;
}

const std::string TSAEventRsp::ToJson()const
{
    return ToJsonWithNode().toStyledString();
}

bool TSAEventRsp::FromJson(const Json::Value& jsonNode)
{
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"msgNo",msgNo);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"sn",sn);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"errCode",errCode);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"errDesc",errDesc);

	return true;
}

bool TSAEventRsp::FromJson(const std::string& json_string)
{
	Json::Reader reader;
	Json::Value jsonNode;

	if (!reader.parse(json_string, jsonNode))
	{
		return false;
	}

	return FromJson(jsonNode);
}


//////////////////////////////////////////////////////////////
const Json::Value TSARegReq::ToJsonWithNode()const
{
	Json::Value node = TSAEventReq::ToJsonWithNode(); 
	JSonHelper::ToJson::AddChildNode(node, "leaderSer", leaderSer);
	JSonHelper::ToJson::AddChildNode(node,"tAdpAddr",tAdpAddr);
	JSonHelper::ToJson::AddChildNode(node,"adpId",adpId);
	JSonHelper::ToJson::AddChildNode(node,"adpName",adpName);
	JSonHelper::ToJson::AddChildNode(node,"proNum",proNum);
	JSonHelper::ToJson::AddChildNode(node,"telPort",telPort);
	JSonHelper::ToJson::AddChildNode(node,"desc",desc);
	JSonHelper::ToJson::AddChildNode(node,"adpType",adpType);
	return node;
}

const std::string TSARegReq::ToJson()const
{
	return ToJsonWithNode().toStyledString();
}


bool TSARegReq::FromJson(const Json::Value& jsonNode)
{
	TSAEventReq::FromJson(jsonNode);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"leaderSer", leaderSer);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"tAdpAddr",tAdpAddr);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"adpId",adpId);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"proNum",proNum);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"telPort",telPort);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"desc",desc);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"adpType",adpType);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"adpName",adpName);

	return true;
}

bool TSARegReq::FromJson(const std::string& json_string)
{
	Json::Reader reader;
	Json::Value jsonNode;

	if (!reader.parse(json_string, jsonNode))
	{
		return false;
	}

	return FromJson(jsonNode);
}

//////////////////////////////////////////////////////////////
const Json::Value TSARegRsp::ToJsonWithNode()const
{
	Json::Value node = TSAEventRsp::ToJsonWithNode(); 
	JSonHelper::ToJson::AddChildNode(node,"adpId",adpId);
	JSonHelper::ToJson::AddChildNode(node,"devNum",devNum);
	JSonHelper::ToJson::AddChildNode(node,"synDevInfo",synDevInfo);

	return node;
}

const std::string TSARegRsp::ToJson()const
{
	return ToJsonWithNode().toStyledString();	
}


bool TSARegRsp::FromJson(const Json::Value& jsonNode)
{
	TSAEventRsp::FromJson(jsonNode);

	JSonHelper::ToStruct::ParseChildNode(jsonNode,"adpId",adpId);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"devNum",devNum);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"synDevInfo",synDevInfo);

	return true;
}

bool TSARegRsp::FromJson(const std::string& json_string)
{
	Json::Reader reader;
	Json::Value jsonNode;

	if (!reader.parse(json_string, jsonNode))
	{
		return false;
	}

	return FromJson(jsonNode);

}

//////////////////////////////////////////////////////////////
// const std::string TSAAdpReq::ToJson()const
// {
// 	Json::Value root; 
// 	root["msgNo"] = msgNo;
// 	root["sn"] = sn;
// 
// 	root["adpId"] = adpId;
// 	root["tAdpAddr"] = tAdpAddr.ToJson();
// 	root["usrName"] = usrName;
// 	root["usrPwd"] = usrPwd;
// 	root["adpName"] = adpName;
// 	root["desc"] = desc;
// 
// 	std::string strSAAdpReq = root.toStyledString();
// 	return strSAAdpReq;
// }
// 
// void TSAAdpReq::FromJson(const std::string& json_string)
// {
// 	Json::Reader reader;
// 	Json::Value tSAAdpReq;
// 
// 	if (reader.parse(json_string, tSAAdpReq))
// 	{
// 		msgNo = tSAAdpReq["msgNo"].asInt();
// 		sn = tSAAdpReq["sn"].asInt();
// 
// 		adpId = tSAAdpReq["adpId"].asString();
// 		tAdpAddr.FromJson(tSAAdpReq["tAdpAddr"].asString());
// 		usrName = tSAAdpReq["usrName"].asString();
// 		usrPwd = tSAAdpReq["usrPwd"].asString();
// 		adpName = tSAAdpReq["adpName"].asString();
// 		desc = tSAAdpReq["desc"].asString();		
// 	}
// }

//////////////////////////////////////////////////////////////
// const std::string TSAAdpRsp::ToJson()const
// {
// 	Json::Value root;  
// 	root["msgNo"] = msgNo;
// 	root["sn"] = sn;
// 	root["ErrCode"] = ErrCode;
// 	root["errDesc"] = errDesc;
// 
// 	std::string strSAAdpRsp = root.toStyledString();
// 	return strSAAdpRsp;
// }
// 
// void TSAAdpRsp::FromJson(const std::string& json_string)
// {
// 	Json::Reader reader;
// 	Json::Value tSAAdpRsp;
// 	if (reader.parse(json_string, tSAAdpRsp))
// 	{
// 		msgNo = tSAAdpRsp["msgNo"].asInt();
// 		sn = tSAAdpRsp["sn"].asInt();
// 		ErrCode = tSAAdpRsp["ErrCode"].asInt();
// 		errDesc = tSAAdpRsp["errDesc"].asString();
// 	}
// 	
// }

//////////////////////////////////////////////////////////////
const Json::Value TSAStatusReq::ToJsonWithNode()const
{
	Json::Value node = TSAEventReq::ToJsonWithNode(); 
	JSonHelper::ToJson::AddChildNode(node,"adpId",adpId);

	return node;
}
const std::string TSAStatusReq::ToJson()const
{
	return ToJsonWithNode().toStyledString();	
}


bool TSAStatusReq::FromJson(const Json::Value& jsonNode)
{
	TSAEventReq::FromJson(jsonNode);

	JSonHelper::ToStruct::ParseChildNode(jsonNode,"adpId",adpId);

	return true;
}

bool TSAStatusReq::FromJson(const std::string& json_string)
{
	Json::Reader reader;
	Json::Value jsonNode;

	if (!reader.parse(json_string, jsonNode))
	{
		return false;
	}

	return FromJson(jsonNode);
}

//////////////////////////////////////////////////////////////
const Json::Value TSAStatusRsp::ToJsonWithNode()const
{
	Json::Value node = TSAEventRsp::ToJsonWithNode(); 

	return node;
}

const std::string TSAStatusRsp::ToJson()const
{
	return ToJsonWithNode().toStyledString();
}


bool TSAStatusRsp::FromJson(const Json::Value& jsonNode)
{
	TSAEventRsp::FromJson(jsonNode);

	return true;
}

bool TSAStatusRsp::FromJson(const std::string& json_string)
{
	Json::Reader reader;
	Json::Value jsonNode;

	if (!reader.parse(json_string, jsonNode))
	{
		return false;
	}

	return FromJson(jsonNode);
}

//////////////////////////////////////////////////////////////
const Json::Value TSAAddDevReq::ToJsonWithNode()const
{
	Json::Value node = TSAEventReq::ToJsonWithNode(); 
	JSonHelper::ToJson::AddChildNode(node, "conType", conType);
	JSonHelper::ToJson::AddChildNode(node,"adpId",adpId);
	JSonHelper::ToJson::AddChildNode(node,"devId",devId);
	JSonHelper::ToJson::AddChildNode(node,"devName",devName);
	JSonHelper::ToJson::AddChildNode(node,"manuFacId",manuFacId);
	JSonHelper::ToJson::AddChildNode(node,"dllName",dllName);
	JSonHelper::ToJson::AddChildNode(node,"subType",subType);
	JSonHelper::ToJson::AddChildNode(node,"subInfo",subInfo);
	JSonHelper::ToJson::AddChildNode(node,"devArea",devArea);
	JSonHelper::ToJson::AddChildNode(node,"tDevAddr",tDevAddr);
	JSonHelper::ToJson::AddChildNode(node,"tSerAddr",tSerAddr);
	JSonHelper::ToJson::AddChildNode(node,"username",username);
	JSonHelper::ToJson::AddChildNode(node,"password",password);
	JSonHelper::ToJson::AddChildNode(node,"comAddr",comAddr);
	JSonHelper::ToJson::AddChildNode(node,"subnetMask",subnetMask);
	JSonHelper::ToJson::AddChildNode(node,"gateWay",gateWay);
	JSonHelper::ToJson::AddChildNode(node,"comPort",comPort);
	JSonHelper::ToJson::AddChildNode(node,"bitRate",bitRate);
	JSonHelper::ToJson::AddChildNode(node,"bitNum",bitNum);
	JSonHelper::ToJson::AddChildNode(node,"checkNum",checkNum);
	JSonHelper::ToJson::AddChildNode(node,"stopNum",stopNum);
	JSonHelper::ToJson::AddChildNode(node,"unitNo",unitNo);
	JSonHelper::ToJson::AddChildNode(node,"advCfg",advCfg);
	JSonHelper::ToJson::AddChildNode(node,"modelId",modelId);
	JSonHelper::ToJson::AddChildNode(node,"devNameSpell",devNameSpell);
	JSonHelper::ToJson::AddChildNode(node,"devNameInitials",devNameInitials);
	JSonHelper::ToJson::AddChildNode(node,"invalid",invalid);
	JSonHelper::ToJson::AddChildNode(node,"defendFlag",defendFlag);
	JSonHelper::ToJson::AddChildNode(node,"domainCode",domainCode);
	return node;
}

const std::string TSAAddDevReq::ToJson()const
{
	return ToJsonWithNode().toStyledString();
}

bool TSAAddDevReq::FromJson(const Json::Value& jsonNode)
{
	TSAEventReq::FromJson(jsonNode);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"conType", conType);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"adpId",adpId);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"devId",devId);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"devName",devName);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"manuFacId",manuFacId);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"dllName",dllName);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"subType",subType);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"subInfo",subInfo);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"devArea",devArea);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"tDevAddr",tDevAddr);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"tSerAddr",tSerAddr);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"username",username);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"password",password);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"comAddr",comAddr);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"subnetMask",subnetMask);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"gateWay",gateWay);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"comPort",comPort);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"bitRate",bitRate);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"bitNum",bitNum);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"checkNum",checkNum);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"stopNum",stopNum);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"unitNo",unitNo);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"advCfg",advCfg);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"devModel",modelId);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"devNameSpell",devNameSpell);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"devNameInitials",devNameInitials);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"invalid",invalid);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"defendFlag",defendFlag);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"domainCode",domainCode);
	
	return true;
}

bool TSAAddDevReq::FromJson(const std::string& json_string)
{
	Json::Reader reader;
	Json::Value jsonNode;

	if (!reader.parse(json_string, jsonNode))
	{
		return false;
	}

	return FromJson(jsonNode);
}

//////////////////////////////////////////////////////////////
const Json::Value TSAAddDevRsp::ToJsonWithNode()const
{
	Json::Value node = TSAEventRsp::ToJsonWithNode(); 
	JSonHelper::ToJson::AddChildNode(node,"devId",devId);

	return node;
}

const std::string TSAAddDevRsp::ToJson()const
{
	return ToJsonWithNode().toStyledString();
}


bool TSAAddDevRsp::FromJson(const Json::Value& jsonNode)
{
	TSAEventRsp::FromJson(jsonNode);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"devId",devId);
	return true;
}

bool TSAAddDevRsp::FromJson(const std::string& json_string)
{
	Json::Reader reader;
	Json::Value jsonNode;

	if (!reader.parse(json_string, jsonNode))
	{
		return false;
	}

	return FromJson(jsonNode);
}

//////////////////////////////////////////////////////////////
const Json::Value TSADelDevReq::ToJsonWithNode()const
{
	Json::Value node = TSAEventReq::ToJsonWithNode(); 
	JSonHelper::ToJson::AddChildNode(node,"devId",devId);

	return node;
}

const std::string TSADelDevReq::ToJson()const
{
	return ToJsonWithNode().toStyledString();
}


bool TSADelDevReq::FromJson(const Json::Value& jsonNode)
{
	TSAEventReq::FromJson(jsonNode);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"devId",devId);
	return true;
}

bool TSADelDevReq::FromJson(const std::string& json_string)
{
	Json::Reader reader;
	Json::Value jsonNode;

	if (!reader.parse(json_string, jsonNode))
	{
		return false;
	}

	return FromJson(jsonNode);
}

//////////////////////////////////////////////////////////////
const Json::Value TSADelDevRsp::ToJsonWithNode()const
{
	Json::Value node = TSAEventRsp::ToJsonWithNode(); 
	JSonHelper::ToJson::AddChildNode(node,"devId",devId);

	return node;
}
const std::string TSADelDevRsp::ToJson()const
{
	return ToJsonWithNode().toStyledString();
}


bool TSADelDevRsp::FromJson(const Json::Value& jsonNode)
{
	TSAEventRsp::FromJson(jsonNode);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"devId",devId);
	return true;
}

bool TSADelDevRsp::FromJson(const std::string& json_string)
{
	Json::Reader reader;
	Json::Value jsonNode;

	if (!reader.parse(json_string, jsonNode))
	{
		return false;
	}

	return FromJson(jsonNode);
}

//////////////////////////////////////////////////////////////
const Json::Value TAblitySet::ToJsonWithNode()const
{
	Json::Value node;
	JSonHelper::ToJson::AddChildNode(node,"abItemId",abItemId);
	JSonHelper::ToJson::AddChildNode(node,"abItemValue",abItemValue);

	return node;
}
const std::string TAblitySet::ToJson()const
{
	return ToJsonWithNode().toStyledString();
}

bool TAblitySet::FromJson(const Json::Value& jsonNode)
{
	
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"abItemId",abItemId);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"abItemValue",abItemValue);
	return true;
}

bool TAblitySet::FromJson(const std::string& json_string)
{
	Json::Reader reader;
	Json::Value jsonNode;

	if (!reader.parse(json_string, jsonNode))
	{
		return false;
	}

	return FromJson(jsonNode);
}

//////////////////////////////////////////////////////////////
const Json::Value TSADevStatus::ToJsonWithNode()const
{
	Json::Value node;
	JSonHelper::ToJson::AddChildNode(node,"devId",devId);
	JSonHelper::ToJson::AddChildNode(node,"state",state);
	JSonHelper::ToJson::AddChildNode(node,"devName",devName);
    JSonHelper::ToJson::AddChildNode(node,"nodeNo",nodeNo);
//	JSonHelper::ToJson::AddChildNode(node,"nodeNo",nodeNo);
	return node;
}
const std::string TSADevStatus::ToJson()const
{
	return ToJsonWithNode().toStyledString();
}


bool TSADevStatus::FromJson(const Json::Value& jsonNode)
{
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"devId",devId);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"state",state);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"devName",devName);
    JSonHelper::ToStruct::ParseChildNode(jsonNode,"nodeNo",nodeNo);
	return true;
}

bool TSADevStatus::FromJson(const std::string& json_string)
{
	Json::Reader reader;
	Json::Value jsonNode;

	if (!reader.parse(json_string, jsonNode))
	{
		return false;
	}

	return FromJson(jsonNode);
}

//////////////////////////////////////////////////////////////
const Json::Value TSATime::ToJsonWithNode()const
{
	Json::Value node;
	JSonHelper::ToJson::AddChildNode(node,"year",year);
	JSonHelper::ToJson::AddChildNode(node,"month",month);
	JSonHelper::ToJson::AddChildNode(node,"day",day);
	JSonHelper::ToJson::AddChildNode(node,"hour",hour);
    JSonHelper::ToJson::AddChildNode(node,"minute",minute); 
	JSonHelper::ToJson::AddChildNode(node,"second",second);
	return node;
}

const std::string TSATime::ToJson()const
{
	return ToJsonWithNode().toStyledString();
}


bool TSATime::FromJson(const Json::Value& jsonNode)
{
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"year",year);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"month",month);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"day",day);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"hour",hour);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"minute",minute);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"second",second);
	return true;
}
bool TSATime::FromJson(const std::string& json_string)
{
	Json::Reader reader;
	Json::Value jsonNode;

	if (!reader.parse(json_string, jsonNode))
	{
		return false;
	}

	return FromJson(jsonNode);
}

//////////////////////////////////////////////////////////////
const Json::Value TSARepDevInfo::ToJsonWithNode()const
{
	Json::Value node;
	JSonHelper::ToJson::AddContainer(node, "devlist", devlist);
	JSonHelper::ToJson::AddChildNode(node,"isRepEnd",isRepEnd);
	return node;
}
const std::string TSARepDevInfo::ToJson()const
{
	return ToJsonWithNode().toStyledString();
}

bool TSARepDevInfo::FromJson(const Json::Value& jsonNode)
{
    JSonHelper::ToStruct::ParseContainer(jsonNode, "devlist", devlist);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"isRepEnd",isRepEnd);

	return true;
}

bool TSARepDevInfo::FromJson(const std::string& json_string)
{
	Json::Reader reader;
	Json::Value jsonNode;

	if (!reader.parse(json_string, jsonNode))
	{
		return false;
	}

	return FromJson(jsonNode);
}
//////////////////////////////////////////////////////////////////
const Json::Value TSARepDevStatus::ToJsonWithNode()const
{
	Json::Value node;
	JSonHelper::ToJson::AddContainer(node, "statuslist", statuslist);
	JSonHelper::ToJson::AddChildNode(node,"isRepEnd",isRepEnd);
	return node;
}
const std::string TSARepDevStatus::ToJson()const
{
	return ToJsonWithNode().toStyledString();
}

bool TSARepDevStatus::FromJson(const Json::Value& jsonNode)
{
	JSonHelper::ToStruct::ParseContainer(jsonNode, "statuslist", statuslist);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"isRepEnd",isRepEnd);

	return true;
}

bool TSARepDevStatus::FromJson(const std::string& json_string)
{
	Json::Reader reader;
	Json::Value jsonNode;

	if (!reader.parse(json_string, jsonNode))
	{
		return false;
	}

	return FromJson(jsonNode);
}

///////////////////////////////////////////////////////////////////////////////

const Json::Value TSACtrlDevReq::ToJsonWithNode()const
{
	Json::Value node;
	JSonHelper::ToJson::AddChildNode(node, "devId", devId);
	JSonHelper::ToJson::AddChildNode(node, "devName", devName);
	JSonHelper::ToJson::AddChildNode(node, "subType", subType);
	JSonHelper::ToJson::AddChildNode(node, "subInfo", subInfo);
	return node;
}

const std::string TSACtrlDevReq::ToJson()const
{
	return ToJsonWithNode().toStyledString();
}

bool TSACtrlDevReq::FromJson(const Json::Value& jsonNode)
{
	JSonHelper::ToStruct::ParseChildNode(jsonNode, "devId", devId);
	JSonHelper::ToStruct::ParseChildNode(jsonNode, "devName", devName);
	JSonHelper::ToStruct::ParseChildNode(jsonNode, "subType", subType);
	JSonHelper::ToStruct::ParseChildNode(jsonNode, "subInfo", subInfo);
	return true;
}

bool TSACtrlDevReq::FromJson(const std::string& json_string)
{
	Json::Reader reader;
	Json::Value jsonNode;

	if (!reader.parse(json_string, jsonNode))
	{
		return false;
	}

	return FromJson(jsonNode);
}

////////////////////////////////////////////////////////////////////
const Json::Value TSADoorCtlReq::ToJsonWithNode()const
{
	Json::Value node = TSAEventReq::ToJsonWithNode(); 
	JSonHelper::ToJson::AddChildNode(node, "cfgInfo", cfgInfo);
	JSonHelper::ToJson::AddChildNode(node, "operatorType", operatorType);
	JSonHelper::ToJson::AddChildNode(node, "channel", channel);

	return node;
}

const std::string TSADoorCtlReq::ToJson()const
{
	return ToJsonWithNode().toStyledString();
}

bool TSADoorCtlReq::FromJson(const Json::Value& jsonNode)
{
	TSAEventReq::FromJson(jsonNode);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"cfgInfo", cfgInfo);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"operatorType",operatorType);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"channel",channel);

	return true;
}
bool TSADoorCtlReq::FromJson(const std::string& json_string)
{
	Json::Reader reader;
	Json::Value jsonNode;

	if (!reader.parse(json_string, jsonNode))
	{
		return false;
	}

	return FromJson(jsonNode);
}
//////////////////////////////////////////////////////////////
const Json::Value TSADoorCtlRsp::ToJsonWithNode()const
{
	Json::Value node = TSAEventRsp::ToJsonWithNode(); 
	JSonHelper::ToJson::AddChildNode(node, "devId", devId);
	JSonHelper::ToJson::AddChildNode(node, "result", result);
	return node;
}

const std::string TSADoorCtlRsp::ToJson()const
{
	return ToJsonWithNode().toStyledString();
}

bool TSADoorCtlRsp::FromJson(const Json::Value& jsonNode)
{
	TSAEventRsp::FromJson(jsonNode);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"devId", devId);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"result", result);
	return true;
}
bool TSADoorCtlRsp::FromJson(const std::string& json_string)
{
	Json::Reader reader;
	Json::Value jsonNode;

	if (!reader.parse(json_string, jsonNode))
	{
		return false;
	}

	return FromJson(jsonNode);
}
//////////////////////////////////////////////////////////////
const Json::Value CItemdata::ToJsonWithNode()const
{
	Json::Value node;
	JSonHelper::ToJson::AddChildNode(node,"statusCode",statusCode);
	return node;
}

const std::string CItemdata::ToJson()const
{
	return ToJsonWithNode().toStyledString();	
}

bool CItemdata::FromJson(const Json::Value& jsonNode)
{
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"statusCode",statusCode);
	return true;
}

bool CItemdata::FromJson(const std::string& json_string)
{
	Json::Reader reader;
	Json::Value jsonNode;

	if (!reader.parse(json_string, jsonNode))
	{
		return false;
	}

	return FromJson(jsonNode);
}
//////////////////////////////////////////////////////////////
const Json::Value CResourceCtrlNtyItem::ToJsonWithNode()const
{
	Json::Value node;
	JSonHelper::ToJson::AddChildNode(node,"resourceId",resourceId);
	JSonHelper::ToJson::AddChildNode(node,"dataTypeCode",dataTypeCode);
	JSonHelper::ToJson::AddChildNode(node,"devTypecode",devTypecode);
	JSonHelper::ToJson::AddChildNode(node,"layerTypeCode",layerTypeCode);
	JSonHelper::ToJson::AddChildNode(node,"data",data);
	return node;
}

const std::string CResourceCtrlNtyItem::ToJson()const
{
	return ToJsonWithNode().toStyledString();	
}

bool CResourceCtrlNtyItem::FromJson(const Json::Value& jsonNode)
{
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"resourceId",resourceId);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"dataTypeCode",dataTypeCode);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"devTypecode",devTypecode);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"layerTypeCode",layerTypeCode);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"data",data);
	return true;
}

bool CResourceCtrlNtyItem::FromJson(const std::string& json_string)
{
	Json::Reader reader;
	Json::Value jsonNode;

	if (!reader.parse(json_string, jsonNode))
	{
		return false;
	}

	return FromJson(jsonNode);
}
//////////////////////////////////////////////////////////////
const Json::Value CSnsNotifySendMessage::ToJsonWithNode()const
{
	Json::Value node; 
	JSonHelper::ToJson::AddChildNode(node,"notifyType",notifyType);
	JSonHelper::ToJson::AddChildNode(node,"dataType",dataType);
	JSonHelper::ToJson::AddChildNode(node,"notifyParam",notifyParam);
	JSonHelper::ToJson::AddChildNode(node,"notifyKey",notifyKey);
	JSonHelper::ToJson::AddChildNode(node,"notifyData",notifyData);
	return node;
}

const std::string CSnsNotifySendMessage::ToJson()const
{
	return ToJsonWithNode().toStyledString();	
}

bool CSnsNotifySendMessage::FromJson(const Json::Value& jsonNode)
{

	JSonHelper::ToStruct::ParseChildNode(jsonNode,"notifyType",notifyType);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"dataType",dataType);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"notifyParam",notifyParam);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"notifyKey",notifyKey);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"notifyData",notifyData);
	return true;
}

bool CSnsNotifySendMessage::FromJson(const std::string& json_string)
{
	Json::Reader reader;
	Json::Value jsonNode;

	if (!reader.parse(json_string, jsonNode))
	{
		return false;
	}

	return FromJson(jsonNode);
}
//////////////////////////////////////////////////////////////
const Json::Value THCHeartNty::ToJsonWithNode()const
{
	Json::Value node; 
	JSonHelper::ToJson::AddChildNode(node,"pid",pid);
	JSonHelper::ToJson::AddChildNode(node,"name",name);
	return node;
}

const std::string THCHeartNty::ToJson()const
{
	return ToJsonWithNode().toStyledString();	
}

bool THCHeartNty::FromJson(const Json::Value& jsonNode)
{

	JSonHelper::ToStruct::ParseChildNode(jsonNode,"pid",pid);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"name",name);
	return true;
}

bool THCHeartNty::FromJson(const std::string& json_string)
{
	Json::Reader reader;
	Json::Value jsonNode;

	if (!reader.parse(json_string, jsonNode))
	{
		return false;
	}

	return FromJson(jsonNode);
}
//////////////////////////////////////////////////////////////
const Json::Value THCHeartNtyRsp::ToJsonWithNode()const
{
	Json::Value node; 
	JSonHelper::ToJson::AddChildNode(node,"errcode",errcode);
	JSonHelper::ToJson::AddChildNode(node,"errdsc",errdsc);
	return node;
}

const std::string THCHeartNtyRsp::ToJson()const
{
	return ToJsonWithNode().toStyledString();	
}

bool THCHeartNtyRsp::FromJson(const Json::Value& jsonNode)
{

	JSonHelper::ToStruct::ParseChildNode(jsonNode,"errcode",errcode);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"errdsc",errdsc);
	return true;
}

bool THCHeartNtyRsp::FromJson(const std::string& json_string)
{
	Json::Reader reader;
	Json::Value jsonNode;

	if (!reader.parse(json_string, jsonNode))
	{
		return false;
	}

	return FromJson(jsonNode);
}

//////////////////////////////////////////////////////////////
const Json::Value TSABatchCtrlDev::ToJsonWithNode()const
{
	Json::Value node = TSAEventReq::ToJsonWithNode(); 
	JSonHelper::ToJson::AddChildNode(node,"resourceId",resourceId);
	JSonHelper::ToJson::AddChildNode(node,"resourceOwerId",resourceOwerId);
	JSonHelper::ToJson::AddChildNode(node,"layerTypeCode",layerTypeCode);
	JSonHelper::ToJson::AddChildNode(node,"optionTypeCode",optionTypeCode);
	JSonHelper::ToJson::AddChildNode(node,"defendSubInfo",defendSubInfo);
	JSonHelper::ToJson::AddChildNode(node,"uptKey",uptKey);
	JSonHelper::ToJson::AddChildNode(node,"subSysNo",subSysNo);
	JSonHelper::ToJson::AddChildNode(node,"zoneNo",zoneNo);
	JSonHelper::ToJson::AddChildNode(node,"kafkaKey",kafkaKey);
	JSonHelper::ToJson::AddChildNode(node,"serialNo",serialNo);

	return node;
}
const std::string TSABatchCtrlDev::ToJson()const
{
	return ToJsonWithNode().toStyledString();
}

bool TSABatchCtrlDev::FromJson(const Json::Value& jsonNode)
{
	TSAEventReq::FromJson(jsonNode);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"resourceId",resourceId);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"resourceOwerId",resourceOwerId);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"layerTypeCode",layerTypeCode);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"optionTypeCode",optionTypeCode);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"defendSubInfo",defendSubInfo);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"uptKey",uptKey);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"subSysNo",subSysNo);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"zoneNo",zoneNo);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"kafkaKey",kafkaKey);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"serialNo",serialNo);
	return true;
}

bool TSABatchCtrlDev::FromJson(const std::string& json_string)
{
	Json::Reader reader;
	Json::Value jsonNode;

	if (!reader.parse(json_string, jsonNode))
	{
		return false;
	}

	return FromJson(jsonNode);
}

///////////////////////////////////////////////////////////////////////////////

const Json::Value TSABatchDefendReq::ToJsonWithNode()const
{
	Json::Value node = TSAEventReq::ToJsonWithNode(); 
	JSonHelper::ToJson::AddContainer(node, "batchCtrlDevs", batchCtrlDevs);
	return node;
}

const std::string TSABatchDefendReq::ToJson()const
{
	return ToJsonWithNode().toStyledString();
}

bool TSABatchDefendReq::FromJson(const Json::Value& jsonNode)
{
	TSAEventReq::FromJson(jsonNode);
	JSonHelper::ToStruct::ParseContainer(jsonNode, "batchCtrlDevs", batchCtrlDevs);
	return true;
}

bool TSABatchDefendReq::FromJson(const std::string& json_string)
{
	Json::Reader reader;
	Json::Value jsonNode;

	if (!reader.parse(json_string, jsonNode))
	{
		return false;
	}

	return FromJson(jsonNode);
}
//////////////////////////////////////////////////////////////
const Json::Value TSABatchDefendRsp::ToJsonWithNode()const
{
	Json::Value node = TSAEventRsp::ToJsonWithNode(); 
	JSonHelper::ToJson::AddChildNode(node, "resourceId", resourceId);
	JSonHelper::ToJson::AddChildNode(node, "uptKey", uptKey);
	JSonHelper::ToJson::AddChildNode(node, "serialNo", serialNo);
	JSonHelper::ToJson::AddChildNode(node, "kafkaKey", kafkaKey);
	JSonHelper::ToJson::AddChildNode(node, "layerTypeCode", layerTypeCode);
	JSonHelper::ToJson::AddChildNode(node, "optionTypeCode", optionTypeCode);
	JSonHelper::ToJson::AddChildNode(node, "resourceOwerId", resourceOwerId);
	return node;
}

const std::string TSABatchDefendRsp::ToJson()const
{
	return ToJsonWithNode().toStyledString();
}

bool TSABatchDefendRsp::FromJson(const Json::Value& jsonNode)
{
	TSAEventRsp::FromJson(jsonNode);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"resourceId", resourceId);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"uptKey", uptKey);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"serialNo", serialNo);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"kafkaKey", kafkaKey);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"layerTypeCode", layerTypeCode);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"optionTypeCode", optionTypeCode);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"resourceOwerId", resourceOwerId);
	return true;
}
bool TSABatchDefendRsp::FromJson(const std::string& json_string)
{
	Json::Reader reader;
	Json::Value jsonNode;

	if (!reader.parse(json_string, jsonNode))
	{
		return false;
	}

	return FromJson(jsonNode);
}
//////////////////////////////////////////////////////////////
const Json::Value TSASecNodeItem::ToJsonWithNode()const
{
	Json::Value node; 
	JSonHelper::ToJson::AddChildNode(node,"nodeNo",nodeNo);
	JSonHelper::ToJson::AddChildNode(node,"nodeName",nodeName);
	JSonHelper::ToJson::AddChildNode(node,"extInfo",extInfo);
	return node;
}

const std::string TSASecNodeItem::ToJson()const
{
	return ToJsonWithNode().toStyledString();	
}

bool TSASecNodeItem::FromJson(const Json::Value& jsonNode)
{

	JSonHelper::ToStruct::ParseChildNode(jsonNode,"nodeNo",nodeNo);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"nodeName",nodeName);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"extInfo",extInfo);
	return true;
}

bool TSASecNodeItem::FromJson(const std::string& json_string)
{
	Json::Reader reader;
	Json::Value jsonNode;

	if (!reader.parse(json_string, jsonNode))
	{
		return false;
	}

	return FromJson(jsonNode);
}

//////////////////////////////////////////////////////////////
const Json::Value TSAFstNodeItem::ToJsonWithNode()const
{
	Json::Value node; 
	JSonHelper::ToJson::AddChildNode(node,"nodeNo",nodeNo);
	JSonHelper::ToJson::AddChildNode(node,"nodeName",nodeName);
	JSonHelper::ToJson::AddChildNode(node,"extInfo",extInfo);
	JSonHelper::ToJson::AddContainer(node,"secNodeItems",secNodeItems);
	return node;
}

const std::string TSAFstNodeItem::ToJson()const
{
	return ToJsonWithNode().toStyledString();	
}

bool TSAFstNodeItem::FromJson(const Json::Value& jsonNode)
{

	JSonHelper::ToStruct::ParseChildNode(jsonNode,"nodeNo",nodeNo);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"nodeName",nodeName);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"extInfo",extInfo);
	JSonHelper::ToStruct::ParseContainer(jsonNode,"secNodeItems",secNodeItems);
	return true;
}

bool TSAFstNodeItem::FromJson(const std::string& json_string)
{
	Json::Reader reader;
	Json::Value jsonNode;

	if (!reader.parse(json_string, jsonNode))
	{
		return false;
	}

	return FromJson(jsonNode);
}

//////////////////////////////////////////////////////////////
const Json::Value TSADevInfo::ToJsonWithNode()const
{
	Json::Value node; 
	JSonHelper::ToJson::AddChildNode(node,"devId",devId);
	JSonHelper::ToJson::AddChildNode(node,"extInfo",extInfo);
	JSonHelper::ToJson::AddContainer(node,"fstNodeItems",fstNodeItems);
	return node;
}

const std::string TSADevInfo::ToJson()const
{
	return ToJsonWithNode().toStyledString();	
}

bool TSADevInfo::FromJson(const Json::Value& jsonNode)
{

	JSonHelper::ToStruct::ParseChildNode(jsonNode,"devId",devId);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"extInfo",extInfo);
	JSonHelper::ToStruct::ParseContainer(jsonNode,"fstNodeItems",fstNodeItems);
	return true;
}

bool TSADevInfo::FromJson(const std::string& json_string)
{
	Json::Reader reader;
	Json::Value jsonNode;

	if (!reader.parse(json_string, jsonNode))
	{
		return false;
	}

	return FromJson(jsonNode);
}

//////////////////////////////////////////////////////////////
const Json::Value TSATransDataNty::ToJsonWithNode()const
{
	Json::Value node; 
	JSonHelper::ToJson::AddChildNode(node,"devId",devId);
	JSonHelper::ToJson::AddChildNode(node,"filed",filed);
	JSonHelper::ToJson::AddChildNode(node,"msgNo",msgNo);
	JSonHelper::ToJson::AddChildNode(node,"dwSn",dwSn);
	JSonHelper::ToJson::AddChildNode(node,"dataFormat",dataFormat);
	JSonHelper::ToJson::AddChildNode(node,"transData",transData);
	return node;
}

const std::string TSATransDataNty::ToJson()const
{
	return ToJsonWithNode().toStyledString();	
}

bool TSATransDataNty::FromJson(const Json::Value& jsonNode)
{

	JSonHelper::ToStruct::ParseChildNode(jsonNode,"devId",devId);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"filed",filed);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"msgNo",msgNo);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"dwSn",dwSn);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"dataFormat",dataFormat);
	JSonHelper::ToStruct::ParseChildNode(jsonNode,"transData",transData);
	return true;
}

bool TSATransDataNty::FromJson(const std::string& json_string)
{
	Json::Reader reader;
	Json::Value jsonNode;

	if (!reader.parse(json_string, jsonNode))
	{
		return false;
	}

	return FromJson(jsonNode);
}

//////////////////////////////////////////////////////////////
const Json::Value TSAExtStatusInfo::ToJsonWithNode()const
{
	Json::Value node; 
	JSonHelper::ToJson::AddChildNode(node,"signStatusCode",signStatusCode);
	return node;
}

const std::string TSAExtStatusInfo::ToJson()const
{
	return ToJsonWithNode().toStyledString();	
}

bool TSAExtStatusInfo::FromJson(const Json::Value& jsonNode)
{

	JSonHelper::ToStruct::ParseChildNode(jsonNode,"signStatusCode",signStatusCode);
	return true;
}

bool TSAExtStatusInfo::FromJson(const std::string& json_string)
{
	Json::Reader reader;
	Json::Value jsonNode;

	if (!reader.parse(json_string, jsonNode))
	{
		return false;
	}

	return FromJson(jsonNode);
}

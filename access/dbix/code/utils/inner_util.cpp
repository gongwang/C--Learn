#if WIN32 && (_MSC_VER <= 1200)
#pragma warning(disable : 4786)
#endif

#include "inner_util.h"

DBIX_NS_BEGIN

void _replace_str(string& str, const string& strSrc, const string& strDst) 
{
    string::size_type pos = 0;
    string::size_type srcLen = strSrc.size();
    string::size_type dstLen = strDst.size();
    while((pos = str.find(strSrc, pos)) != string::npos)
    {         
        str.replace(pos, srcLen, strDst);                
        pos += dstLen;        
    }
}

std::string DBData( const Variant &val, ENDBDataType enDBDataType )
{
    if (enDBDataTypeString == enDBDataType)
    {
        string ttt = variant_cast<string>(val);
        return DBData(ttt);
    }
    else if (enDBDataTypeS32 == enDBDataType)
    {
        return DBData(variant_cast<s32>(val));
    }
    else if (enDBDataTypeBool == enDBDataType)
    {
        return DBData(variant_cast<bool>(val));
    }
    else if (enDBDataTypeDouble == enDBDataType)
    {
        return DBData(variant_cast<double>(val));
    }
    else if (enDBDataTypeDate == enDBDataType)
    {
        return DBData(variant_cast<CCMSTime>(val));
    }
    else
    {
        assert(false && "DBData detected not supportted datatype");
        return "";
    }
}

DBIX_NS_END

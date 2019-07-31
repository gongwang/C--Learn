#include "base64/base64.h"
#include "stdio.h"
#include "string.h"
#include "windows.h"

using namespace std;

#define READ_FILE	0

void main()
{
// 	FILE* pFile = fopen("E:/VS2008 Project/Base64Demo/ace.bmp", "rb");
// 	if(pFile)
// 	{
// 		fseek(pFile, 0, SEEK_END);
// 		int nSize = ftell(pFile);
// 		fseek(pFile, 0, SEEK_SET);
// 		char* pSrcBmp = (char*)malloc(nSize);
// 		fread(pSrcBmp, 1, nSize, pFile);
// 		printf("start %u\n", GetTickCount());
// 		std::string strData = base64_encode((unsigned char *)pSrcBmp, nSize);
// 		printf("end %u\n", GetTickCount());
// 		std::string strData2 = base64_decode(strData);
// 
// 		{
// 			FILE* pDstFile = fopen("d:/result_enc.dat", "wb");
// 			if(pDstFile)
// 			{
// 				fwrite(strData.data(), strData.size(), 1, pDstFile);
// 				fclose(pDstFile);
// 
// 			}
// 		}
// 		{
// 			FILE* pDstFile = fopen("d:/result_dec.bmp", "wb");
// 			if(pDstFile)
// 			{
// 				fwrite(strData2.data(), strData2.size(), 1, pDstFile);
// 				fclose(pDstFile);
// 			}
// 		}
// 		fclose(pFile);
	// 	}
// 	{
// 		std::string strDeData = "{\"logon\":{\"username\":\"admin\"}}";
// 		std::string strData = base64_encode((unsigned char*)strDeData.c_str(), strDeData.size());
// 		int a = 0;
// 	}
	{
// 		std::string strDeData = "JTdCJTIyY21kJTIyJTNBJTIybG9nb24lMjIlMkMlMjJoZWFkJTIyJTNBJTdCJTIyc2Vzc2lvbmlkJTIyJTNBMTU1MzY2NjYwMiUyQyUyMnNlcWlkJTIyJTNBMTU1MzY1NDI1NyU3RCUyQyUyMnJlY29kZSUyMiUzQSU3QiUyMnVzZXJuYW1lJTIyJTNBJTIyYWRtaW4lMjIlMkMlMjJwYXNzd2QlMjIlM0ElMjJZV1J0YVc0eE1qTSUzRCUyMiUyQyUyMmxvY2FsaXAlMjIlM0ElMjIwLjAuMC4wJTIyJTdEJTdE";
// 		std::string strData = base64_decode(strDeData);

		int nSize = 1*1000*1000;
		char *pData = new char[nSize];
		memset(pData, 0,nSize);
		FILE *pFileSrc = fopen("src.data", "rb");
		if(pFileSrc)
		{
			fread(pData, 1, nSize, pFileSrc);
			fclose(pFileSrc);
		}
		std::string strData = base64_decode(pData);

		FILE* pFile = fopen("aaaa.jpg", "wb+");
		if(pFile)
		{
			fwrite(strData.data(), 1, strData.size(), pFile);
			fclose(pFile);
		}
		int a = 0;
	}

	system("pause");
	int a = 0;
}

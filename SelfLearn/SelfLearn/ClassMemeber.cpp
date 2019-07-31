#include <iostream>
#include<string.h>
#include <time.h>

using namespace std;

class point
{
public:
    int x;
    int y;
    void intput(int a ,int b)
    {
        x = a;
        y = b;
    }
    void output()
    {
        cout<<x<<endl<<y<<endl;
    }
protected:

private:

};

//void main()
//{
//    point pt;
//    pt.intput(5,10);
//    pt.output();
//    system("pause");
//}

void main()
{
	double i = 0.0;
	double j = 0.0;
	double k = fabs(i - j);
	if (k == 0)
	{
		printf("hello \n");
	}

	for (size_t i = 0; i < 0; i++)
	{
		cout << "loop" << endl;
		_sleep(100);
	}

	string s1("172.16.128.51");
	string s2(".");
	int nPos = s1.compare(s2);
	if (s1.compare(s2) > 0)
	{
		cout << "nPos is at: " << nPos << endl;
	}


	wchar_t *achPrintBuf = new wchar_t[2024];
	memset(achPrintBuf, NULL, 2024);

    //2019-05-23 10:11:12
    char pDataTime[128] ="2019-05-23 10:11:12";
    char cTimeYear[8] = {0};
    char cTimeMonth[8] = {0};
    char cTimeDay[8] = {0};
    char cTimeHour[8] = {0};
    char cTimeMin[8] = {0};
    char cTimeSec[8] = {0};
    int length  = strlen(pDataTime);
    if (strlen(pDataTime) < 19)
    {
        printf("时间格式不正确!");
    }else
    { //2019-05-23 10:11:12
        //strncpy(cTimeYear, pDataTime, 4);
        memcpy_s(cTimeYear,sizeof(cTimeYear),pDataTime, 4);
        memcpy_s(cTimeMonth,sizeof(cTimeMonth),pDataTime+5, 2);
        memcpy_s(cTimeDay,sizeof(cTimeDay),pDataTime+8, 2);
        memcpy_s(cTimeHour,sizeof(cTimeHour),pDataTime+ 11, 2);       
        memcpy_s(cTimeMin,sizeof(cTimeMin),pDataTime+14, 2);
        memcpy_s(cTimeSec,sizeof(cTimeSec),pDataTime+17, 2);
    }
    int m_wYear = atoi(cTimeYear);
    int m_byMonth = atoi(cTimeMonth);
    int m_byDay = atoi(cTimeDay);
    int m_byHour = atoi(cTimeHour);
    int m_byMinute = atoi(cTimeMin);
    int m_bySecond =atoi(cTimeSec);
    
    //2019-05-23 10:11:12

    struct tm t_time;
    t_time.tm_year = atoi(cTimeYear) - 1900;
    t_time.tm_mon = atoi(cTimeMonth) - 1;
    t_time.tm_mday = atoi(cTimeDay);
    t_time.tm_hour = atoi(cTimeHour);
    t_time.tm_min = atoi(cTimeMin);
    t_time.tm_sec =atoi(cTimeSec);

    long long time  = mktime(&t_time);

    printf("time : %d", time);
    system("pause");
}
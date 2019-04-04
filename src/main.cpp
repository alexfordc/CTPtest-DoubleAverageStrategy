#include <iostream>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include "CustomMdSpi.h"
#include "CustomTradeSpi.h"
#include "TickToKlineHelper.h"
#include "StrategyTrade.h"
#include <map>
#include <thread>

using namespace std;

// ���ӿ�
#pragma comment (lib, "thostmduserapi.lib")
#pragma comment (lib, "thosttraderapi.lib")


// ---- ȫ�ֱ��� ---- //
// ��������
TThostFtdcBrokerIDType gBrokerID = "9999";                         // ģ�⾭���̴���
TThostFtdcInvestorIDType gInvesterID = "135267";                         // Ͷ�����˻���
TThostFtdcPasswordType gInvesterPassword = "simnow314";                     // Ͷ��������

// �������
CThostFtdcMdApi *g_pMdUserApi = nullptr;                           // ����ָ��
char gMdFrontAddr[] = "tcp://180.168.146.187:10031";               // ģ������ǰ�õ�ַ
char *g_pInstrumentID[] = { "rb1910" };							   // �����Լ�����б��С��ϡ���֣��������ѡһ��
int instrumentNum = 1;                                             // �����Լ��������
unordered_map<string, TickToKlineHelper> g_KlineHash;              // ��ͬ��Լ��k�ߴ洢��

// ���ײ���
CThostFtdcTraderApi *g_pTradeUserApi = nullptr;                    // ����ָ��
char gTradeFrontAddr[] = "tcp://180.168.146.187:10030";            // ģ�⽻��ǰ�õ�ַ
TThostFtdcInstrumentIDType g_pTradeInstrumentID = "rb1910";        // �����׵ĺ�Լ����
TThostFtdcDirectionType gTradeDirection = THOST_FTDC_D_Sell;       // ��������
TThostFtdcPriceType gLimitPrice = 22735;                           // ���׼۸�
CThostFtdcInvestorPositionField *g_InvestorPosition;				   // ��ĳֲ����

CThostFtdcDepthMarketDataField *depthdata[200] = { 0 };			   //�������
CustomMdSpi *gMDSpi;											   // ����ӿ�ʵ��	
CustomTradeSpi *pTradeSpi;										   // ����ʵ��
CThostFtdcMdSpi *pMdUserSpi;									   // ����ӿ�

// ��¼
int OnFrontConnected() {
	CThostFtdcReqUserLoginField *reqUserLogin = new CThostFtdcReqUserLoginField();
	strcpy(reqUserLogin->BrokerID, gBrokerID);
	strcpy(reqUserLogin->UserID, gInvesterID);
	strcpy(reqUserLogin->Password, gInvesterPassword);

	return g_pTradeUserApi->ReqUserLogin(reqUserLogin, 0); // ��¼
}


void  SetRejectdataTime(double  begintime1, double endtime1, double begintime2, double endtime2, double begintime3, double endtime3, double begintime4, double endtime4)
{
	if (begintime1 < 0 || endtime1 < 0 || begintime2 < 0 || endtime2 < 0 || begintime3 < 0 || endtime3 < 0 || begintime4 < 0 || endtime4 < 0)
	{
		printf("���þ��������ʱ��α�����ڵ���0\n");
	}

	if (begintime1 != 100 && endtime1 != 100)
	{
		printf("[%0.06f ~ %0.06f]�������ݵ�ʱ���\n", begintime1, endtime1);
		gMDSpi->begintime1 = begintime1;
		gMDSpi->endtime1 = endtime1;
	}

	if (begintime2 != 100 && endtime2 != 100)
	{
		printf("[%0.06f ~ %0.06f]�������ݵ�ʱ���\n", begintime2, endtime2);
		gMDSpi->begintime2 = begintime2;
		gMDSpi->endtime2 = endtime2;
	}

	if (begintime3 != 100 && endtime3 != 100)
	{
		printf("[%0.06f ~ %0.06f]�������ݵ�ʱ���\n", begintime3, endtime3);
		gMDSpi->begintime3 = begintime3;
		gMDSpi->endtime3 = endtime3;

	}

	if (begintime3 != 100 && endtime3 != 100)
	{
		printf("[%0.06f ~ %0.06f]�������ݵ�ʱ���\n", begintime4, endtime4);
		gMDSpi->begintime4 = begintime4;
		gMDSpi->endtime4 = endtime4;
	}

}

double GetTimeSec(SYSTEMTIME sys_time)
{
	double system_times;
	system_times = (double)((sys_time.wHour) / 10e1) + (double)((sys_time.wMinute) / 10e3) + (double)((sys_time.wSecond) / 10e5);	//��ʽʱ��0.145100
	return system_times;
}

// �ж��Ƿ��ǽ���ʱ��
bool IsTradeTime() {
	SYSTEMTIME sys_time;
	//������ֵ����Ϊ����ʱ��
	GetLocalTime(&sys_time);

	SYSTEMTIME beginTime1 = { sys_time.wYear,sys_time.wMonth,sys_time.wDayOfWeek ,sys_time.wDay ,9 ,30 ,0,0 };
	SYSTEMTIME endTime1 = { sys_time.wYear,sys_time.wMonth,sys_time.wDayOfWeek ,sys_time.wDay ,11 ,30 ,0,0 };
	SYSTEMTIME beginTime2 = { sys_time.wYear,sys_time.wMonth,sys_time.wDayOfWeek ,sys_time.wDay ,13 ,0 ,0,0 };
	SYSTEMTIME endTime2 = { sys_time.wYear,sys_time.wMonth,sys_time.wDayOfWeek ,sys_time.wDay ,15 ,0 ,0,0 };

	double STime1 = GetTimeSec(beginTime1);
	double ETime1 = GetTimeSec(endTime1);
	double STime2 = GetTimeSec(beginTime2);
	double ETime2 = GetTimeSec(endTime2);

	double NowTime = GetTimeSec(sys_time);

	if (NowTime >= STime1&&NowTime <= ETime1) {
		return true;
	}

	if (NowTime >= STime2&&NowTime <= ETime2) {
		return true;
	}

	return false;
}

// �ɽ��ص�ƽ��
void CallBackCloseOrder(bool flag, int iOrderRef) 
{
	// ƽ��
	int orderRef = pTradeSpi->InsertOrder(g_pInstrumentID[0], '0', '1', '2', depthdata[gMDSpi->gMarket[g_pInstrumentID[0]]]->LastPrice + 20, 1);
}

// �鿴�����Ƿ�ɽ�
bool isOrderSuccess(int orderRef) {
	bool flag = false;
	char *vorderRef = new char[13];
	_snprintf_s(vorderRef, sizeof(TThostFtdcOrderRefType), sizeof(TThostFtdcOrderRefType) - 1, "%012d", orderRef);
	if (pTradeSpi->tradeOrderRefs.find(vorderRef) != pTradeSpi->tradeOrderRefs.end()) {
		// ���׳ɽ�Ϊ1��ʧ��0
		if (pTradeSpi->tradeOrderRefs[vorderRef] == 1) 
		{
			flag = true;
		}
	}

	return flag;
}


int main()
{
	// �˺�����
	//cout << "�������˺ţ� ";
	//scanf("%s", gInvesterID);
	//cout << "���������룺 ";
	//scanf("%s", gInvesterPassword);

	// ��ʼ�������߳�
	cout << "��ʼ������..." << endl;
	g_pMdUserApi = CThostFtdcMdApi::CreateFtdcMdApi();   // ��������ʵ��
	pMdUserSpi = new CustomMdSpi;       // ��������ص�ʵ��
	gMDSpi = new CustomMdSpi();			// ����ʵ��ʵ��
	g_pMdUserApi->RegisterSpi(pMdUserSpi);               // ע���¼���
	g_pMdUserApi->RegisterFront(gMdFrontAddr);           // ��������ǰ�õ�ַ
	g_pMdUserApi->Init();                                // ��������


	// ��ʼ�������߳�
	cout << "��ʼ������..." << endl;
	g_pTradeUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi(); // ��������ʵ��
	//CThostFtdcTraderSpi *pTradeSpi = new CustomTradeSpi;
	pTradeSpi = new CustomTradeSpi;               // �������׻ص�ʵ��
	g_pTradeUserApi->RegisterSpi(pTradeSpi);                      // ע���¼���
	g_pTradeUserApi->SubscribePublicTopic(THOST_TERT_RESTART);    // ���Ĺ�����
	g_pTradeUserApi->SubscribePrivateTopic(THOST_TERT_RESTART);   // ����˽����
	g_pTradeUserApi->RegisterFront(gTradeFrontAddr);              // ���ý���ǰ�õ�ַ
	g_pTradeUserApi->Init();                                      // ��������
	pTradeSpi->OnFrontConnected();								  // ���׵�¼ 


	// ******* ����ҵ��Ա *********
	// ���þ���ʱ��
	SetRejectdataTime(0.0400, 0.0840, 0.1530, 0.2030, NULL, NULL, NULL, NULL);

	// ������������
	if (!IsTradeTime()) 
	{
		// ��������
		g_pMdUserApi->SubscribeMarketData(&g_pInstrumentID[0], 1);	// rb1910

		// �ȴ�����Ԥ��
		Sleep(2000);
		DoubleAverageStrategy DAS;									// ���ɲ���ʵ��
		while(1)
		{
			// ���Խ���
			std::cout << "=====��ʼ������Խ���=====" << std::endl;
			cout<<"��ʱK�ߴ洢�����ݸ���Ϊ��"<<g_KlineHash[g_pInstrumentID[0]].m_KLineDataArray.size()<<endl;
			//cout<<"��ʱK�ߴ洢�Ƿ�Ϊ��:"<<g_KlineHash[g_pInstrumentID[0]].m_KLineDataArray.empty()<<endl;
		
			DAS.StrategyCheckAndTrade(&g_pTradeInstrumentID[0], pTradeSpi);
			Sleep(500);
		}
	}

	// �ȵ��߳��˳�
	g_pMdUserApi->Join();
	delete pMdUserSpi;
	g_pMdUserApi->Release();

	g_pTradeUserApi->Join();
	delete pTradeSpi;
	g_pTradeUserApi->Release();

	getchar();
	return 0;
}
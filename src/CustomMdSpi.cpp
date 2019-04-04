#include <iostream>
#include <fstream>
#include <unordered_map>
#include "CustomMdSpi.h"
#include "TickToKlineHelper.h"
#include <time.h>
#include <Windows.h>

#pragma region MyRegion
int max_pure_tick_num = 130;  //����120TICK�������� ������
int max_pure_tick_move_num = 129; //����120TICK�������� ������

int max_pure_m1_num = 480;  //ÿ1���� 120TICK
int max_pure_m1_move_num = 479;  //ÿ1���� 120TICK

int max_pure_m3_num = 361;  //160  //ÿ3���� 360TICK
int max_pure_m3_move_num = 360;  //159  //ÿ3���� 360TICK

int max_pure_m5_num = 96;  //ÿ3���� 360TICK
int max_pure_m5_move_num = 95; //ÿ3���� 360TICK

int max_pure_m10_num = 96;  //ÿ3���� 360TICK
int max_pure_m10_move_num = 95; //ÿ3���� 360TICK

int  max_pure_m15_num = 96;  //ÿ3���� 360TICK
int  max_pure_m15_move_num = 95; //ÿ3���� 360TICK

int  max_pure_m30_num = 96;  //ÿ3���� 360TICK
int  max_pure_m30_move_num = 95; //ÿ3���� 360TICK

int  max_pure_m60_num = 96; //ÿ3���� 360TICK
int  max_pure_m60_move_num = 95; //ÿ3���� 360TICK

int  max_pure_m120_num = 96;  //ÿ3���� 360TICK
int  max_pure_m120_move_num = 95; //ÿ3���� 360TICK

int  max_pure_m240_num = 96;  //ÿ3���� 360TICK
int  max_pure_m240_move_num = 95;//ÿ3���� 360TICK

int  max_pure_d1_num = 30;  //ÿ3���� 360TICK
int  max_pure_d1_move_num = 29; //ÿ3���� 360TICK

int  max_pure_m480_num = 100;  //��ÿ��8Сʱ����Ϊ480����
int  max_pure_m480_move_num = 99;  //��ÿ��8Сʱ����Ϊ480����
#pragma endregion


// ---- ȫ�ֲ������� ---- //
extern CThostFtdcMdApi *g_pMdUserApi;            // ����ָ��
extern char gMdFrontAddr[];                      // ģ������ǰ�õ�ַ
extern TThostFtdcBrokerIDType gBrokerID;         // ģ�⾭���̴���
extern TThostFtdcInvestorIDType gInvesterID;     // Ͷ�����˻���
extern TThostFtdcPasswordType gInvesterPassword; // Ͷ��������
extern char *g_pInstrumentID[];                  // �����Լ�����б��С��ϡ���֣��������ѡһ��
extern int instrumentNum;                        // �����Լ��������
extern std::unordered_map<std::string, TickToKlineHelper> g_KlineHash; // k�ߴ洢��

extern char* ppInstrumentID[];
extern CThostFtdcDepthMarketDataField *depthdata[];
extern int size;
extern int amount;

#pragma region ��������
// ��ȡ��ǰʱ��doubleֵ
double GetLocalTimeSec2()
{
	SYSTEMTIME sys_time;
	GetLocalTime(&sys_time);
	double system_times;
	system_times = (double)((sys_time.wHour) / 10e1) + (double)((sys_time.wMinute) / 10e3) + (double)((sys_time.wSecond) / 10e5);	//��ʽʱ��0.145100
	return system_times;
}

#pragma endregion


// ---- ctp_api�ص����� ---- //
// ���ӳɹ�Ӧ��
void CustomMdSpi::OnFrontConnected()
{
	std::cout << "=====����˿ڽ����������ӳɹ�=====" << std::endl;
	// ��ʼ��¼
	CThostFtdcReqUserLoginField loginReq;
	memset(&loginReq, 0, sizeof(loginReq));
	strcpy(loginReq.BrokerID, gBrokerID);
	strcpy(loginReq.UserID, gInvesterID);
	strcpy(loginReq.Password, gInvesterPassword);
	static int requestID = 0; // ������
	int rt = g_pMdUserApi->ReqUserLogin(&loginReq, requestID);
	if (!rt)
		std::cout << ">>>>>>����˿ڷ��͵�¼����ɹ�" << std::endl;
	else
		std::cerr << "--->>>����˿ڷ��͵�¼����ʧ��" << std::endl;
}

// �Ͽ�����֪ͨ
void CustomMdSpi::OnFrontDisconnected(int nReason)
{
	std::cerr << "=====����˿��������ӶϿ�=====" << std::endl;
	std::cerr << "����˿ڴ����룺 " << nReason << std::endl;
}

// ������ʱ����
void CustomMdSpi::OnHeartBeatWarning(int nTimeLapse)
{
	std::cerr << "=====����˿�����������ʱ=====" << std::endl;
	std::cerr << "����˿ھ��ϴ�����ʱ�䣺 " << nTimeLapse << std::endl;
}

// ��¼Ӧ��
void CustomMdSpi::OnRspUserLogin(
	CThostFtdcRspUserLoginField *pRspUserLogin,
	CThostFtdcRspInfoField *pRspInfo,
	int nRequestID,
	bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		std::cout << "=====����˿��˻���¼�ɹ�=====" << std::endl;
		std::cout << "�����գ� " << pRspUserLogin->TradingDay << std::endl;
		std::cout << "��¼ʱ�䣺 " << pRspUserLogin->LoginTime << std::endl;
		std::cout << "�����̣� " << pRspUserLogin->BrokerID << std::endl;
		std::cout << "�ʻ����� " << pRspUserLogin->UserID << std::endl;
		// ��ʼ��������
		int rt = g_pMdUserApi->SubscribeMarketData(g_pInstrumentID, instrumentNum);
		if (!rt)
			std::cout << ">>>>>>����˿ڷ��Ͷ�����������ɹ�" << std::endl;
		else
			std::cerr << "--->>>����˿ڷ��Ͷ�����������ʧ��" << std::endl;
	}
	else
		std::cerr << "����˿ڷ��ش���--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
}

// �ǳ�Ӧ��
void CustomMdSpi::OnRspUserLogout(
	CThostFtdcUserLogoutField *pUserLogout,
	CThostFtdcRspInfoField *pRspInfo,
	int nRequestID,
	bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		std::cout << "=====����˿��˻��ǳ��ɹ�=====" << std::endl;
		std::cout << "�����̣� " << pUserLogout->BrokerID << std::endl;
		std::cout << "�ʻ����� " << pUserLogout->UserID << std::endl;
	}
	else
		std::cerr << "����˿ڷ��ش���--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
}

// ����֪ͨ
void CustomMdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (bResult)
		std::cerr << "����˿ڷ��ش���--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
}

// ��������Ӧ��
void CustomMdSpi::OnRspSubMarketData(
	CThostFtdcSpecificInstrumentField *pSpecificInstrument,
	CThostFtdcRspInfoField *pRspInfo,
	int nRequestID,
	bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		std::cout << "=====����˿ڶ�������ɹ�=====" << std::endl;
		std::cout << "��Լ���룺 " << pSpecificInstrument->InstrumentID << std::endl;
		// �����Ҫ�����ļ��������ݿ⣬�����ﴴ����ͷ,��ͬ�ĺ�Լ�����洢
		char filePath[100] = { '\0' };
		sprintf(filePath, "%s_market_data.csv", pSpecificInstrument->InstrumentID);
		std::ofstream outFile;
		outFile.open(filePath, std::ios::out); // �¿��ļ�
		outFile << "��Լ����" << ","
			<< "����ʱ��" << ","
			<< "���¼�" << ","
			<< "�ɽ���" << ","
			<< "���һ" << ","
			<< "����һ" << ","
			<< "����һ" << ","
			<< "����һ" << ","
			<< "�ֲ���" << ","
			<< "������"
			<< std::endl;
		outFile.close();
	}
	else
		std::cerr << "����˿ڷ��ش���--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
}

// ȡ����������Ӧ��
void CustomMdSpi::OnRspUnSubMarketData(
	CThostFtdcSpecificInstrumentField *pSpecificInstrument,
	CThostFtdcRspInfoField *pRspInfo,
	int nRequestID,
	bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		std::cout << "=====����˿�ȡ����������ɹ�=====" << std::endl;
		std::cout << "��Լ���룺 " << pSpecificInstrument->InstrumentID << std::endl;
	}
	else
		std::cerr << "����˿ڷ��ش���--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
}

// ����ѯ��Ӧ��
void CustomMdSpi::OnRspSubForQuoteRsp(
	CThostFtdcSpecificInstrumentField *pSpecificInstrument,
	CThostFtdcRspInfoField *pRspInfo,
	int nRequestID,
	bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		std::cout << "=====����˿ڶ���ѯ�۳ɹ�=====" << std::endl;
		std::cout << "��Լ���룺 " << pSpecificInstrument->InstrumentID << std::endl;
	}
	else
		std::cerr << "����˿ڷ��ش���--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
}

// ȡ������ѯ��Ӧ��
void CustomMdSpi::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		std::cout << "=====����˿�ȡ������ѯ�۳ɹ�=====" << std::endl;
		std::cout << "��Լ���룺 " << pSpecificInstrument->InstrumentID << std::endl;
	}
	else
		std::cerr << "����˿ڷ��ش���--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
}

// ��������֪ͨ
void CustomMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	double Nowtime = GetLocalTimeSec2();
	if (begintime1 > -0.1 && endtime1 > -0.1)
	{
		if (Nowtime<begintime1 && Nowtime>endtime1)
		{

			printf("���ڽ���ʱ���ڣ��������ݱ��ܾ�[%s %0.02f %s]\n", pDepthMarketData->InstrumentID, pDepthMarketData->LastPrice, pDepthMarketData->UpdateTime);
			printf("not in trade time��market information is not access[%s %0.02f %s]\n", pDepthMarketData->InstrumentID, pDepthMarketData->LastPrice, pDepthMarketData->UpdateTime);
			//WirteTradeRecordToFileMainThreadB(Nowtime, "���ڽ���ʱ���ڣ�ֱ�ӷ���");
			return;
		}

	}
	if (begintime2 > -0.1 && endtime2 > -0.1)
	{
		if (Nowtime > begintime2 && Nowtime < endtime2)
		{
			printf("���ڽ���ʱ���ڣ��������ݱ��ܾ�[%s %0.02f %s]\n", pDepthMarketData->InstrumentID, pDepthMarketData->LastPrice, pDepthMarketData->UpdateTime);
			//WirteTradeRecordToFileMainThreadB(Nowtime, "���ڽ���ʱ���ڣ�ֱ�ӷ���");
			return;
		}
	}
	if (begintime3 > -0.1 && endtime3 > -0.1)
	{
		if (Nowtime > begintime3 && Nowtime < endtime3)
		{
			printf("���ڽ���ʱ���ڣ��������ݱ��ܾ�[%s %0.02f %s]\n", pDepthMarketData->InstrumentID, pDepthMarketData->LastPrice, pDepthMarketData->UpdateTime);
			//WirteTradeRecordToFileMainThreadB(Nowtime, "���ڽ���ʱ���ڣ�ֱ�ӷ���");
			return;
		}
	}
	if (begintime4 > -0.1 && endtime4 > -0.1)
	{
		if (Nowtime > begintime4 && Nowtime < endtime4)
		{
			printf("���ڽ���ʱ���ڣ��������ݱ��ܾ�[%s %0.02f %s]\n", pDepthMarketData->InstrumentID, pDepthMarketData->LastPrice, pDepthMarketData->UpdateTime);
			//WirteTradeRecordToFileMainThreadB(Nowtime, "���ڽ���ʱ���ڣ�ֱ�ӷ���");
			return;
		}
	}

	depthdata[gMarket[pDepthMarketData->InstrumentID]] = pDepthMarketData;

	/*
	// ��ӡ���飬�ֶν϶࣬��ȡ����
	std::cout << "=====����������=====" << std::endl;
	std::cout << "�����գ� " << pDepthMarketData->TradingDay << std::endl;
	std::cout << "���������룺 " << pDepthMarketData->ExchangeID << std::endl;
	std::cout << "��Լ���룺 " << pDepthMarketData->InstrumentID << std::endl;
	std::cout << "��Լ�ڽ������Ĵ��룺 " << pDepthMarketData->ExchangeInstID << std::endl;
	std::cout << "���¼ۣ� " << pDepthMarketData->LastPrice << std::endl;
	std::cout << "������ " << pDepthMarketData->Volume << std::endl;
	// ���ֻ��ȡĳһ����Լ���飬������tick�ش����ļ������ݿ�
	*/
	char filePath[100] = {'\0'};
	sprintf(filePath, "%s_market_data.csv", pDepthMarketData->InstrumentID);
	std::ofstream outFile;
	outFile.open(filePath, std::ios::app); // �ļ�׷��д�� 
	outFile << pDepthMarketData->InstrumentID << "," 
		<< pDepthMarketData->UpdateTime << "." << pDepthMarketData->UpdateMillisec << "," 
		<< pDepthMarketData->LastPrice << "," 
		<< pDepthMarketData->Volume << "," 
		<< pDepthMarketData->BidPrice1 << "," 
		<< pDepthMarketData->BidVolume1 << "," 
		<< pDepthMarketData->AskPrice1 << "," 
		<< pDepthMarketData->AskVolume1 << "," 
		<< pDepthMarketData->OpenInterest << "," 
		<< pDepthMarketData->Turnover << std::endl;
	outFile.close();

	// ����ʵʱk��
	std::string instrumentKey = std::string(pDepthMarketData->InstrumentID);
	if (g_KlineHash.find(instrumentKey) == g_KlineHash.end())
		g_KlineHash[instrumentKey] = TickToKlineHelper();
	g_KlineHash[instrumentKey].KLineFromRealtimeData(pDepthMarketData);

}

// ѯ������֪ͨ
void CustomMdSpi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp)
{
	// ����ѯ�۽��
	std::cout << "=====����˿ڻ��ѯ�۽��=====" << std::endl;
	std::cout << "�����գ� " << pForQuoteRsp->TradingDay << std::endl;
	std::cout << "���������룺 " << pForQuoteRsp->ExchangeID << std::endl;
	std::cout << "��Լ���룺 " << pForQuoteRsp->InstrumentID << std::endl;
	std::cout << "ѯ�۱�ţ� " << pForQuoteRsp->ForQuoteSysID << std::endl;
}


void UpdatePriceHash(const char * InstrumentID, CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	if (pDepthMarketData->LastPrice < 1) //�ָ�
										 //if (NewPrice < 5 || thistime<-1e-10) //С��5Ԫ������۸񣬲�������
	{
		return;// false;
	}

	
}




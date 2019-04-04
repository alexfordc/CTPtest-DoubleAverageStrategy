#include <vector>
#include <string>
#include <unordered_map>
#include <thread>
#include <mutex>
#include "StrategyTrade.h"
#include "CustomTradeSpi.h"

extern std::unordered_map<std::string, TickToKlineHelper> g_KlineHash;
extern CThostFtdcInvestorPositionField *g_InvestorPosition;	  // ��ĳֲ����

// �̻߳�����
std::mutex marketDataMutex;
using namespace std;

void DoubleAverageStrategy::StrategyCheckAndTrade(TThostFtdcInstrumentIDType instrumentID, CustomTradeSpi *customTradeSpi)
{
	// ����
	std::lock_guard<std::mutex> lk(marketDataMutex);
	TickToKlineHelper tickToKlineObject = g_KlineHash.at(std::string(instrumentID));
	// ����
	std::vector<KLineDataType> KLineVec = tickToKlineObject.m_KLineDataArray;

	int shortPeriod = 5;
	int longPeriod = 20;
	if (KLineVec.size() >= (longPeriod + 1))									// �������ɼ��ľ��߲���20min���򲻽����жϡ������������ݿ�ά����������ʷ���ݡ�
	{
		int len = KLineVec.size();
		double last_5min_averageprice = 0;										// ���ڴ����ʱ�̵�5min����
		double now_5min_averageprice =	0;										// ���ڴ�ű�ʱ�̵��5min����
		int i = 0;
		while (i < shortPeriod)
		{
			last_5min_averageprice += KLineVec[len-i-2].close_price/shortPeriod;
			now_5min_averageprice += KLineVec[len-i-1].close_price/shortPeriod;
			i++;
		};

		double last_20min_averageprice = 0;						// ���ڴ��20min��������
		double now_20min_averageprice = 0;
		int j = 0;
		while (j < longPeriod)
		{
			last_20min_averageprice += KLineVec[len-i-2].close_price/longPeriod;
			now_20min_averageprice += KLineVec[len-i-1].close_price/longPeriod;
			j++;
		};

		
		bool crossover = (last_5min_averageprice < last_20min_averageprice) && (now_5min_averageprice > now_20min_averageprice);	// ��棺5min�����ϴ�20min����														
		bool crossbelow = (last_5min_averageprice > last_20min_averageprice) && (now_5min_averageprice < now_20min_averageprice);	// ���棺5min�����´�20min����

		cout<<"��ǰ�źŴ������"<<'\t'<<"->"<<'\t'<<"crossover:"<<crossover<<'\t'<<"crossbelow:"<<crossbelow<<endl;
		if (crossover)
		{
			if (NULL == g_InvestorPosition)											// ����ǽ��Ϊ�գ���ֱ������
			{
				cout<<"���ֽ�棬�ղ�ֱ������"<<endl;
				customTradeSpi->reqOrderInsert(instrumentID, KLineVec[len - 1].close_price + 20, 1, THOST_FTDC_D_Buy);
			}
			else if ((*g_InvestorPosition).PosiDirection == THOST_FTDC_PD_Short)				// �����ִ��ڿ�ͷ������ƽ��֣�Ȼ�����ࡣ
			{
				cout<<"���ֽ�棬���ڿ�ͷ����ƽ��������"<<endl;
				customTradeSpi->reqOrderInsert(instrumentID, KLineVec[len - 1].close_price + 20, 1, THOST_FTDC_D_Sell);
				customTradeSpi->reqOrderInsert(instrumentID, KLineVec[len - 1].close_price + 20, 1, THOST_FTDC_D_Buy);
			}
		}
		else if (crossbelow)
		{
			
			if (NULL == g_InvestorPosition)											// ����ǽ��Ϊ�գ���ֱ������
			{
				cout<<"�������棬�ղ�ֱ������"<<endl;
				customTradeSpi->reqOrderInsert(instrumentID, KLineVec[len - 1].close_price - 20, 1, THOST_FTDC_D_Buy);
			}
			else if ((*g_InvestorPosition).PosiDirection == THOST_FTDC_PD_Long)				// �����ִ��ڶ�ͷ������ƽ��֣�Ȼ�����ࡣ
			{
				cout<<"���ֽ�棬���ڶ�ͷ����ƽ��������"<<endl;
				customTradeSpi->reqOrderInsert(instrumentID, KLineVec[len - 1].close_price - 20, 1, THOST_FTDC_D_Sell);
				customTradeSpi->reqOrderInsert(instrumentID, KLineVec[len - 1].close_price - 20, 1, THOST_FTDC_D_Buy);
			}
		}
	}
}

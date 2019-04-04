#pragma once

#ifdef PYCTPTRADER_EXPORTS
#	define QUICKLIB_TD_API __declspec(dllexport)
#else
#	define CTP_TD_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

	//xx	
	int CTP_TD_API QryQueryMaxOrderVolume(char *BrokerID, char * InvestorID, char * Instrument, char * Direction, char * OffsetFlag, char * HedgeFlag, int MaxVolume);

	int CTP_TD_API Login();

	int CTP_TD_API InsertOrderByRate(char *Instrument, char direction, char offsetFlag, char priceType, double price, double rate, bool BalanceType, int multiplier);
	int CTP_TD_API InsertOrder(char *Instrument, char direction, char offsetFlag, char priceType, double price, int num);
	int CTP_TD_API DeleteOrder(char *Instrument, int OrderRef);
	int CTP_TD_API QryTradedVol(int OrderRef);
	int CTP_TD_API QryPosition(char *Instrument, int positiontype);

	int CTP_TD_API ReqQueryMaxOrderVolume(char * BrokerID, char * InvestorID, char * InstrumentID, char Direction, char  OffsetFlag, char  HedgeFlag, int MaxVolume);

	int  CTP_TD_API ReqQryContractBank();
	double  CTP_TD_API QryExchangeMarginRate(char *Instrument, int positiontype);    //��֤����
	double  CTP_TD_API QryUnderlyingMultiple(char *Instrument);    //����

																		///�ڻ����������ʽ�ת�ڻ�����
																		//int  QUICKLIB_TD_API  ReqFromBankToFutureByFuture(int nRequestID);  //ADD

	int  CTP_TD_API  ReqFromBankToFutureByFuture(
		char * BankID,
		//char * BankBranchID,
		//char * BrokerID,
		char *  BrokerBranchID,
		char *BankAccount,
		char * BankPassWord,
		char * AccountID,
		//char *Password,
		//char * CurrencyID,
		double  TradeAmount,
		int nRequestID);

	///�ڻ������ڻ��ʽ�ת��������
	int  CTP_TD_API  ReqFromFutureToBankByFuture(
		char * BankID,
		//char * BankBranchID,
		//char * BrokerID,
		char *  BrokerBranchID,
		char *BankAccount,
		char * BankPassWord,
		char * AccountID,
		//char *Password,
		//char * CurrencyID,
		double  TradeAmount,
		int nRequestID);  //ADD



	int CTP_TD_API ReqQryInstrument();



	void CTP_TD_API *QryPositionList(int i);
	double CTP_TD_API QryBalance(bool BalanceType);     //1��̬Ȩ�� 0��̬Ȩ�� �Լ�����
	double CTP_TD_API QryAvailable();   //�����ʽ�
	void CTP_TD_API SetShowPosition(bool showstate);   //�����ʽ�

	int CTP_TD_API GetCmd();
	CTP_TD_API char * GetCmdContent2();

	void CTP_TD_API * GetCmdContent();

	void CTP_TD_API * GetCmdContent_Order();
	void CTP_TD_API * GetCmdContent_Trade();


	void CTP_TD_API * GetCmdContent_BankToFutureByFuture();
	void CTP_TD_API * GetCmdContent_FutureToBankByFuture();

	void CTP_TD_API * GetCmdContent_QueryMaxOrderVolume();


	void CTP_TD_API * GetCmdContent_Settlement();
	void CTP_TD_API * GetCmdContent_Error();

	void CTP_TD_API * GetCmdContent_LoginScuess();
	void CTP_TD_API * GetCmdContent_Connected();
	void CTP_TD_API * GetCmdContent_ProductGroupMargin();
	void CTP_TD_API * GetCmdContent_CommissionRate();


	void CTP_TD_API * GetCmdContent_AccountRegist();

	void CTP_TD_API * GetCmdContent_BankToFutureByFuture();
	void CTP_TD_API * GetCmdContent_FutureToBankByFuture();



	int CTP_TD_API OnCmd();
	int CTP_TD_API GetUnGetCmdSize();

	//double QUICKLIB_TD_API QryAvailable2();   //�����ʽ�
#ifdef __cplusplus
}
#endif
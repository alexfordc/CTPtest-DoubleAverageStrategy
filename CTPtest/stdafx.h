/*
1.���ļ�ΪQuicklib �ڻ������ײ����
2.���ļ������õĿ��ļ�Quicklib CTP�ڻ������ͽ��׿���ѭ ��ԴЭ��GPL v3
�򵥵�˵���Ի���GPL v3Э���Դ���룬�����˻�����������Լ�ʹ�ã�����Ա�Դ��
�����ڸÿ�Դ���룬�����������������Ʒ������ҵ��Ϊ��Ҳ���뿪Դ��

Quciklib Python��ܺ͹�����ѭGPL v3Э�������
��1��Quicklib CTP   �ڻ������
��2��Quicklib CTP   �ڻ����׿�
��3��Quicklib CTP2  A�������
��4��Quicklib MOMģʽ  �����ʹܽ��׿�
�����ڽ����ʹ�Ͷ��ϵͳ��MOMģʽ��ʵ��˽ļ����Ͷ�˵�ѡ�ο��ˣ���ͨ���Լ��ķ��ϵͳ����ʵ�̣�

Quciklib Python��ܺ͹����ݲ���ѭ��ԴЭ�������
��5��Quicklib ������⣨Ԥ������ء������ź����ݸ��ơ�����������������ã�

��Quciklib��Դ���������׵ģ����õ�ԭʼ���߿϶��ģ���������http://www.quciklib.cn��վ�ϣ�
������ڡ���Դ˵���͸�л.txt�����������ļ����ϸ��·���ÿһ���°汾��Quicklib���

ԭʼ���ߣ�QQ 147423661 ��(���Ǹ�)
�ٷ���վ��http://www.quicklib.cn
�ٷ�QQȺ��5172183(1Ⱥ)��25884087(2Ⱥ)
*/
#pragma once

#undef UNICODE
#undef _UNICODE

#define WM_STRATEGY WM_USER+1 

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

//#pragma comment(lib,"ThostTraderApi32λ\\thostmduserapi.lib")
//#pragma comment(lib,"ThostTraderApi64λ\\thostmduserapi.lib")
//#ifndef _WIN64  
//  #include "ThostTraderApi64λ/ThostFtdcUserApiDataType.h"
//  #include "ThostTraderApi64λ/ThostFtdcUserApiStruct.h"
//  #include "ThostTraderApi64λ/ThostFtdcMdApi.h"
//#else  
//  #include "ThostTraderApi32λ/ThostFtdcUserApiDataType.h"
//  #include "ThostTraderApi32λ/ThostFtdcUserApiStruct.h"
//  #include "ThostTraderApi32λ/ThostFtdcMdApi.h"
//#endif  



#include <string>
#include <fstream>
#include <iostream>
#include <map>
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
#include <hash_map>
#define PYCTPMARKET_EXPORTS

#include <list>
struct cmdcontent
{
	int cmd;
	char content[31];
};
#define MAX_EVENTNUM  64

#define  EID_OnFrontDisconnected   0
#define  EID_OnFrontConnected      1
#define  EID_OnRspUserLogin_Failer 2
#define  EID_OnRspUserLogin_Scuess 3
#define  EID_OnRtnDepthMarketData  4
#define  EID_IsErrorRspInfo        5
#define  EID_OnRspSubMarketData    6
#define  EID_OnRspUnSubMarketData  7
#define  EID_OnRspUserLogout       8
#define  EID_OnRspForQuote         9
//#define  EID_OnLog                 10

//�ص�����
#define SYSTEM_EMPTY                  8000 //��
#define SYSTEM_LOGIN_SCUESS           8001 //��¼�ɹ�
#define SYSTEM_LOGIN_DENIED           8002 //��¼���ܾ�
//#define SYSTEM_LOGIN_ERRORPASSWORD  8003 //������� ??
#define SYSTEM_LOGINOUT_SCUESS        8004 //�ǳ��ɹ�
#define SYSTEM_NETCONNECT_SCUESS      8005 //���ӳɹ�
#define SYSTEM_NETCONNECT_BREAK       8006 //�Ͽ�����
#define SYSTEM_NETCONNECT_FAILER      8007 //����ʧ�� ??
#define SYSTEM_SUBCRIBE_SCUESS        8008 //���ĳɹ�
#define SYSTEM_UNSUBCRIBE_SCUESS      8009 //ȡ�����ĳɹ�
#define SYSTEM_NEWTICK                8010 //��Tick����
#define SYSTEM_SYSTEM_ERROR           8011 //����Ӧ��
#define SYSTEM_QRY_FORQUOTE           8012 //
//#define SYSTEM_LOG                    8013 //��־
struct TThostFtdcInstrumentIDTypeStruct
{
	//TThostFtdcInstrumentIDType Instrument;
};
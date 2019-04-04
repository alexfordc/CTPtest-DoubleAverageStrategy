
#pragma once

#include <iostream>
using  namespace std;
#include <string.h>
#define NULL 0
#include <math.h>
#include "def.h"

#include "CRC32.h"

struct strategys
{
	char instructment[20];
	int type;
	int strategy[100];
};
extern strategys * its;

struct Instructment
{

	char instructment[20];
	int type;
	int MD_begin1;
	int MD_end1;
	int MD_begin2;
	int MD_end2;
	int MD_begin3;
	int MD_end3;
	int MD_begin4;
	int MD_end4;
	int MD_begin5;
	int MD_end5;

	int TD_begin1;
	int TD_end1;
	int TD_begin2;
	int TD_end2;
	int TD_begin3;
	int TD_end3;
	int TD_begin4;
	int TD_end4;
	int TD_begin5;
	int TD_end5;

};

extern Instructment * it;
//Instructment * it;
//bool crcstate = false; //true����crc32,fasle������
/*
//CRC32
GetCrc32Value(keystr, keystr, 10);
printf("CRC32:[%s]\n", keystr);
*/

//������false  41 36 50 54 48 62  (47/47/16/31/47/31����)
//crc32 true   41 42 44 45 51 62  (47/47/47/47/47/47����)
struct hashinfo
{
	double usedrate;
	double max_len;
	double ave_len;
	int maxlen_num;
};

//�����ñ���HASH�Ľṹ��
struct HashData
{
	int cycle;
	int key;
	int M;
	int N;
	int sort;
	int emaspace;
};


//Instructment * it2=NULL;

/*
int cmp2(const void *a, const void *b)
{
return (*(HashData *)a)->sort > (*(HashData *)b)->sort ? 1 : -1;
}


struct Sort_Struct2
{
int id;
int value;
}SortPoint2[TYPE_NUM], SortSpace2[TYPE_NUM];
*/



//char Crcresult[9] = { 0 };
//sprintf_s(Crcresult, sizeof(Crcresult), "%.8X", GetCRC32BFromBuffer((unsigned char *)(LPCTSTR)(datastr), strlen(datastr)));



//CRC32BУ����


//#define GUEST_ONLINE_MaxSize 5000//      10000              //��ϣ��ĳ���   Ӧ����M��ȣ� 2����ʹ��װ������С��0.5 
//#define M_GUEST_ONLINE  4997//9997  //����



//unsigned long GUEST_ONLINE_MaxSize[CYCLE_NUM] = { 9899,6400,3600,2500,900,400 };
//int M_GUEST_ONLINE[CYCLE_NUM] = { 9898,5533,3597,2497,897,397 };
#include <time.h>

//#include "comm.h"

/*
typedef struct hterm1
{
	DWORD dwLastActiveTime;			 // ��¼���û��Ļʱ�䣨������ʹ�ã�
	DWORD LoginErrorTime;
	int LoginErrorNum;
	char cpsname[MAX_USERNAME];	 // �û���
	char checkcode[4];
	char ip[30];  //6���ַ�+1λ\0
	//int k;      //ƴ������Ӧ������
	//int si;     //���ҳ���
				//PEER_RECORD_INFO deallist;
				//ADDR_RADER_INFO addr[MAX_ADDR_NUMBER]; // �ɽڵ��˽���ն˺͹����ն���ɵ�����
				//u_char AddrNum;					 // addr����Ԫ������
				//ADDR_RADER_INFO p2pAddr;				 // P2Pͨ��ʱӦʹ�õĵ�ַ���ͻ���ʹ�ã�
				//char oncehash[7];

	//enum  entry_status states;

	PEER_INFO   OrderUserPeerList;  // �����ò����ֵ��û��б�;

}GuestOnlineHash;

GuestOnlineHash *CpsOnlineList = new GuestOnlineHash[GUEST_ONLINE_MaxSize]();
*/

// һ���ն���Ϣ

/*
struct ADDR_INFO2
{
	DWORD dwIp;
	u_short nPort;
};
// һ���ڵ���Ϣ
struct PEER_INFO2
{
	DWORD dwLastActiveTime;			 // ��¼���û��Ļʱ�䣨������ʹ�ã�
	char szUserName[MAX_USERNAME];	 // �û���
	u_char AddrNum;					 // addr����Ԫ������
	ADDR_INFO2 addr[MAX_ADDR_NUMBER]; // �ɽڵ��˽���ն˺͹����ն���ɵ�����

	ADDR_INFO2 p2pAddr;				 // P2Pͨ��ʱӦʹ�õĵ�ַ���ͻ���ʹ�ã�

};
*/
/*
struct MaxSpaceCX
{
	int M=0;
	int N=0;
	double EMA_SPACE_MN=0;
};
MaxSpaceCX Mcx;
*/
 
#define PERIOD_NUM   8

struct PERIODTYPE
{
  int periodtype[PERIOD_NUM];
 
};


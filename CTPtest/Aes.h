////////////////////////////////////////////////////////////////////////
//	aes.h (AESģ��) 2006.11.15 ����:Qitao,Liusong,Zhangyong,��·
////////////////////////////////////////////////////////////////////////
#ifndef AES_H_INCLUDED
#define AES_H_INCLUDED
////////////////////////////////////////////////////////////////////////
class Aes
{
	enum {N=4};					// 4��32λ,128λ
	unsigned rounds;
	unsigned exp_key[N*(14+1)];
public:
	Aes():rounds(0) {}
	Aes(const void *keydata,unsigned keysize)
	{
		LoadKey(keydata,keysize);
	}
	bool LoadKey(const void *keydata,unsigned keysize);		// 16/24/32
	void LoadExtKey(const void *keydata,unsigned round);	// 240 bits
	void SaveExtKey(void *keydata,unsigned *round) const;	// 240 bits
	void EncryptBlock(void *dst,const void *src) const;
	void DecryptBlock(void *dst,const void *src) const;
};
////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////
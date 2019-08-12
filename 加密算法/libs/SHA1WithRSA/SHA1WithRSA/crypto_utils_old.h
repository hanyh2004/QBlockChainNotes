/****************************************************************
author: yqq
date: 2019-08-02
email: youngqqcn@gmail.com
descriptions: crypto tools
******************************************************************/



#ifndef __CRYPTO_UTILS__
#define __CRYPTO_UTILS__

extern "C" {
#include <openssl/rsa.h>
#include <openssl/err.h>
#include <openssl/pem.h> 

#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>


#include <openssl/aes.h>

};

#include<stdio.h>
#include<string.h>
#include <string>
#include <iostream>




namespace  CryptoUtils 
{

	//���� ����������˵��
	//https://linux.die.net/man/3/pem_read_rsa_pubkey


	//���� OpenSSL EVP �����ϸ����  
	//https://blog.csdn.net/liao20081228/article/details/76285896

	//���� OpenSSL BIO �����ϸ���� 
	//https://blog.csdn.net/liao20081228/article/details/77193729
	//https://blog.csdn.net/liujiayu2/article/details/51860172


	//
	/*my_rsa_public_encrypt��Կ������my_rsa_public_encrypt��Կ�������
		��Կ����,��ȡpubkey.pem��������
		���filename,���湫Կ���ļ����ƣ�src���������ݣ��������ݳ���
		����des���ܺ����ݣ�deslen�������ݳ���
		src���ȴ��ڹ�Կ����ʱ��RSA_size��,��ֶμ��ܡ�
		���ܽ��des��src���ȴ�,������Ҫ��des�����src����Ŀռ�
		������Կ���ȣ�RSA_size��Ϊ128��src����Ϊ1~117,���ܽ��Ϊ128���ֽڡ�src����Ϊ118~234,���ܽ��Ϊ256�ֽ�
	*/
	int RSA_EncrptByPubkey(
		char *pszPubKeyFilePath,
		unsigned char *puszInData,
		int uInDataLen,
		unsigned char *puszOutData,
		int *puOutDataLen);


	/*˽Կ����,��ȡprikey.pem��������
	���filename,���湫Կ���ļ����ƣ�src���������ݣ�srclen�������ݳ���
	����des���ܺ����ݣ�deslen�������ݳ���*/
	int RSA_DecrptByPrivkey(
		char *pszPrivKeyFilePath,
		unsigned char *puszInData,
		int uInDataLen,
		unsigned char *puszOutData,
		int *puOutDataLen);


	/*my_rsa_prikey_encrypt˽Կ������my_rsa_public_decrypt��Կ�������
			˽Կ����,��ȡprikey.pem�������ܡ�
			���filename,���湫Կ���ļ����ƣ�src���������ݣ��������ݳ���
			����des���ܺ����ݣ�deslen�������ݳ���
			src���ȴ��ڹ�Կ����ʱ��RSA_size��,��ֶμ��ܡ�
			���ܽ��des��src���ȴ�,������Ҫ��des�����src����Ŀռ�
			������Կ���ȣ�RSA_size��Ϊ128��src����Ϊ1~117,���ܽ��Ϊ128���ֽڡ�src����Ϊ118~234,���ܽ��Ϊ256�ֽ�
			���des�ĳ��ȣ�Ӧ�ñ� RSA_size*��strlen(src)/(RSA_size-11)+1������С�
		*/
		//˽Կ����
	int RSA_EncrptByPrivKey(
		char *pszPrivKeyFilePath,
		unsigned char *puszInData,
		int uInDataLen,
		unsigned char *puszOutData,
		int *puOutDataLen);


	/*
		��Կ����,��ȡpublic.pem��������
		���filename,���湫Կ���ļ����ƣ�src���������ݣ�srclen�������ݳ���
		����des���ܺ����ݣ�deslen�������ݳ���
	*/
	//��Կ����
	int RSA_DecryptByPubkey(
		char *pszPubKeyFilePath,
		unsigned char *puszInData,
		int uInDataLen,
		unsigned char *puszOutData,
		int *puOutLen);


	/* my_EVP_rsa_prikey_sign˽Կǩ��,ʹ��EVP
		���filename,˽Կ�ļ����ƣ�
				src��ǩ�����ݣ�
				srclen��ǩ�����ݳ���
		����desǩ�����,���ؽ������Ϊ��Կ����ֵ,��������Կ���ȸ���Ŀռ䣬�����ڴ�Խ��
				deslenǩ�����ݳ��ȣ���ʼ��Ϊdes�����С
	*/
	int SHA256WithRSA_Sign(
		char *pszPrivKeyFilePath,
		unsigned char *puszInData,
		unsigned int uInDataLen,
		unsigned char *puszOutSig,
		unsigned int *uOutSigLen);



	/* my_EVP_rsa_public_verify��Կǩ����֤,ʹ��EVP
		���filename,��Կ�ļ�����
				src�������ݣ�
				srclen�������ݳ���
			signǩ��
				signlenǩ������
	*/
	//��Կǩ��EVP��֤
	int SHA256WithRSA_Verify(
		char *pszPubKeyFilePath,
		unsigned char *puszInData,
		int uInDataLen,
		unsigned char *puszOutSig,
		unsigned int uOutSigLen);



	//base64����
	//************************************
	// Method:    Base64Encode
	// FullName:  CryptoUtils::Base64Encode
	// Access:    public 
	// Returns:   std::string  �������ַ���
	// Qualifier:
	// Parameter: const char * input
	// Parameter: int length   �������ݳ���
	// Parameter: bool bWithNewLine   �Ƿ��������
	//************************************
	std::string Base64Encode(
		const char *pszInData, 
		int nInDataLen, 
		bool bWithNewLine);



	//base64����
	//************************************
	// Method:    Base64Decode
	// FullName:  CryptoUtils::Base64Decode
	// Access:    public 
	// Returns:   std::string  ����������
	// Qualifier:
	// Parameter: const char * pszInData  ��������
	// Parameter: int nInDataLen   �������ݳ���
	// Parameter: bool bWithNewLine �Ƿ�������� 
	//************************************
	std::string Base64Decode(
		const char * pszInData,  
		int nInDataLen, 
		bool bWithNewLine);



	//AES128��CBCģʽ����
	int AES_128_CBC_Encrypt(
		unsigned char *puszInData,
		int nInDataLen,
		unsigned char *puszKey,
		unsigned char *puszIV,
		unsigned char *puszOutData,
		unsigned int *pOutDataLen);


	//AES128��CBCģʽ����
	int AES_128_CBC_Decrypt(
		unsigned char *puszInData,
		int nInDataLen,
		unsigned char *puszKey,
		unsigned char *puszIV,
		unsigned char *puszOutData,
		unsigned int *pOutDataLen);

}

#endif//__CRYPTO_UTILS__
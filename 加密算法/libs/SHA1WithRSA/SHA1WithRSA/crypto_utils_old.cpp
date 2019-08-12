//������Դ�ļ������, ������ͷ�ļ������
extern "C" {
#include <openssl/applink.c>
};

#include "crypto_utils.h"


namespace  CryptoUtils
{

	std::string Base64Encode(const char *input, int length, bool with_new_line)
	{
		BIO * bmem = NULL;
		BIO * b64 = NULL;
		BUF_MEM * bptr = NULL;

		b64 = BIO_new(BIO_f_base64());
		if (!with_new_line) {
			BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
		}
		bmem = BIO_new(BIO_s_mem());
		b64 = BIO_push(b64, bmem);
		BIO_write(b64, input, length);
		BIO_flush(b64);
		BIO_get_mem_ptr(b64, &bptr);

		char * buff = (char *)malloc(bptr->length + 1);
		memcpy(buff, bptr->data, bptr->length);
		buff[bptr->length] = 0;
		BIO_free_all(b64);


		std::string  strRet(buff);
		free(buff); buff = NULL;
		return strRet;
	}

	std::string Base64Decode(const char * input, int length, bool with_new_line)
	{
		BIO * b64 = NULL;
		BIO * bmem = NULL;
		char * buffer = (char *)malloc(length);
		memset(buffer, 0, length);

		b64 = BIO_new(BIO_f_base64());
		if (!with_new_line) {
			BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
		}
		bmem = BIO_new_mem_buf((void *)input, length);
		bmem = BIO_push(b64, bmem);
		int iDecodeLen = BIO_read(bmem, buffer, length);

		BIO_free_all(bmem);

		std::string strRet;
		strRet.append(buffer, iDecodeLen);

		free(buffer); buffer = NULL;
		return strRet;
	}


	//��Կ����
	int RSA_EncrptByPubkey(
		char *pszPubKeyFilePath,
		unsigned char *puszInData,
		int uInDataLen,
		unsigned char *puszOutData,
		int *puOutDataLen)
	{
		int ret = 0, isrclen = 0, ideslen = 0, i = 0, n = 0, ienclen = 0, rsalen = 0;
		if (pszPubKeyFilePath == NULL || puszInData == NULL || puszOutData == NULL)
			return -1;

		RSA *r = RSA_new();

		FILE *fp = NULL;
		fp = fopen(pszPubKeyFilePath, "rt");
		if (fp == NULL)
		{
			printf("fopen [%s] error\n", pszPubKeyFilePath);
			return -2;
		}

		if (NULL == (r = PEM_read_RSA_PUBKEY(fp, NULL, NULL, NULL)))
		{
			fclose(fp);
			printf("PEM_read_RSA_PUBKEY error\n");
			return -3;
		}
		fclose(fp);
		//RSA_print_fp(stdout, r, 5);  

		   //����������󳤶ȣ�����Կ����-11,���ܽ������RSA_size�ķ���ֵ��
		rsalen = RSA_size(r) - 11;
		unsigned char *out = (unsigned char *)malloc(rsalen + 12);

		n = uInDataLen / rsalen;
		for (i = 0; i <= n; i++)
		{
			ienclen = (i != n ? rsalen : uInDataLen % (rsalen));
			if (ienclen == 0)
				break;
			memset(out, 0, rsalen + 12);
			ret = RSA_public_encrypt(ienclen, puszInData + isrclen, out, r, RSA_PKCS1_PADDING);
			if (ret < 0)
			{
				printf("RSA_public_encrypt failed,ret is [%d]\n", ret);
				return -4;
			}

			isrclen += ienclen;
			memcpy(puszOutData + ideslen, out, ret);
			ideslen += ret;
		}
		puszOutData[ideslen] = 0;
		*puOutDataLen = ideslen;

		free(out);
		RSA_free(r);
		CRYPTO_cleanup_all_ex_data();

		return 0;
	}



	//˽Կ����
	int RSA_DecrptByPrivkey(
		char *pszPrivKeyFilePath,
		unsigned char *puszInData,
		int uInDataLen,
		unsigned char *puszOutData,
		int *puOutDataLen)
	{
		int ret = 0, isrclen = 0, ideslen = 0, i = 0, n = 0, ienclen = 0, rsalen = 0;

		if (pszPrivKeyFilePath == NULL || puszInData == NULL || puszOutData == NULL)
			return -1;

		RSA *r = RSA_new();

		FILE *fp = NULL;
		fp = fopen(pszPrivKeyFilePath, "rt");
		if (fp == NULL)
		{
			printf("fopen [%s] error\n", pszPrivKeyFilePath);
			return -2;
		}


		if (PEM_read_RSAPrivateKey(fp, &r, 0, 0) == NULL)
		{
			fclose(fp);
			printf("PEM_read_RSAPrivateKey error\n");
			return -3;
		}
		fclose(fp);
		//RSA_print_fp(stdout, r, 5);

		rsalen = RSA_size(r);
		unsigned char *out = (unsigned char *)malloc(rsalen + 1);

		n = uInDataLen / rsalen;
		for (i = 0; i <= n; i++)
		{

			ienclen = (i != n ? rsalen : uInDataLen % (rsalen));
			if (ienclen == 0)
				break;
			memset(out, 0, rsalen + 1);
			ret = RSA_private_decrypt(ienclen, puszInData + isrclen, out, r, RSA_PKCS1_PADDING);
			if (ret < 0)
			{
				printf("RSA_private_decrypt failed,ret is [%d]\n", ret);
				return -4;
			}

			isrclen += ienclen;
			memcpy(puszOutData + ideslen, out, ret);
			ideslen += ret;
		}
		puszOutData[ideslen] = 0;
		*puOutDataLen = ideslen;

		free(out);
		RSA_free(r);
		CRYPTO_cleanup_all_ex_data();

		return 0;
	}


	//˽Կ����
	int RSA_EncrptByPrivKey(
		char *pszPrivKeyFilePath,
		unsigned char *puszInData,
		int uInDataLen,
		unsigned char *puszOutData,
		int *puOutDataLen)
	{
		int ret = 0, isrclen = 0, ideslen = 0, i = 0, n = 0, ienclen = 0, rsalen = 0;
		if (pszPrivKeyFilePath == NULL || puszInData == NULL || puszOutData == NULL)
			return -1;

		RSA *r = RSA_new();

		FILE *fp = NULL;
		fp = fopen(pszPrivKeyFilePath, "rt");
		if (fp == NULL)
		{
			RSA_free(r);
			printf("fopen [%s] error\n", pszPrivKeyFilePath);
			return -2;
		}

		if (PEM_read_RSAPrivateKey(fp, &r, 0, 0) == NULL)
		{
			RSA_free(r);
			fclose(fp);
			printf("PEM_read_RSAPrivateKey error\n");
			return -3;
		}
		fclose(fp);


		//RSA_print_fp(stdout, r, 5);
		//����������󳤶ȣ�����Կ����-11,���ܽ������RSA_size�ķ���ֵ��
		rsalen = RSA_size(r) - 11;
		unsigned char *out = (unsigned char *)malloc(rsalen + 12);

		n = uInDataLen / rsalen;
		for (i = 0; i <= n; i++)
		{
			ienclen = (i != n ? rsalen : uInDataLen % (rsalen));
			if (ienclen == 0)
				break;
			memset(out, 0, rsalen + 12);
			ret = RSA_private_encrypt(ienclen, puszInData + isrclen, out, r, RSA_PKCS1_PADDING);
			if (ret < 0)
			{
				free(out);
				RSA_free(r);
				printf("RSA_private_encrypt failed,ret is [%d]\n", ret);
				return -4;
			}

			isrclen += ienclen;
			memcpy(puszOutData + ideslen, out, ret);
			ideslen += ret;
		}
		puszOutData[ideslen] = 0;
		*puOutDataLen = ideslen;
		free(out);
		RSA_free(r);
		CRYPTO_cleanup_all_ex_data();

		return 0;
	}



	//��Կ����
	int RSA_DecryptByPubkey(
		char *pszPubKeyFilePath,
		unsigned char *puszInData,
		int uInDataLen,
		unsigned char *puszOutData,
		int *puOutLen)
	{
		int ret = 0, isrclen = 0, ideslen = 0, i = 0, n = 0, ienclen = 0, rsalen = 0;

		if (pszPubKeyFilePath == NULL || puszInData == NULL || puszOutData == NULL)
			return -1;

		RSA *r = RSA_new();

		FILE *fp = NULL;
		fp = fopen(pszPubKeyFilePath, "rt");
		if (fp == NULL)
		{
			RSA_free(r);
			printf("fopen [%s] error\n", pszPubKeyFilePath);
			return -2;
		}
		if (PEM_read_RSA_PUBKEY(fp, &r, 0, 0) == NULL)
		{
			RSA_free(r);
			fclose(fp);
			printf("PEM_read_RSA_PUBKEY error\n");
			return -3;
		}
		fclose(fp);
		//RSA_print_fp(stdout, r, 5);
		rsalen = RSA_size(r);
		unsigned char *out = (unsigned char *)malloc(rsalen + 1);

		n = uInDataLen / rsalen;
		for (i = 0; i <= n; i++)
		{

			ienclen = (i != n ? rsalen : uInDataLen % (rsalen));
			if (ienclen == 0)
				break;
			memset(out, 0, rsalen + 1);
			ret = RSA_public_decrypt(ienclen, puszInData + isrclen, out, r, RSA_PKCS1_PADDING);
			if (ret < 0)
			{
				free(out);
				RSA_free(r);
				printf("RSA_public_decrypt failed,ret is [%d]\n", ret);
				return -4;
			}

			isrclen += ienclen;
			memcpy(puszOutData + ideslen, out, ret);
			ideslen += ret;
		}
		puszOutData[ideslen] = 0;
		*puOutLen = ideslen;

		free(out);
		RSA_free(r);
		CRYPTO_cleanup_all_ex_data();

		return 0;
	}




	//˽Կǩ��EVP
	int SHA256WithRSA_Sign(
		char *pszPrivKeyFilePath,
		unsigned char *puszInData,
		unsigned int uInDataLen,
		unsigned char *puszOutSig,
		unsigned int *puOutSigLen)
	{
		if (NULL == pszPrivKeyFilePath || NULL == puszInData || NULL == puszOutSig)
		{
			printf("SHA256WithRSA_Verify args error!\n");
			return -1;
		}

		//RSA�ṹ�����
		RSA *rsa_pri_key = RSA_new();

		FILE *fp = NULL;
		fp = fopen(pszPrivKeyFilePath, "rt");
		if (fp == NULL)
		{
			printf("fopen [rsa_private_key] error\n");
			return -1;
		}
		if (!PEM_read_RSAPrivateKey(fp, &rsa_pri_key, 0, 0))
		{
			fclose(fp);
			printf("PEM_read_RSAPrivateKey error\n");
			return -1;
		}
		fclose(fp);

		//�½�һ��EVP_PKEY����
		EVP_PKEY *evpKey = EVP_PKEY_new();

		if (evpKey == NULL)
		{
			printf("EVP_PKEY_new err\n");
			RSA_free(rsa_pri_key);
			return -1;
		}
		if (EVP_PKEY_set1_RSA(evpKey, rsa_pri_key) != 1)   //����RSA�ṹ�嵽EVP_PKEY�ṹ��  
		{
			printf("EVP_PKEY_set1_RSA err\n");
			RSA_free(rsa_pri_key);
			EVP_PKEY_free(evpKey);
			return -1;
		}
		//ժҪ�㷨�����ı���  
		EVP_MD_CTX mdctx;
		//�����Ǽ���ǩ������   
		EVP_MD_CTX_init(&mdctx);
		//��ʼ��ժҪ������   
		//if (!EVP_SignInit_ex(&mdctx, EVP_sha1(), NULL)) //ǩ����ʼ��������ժҪ�㷨������Ϊsha1  
		if (!EVP_SignInit_ex(&mdctx, EVP_sha256(), NULL)) //ǩ����ʼ��������ժҪ�㷨������Ϊsha1  
		{
			printf("err\n");
			EVP_PKEY_free(evpKey);
			RSA_free(rsa_pri_key);
			return -1;
		}
		if (!EVP_SignUpdate(&mdctx, puszInData, uInDataLen)) //����ǩ����ժҪ��Update
		{
			printf("err\n");
			EVP_PKEY_free(evpKey);
			RSA_free(rsa_pri_key);
			return -1;
		}
		if (!EVP_SignFinal(&mdctx, puszOutSig, puOutSigLen, evpKey)) //ǩ�����  
		{
			printf("err\n");
			EVP_PKEY_free(evpKey);
			RSA_free(rsa_pri_key);
			return -1;
		}

		//�ͷ��ڴ�
		EVP_PKEY_free(evpKey);
		RSA_free(rsa_pri_key);
		EVP_MD_CTX_cleanup(&mdctx);

		return 0;
	}


	//��Կǩ��EVP��֤
	int SHA256WithRSA_Verify(
		char *pszPubKeyFilePath,
		unsigned char *puszInData,
		int uInDataLen,
		unsigned char *puszOutSig,
		unsigned int puOutSigLen)
	{

		if (NULL == pszPubKeyFilePath || NULL == puszInData || NULL == puszOutSig)
		{
			printf("SHA256WithRSA_Verify args error!\n");
			return -1;
		}

		//ժҪ�㷨�����ı��� 
		EVP_MD_CTX mdctx;

		RSA *rsa_pub_key = RSA_new();

		//rsa_public_key����
		FILE *fp = NULL;
		fp = fopen(pszPubKeyFilePath, "rt");
		if (fp == NULL)
		{
			printf("fopen [rsa_private_key] error\n");
			return -2;
		}
		if (!PEM_read_RSA_PUBKEY(fp, &rsa_pub_key, NULL, NULL))
		{
			fclose(fp);
			printf("PEM_read_RSAPrivateKey error\n");
			return -3;
		}
		fclose(fp);

		EVP_PKEY *pubKey = EVP_PKEY_new();
		//�½�һ��EVP_PKEY����
		if (pubKey == NULL)
		{
			printf("EVP_PKEY_new err\n");
			RSA_free(rsa_pub_key);
			return -4;
		}
		if (EVP_PKEY_set1_RSA(pubKey, rsa_pub_key) != 1)   //����RSA�ṹ�嵽EVP_PKEY�ṹ��  
		{
			printf("EVP_PKEY_set1_RSA err\n");
			RSA_free(rsa_pub_key);
			EVP_PKEY_free(pubKey);
			return -5;
		}

		// printf("\n������֤ǩ��...\n");
		 //��������֤ǩ������   
		EVP_MD_CTX_init(&mdctx);
		//��ʼ��ժҪ������   
		//if (!EVP_VerifyInit_ex(&mdctx, EVP_sha1(), NULL))  //��֤��ʼ��������ժҪ�㷨��һ��Ҫ��ǩ��һ�¡�  
		if (!EVP_VerifyInit_ex(&mdctx, EVP_sha256(), NULL))  //��֤��ʼ��������ժҪ�㷨��һ��Ҫ��ǩ��һ�¡�  
		{
			printf("EVP_VerifyInit_ex err\n");
			EVP_PKEY_free(pubKey);
			RSA_free(rsa_pub_key);
			return -6;
		}
		if (!EVP_VerifyUpdate(&mdctx, puszInData, uInDataLen))
			//��֤ǩ����ժҪ��Update
		{
			printf("err\n");
			EVP_PKEY_free(pubKey);
			RSA_free(rsa_pub_key);
			return -7;
		}
		if (!EVP_VerifyFinal(&mdctx, puszOutSig, puOutSigLen, pubKey))//��֤ǩ��
		{
			printf("verify err\n");
			EVP_PKEY_free(pubKey);
			RSA_free(rsa_pub_key);
			return -8;
		}
		//else
		//{
			//	printf("��֤ǩ����ȷ.\n");
		//}

		EVP_PKEY_free(pubKey);
		RSA_free(rsa_pub_key);
		EVP_MD_CTX_cleanup(&mdctx);

		return 0;
	}



	/*
	https://www.cnblogs.com/cocoajin/p/6121706.html

	EVP_CipherInit_ex ��ʼ������ʹ�õ�key, iv���㷨ģʽ����� һ��������1��ʾ���ܣ�0��ʾ����

	EVP_CipherUpdate ���ܽ��ܴ���

	EVP_CipherFinal ��ȡ���
	*/
	//����
	int AES_128_CBC_Encrypt(
		unsigned char *puszInData,
		int nInDataLen,
		unsigned char *puszKey,
		unsigned char *puszIV,
		unsigned char *puszOutData,
		unsigned int *pOutDataLen)
	{
		EVP_CIPHER_CTX *ctx;

		int nTmpLen;

		int nOutDataLen;
		ctx = EVP_CIPHER_CTX_new();
		EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, puszKey, puszIV);
		EVP_EncryptUpdate(ctx, puszOutData, &nTmpLen, puszInData, nInDataLen);
		nOutDataLen = nTmpLen;
		EVP_EncryptFinal_ex(ctx, puszOutData + nTmpLen, &nTmpLen);
		nOutDataLen += nTmpLen;
		EVP_CIPHER_CTX_free(ctx);

		//return nOutDataLen;
		*pOutDataLen = nOutDataLen;
		return nOutDataLen;
	}

	//����
	int AES_128_CBC_Decrypt(
		unsigned char *puszInData,
		int nInDataLen,
		unsigned char *puszKey,
		unsigned char *puszIV,
		unsigned char *puszOutData,
		unsigned int *pOutDataLen
	)
	{
		EVP_CIPHER_CTX *ctx;

		int nTmpLen;
		unsigned int nOutDataLen;

		ctx = EVP_CIPHER_CTX_new();

		EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, puszKey, puszIV);
		EVP_DecryptUpdate(ctx, puszOutData, &nTmpLen, puszInData, nInDataLen);
		nOutDataLen = nTmpLen;

		EVP_DecryptFinal_ex(ctx, puszOutData + nTmpLen, &nTmpLen);
		nOutDataLen += nTmpLen;

		EVP_CIPHER_CTX_free(ctx);

		*pOutDataLen = nOutDataLen;
		return nOutDataLen;
	}

}
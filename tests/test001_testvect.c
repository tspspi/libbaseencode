#define TESTVECT_MAXIMUM_BUFFER_SIZE		1024

#include "../include/baseencode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char* testvectEncodeBase64In[7] = {
	"",
	"f",
	"fo",
	"foo",
	"foob",
	"fooba",
	"foobar"
};
static char* textvectEncodeBase64Out[7] = {
	"",
	"Zg==",
	"Zm8=",
	"Zm9v",
	"Zm9vYg==",
	"Zm9vYmE=",
	"Zm9vYmFy"
};


static char* testvectEncodeBase32In[7] = {
	"",
	"f",
	"fo",
	"foo",
	"foob",
	"fooba",
	"foobar"
};
static char* textvectEncodeBase32Out[7] = {
	"",
	"MY======",
	"MZXQ====",
	"MZXW6===",
	"MZXW6YQ=",
	"MZXW6YTB",
	"MZXW6YTBOI======"
};

static char* testvectEncodeBase32HexIn[7] = {
	"",
	"f",
	"fo",
	"foo",
	"foob",
	"fooba",
	"foobar"
};
static char* textvectEncodeBase32HexOut[7] = {
	"",
	"CO======",
	"CPNG====",
	"CPNMU===",
	"CPNMUOG=",
	"CPNMUOJ1",
	"CPNMUOJ1E8======"
};

static char* testvectEncodeBase16In[7] = {
	"",
	"f",
	"fo",
	"foo",
	"foob",
	"fooba",
	"foobar"
};
static char* textvectEncodeBase16Out[7] = {
	"",
	"66",
	"666F",
	"666F6F",
	"666F6F62",
	"666F6F6261",
	"666F6F626172"
};

static char* strTest2Base64In = "VGhpcyBpcyBqdXN0IGEgc2ltcGxlIHRlc3QNCndpdGggbXVsdGlwbGUgbGluZXMgZW5jb2RlZA0K\r\naW4gb3VyIGJhc2U2NCByZXByZXNlbnRhdGlvbi4NClRoaXMgc2hvdWxkIGFsc28gYmUgZW5jb2Rl\r\nZCBpbiBhDQp3YXkgdGhhdCBpbmNsdWRlcyBsaW5lYnJlYWtzDQppbiB0aGUgYmFzZTY0IG91dHB1\r\ndA==";
static char* strTest2Base64Out = "This is just a simple test\r\nwith multiple lines encoded\r\nin our base64 representation.\r\nThis should also be encoded in a\r\nway that includes linebreaks\r\nin the base64 output";

static char bBuffer[TESTVECT_MAXIMUM_BUFFER_SIZE];

int main(int argc, char* argv[]) {
	unsigned long int dwErrorCount;
	unsigned long int dwTestCount;
	unsigned long int i;
	unsigned long int dwOutExpect;
	unsigned long int dwOutWritten;
	enum baseencodeError e;

	dwErrorCount = 0;
	dwTestCount = 0;

	printf("Testing libbaseencode with RFC4648 test vectors\n");

	/* Test base64 encoding with test vectors */
	for(i = 0; i < sizeof(testvectEncodeBase64In)/sizeof(unsigned char*); i=i+1) {
		dwTestCount = dwTestCount + 1;
		printf("Base64Encode(%s): ", testvectEncodeBase64In[i]);
		dwOutExpect = baseencGetEncodedLength(baseencodeEnc_Base64, strlen(testvectEncodeBase64In[i]));
		printf("expect %lu bytes; ", dwOutExpect);
		if(dwOutExpect != strlen(textvectEncodeBase64Out[i])) {
			printf("INVALID LENGTH, EXPECT %u\n", strlen(textvectEncodeBase64Out[i]));
			dwErrorCount = dwErrorCount + 1;
		} else {
			e = baseencEncode((unsigned char*)bBuffer, sizeof(bBuffer), (unsigned char*)(testvectEncodeBase64In[i]), strlen(testvectEncodeBase64In[i]), baseencodeEnc_Base64, 0);
			if(e != baseencodeE_Ok) {
				printf("Unexpected error %u\n", e);
				dwErrorCount = dwErrorCount + 1;
			} else {
				printf("Result = %s", bBuffer);
				if(strcmp(textvectEncodeBase64Out[i], bBuffer) != 0) {
					printf("; Expect %s; TEST FAILED\n", textvectEncodeBase64Out[i]);
					dwErrorCount = dwErrorCount + 1;
				} else {
					printf("; OK\n");
				}
			}
		}
	}

	for(i = 0; i < sizeof(testvectEncodeBase32In)/sizeof(unsigned char*); i=i+1) {
		dwTestCount = dwTestCount + 1;
		printf("Base32Encode(%s): ", testvectEncodeBase32In[i]);
		dwOutExpect = baseencGetEncodedLength(baseencodeEnc_Base32, strlen(testvectEncodeBase32In[i]));
		printf("expect %lu bytes; ", dwOutExpect);
		if(dwOutExpect != strlen(textvectEncodeBase32Out[i])) {
			printf("INVALID LENGTH, EXPECT %u\n", strlen(textvectEncodeBase32Out[i]));
			dwErrorCount = dwErrorCount + 1;
		} else {
			e = baseencEncode((unsigned char*)bBuffer, sizeof(bBuffer), (unsigned char*)(testvectEncodeBase32In[i]), strlen(testvectEncodeBase32In[i]), baseencodeEnc_Base32, 0);
			if(e != baseencodeE_Ok) {
				printf("Unexpected error %u\n", e);
				dwErrorCount = dwErrorCount + 1;
			} else {
				printf("Result = %s", bBuffer);
				if(strcmp(textvectEncodeBase32Out[i], bBuffer) != 0) {
					printf("; Expect %s; TEST FAILED\n", textvectEncodeBase32Out[i]);
					dwErrorCount = dwErrorCount + 1;
				} else {
					printf("; OK\n");
				}
			}
		}
	}

	for(i = 0; i < sizeof(testvectEncodeBase32HexIn)/sizeof(unsigned char*); i=i+1) {
		dwTestCount = dwTestCount + 1;
		printf("Base32HexEncode(%s): ", testvectEncodeBase32HexIn[i]);
		dwOutExpect = baseencGetEncodedLength(baseencodeEnc_Base32Hex, strlen(testvectEncodeBase32HexIn[i]));
		printf("expect %lu bytes; ", dwOutExpect);
		if(dwOutExpect != strlen(textvectEncodeBase32HexOut[i])) {
			printf("INVALID LENGTH, EXPECT %u\n", strlen(textvectEncodeBase32HexOut[i]));
			dwErrorCount = dwErrorCount + 1;
		} else {
			e = baseencEncode((unsigned char*)bBuffer, sizeof(bBuffer), (unsigned char*)(testvectEncodeBase32HexIn[i]), strlen(testvectEncodeBase32HexIn[i]), baseencodeEnc_Base32Hex, 0);
			if(e != baseencodeE_Ok) {
				printf("Unexpected error %u\n", e);
				dwErrorCount = dwErrorCount + 1;
			} else {
				printf("Result = %s", bBuffer);
				if(strcmp(textvectEncodeBase32HexOut[i], bBuffer) != 0) {
					printf("; Expect %s; TEST FAILED\n", textvectEncodeBase32HexOut[i]);
					dwErrorCount = dwErrorCount + 1;
				} else {
					printf("; OK\n");
				}
			}
		}
	}

	for(i = 0; i < sizeof(testvectEncodeBase16In)/sizeof(unsigned char*); i=i+1) {
		dwTestCount = dwTestCount + 1;
		printf("Base16Encode(%s): ", testvectEncodeBase16In[i]);
		dwOutExpect = baseencGetEncodedLength(baseencodeEnc_Base16, strlen(testvectEncodeBase16In[i]));
		printf("expect %lu bytes; ", dwOutExpect);
		if(dwOutExpect != strlen(textvectEncodeBase16Out[i])) {
			printf("INVALID LENGTH, EXPECT %u\n", strlen(textvectEncodeBase16Out[i]));
			dwErrorCount = dwErrorCount + 1;
		} else {
			e = baseencEncode((unsigned char*)bBuffer, sizeof(bBuffer), (unsigned char*)(testvectEncodeBase16In[i]), strlen(testvectEncodeBase16In[i]), baseencodeEnc_Base16, 0);
			if(e != baseencodeE_Ok) {
				printf("Unexpected error %u\n", e);
				dwErrorCount = dwErrorCount + 1;
			} else {
				printf("Result = %s", bBuffer);
				if(strcmp(textvectEncodeBase16Out[i], bBuffer) != 0) {
					printf("; Expect %s; TEST FAILED\n", textvectEncodeBase16Out[i]);
					dwErrorCount = dwErrorCount + 1;
				} else {
					printf("; OK\n");
				}
			}
		}
	}




	/*
		DECODER TEST
	*/

	dwOutWritten = ~0;
	for(i = 0; i < sizeof(testvectEncodeBase64In)/sizeof(unsigned char*); i=i+1) {
		dwTestCount = dwTestCount + 1;
		printf("Base64Decode(%s): ", textvectEncodeBase64Out[i]);
		dwOutExpect = baseencGetDecodedMaxLength(baseencodeEnc_Base64, strlen(textvectEncodeBase64Out[i]));
		printf("expect max. %lu bytes; ", dwOutExpect);
		if(dwOutExpect < strlen(testvectEncodeBase64In[i])) {
			printf("INVALID LENGTH %lu, SHOULD BE AT LEAST %u\n", dwOutExpect, strlen(testvectEncodeBase64In[i]));
			dwErrorCount = dwErrorCount + 1;
		} else {
			e = baseencDecode((unsigned char*)bBuffer, sizeof(bBuffer), &dwOutWritten, (unsigned char*)(textvectEncodeBase64Out[i]), strlen(textvectEncodeBase64Out[i]), baseencodeEnc_Base64, 0);
			if(e != baseencodeE_Ok) {
				printf("Unexpected error %u\n", e);
				dwErrorCount = dwErrorCount + 1;
			} else {
				printf("Result = %s", bBuffer);
				if(dwOutWritten > dwOutExpect) {
					printf("; TEST FAILED - INVALID LENGTH EXPECTED VS WRITTEN; Written: %lu\n", dwOutWritten);
					dwErrorCount = dwErrorCount + 1;
				} else if(strcmp(testvectEncodeBase64In[i], bBuffer) != 0) {
					printf("; Expect %s; TEST FAILED\n", testvectEncodeBase64In[i]);
					dwErrorCount = dwErrorCount + 1;
				} else {
					printf("; OK\n");
				}
			}
		}
	}

	dwOutWritten = ~0;
	for(i = 0; i < sizeof(testvectEncodeBase32In)/sizeof(unsigned char*); i=i+1) {
		dwTestCount = dwTestCount + 1;
		printf("Base32Decode(%s): ", textvectEncodeBase32Out[i]);
		dwOutExpect = baseencGetDecodedMaxLength(baseencodeEnc_Base32, strlen(textvectEncodeBase32Out[i]));
		printf("expect max. %lu bytes; ", dwOutExpect);
		if(dwOutExpect < strlen(testvectEncodeBase32In[i])) {
			printf("INVALID LENGTH %lu, SHOULD BE AT LEAST %u\n", dwOutExpect, strlen(testvectEncodeBase32In[i]));
			dwErrorCount = dwErrorCount + 1;
		} else {
			e = baseencDecode((unsigned char*)bBuffer, sizeof(bBuffer), &dwOutWritten, (unsigned char*)(textvectEncodeBase32Out[i]), strlen(textvectEncodeBase32Out[i]), baseencodeEnc_Base32, 0);
			if(e != baseencodeE_Ok) {
				printf("Unexpected error %u\n", e);
				dwErrorCount = dwErrorCount + 1;
			} else {
				printf("Result = %s", bBuffer);
				if(dwOutWritten > dwOutExpect) {
					printf("; TEST FAILED - INVALID LENGTH EXPECTED VS WRITTEN; Written: %lu\n", dwOutWritten);
					dwErrorCount = dwErrorCount + 1;
				} else if(strcmp(testvectEncodeBase32In[i], bBuffer) != 0) {
					printf("; Expect %s; TEST FAILED\n", testvectEncodeBase32In[i]);
					dwErrorCount = dwErrorCount + 1;
				} else {
					printf("; OK\n");
				}
			}
		}
	}

	dwOutWritten = ~0;
	for(i = 0; i < sizeof(testvectEncodeBase32HexIn)/sizeof(unsigned char*); i=i+1) {
		dwTestCount = dwTestCount + 1;
		printf("Base32HexDecode(%s): ", textvectEncodeBase32HexOut[i]);
		dwOutExpect = baseencGetDecodedMaxLength(baseencodeEnc_Base32Hex, strlen(textvectEncodeBase32HexOut[i]));
		printf("expect max. %lu bytes; ", dwOutExpect);
		if(dwOutExpect < strlen(testvectEncodeBase32HexIn[i])) {
			printf("INVALID LENGTH %lu, SHOULD BE AT LEAST %u\n", dwOutExpect, strlen(testvectEncodeBase32HexIn[i]));
			dwErrorCount = dwErrorCount + 1;
		} else {
			e = baseencDecode((unsigned char*)bBuffer, sizeof(bBuffer), &dwOutWritten, (unsigned char*)(textvectEncodeBase32HexOut[i]), strlen(textvectEncodeBase32HexOut[i]), baseencodeEnc_Base32Hex, 0);
			if(e != baseencodeE_Ok) {
				printf("Unexpected error %u\n", e);
				dwErrorCount = dwErrorCount + 1;
			} else {
				printf("Result = %s", bBuffer);
				if(dwOutWritten > dwOutExpect) {
					printf("; TEST FAILED - INVALID LENGTH EXPECTED VS WRITTEN; Written: %lu\n", dwOutWritten);
					dwErrorCount = dwErrorCount + 1;
				} else if(strcmp(testvectEncodeBase32HexIn[i], bBuffer) != 0) {
					printf("; Expect %s; TEST FAILED\n", testvectEncodeBase32HexIn[i]);
					dwErrorCount = dwErrorCount + 1;
				} else {
					printf("; OK\n");
				}
			}
		}
	}

	dwOutWritten = ~0;
	for(i = 0; i < sizeof(testvectEncodeBase16In)/sizeof(unsigned char*); i=i+1) {
		dwTestCount = dwTestCount + 1;
		printf("Base16Decode(%s): ", textvectEncodeBase16Out[i]);
		dwOutExpect = baseencGetDecodedMaxLength(baseencodeEnc_Base16, strlen(textvectEncodeBase16Out[i]));
		printf("expect max. %lu bytes; ", dwOutExpect);
		if(dwOutExpect < strlen(testvectEncodeBase16In[i])) {
			printf("INVALID LENGTH %lu, SHOULD BE AT LEAST %u\n", dwOutExpect, strlen(testvectEncodeBase16In[i]));
			dwErrorCount = dwErrorCount + 1;
		} else {
			e = baseencDecode((unsigned char*)bBuffer, sizeof(bBuffer), &dwOutWritten, (unsigned char*)(textvectEncodeBase16Out[i]), strlen(textvectEncodeBase16Out[i]), baseencodeEnc_Base16, 0);
			if(e != baseencodeE_Ok) {
				printf("Unexpected error %u\n", e);
				dwErrorCount = dwErrorCount + 1;
			} else {
				printf("Result = %s", bBuffer);
				if(dwOutWritten > dwOutExpect) {
					printf("; TEST FAILED - INVALID LENGTH EXPECTED VS WRITTEN; Written: %lu\n", dwOutWritten);
					dwErrorCount = dwErrorCount + 1;
				} else if(strcmp(testvectEncodeBase16In[i], bBuffer) != 0) {
					printf("; Expect %s; TEST FAILED\n", testvectEncodeBase16In[i]);
					dwErrorCount = dwErrorCount + 1;
				} else {
					printf("; OK\n");
				}
			}
		}
	}

	/* Try to decode a string that includes non-alphabet chars */
	e = baseencDecode((unsigned char*)bBuffer, sizeof(bBuffer), &dwOutWritten, (unsigned char*)strTest2Base64In, strlen(strTest2Base64In), baseencodeEnc_Base64, 0);
	if((e != baseencodeE_InvalidFormat) && (e != baseencodeE_InvalidParam)) {
		printf("%s:%u Failed to detect invalid characters included inside base64 data stream\n", __FILE__, __LINE__);
		dwErrorCount = dwErrorCount + 1;
	} else {
		printf("Invalid characters recognized correctly: OK\n");
	}

	e = baseencDecode((unsigned char*)bBuffer, sizeof(bBuffer), &dwOutWritten, (unsigned char*)strTest2Base64In, strlen(strTest2Base64In), baseencodeEnc_Base64, BASEENCODE_FLAG__ACCEPTNOALPHABET);
	if(e != baseencodeE_Ok) {
		printf("%s:%u Failed to decode complex string\n", __FILE__, __LINE__);
		dwErrorCount = dwErrorCount + 1;
	} else {
		if(dwOutWritten != strlen(strTest2Base64Out)) {
			printf("%s:%u Length mismatch. Decoded is %lu bytes, should be %u\n", __FILE__, __LINE__, dwOutWritten, strlen(strTest2Base64Out));
			dwErrorCount = dwErrorCount + 1;
		} else {
			printf("Decoded string:\n\n%s\n\n", bBuffer);
			if(strncmp(bBuffer, strTest2Base64Out, dwOutWritten) != 0) {
				printf("Mismatch.");
				dwErrorCount = dwErrorCount + 1;
			} else {
				printf("Matches expected string\n");
			}
		}
	}

	printf("\n------------------------------------\n");
	if(dwErrorCount > 0) {
		printf("%lu of %lu failed\n", dwErrorCount, dwTestCount);
		printf("------------------------------------\n\n");
		return -1;
	} else {
		printf("All succeeded\n");
		printf("------------------------------------\n\n");
		return 0;
	}
}

#include "../include/baseencode.h"
#include <string.h>

#ifdef __cplusplus
	extern "C" {
#endif

unsigned long int baseencGetEncodedLength(
	enum baseencodeEnc encoding,
	unsigned long int dwRawInputLength
) {
	switch(encoding) {
		case baseencodeEnc_Base64:
		case baseencodeEnc_Base64Url:
			return (dwRawInputLength / 3) * 4 + ((dwRawInputLength % 3 != 0) ? 4 : 0);

		case baseencodeEnc_Base32:
		case baseencodeEnc_Base32Hex:
			return (dwRawInputLength / 5) * 8 + ((dwRawInputLength % 5 != 0) ? 8 : 0);

		case baseencodeEnc_Base16:
			return (dwRawInputLength) * 2;

		default:
			return ~0;
	}
}

unsigned long int baseencGetDecodedMaxLength(
	enum baseencodeEnc encoding,
	unsigned long int dwEncLen
) {
	switch(encoding) {
		case baseencodeEnc_Base64:
		case baseencodeEnc_Base64Url:
			if((dwEncLen % 4) != 0) { return ~0; } /* Should (including padding) always be a multiple of 4 */
			return (dwEncLen / 4) * 3;

		case baseencodeEnc_Base32:
		case baseencodeEnc_Base32Hex:
			if((dwEncLen % 8) != 0) { return ~0; } /* Should (including padding) always be a multiple of 8 */
			return (dwEncLen / 8) * 5;

		case baseencodeEnc_Base16:
			if((dwEncLen % 2) != 0) { return ~0; } /* Should (including padding) always be a multiple of 2 */
			return dwEncLen / 2;

		default:
			return ~0;
	}
}

static unsigned char baseencAlphabetBase64[65]= {
	65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,
	97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,
	48,49,50,51,52,53,54,55,56,57,
	43,47,
	61
};
static unsigned char baseencAlphabetBase64Url[65]= {
	65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,
	97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,
	48,49,50,51,52,53,54,55,56,57,
	45,95,
	61
};
static unsigned char baseencAlphabetBase32[33] = {
	65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,
	50,51,52,53,54,55,
	61
};
static unsigned char baseencAlphabetBase32Hex[33] = {
	48,49,50,51,52,53,54,55,56,57,
	65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,
	61
};
static unsigned char baseencAlphabetBase16[16] = {
	48,49,50,51,52,53,54,55,56,57,
	65,66,67,68,69,70
};

static enum baseencodeError baseencEncode_Base16(
	unsigned char* lpBufferOut,
	unsigned long int dwOutputSize,

	const unsigned char* lpData,
	unsigned long int dwRawLength,

	enum baseencodeEnc encoding,
	uint32_t dwFlags
) {
	unsigned long int dwCurrentByte;

	for(dwCurrentByte = 0; dwCurrentByte < dwRawLength; dwCurrentByte = dwCurrentByte + 1) {
		lpBufferOut[dwCurrentByte*2 + 0] = baseencAlphabetBase16[(lpData[dwCurrentByte] >> 4) & 0x0F];
		lpBufferOut[dwCurrentByte*2 + 1] = baseencAlphabetBase16[(lpData[dwCurrentByte]) & 0x0F];
	}

	return baseencodeE_Ok;
}

static enum baseencodeError baseencEncode_Base32(
	unsigned char* lpBufferOut,
	unsigned long int dwOutputSize,

	const unsigned char* lpData,
	unsigned long int dwRawLength,

	enum baseencodeEnc encoding,
	uint32_t dwFlags
) {
	unsigned long int idx[8];
	uint8_t bIn[5];
	unsigned long int dwCurrentGroup;

	for(dwCurrentGroup = 0; dwCurrentGroup < (dwRawLength / 5); dwCurrentGroup = dwCurrentGroup + 1) {
		idx[0] = (lpData[dwCurrentGroup*5+0] >> 3) & 0x1F;
		idx[1] = ((lpData[dwCurrentGroup*5+0] & 0x07) << 2) | ((lpData[dwCurrentGroup*5+1] >> 6) & 0x03);
		idx[2] = ((lpData[dwCurrentGroup*5+1] >> 1) & 0x1F);
		idx[3] = ((lpData[dwCurrentGroup*5+1] << 4) & 0x10) | ((lpData[dwCurrentGroup*5+2] >> 4) & 0x0F);
		idx[4] = ((lpData[dwCurrentGroup*5+2] & 0x0F) << 1) | ((lpData[dwCurrentGroup*5+3] >> 7) & 0x01);
		idx[5] = ((lpData[dwCurrentGroup*5+3] >> 2) & 0x1F);
		idx[6] = ((lpData[dwCurrentGroup*5+3] & 0x03) << 3) | ((lpData[dwCurrentGroup*5+4] >> 5) & 0x07);
		idx[7] = ((lpData[dwCurrentGroup*5+4]) & 0x1F);

		if(encoding == baseencodeEnc_Base32) {
			lpBufferOut[dwCurrentGroup * 8 + 0] = baseencAlphabetBase32[idx[0]];
			lpBufferOut[dwCurrentGroup * 8 + 1] = baseencAlphabetBase32[idx[1]];
			lpBufferOut[dwCurrentGroup * 8 + 2] = baseencAlphabetBase32[idx[2]];
			lpBufferOut[dwCurrentGroup * 8 + 3] = baseencAlphabetBase32[idx[3]];
			lpBufferOut[dwCurrentGroup * 8 + 4] = baseencAlphabetBase32[idx[4]];
			lpBufferOut[dwCurrentGroup * 8 + 5] = baseencAlphabetBase32[idx[5]];
			lpBufferOut[dwCurrentGroup * 8 + 6] = baseencAlphabetBase32[idx[6]];
			lpBufferOut[dwCurrentGroup * 8 + 7] = baseencAlphabetBase32[idx[7]];
		} else {
			lpBufferOut[dwCurrentGroup * 8 + 0] = baseencAlphabetBase32Hex[idx[0]];
			lpBufferOut[dwCurrentGroup * 8 + 1] = baseencAlphabetBase32Hex[idx[1]];
			lpBufferOut[dwCurrentGroup * 8 + 2] = baseencAlphabetBase32Hex[idx[2]];
			lpBufferOut[dwCurrentGroup * 8 + 3] = baseencAlphabetBase32Hex[idx[3]];
			lpBufferOut[dwCurrentGroup * 8 + 4] = baseencAlphabetBase32Hex[idx[4]];
			lpBufferOut[dwCurrentGroup * 8 + 5] = baseencAlphabetBase32Hex[idx[5]];
			lpBufferOut[dwCurrentGroup * 8 + 6] = baseencAlphabetBase32Hex[idx[6]];
			lpBufferOut[dwCurrentGroup * 8 + 7] = baseencAlphabetBase32Hex[idx[7]];
		}
	}

	/* With padding ... */
	if(dwRawLength % 5 != 0) {
		bIn[0] = lpData[dwCurrentGroup * 5 + 0];
		bIn[1] = (dwRawLength % 5 > 1) ? lpData[dwCurrentGroup * 5 + 1] : 0;
		bIn[2] = (dwRawLength % 5 > 2) ? lpData[dwCurrentGroup * 5 + 2] : 0;
		bIn[3] = (dwRawLength % 5 > 3) ? lpData[dwCurrentGroup * 5 + 3] : 0;
		bIn[4] = (dwRawLength % 5 > 4) ? lpData[dwCurrentGroup * 5 + 4] : 0;

		idx[0] = (bIn[0] >> 3) & 0x1F;
		idx[1] = ((bIn[0] & 0x07) << 2) | ((bIn[1] >> 6) & 0x03);
		if(dwRawLength % 5 > 1) {
			idx[2] = ((bIn[1] >> 1) & 0x1F);
			idx[3] = ((bIn[1] << 4) & 0x10) | ((bIn[2] >> 4) & 0x0F);
		} else {
			idx[2] = 32;
			idx[3] = 32;
		}
		if(dwRawLength % 5 > 2) {
			idx[4] = ((bIn[2] & 0x0F) << 1) | ((bIn[3] >> 7) & 0x01);
		} else {
			idx[4] = 32;
		}
		if(dwRawLength % 5 > 3) {
			idx[5] = ((bIn[3] >> 2) & 0x1F);
			idx[6] = ((bIn[3] & 0x03) << 3) | ((bIn[4] >> 5) & 0x07);
		} else {
			idx[5] = 32;
			idx[6] = 32;
		}
		idx[7] = 32;

		if(encoding == baseencodeEnc_Base32) {
			lpBufferOut[dwCurrentGroup * 8 + 0] = baseencAlphabetBase32[idx[0]];
			lpBufferOut[dwCurrentGroup * 8 + 1] = baseencAlphabetBase32[idx[1]];
			lpBufferOut[dwCurrentGroup * 8 + 2] = baseencAlphabetBase32[idx[2]];
			lpBufferOut[dwCurrentGroup * 8 + 3] = baseencAlphabetBase32[idx[3]];
			lpBufferOut[dwCurrentGroup * 8 + 4] = baseencAlphabetBase32[idx[4]];
			lpBufferOut[dwCurrentGroup * 8 + 5] = baseencAlphabetBase32[idx[5]];
			lpBufferOut[dwCurrentGroup * 8 + 6] = baseencAlphabetBase32[idx[6]];
			lpBufferOut[dwCurrentGroup * 8 + 7] = baseencAlphabetBase32[idx[7]];
		} else {
			lpBufferOut[dwCurrentGroup * 8 + 0] = baseencAlphabetBase32Hex[idx[0]];
			lpBufferOut[dwCurrentGroup * 8 + 1] = baseencAlphabetBase32Hex[idx[1]];
			lpBufferOut[dwCurrentGroup * 8 + 2] = baseencAlphabetBase32Hex[idx[2]];
			lpBufferOut[dwCurrentGroup * 8 + 3] = baseencAlphabetBase32Hex[idx[3]];
			lpBufferOut[dwCurrentGroup * 8 + 4] = baseencAlphabetBase32Hex[idx[4]];
			lpBufferOut[dwCurrentGroup * 8 + 5] = baseencAlphabetBase32Hex[idx[5]];
			lpBufferOut[dwCurrentGroup * 8 + 6] = baseencAlphabetBase32Hex[idx[6]];
			lpBufferOut[dwCurrentGroup * 8 + 7] = baseencAlphabetBase32Hex[idx[7]];
		}
	}

	return baseencodeE_Ok;
}

static enum baseencodeError baseencEncode_Base64(
	unsigned char* lpBufferOut,
	unsigned long int dwOutputSize,

	const unsigned char* lpData,
	unsigned long int dwRawLength,

	enum baseencodeEnc encoding,
	uint32_t dwFlags
) {
	unsigned long int idx[4];
	unsigned long int dwCurrentGroup;

	for(dwCurrentGroup = 0; dwCurrentGroup < (dwRawLength / 3); dwCurrentGroup = dwCurrentGroup + 1) {
		/* Read indices */
		idx[0] = (lpData[dwCurrentGroup*3 + 0] >> 2) & 0x3F;
		idx[1] = ((lpData[dwCurrentGroup*3 + 0] & 0x03) << 4) | ((lpData[dwCurrentGroup * 3 + 1] >> 4) & 0x0F);
		idx[2] = ((lpData[dwCurrentGroup * 3 + 1] & 0x0F) << 2) | ((lpData[dwCurrentGroup * 3 + 2] >> 6) & 0x03);
		idx[3] = lpData[dwCurrentGroup * 3 + 2] & 0x3F;

		/* Write output symbols */
		if(encoding == baseencodeEnc_Base64) {
			lpBufferOut[dwCurrentGroup * 4 + 0] = baseencAlphabetBase64[idx[0]];
			lpBufferOut[dwCurrentGroup * 4 + 1] = baseencAlphabetBase64[idx[1]];
			lpBufferOut[dwCurrentGroup * 4 + 2] = baseencAlphabetBase64[idx[2]];
			lpBufferOut[dwCurrentGroup * 4 + 3] = baseencAlphabetBase64[idx[3]];
		} else {
			lpBufferOut[dwCurrentGroup * 4 + 0] = baseencAlphabetBase64Url[idx[0]];
			lpBufferOut[dwCurrentGroup * 4 + 1] = baseencAlphabetBase64Url[idx[1]];
			lpBufferOut[dwCurrentGroup * 4 + 2] = baseencAlphabetBase64Url[idx[2]];
			lpBufferOut[dwCurrentGroup * 4 + 3] = baseencAlphabetBase64Url[idx[3]];
		}
	}

	/* Do we require any padding? */
	if(dwRawLength % 3 != 0) {
		if(dwRawLength % 3 == 1) {
			/* 8 bit in last block */
		} else {
			/* 16 bit in last block */
		}
		idx[0] = (lpData[dwCurrentGroup*3 + 0] >> 2) & 0x3F;
		if(dwRawLength % 3 > 1) {
			idx[1] = ((lpData[dwCurrentGroup*3 + 0] & 0x03) << 4) | ((lpData[dwCurrentGroup * 3 + 1] >> 4) & 0x0F);
			idx[2] = ((lpData[dwCurrentGroup * 3 + 1] & 0x0F) << 2);
		} else {
			idx[1] = ((lpData[dwCurrentGroup*3 + 0] & 0x03) << 4);
			idx[2] = 64;
		}
		idx[3] = 64;

		/* Write output symbols */
		if(encoding == baseencodeEnc_Base64) {
			lpBufferOut[dwCurrentGroup * 4 + 0] = baseencAlphabetBase64[idx[0]];
			lpBufferOut[dwCurrentGroup * 4 + 1] = baseencAlphabetBase64[idx[1]];
			lpBufferOut[dwCurrentGroup * 4 + 2] = baseencAlphabetBase64[idx[2]];
			lpBufferOut[dwCurrentGroup * 4 + 3] = baseencAlphabetBase64[idx[3]];
		} else {
			lpBufferOut[dwCurrentGroup * 4 + 0] = baseencAlphabetBase64Url[idx[0]];
			lpBufferOut[dwCurrentGroup * 4 + 1] = baseencAlphabetBase64Url[idx[1]];
			lpBufferOut[dwCurrentGroup * 4 + 2] = baseencAlphabetBase64Url[idx[2]];
			lpBufferOut[dwCurrentGroup * 4 + 3] = baseencAlphabetBase64Url[idx[3]];
		}
	}
	return baseencodeE_Ok;
}

enum baseencodeError baseencEncode(
	unsigned char* lpBufferOut,
	unsigned long int dwOutputSize,

	const unsigned char* lpData,
	unsigned long int dwRawLength,

	enum baseencodeEnc encoding,
	uint32_t dwFlags
) {
	unsigned long int dwExpectedBufferSize;

	if(dwRawLength == 0) {
		if((dwOutputSize > 0) && (lpBufferOut == NULL)) { return baseencodeE_InvalidParam; }
		if((dwOutputSize > 0) && (lpBufferOut != NULL)) { memset((void*)lpBufferOut, 0, dwOutputSize); }
		return baseencodeE_Ok;
	}

	dwExpectedBufferSize = baseencGetEncodedLength(encoding, dwRawLength);
	if(dwOutputSize > dwExpectedBufferSize) {
		memset((void*)(&(lpBufferOut[dwExpectedBufferSize])), 0, dwOutputSize-dwExpectedBufferSize);
	}
	if(dwExpectedBufferSize == ~0) { return baseencodeE_InvalidParam; }

	if((lpBufferOut == NULL) || (dwOutputSize < dwExpectedBufferSize)) { return baseencodeE_InvalidParam; }
	if((lpData == NULL) && (dwRawLength > 0)) { return baseencodeE_InvalidParam; }

	/* Encode data ... */
	switch(encoding) {
		case baseencodeEnc_Base64:
		case baseencodeEnc_Base64Url:
			return baseencEncode_Base64(lpBufferOut, dwOutputSize, lpData, dwRawLength, encoding, dwFlags);

		case baseencodeEnc_Base32:
		case baseencodeEnc_Base32Hex:
			return baseencEncode_Base32(lpBufferOut, dwOutputSize, lpData, dwRawLength, encoding, dwFlags);

		case baseencodeEnc_Base16:
			return baseencEncode_Base16(lpBufferOut, dwOutputSize, lpData, dwRawLength, encoding, dwFlags);

		default:
			return baseencodeE_InvalidParam;
	}
}

/*
	Inverse tables.

	These have been generated via /tools/invtablegen.c

	baseencAlphabetBase16Inv has been patched manually to
	recognize A-F also case insensitive (a-f).
*/

static unsigned char baseencAlphabetBase64Inv[256] = {
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255, 62,255,255,255, 63, 52, 53,
         54, 55, 56, 57, 58, 59, 60, 61,255,255,
        255, 64,255,255,255,  0,  1,  2,  3,  4,
          5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
         15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
         25,255,255,255,255,255,255, 26, 27, 28,
         29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
         39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
         49, 50, 51,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255
};

static unsigned char baseencAlphabetBase64UrlInv[256] = {
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255, 62,255,255, 52, 53,
         54, 55, 56, 57, 58, 59, 60, 61,255,255,
        255, 64,255,255,255,  0,  1,  2,  3,  4,
          5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
         15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
         25,255,255,255,255, 63,255, 26, 27, 28,
         29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
         39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
         49, 50, 51,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255
};

static unsigned char baseencAlphabetBase32Inv[256] = {
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
         26, 27, 28, 29, 30, 31,255,255,255,255,
        255, 32,255,255,255,  0,  1,  2,  3,  4,
          5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
         15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
         25,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255
};

static unsigned char baseencAlphabetBase32HexInv[256] = {
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,  0,  1,
          2,  3,  4,  5,  6,  7,  8,  9,255,255,
        255, 32,255,255,255, 10, 11, 12, 13, 14,
         15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
         25, 26, 27, 28, 29, 30, 31,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255
};

static unsigned char baseencAlphabetBase16Inv[256] = {
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,  0,  1,
          2,  3,  4,  5,  6,  7,  8,  9,255,255,
        255,255,255,255,255, 10, 11, 12, 13, 14,
         15,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255, 10, 11, 12,
         13, 14, 15,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255
};

static enum baseencodeError baseencDecode_Base16(
	unsigned char* lpBufferOut,
	unsigned long int dwOutputSize,
	unsigned long int* lpOutputWrittenOut,

	const unsigned char* lpEncData,
	unsigned long int dwEncLength,

	enum baseencodeEnc encoding,
	uint32_t dwFlags
) {
	unsigned char bIn[2];
	unsigned char bDecoded;

	unsigned long int dwCurrentByteIn;
	unsigned long int dwCachedBytes;
	unsigned long int dwCurrentByteOut;

	/*
		We start without any cached data
		and without any written data ...
	*/
	dwCachedBytes = 0;
	dwCurrentByteOut = 0;

	/*
		... and iterate over our input bytes
	*/
	for(dwCurrentByteIn = 0; dwCurrentByteIn < dwEncLength; dwCurrentByteIn = dwCurrentByteIn + 1) {
		bDecoded = baseencAlphabetBase16Inv[lpEncData[dwCurrentByteIn]];

		if(bDecoded == 255) {
			if((dwFlags & BASEENCODE_FLAG__ACCEPTNOALPHABET) == 0) { return baseencodeE_InvalidFormat; }
			continue; /* Skip unknown bytes */
		}

		bIn[dwCachedBytes] = bDecoded;
		dwCachedBytes = dwCachedBytes + 1;
		if(dwCachedBytes < 2) {
			continue; /* Require more bytes for the next 3 byte group */
		}
		dwCachedBytes = 0;

		if(dwCurrentByteOut >= dwOutputSize) { return baseencodeE_BufferTooSmall; }
		lpBufferOut[dwCurrentByteOut] = ((bIn[0] << 4) & 0xF0) | ((bIn[1]) & 0x0F);
		dwCurrentByteOut = dwCurrentByteOut + 1;
	}

	if(lpOutputWrittenOut != NULL) { (*lpOutputWrittenOut) = dwCurrentByteOut; }
	return baseencodeE_Ok;
}

static enum baseencodeError baseencDecode_Base32(
	unsigned char* lpBufferOut,
	unsigned long int dwOutputSize,
	unsigned long int* lpOutputWrittenOut,

	const unsigned char* lpEncData,
	unsigned long int dwEncLength,

	enum baseencodeEnc encoding,
	uint32_t dwFlags
) {
	unsigned char bIn[8];
	unsigned char bDecoded;

	unsigned long int dwCurrentByteIn;
	unsigned long int dwCachedBytes;
	unsigned long int dwCurrentByteOut;

	/*
		We start without any cached data
		and without any written data ...
	*/
	dwCachedBytes = 0;
	dwCurrentByteOut = 0;

	/*
		... and iterate over our input bytes
	*/
	for(dwCurrentByteIn = 0; dwCurrentByteIn < dwEncLength; dwCurrentByteIn = dwCurrentByteIn + 1) {
		bDecoded = ((encoding == baseencodeEnc_Base32) ? baseencAlphabetBase32Inv[lpEncData[dwCurrentByteIn]] : baseencAlphabetBase32HexInv[lpEncData[dwCurrentByteIn]]);

		if(bDecoded == 255) {
			if((dwFlags & BASEENCODE_FLAG__ACCEPTNOALPHABET) == 0) { return baseencodeE_InvalidFormat; }
			continue; /* Skip unknown bytes */
		}

		bIn[dwCachedBytes] = bDecoded;
		dwCachedBytes = dwCachedBytes + 1;
		if(dwCachedBytes < 8) {
			continue; /* Require more bytes for the next 3 byte group */
		}
		dwCachedBytes = 0;

		/* Fetched all we require to decode the next 5 byte group */
		if((bIn[0] == 64) || (bIn[1] == 64)) {
			/* The FIRST byte would be a padding symbol - that's invalid */
			return baseencodeE_InvalidFormat;
		}

		if(dwCurrentByteOut >= dwOutputSize) { return baseencodeE_BufferTooSmall; }
		lpBufferOut[dwCurrentByteOut] = ((bIn[0] << 3) & 0xF8) | ((bIn[1] >> 2) & 0x07);
		dwCurrentByteOut = dwCurrentByteOut + 1;

		if(bIn[2] == 32) { break; } /* Read everything */
		if(bIn[3] == 32) { return baseencodeE_InvalidFormat; }
		if(dwCurrentByteOut >= dwOutputSize) { return baseencodeE_BufferTooSmall; }
		lpBufferOut[dwCurrentByteOut] = ((bIn[1] << 6) & 0xC0) | ((bIn[2] << 1) & 0x3E) | ((bIn[3]>> 4) & 0x01);
		dwCurrentByteOut = dwCurrentByteOut + 1;

		if(bIn[4] == 32) { break; } /* Read everything */
		if(dwCurrentByteOut >= dwOutputSize) { return baseencodeE_BufferTooSmall; }
		lpBufferOut[dwCurrentByteOut] = ((bIn[3] << 4) & 0xF0) | ((bIn[4] >> 1) & 0x0F);
		dwCurrentByteOut = dwCurrentByteOut + 1;

		if(bIn[5] == 32) { break; } /* Read everything */
		if(bIn[6] == 32) { return baseencodeE_InvalidFormat; }
		lpBufferOut[dwCurrentByteOut] = ((bIn[4] << 7) & 0x80) | ((bIn[5] << 2) & 0x7C) | ((bIn[6] >> 3) & 0x03);
		dwCurrentByteOut = dwCurrentByteOut + 1;

		if(bIn[7] == 32) { break; }
		lpBufferOut[dwCurrentByteOut] = ((bIn[6] << 5) & 0xE0) | (bIn[7] & 0x1F);
		dwCurrentByteOut = dwCurrentByteOut + 1;
	}

	if(lpOutputWrittenOut != NULL) { (*lpOutputWrittenOut) = dwCurrentByteOut; }
	return baseencodeE_Ok;
}

static enum baseencodeError baseencDecode_Base64(
	unsigned char* lpBufferOut,
	unsigned long int dwOutputSize,
	unsigned long int* lpOutputWrittenOut,

	const unsigned char* lpEncData,
	unsigned long int dwEncLength,

	enum baseencodeEnc encoding,
	uint32_t dwFlags
) {
	unsigned char bIn[4];
	unsigned char bDecoded;

	unsigned long int dwCurrentByteIn;
	unsigned long int dwCachedBytes;
	unsigned long int dwCurrentByteOut;

	/*
		We start without any cached data
		and without any written data ...
	*/
	dwCachedBytes = 0;
	dwCurrentByteOut = 0;

	/*
		... and iterate over our input bytes
	*/
	for(dwCurrentByteIn = 0; dwCurrentByteIn < dwEncLength; dwCurrentByteIn = dwCurrentByteIn + 1) {
		bDecoded = ((encoding == baseencodeEnc_Base64) ? baseencAlphabetBase64Inv[lpEncData[dwCurrentByteIn]] : baseencAlphabetBase64UrlInv[lpEncData[dwCurrentByteIn]]);

		if(bDecoded == 255) {
			if((dwFlags & BASEENCODE_FLAG__ACCEPTNOALPHABET) == 0) { return baseencodeE_InvalidFormat; }
			continue; /* Skip unknown bytes */
		}

		bIn[dwCachedBytes] = bDecoded;
		dwCachedBytes = dwCachedBytes + 1;
		if(dwCachedBytes < 4) {
			continue; /* Require more bytes for the next 3 byte group */
		}
		dwCachedBytes = 0;

		/* Fetched all we require to decode the next 3 byte group */
		if((bIn[0] == 64) || (bIn[1] == 64)) {
			/* The FIRST byte would be a padding symbol - that's invalid */
			return baseencodeE_InvalidFormat;
		}

		if(dwCurrentByteOut >= dwOutputSize) { return baseencodeE_BufferTooSmall; }
		lpBufferOut[dwCurrentByteOut] = ((bIn[0] << 2) & 0xFC) | ((bIn[1] >> 4) & 0x03);
		dwCurrentByteOut = dwCurrentByteOut + 1;

		if(bIn[2] == 64) { break; } /* Read everything */
		if(dwCurrentByteOut >= dwOutputSize) { return baseencodeE_BufferTooSmall; }
		lpBufferOut[dwCurrentByteOut] = ((bIn[1] << 4) & 0xF0) | ((bIn[2] >> 2) & 0x0F);
		dwCurrentByteOut = dwCurrentByteOut + 1;

		if(bIn[3] == 64) { break; } /* Read everything */
		if(dwCurrentByteOut >= dwOutputSize) { return baseencodeE_BufferTooSmall; }
		lpBufferOut[dwCurrentByteOut] = ((bIn[2] << 6) & 0xC0) | (bIn[3] & 0x3F);
		dwCurrentByteOut = dwCurrentByteOut + 1;
	}

	if(lpOutputWrittenOut != NULL) { (*lpOutputWrittenOut) = dwCurrentByteOut; }
	return baseencodeE_Ok;
}

enum baseencodeError baseencDecode(
	unsigned char* lpBufferOut,
	unsigned long int dwOutputSize,
	unsigned long int* lpOutputWrittenOut,

	const unsigned char* lpEncData,
	unsigned long int dwEncLength,

	enum baseencodeEnc encoding,
	uint32_t dwFlags
) {
	unsigned long int dwExpectedBufferSize;

	if(dwEncLength == 0) {
		if((dwOutputSize > 0) && (lpBufferOut == NULL)) { return baseencodeE_InvalidParam; }
		if((dwOutputSize > 0) && (lpBufferOut != NULL)) { memset((void*)lpBufferOut, 0, dwOutputSize); }
		if(lpOutputWrittenOut != NULL) { (*lpOutputWrittenOut) = 0; }
		return baseencodeE_Ok;
	}

	if((dwFlags & BASEENCODE_FLAG__ACCEPTNOALPHABET) == 0) {
		dwExpectedBufferSize = baseencGetDecodedMaxLength(encoding, dwEncLength);
		if(dwExpectedBufferSize == ~0) {
			if(lpOutputWrittenOut != NULL) { (*lpOutputWrittenOut) = 0; }
			return baseencodeE_InvalidParam;
		}
	}

	if(lpBufferOut == NULL) { return baseencodeE_InvalidParam; }

	switch(encoding) {
		case baseencodeEnc_Base64:
		case baseencodeEnc_Base64Url:
			return baseencDecode_Base64(lpBufferOut, dwOutputSize, lpOutputWrittenOut, lpEncData, dwEncLength, encoding, dwFlags);

		case baseencodeEnc_Base32:
		case baseencodeEnc_Base32Hex:
			return baseencDecode_Base32(lpBufferOut, dwOutputSize, lpOutputWrittenOut, lpEncData, dwEncLength, encoding, dwFlags);

		case baseencodeEnc_Base16:
			return baseencDecode_Base16(lpBufferOut, dwOutputSize, lpOutputWrittenOut, lpEncData, dwEncLength, encoding, dwFlags);

		default:
			if(lpOutputWrittenOut != NULL) { (*lpOutputWrittenOut) = 0; }
			return baseencodeE_InvalidParam;
	}
}

#ifdef __cplusplus
	} /* extern "C" { */
#endif

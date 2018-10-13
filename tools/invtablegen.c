/*
	Inverse mapping table generator. This is used to generate the
	lookup tables used in src/base.c
*/

#include <stdio.h>
#include <stdlib.h>

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

int main(int argc, char* argv[]) {
	unsigned long int i,j;

	printf("static unsigned char baseencAlphabetBase64Inv[256] = {\n\t");
	for(i = 0; i < 256; i=i+1) {
		for(j = 0; j < sizeof(baseencAlphabetBase64)/sizeof(unsigned char); j=j+1) {
			if(baseencAlphabetBase64[j] == i) {
				if(i != 255) {
					printf("%3lu,", j);
				} else {
					printf("%3lu", j);
				}
				break;
			}
		}
		if(j == sizeof(baseencAlphabetBase64)/sizeof(unsigned char)) {
			if(i != 255) {
				printf("%3u,", 255);
			} else {
				printf("%3u", 255);
			}
		}
		if(i % 10 == 9) {
			printf("\n");
			if(i != 255) { printf("\t"); }
		}
	}
	printf("\n};\n\n");

	printf("static unsigned char baseencAlphabetBase64UrlInv[256] = {\n\t");
	for(i = 0; i < 256; i=i+1) {
		for(j = 0; j < sizeof(baseencAlphabetBase64Url)/sizeof(unsigned char); j=j+1) {
			if(baseencAlphabetBase64Url[j] == i) {
				if(i != 255) {
					printf("%3lu,", j);
				} else {
					printf("%3lu", j);
				}
				break;
			}
		}
		if(j == sizeof(baseencAlphabetBase64Url)/sizeof(unsigned char)) {
			if(i != 255) {
				printf("%3u,", 255);
			} else {
				printf("%3u", 255);
			}
		}
		if(i % 10 == 9) {
			printf("\n");
			if(i != 255) { printf("\t"); }
		}
	}
	printf("\n};\n\n");

	printf("static unsigned char baseencAlphabetBase32Inv[256] = {\n\t");
	for(i = 0; i < 256; i=i+1) {
		for(j = 0; j < sizeof(baseencAlphabetBase32)/sizeof(unsigned char); j=j+1) {
			if(baseencAlphabetBase32[j] == i) {
				if(i != 255) {
					printf("%3lu,", j);
				} else {
					printf("%3lu", j);
				}
				break;
			}
		}
		if(j == sizeof(baseencAlphabetBase32)/sizeof(unsigned char)) {
			if(i != 255) {
				printf("%3u,", 255);
			} else {
				printf("%3u", 255);
			}
		}
		if(i % 10 == 9) {
			printf("\n");
			if(i != 255) { printf("\t"); }
		}
	}
	printf("\n};\n\n");

	printf("static unsigned char baseencAlphabetBase32HexInv[256] = {\n\t");
	for(i = 0; i < 256; i=i+1) {
		for(j = 0; j < sizeof(baseencAlphabetBase32Hex)/sizeof(unsigned char); j=j+1) {
			if(baseencAlphabetBase32Hex[j] == i) {
				if(i != 255) {
					printf("%3lu,", j);
				} else {
					printf("%3lu", j);
				}
				break;
			}
		}
		if(j == sizeof(baseencAlphabetBase32Hex)/sizeof(unsigned char)) {
			if(i != 255) {
				printf("%3u,", 255);
			} else {
				printf("%3u", 255);
			}
		}
		if(i % 10 == 9) {
			printf("\n");
			if(i != 255) { printf("\t"); }
		}
	}
	printf("\n};\n\n");

	printf("static unsigned char baseencAlphabetBase16Inv[256] = {\n\t");
	for(i = 0; i < 256; i=i+1) {
		for(j = 0; j < sizeof(baseencAlphabetBase16)/sizeof(unsigned char); j=j+1) {
			if(baseencAlphabetBase16[j] == i) {
				if(i != 255) {
					printf("%3lu,", j);
				} else {
					printf("%3lu", j);
				}
				break;
			}
		}
		if(j == sizeof(baseencAlphabetBase16)/sizeof(unsigned char)) {
			if(i != 255) {
				printf("%3u,", 255);
			} else {
				printf("%3u", 255);
			}
		}
		if(i % 10 == 9) {
			printf("\n");
			if(i != 255) { printf("\t"); }
		}
	}
	printf("\n};\n\n");

	return 0;
}

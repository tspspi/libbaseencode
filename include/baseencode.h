#ifndef __is_included__79e26415_71d6_4f38_9218_db7ac560f2de
#define __is_included__79e26415_71d6_4f38_9218_db7ac560f2de 1

#include <stdint.h>

#ifdef __cplusplus
	extern "C" {
#endif

/*
	Flags that modify behaviour of encoder and decoder:

	BASEENCODE_FLAG_ACCEPTNOALPHABET
		If set the decoder simply skips characters that are
		not contained in the specific alphabet. This can be useful
		if an protocol embeds linebreaks or similar in it's
		encoded data.
	BASEENCODE_FLAG_ACCEPTINVALIDPAD
		Accepts invalid padding (if padding bits are not zero
		for example). Should not be set for cryptographic applications
		where alteration is critical.
*/
#define BASEENCODE_FLAG__ACCEPTNOALPHABET		0x00000001
#define BASEENCODE_FLAG__ACCEPTINVALIDPAD		0x00000002

enum baseencodeError {
	baseencodeE_Ok					= 0,
	baseencodeE_InvalidParam		= 1,
	baseencodeE_BufferTooSmall		= 2,
	baseencodeE_ImplementationError	= 3,
	baseencodeE_InvalidFormat		= 4,
};

enum baseencodeEnc {
	baseencodeEnc_Base64			= 0,
	baseencodeEnc_Base64Url			= 1,
	baseencodeEnc_Base32			= 2,
	baseencodeEnc_Base32Hex			= 3,
	baseencodeEnc_Base16			= 4,
};

/*
	Supported encodings:
		base64
		base64url
		base32
		base32hex
		base16
*/

/*
	Get encoded length for given data input (this allows allocation of
	appropriate sized buffers)
*/
unsigned long int baseencGetEncodedLength(
	enum baseencodeEnc encoding,
	unsigned long int dwRawInputLength
);

/*
	Get maximum decoded length for given encoded input (this allows
	allocation of appropriatly sized buffers). Note that this can
	of course also be used if additional characters have been included
	in the input - the result will always fit this buffer.
*/
unsigned long int baseencGetDecodedMaxLength(
	enum baseencodeEnc encoding,
	unsigned long int dwEncLen
);

/*
	Encoder routines
*/

enum baseencodeError baseencEncode(
	unsigned char* lpBufferOut,
	unsigned long int dwOutputSize,

	const unsigned char* lpData,
	unsigned long int dwRawLength,

	enum baseencodeEnc encoding,
	uint32_t dwFlags
);

/*
	Decoding routines
*/

enum baseencodeError baseencDecode(
	unsigned char* lpBufferOut,
	unsigned long int dwOutputSize,
	unsigned long int* lpOutputWrittenOut,

	const unsigned char* lpData,
	unsigned long int dwEncLength,

	enum baseencodeEnc encoding,
	uint32_t dwFlags
);

#ifdef __cplusplus
	} /* extern "C" { */
#endif

#endif /* #ifndef __is_included__79e26415_71d6_4f38_9218_db7ac560f2de */

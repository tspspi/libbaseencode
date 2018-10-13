# Base64, Base32 and Base16 Encode/Decode "Library"

This is a small ANSI C99 library that provides basic encoding and decoding
routines for the Base16, Base32 and Base64 data encodings as
specified in RFC4648

## Usage

Basically the library provides four different functions to:

- Determine the size of an output buffer when encoding data
  according to one of the different base-N encodings.
- Determine the maximum buffer size required by raw data
  when decoding base-N encoded data. The maximum size is
  used because the decoder may be enabled to skip non-alphabet
  characters like linebreaks and there currently is no
  elimination of padding bytes during this calculation
- Encoding data
- Decoding data

The application is entirely responsible for buffer allocation.

The encoding supported are:

- Base64
- Base64Url (Base64 with Url safe alphabet from RFC4648)
- Base32
- Base32Hex (in contrast to Base32 this encoding preserves sort order)
- Base16 (Encoding with case insensitive hexadecimal symbols)

This examples use Base64 but can easily be adapted to other
encodings by simply replacing the encoding constant:

### Encoding directly in memory

First one has to determine the required buffer size for encoded
data. This is always exact:

```
outputBufferSize = baseencGetEncodedLength(baseencodeEnc_Base64, dwInputByteLength);
```

Then the data can be encoded

```
unsigned char* lpBuffer;
unsigned char* lpInputBytes;

baseencEncode(
	lpBuffer,
	sizeof(outputBuffer),
	lpInputBytes,
	strlen(lpInputBytes),
	baseencodeEnc_Base64,
	0
);
```

The return code should always be checked to determine any encoding or
parameter errors. If the result is _baseencodeE_Ok_ the encode was
successful.

### Decoding directly in memory

First one has to determine the maximum required buffer size for
encoded data. Note that the effective data size might be smaller
in case there are padding bytes or non-alphabet characters embedded
into the data stream.

```
decodedBufferSize = baseencGetDecodedMaxLength(baseencodeEnc_Base64, inputBufferSize);
```

Then the data can be decoded either with allowed extra bytes like linebreaks:

```
unsigned char* lpDecodedBuffer;
unsigned long int dwOutWritten;
unsigned char* lpInputBytes;

baseencDecode(
	lpDecodedBuffer,
	sizeof(outputBuffer),
	&dwOutWritten,
	lpInputBytes,
	strlen(lpInputBytes),
	baseencodeEnc_Base64,
	BASEENCODE_FLAG__ACCEPTNOALPHABET
);
```

or without allowed extra bytes (i.e. any character not included in
the base-N alphabet will trigger an encoding error):

```
unsigned char* lpDecodedBuffer;
unsigned long int dwOutWritten;
unsigned char* lpInputBytes;

baseencDecode(
	lpDecodedBuffer,
	sizeof(outputBuffer),
	&dwOutWritten,
	lpInputBytes,
	strlen(lpInputBytes),
	baseencodeEnc_Base64,
	0
);
```

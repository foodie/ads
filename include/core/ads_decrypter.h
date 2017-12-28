#ifndef _ADS_DECRYPTER_H
#define _ADS_DECRYPTER_H

#include <endian.h>
#include <netinet/in.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <string>
#include <string.h>
#include <sys/time.h>

typedef int                 int32;
typedef long long           int64;
typedef unsigned int        uint32;
typedef unsigned long long  uint64;
typedef unsigned char       uchar;

int google_decrypt_winning_price(const char *encodedValue, const char *kEncryptionKey, const char *kIntegrityKey);

#endif

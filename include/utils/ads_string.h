#ifndef _ADS_STRING_H
#define _ADS_STRING_H

#include <string.h>
#include <string>
#include <vector>

using std::string;
using std::vector;

#define ads_tolower(c) (char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#define ads_toupper(c) (char) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)

#define ads_strcpy(dst, src)		strcpy(dst, src);dst[strlen(dst)]=0
#define ads_strncpy(dst, src, n)	strncpy(dst, src, n);dst[n]=0

#define ads_stricmp(s1, s2)			strcasecmp((const char *) s1, (const char *) s2)
#define ads_strnicmp(s1, s2, n)		strncasecmp((const char *) s1, (const char *) s2, n)

#define ads_memzero(buf, n)   		(void) memset(buf, 0, n)

/**
 * 字符串转储
 */
char *ads_str_dump(const char *str);
inline void ads_str_free(char *str)
{
	free(str);
}

/**
 * 在源字符串之后追加字符串
 */
size_t ads_str_append(char *dst, size_t maxsize, const char *src);
size_t ads_str_append_secure(char *dst, size_t maxsize, const char *src);
size_t ads_str_append_snprintf(char *dst, size_t offset, size_t maxsize, const char* fmt, ...);

/**
 * 字符串大小写转换
 */
void ads_str_toupper(char *src, char *dst=NULL, size_t n=-1);
void ads_str_tolower(char *src, char *dst=NULL, size_t n=-1);

void ads_string_toupper(string &str, size_t n=-1);
void ads_string_tolower(string &str, size_t n=-1);

string ads_string_toupper(const string& str, size_t n=-1);
string ads_string_tolower(const string& str, size_t n=-1);

/**
 *  @brief      去除字符串首尾处的空白字符（或者其他字符）
 *  
 *  " "  	(ASCII 32 (0x20))，普通空格符。  
 *  "\t" 	(ASCII 9  (0x09))，制表符。  
 *  "\n" 	(ASCII 10 (0x0A))，换行符。  
 *  "\r" 	(ASCII 13 (0x0D))，回车符。  
 *  "\0" 	(ASCII 0  (0x00))，空字节符。  
 *  "\x0B" 	(ASCII 11 (0x0B))，垂直制表符
 */
void ads_str_ltrim(char *src, char *dst, const char *charlist=" \t\n\r\0\x0B");
void ads_str_rtrim(char *src, char *dst, const char *charlist=" \t\n\r\0\x0B");
void ads_str_trim(char *src, char *dst, const char *charlist=" \t\n\r\0\x0B");

void ads_string_ltrim(string &str, const char *charlist=" \t\n\r\0\x0B");
void ads_string_rtrim(string &str, const char *charlist=" \t\n\r\0\x0B");
void ads_string_trim(string &str, const char *charlist=" \t\n\r\0\x0B");

/**
 * 字符串替换
 */
void   ads_str_replace(const char *src, char *dst, 
	const char *search, const char *replace, size_t limit=-1);
string ads_string_replace(const string &src, const char *search, 
	const char *replace, size_t limit=-1);

/**
 * 字符串分割
 */
void ads_string_split(const string &src, const string &sep, 
	vector<string> &dst, size_t limit=-1);

/**
 * 进制转换
 */
void 	ads_bin_to_hex(const char *bin, size_t n, char *hex, int upper=0);
size_t 	ads_hex_to_bin(const char* hex, char* bin, size_t n=0);

//int 	ads_hex_to_int(const char* hex);

/**
 * url编码
 */
void ads_url_encode(const char *src, char *dst, int upper=1);
void ads_url_decode(const char *src, char *dst);

#endif
/* vim: set ts=4 sw=4 noet: */

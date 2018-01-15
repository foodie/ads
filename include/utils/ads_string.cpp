#include "utils/ads_string.h"

#include "stdarg.h"

/**
 * 字符串转储
 */
char *ads_str_dump(const char *str)
{
	size_t size = strlen(str) + 1;
	if (size == 1) {
		return NULL;
	}
	char *dump = (char*) malloc(size);
	memcpy(dump, str, size);
	return dump;
}

/**
 * 字符串追加
 */
size_t ads_str_append(char *dst, size_t maxsize, const char *src) {
	size_t offset = strlen(dst);
	return snprintf(dst + offset, offset < maxsize ? maxsize - offset : 0, "%s", src);
}
size_t ads_str_append_secure(char *dst, size_t maxsize, const char *src) {
	size_t offset = strlen(dst);
	size_t need = strlen(src);
	if (offset + need > maxsize) {
		return 0;
	}
	return snprintf(dst + offset, maxsize, "%s", src);
}
size_t ads_str_append_snprintf(char *dst, size_t offset, size_t maxsize, const char* fmt, ...)
{
	size_t rest = offset < maxsize ? maxsize - offset : 0; // 剩余可写入数据长度

	va_list args;
	va_start(args, fmt);
	offset += vsnprintf(dst + offset, rest, fmt, args);
	va_end(args);

	return offset;
}

/**
 * 字符串大小写转换
 */
void ads_str_toupper(char *src, char *dst, size_t n)
{
	dst = dst != NULL ? dst : src;
	while ( *src != '\0' && n ) {
		*dst = ads_toupper(*src);
		dst++;
		src++;
		n--;
	}
}
void ads_str_tolower(char *src, char *dst, size_t n)
{
	dst = dst != NULL ? dst : src;
	while ( *src != '\0' && n ) {
		*dst = ads_tolower(*src);
		dst++;
		src++;
		n--;
	}
}

void ads_string_toupper(string &str, size_t n)
{
	n = n < str.size() ? n : str.size();
	for (size_t i = 0; i < n; i++) {
		str[i] = ads_toupper(str[i]);
	}
}
void ads_string_tolower(string &str, size_t n)
{
	n = n < str.size() ? n : str.size();
	for (size_t i = 0; i < n; i++) {
		str[i] = ads_tolower(str[i]);
	}
}

string ads_string_toupper(const string& str, size_t n)
{
	string res(str);
	ads_string_toupper(res, n);
	return res;
}
string ads_string_tolower(const string& str, size_t n)
{
	string res(str);
	ads_string_tolower(res, n);
	return res;
}

/**
 * 去除字符串首尾处的空白字符（或者其他字符）
 */
void ads_str_ltrim(char *src, char *dst, const char *charlist)
{
	dst = dst != NULL ? dst : src;
	while ( *src && strchr(charlist, *src) != NULL ) {
		src++;
	}
	while (*src) {
		*dst = *src;
		src++;
		dst++;
	}
	*dst = 0;
}

void ads_str_rtrim(char *src, char *dst, const char *charlist)
{
	dst = dst != NULL ? dst : src;
	int i = strlen(src);
	while ( i && strchr(charlist, src[i-1]) != NULL ) {
		i--;
	}
	while (i) {
		*dst = *src;
		src++;
		dst++;
		i--;
	}
	*dst = 0;
}

void ads_str_trim(char *src, char *dst, const char *charlist)
{
	ads_str_ltrim(src, dst, charlist);
	ads_str_rtrim(dst, NULL, charlist);
}

void ads_string_ltrim(string &str, const char *charlist)
{
	if (!str.empty()) {
		str.erase(0, str.find_first_not_of(charlist));
	}
}

void ads_string_rtrim(string &str, const char *charlist)
{
	if (!str.empty()) {
		str.erase(str.find_last_not_of(charlist) + 1);
	}
}

void ads_string_trim(string &str, const char *charlist)
{
	if (!str.empty()) {
		str.erase(0, str.find_first_not_of(charlist));  
		str.erase(str.find_last_not_of(charlist) + 1);
	}
}

/**
 * 字符串替换
 */
void ads_str_replace(const char *src, char *dst, 
	const char *search, const char *replace, size_t limit)
{
	const char *p1, *p2;
	p1 = src;
	while (limit--) {
		p2 = (const char*) strstr(p1, search);
		if (p2 == NULL) {
			break;
		}
		strncpy(dst, p1, p2 - p1);
		dst += p2 - p1;
		strcpy(dst, replace);
		dst += strlen(replace);
		p1 = p2 + strlen(search);
	}
	if (*p1 != '\0') {
		strcpy(dst, p1);
	}
}

string ads_string_replace(const std::string &src, const char *search, 
	const char *replace, size_t limit)
{	
	string dst;
	string::size_type pos1=0, pos2;

	while(limit--) {
		pos2 = src.find(search, pos1);
		if (std::string::npos == pos2) {
			break;
		}
		dst.append(src.data() + pos1, pos2 - pos1);
		dst.append(replace);
		pos1 = pos2 + strlen(search);
	}
	dst.append(src.begin() + pos1, src.end());
	
	return dst;
}

/**
 * 字符串分割
 */
void ads_string_split(const string &src, const string &sep, 
	vector<string> &dst, size_t limit)
{

}

/**
 * 进制转换
 */
void ads_bin_to_hex(const char *bin, size_t n, char *hex, int upper)
{
	unsigned char t,c;
	for (size_t i = 0; i < n; i++) {
		t = bin[i];
		for (size_t j = 0; j < 2; j++) {
			c = (t & 0x0f);
			if (c < 10) {
				c += '0';
			} else {
				c += ((upper?'A':'a') - 10);
			}
			hex[2 * i + 1 - j] = c;
			t >>= 4;
		}
	}
	hex[2*n] = '\0';
}

size_t ads_hex_to_bin(const char* hex, char* bin, size_t n)
{
	n -= (n % 2);

	unsigned char t,c;
	size_t i;
	for (i = 0; i < n/2; i++) {
		t = 0;
		for (size_t j = 0; j < 2; j++) {
			c = hex[2 * i + j];
			if (c >= '0' && c <= '9') {
				t = (t << 4) + (c - '0');
			} else if (c >= 'a' && c <= 'f') {
				t = (t << 4) + (c - 'a' + 10);
			} else if (c >= 'A' && c <= 'F') {
				t = (t << 4) + (c - 'A' + 10);
			} else {
				return i;
			}
		}
		bin[i] = t;
	}
	return i;
}

/**
 * url编码
 */
void ads_url_encode(const char *src, char *dst, int upper)
{
	size_t len = (src == NULL ? 0 : strlen(src));
	if (0 == len) {
		*dst = 0;
		return;
	}

	size_t i, n;
    unsigned char c;
	for (i = 0, n = 0; i < len; i++) {
		c = src[i];
		if (c >= 'A' && c <= 'Z') {
			dst[n++] = c;
		} else if (c >= 'a' && c <= 'z') {
			dst[n++] = c;
		} else if (c >= '0' && c <= '9') {
			dst[n++] = c;
		} else if (c == '-' || c == '_' || c == '.' || c == '~') {
			dst[n++] = c;
		} else {
			if (upper) {
				n += snprintf(dst + n, 4, "%%%02X", c);
			} else {
				n += snprintf(dst + n, 4, "%%%02x", c);
			}
		}
	}
	dst[n] = '\0';
}

void ads_url_decode(const char *src, char *dst)
{
	size_t len = (src == NULL ? 0 : strlen(src));
	if (0 == len) {
		*dst = 0;
		return;
	}

	size_t i, n;
    unsigned char c;
	for (i = 0, n = 0; i < len; i++, n++) {
		c = src[i];
        if ('%' == c) {
			if(i + 2 >= len) {
				break;
            }
            c = src[i + 1];
            if('A' <= c && 'F' >= c) {
                dst[n] = (c - 'A' + 0x0A) * 0x10;
            } else if('a' <= c && 'f' >= c) {
                dst[n] = (c - 'a' + 0x0A) * 0x10;
            } else {
				dst[n] = (c - '0') * 0x10;
            }  
  
            c = src[i + 2];  
            if('A' <= c && 'F' >= c) {
                dst[n] += (c - 'A' + 0x0A);
            } else if('a' <= c && 'f' >= c) {
                dst[n] += (c - 'a' + 0x0A);
            } else {
                dst[n] += (c - '0');
            }
            i += 2;
        } else if('+' == c) {
            dst[n] = ' ';
        } else {
            dst[n] = c;
        }
    }
	dst[n] = '\0';
}

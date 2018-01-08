#include "utils/ads_xml.h"

int ads_xml_compress(const char *in, char *out, int maxsize)
{
	int len = 0;
	char c, p=0;
	maxsize--;
	while( (c = *in++) != 0) {
		if (c == '\n')
			continue;
		if (c == ' ' && p == '>')
			continue;
		out[len++] = c;
		p = c;

		if (len >= maxsize)
			break;
	}
	out[len] = '\0';
	return len;
}

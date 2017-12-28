#ifndef __CONV_H__
#define __CONV_H__

#include <iconv.h>

#ifndef u_char
#define u_char		unsigned char
#endif

// This init function should be called in main thread.
// Return : 0, success; -1, failed
int jcode_transfer_init();
unsigned short get_sjis_fa2gb(unsigned short index);

// in this module , gb means GB18030
typedef struct _jp2gb_t
{
	iconv_t unicode2gb;
	iconv_t utf82gb;
	iconv_t jis2gb;
	iconv_t sjis2gb;
	iconv_t eucjp2gb;
} jp2gb_t;

typedef struct _tr2gb_t
{
    iconv_t iso12gb; //iso-8859-1 to gb18030
    //iconv_t iso885992gb;
    iconv_t win12542gb;
    //iconv_t ibm8572gb;
    //iconv_t macturkish2gb;
} tr2gb_t;

// These function should be called in every sub thread.
// Return : 0, success; -1, failed
int jp2gb_open(jp2gb_t *pConv);
int jp2gb_close(jp2gb_t *pConv);

int tr2gb_open(tr2gb_t *pConv);
int tr2gb_close(tr2gb_t *pConv);

typedef struct _gb2jp_t
{
	iconv_t gb2unicode;
	iconv_t gb2utf8;
	iconv_t gb2jis;
	iconv_t gb2sjis;
	iconv_t gb2eucjp;
} gb2jp_t;

typedef struct _gb2tr_t
{
    iconv_t gb2iso1; //gb18030 to iso-8859-1
    //iconv_t gb2iso88599;
    iconv_t gb2win1254;
    //iconv_t gb2ibm857;
    //iconv_t gb2macturkish;
} gb2tr_t;

// These function should be called in every sub thread.
// Return : 0, success; -1, failed
int gb2jp_open(gb2jp_t *pConv);
int gb2jp_close(gb2jp_t *pConv);

int gb2tr_open(gb2tr_t *pConv);
int gb2tr_close(gb2tr_t *pConv);

// transfer functions
// Input : psrc, end with '\0'
// 	 : pdes, dest buffer
// 	 : destlen, length of dest buffer, at least >= strlen(psrc)+1, 
// 	 	    2*strlen(psrc) may be safe enough as GB18030 code
//
// Output: pdes, end with '\0'
// 	 	 
// Return : -1, failed; >=0, success, the size of un-converted bytes

int jis_gb_str(char *psrc, char *pdes, int destlen, jp2gb_t *pConv);
int jis_gb_str(char *psrc, char *pdes, int destlen);

int sjis_gb_str(char *psrc, char *pdes, int destlen, jp2gb_t *pConv);
int sjis_gb_str(char *psrc, char *pdes, int destlen);

int eucjp_gb_str(char *psrc, char *pdes, int destlen, jp2gb_t *pConv);
int eucjp_gb_str(char *psrc, char *pdes, int destlen);

int utf8_gb_str(char *psrc, char *pdes, int destlen, jp2gb_t *pConv);
int utf8_gb_str(char *psrc, char *pdes, int destlen);


// the following are functions of gb str transfer to jp encoded str
// Input : destlen , length of dest buffer, at least >= 1.5 * strlen(psrc) + 1

int gb_jis_str(char *psrc, char *pdes, int destlen, gb2jp_t *pConv);
int gb_jis_str(char *psrc, char *pdes, int destlen);

int gb_sjis_str(char *psrc, char *pdes, int destlen, gb2jp_t *pConv);
int gb_sjis_str(char *psrc, char *pdes, int destlen);

int gb_eucjp_str(char *psrc, char *pdes, int destlen, gb2jp_t *pConv);
int gb_eucjp_str(char *psrc, char *pdes, int destlen);

int gb_utf8_str(char *psrc, char *pdes, int destlen, gb2jp_t *pConv);
int gb_utf8_str(char *psrc, char *pdes, int destlen);


/* 将 gb18030 编码 中的 半角假名 转换到全角假名 
 * pdes and  psrc can same memory 
 * destlen >= strlen(psrc) + 1 ;
 *
 * return : 转换后pdes 里面的字符个数 , not include '\0'
 * 	    failed return -1  if parameter error
 **/
int gb_jm_bj2qj(char * psrc , char *pdes , unsigned int destlen) ;

/*是否是gb18030 1字节*/
inline int is_gb_byte1(u_char *pstr)
{
	return ( pstr[0]<=0x7F ) ;
}

/*是否是gb18030 2字节*/
inline int is_gb_byte2(u_char *pstr)
{
	return ( pstr[0] >= 0x81 && pstr[0] <= 0xFE  && pstr[1] >= 0x40 && pstr[1] <= 0xFE && pstr[1] != 0x7F ) ;
}

/*是否是gb18030 4字节*/
inline int is_gb_byte4(u_char *pstr)
{
	return (   pstr[0] >= 0x81 && pstr[0] <= 0xFE
	       	&& pstr[1] >= 0x30 && pstr[1] <= 0x39
	        && pstr[2] >= 0x81 && pstr[2] <= 0xFE
	        && pstr[3] >= 0x30 && pstr[3] <= 0x39 ) ;
}
/** 
 * @brief 基于iconv_t的编码转换结构体，通过coding_conv_open和coding_conv_close打开和关闭，非线程安全，各子线程需维护单独的结构
 */
typedef struct _coding_conv_t
{
	jp2gb_t jp2gb;
	gb2jp_t gb2jp;

    tr2gb_t tr2gb;
    gb2tr_t gb2tr;
} coding_conv_t;


/**
 * @brief 根据编码类型返回 编码类型字符串
 * @param codetypestr 编码类型字符串
 * @return 识别成功返回-1  到11，出错返回-2
 */

int getcodetype(const char* codetypestr);


/**
 * @brief 根据编码类型返回 编码类型字符串
 * @param codetypestr 编码类型
 * @return 识别成功返回编码类型字符串  出错返回NULL
 */
const char* getcodetype(int codetype);


/**
 * @brief 根据编码类型返回 编码类型字符串,特别需要注意的是psrc，slen 和pdes，dlen 必须满足对应编码srcoding到descode转换函数的约束。
 * @param srcoding   源 编码类型
 * @param descode  目的 编码类型
 * @param psrc        待转换源字符串
 * @param pdes       转换后的缓冲区
 * @param slen        源字符串长度只有当iconv 调用gb18030_big5_str  big5_gb18030_str 才有用
 * @param dlen       目的字符串长度 
   @param conv      conv 编码转化数据结构 ，可以是NULL              
 */
int iconv(int srcoding, int descode, u_char *psrc, u_char *pdes, int slen, int dlen, coding_conv_t *conv=NULL);

/**
 * @brief 将给定编码类型的字符串转换为gb18030，支持以下编码：UTF8,BIG5,JIS,SJIS,EUCJP,ISO-8859-1,ISO-8859-9,Windows-1254
 *
 * @param [in] input   : char* 输入字符串
 * @param [out] output   : char* 输出字符串
 * @param [in] outlen   : int 输出buffer长度, 输入串为ISO-8859-1,ISO-8859-9,WIN1254时此长度最多需为输入的4倍
 * @param [in] codetype   : int 输入编码类型
 * @return  int 
 * @retval   -1:参数异常、转换失败；0：转换成功
 * @see 
 * @note 
**/
int coding_to_gb18030(char *psrc, char *pdes, int destlen, int codetype);

/**
 * @brief 将gb18030编码字符串转换为给定编码类型的字符串，支持以下编码：UTF8,BIG5,JIS,SJIS,EUCJP,ISO-8859-1,ISO-8859-9,Windows-1254
 *
 * @param [in] input   : char* 输入字符串
 * @param [out] output   : char* 输出字符串
 * @param [in] outlen   : int 输出buffer长度
 * @param [in] codetype   : int 输入编码类型
 * @return  int 
 * @retval   -1:参数异常、转换失败；0：转换成功 
 * @see 
 * @note 
**/
int coding_from_gb18030(char *psrc, char *pdes, int destlen, int codetype);

#endif



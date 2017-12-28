#ifndef __CODING_H__
#define __CODING_H__

#include "conv.h"
#include "strings.h"

#define	TRBUF_SIZE	512

#define BIG5_NUM	0x3695	//13973

#define BIG5_1LO	0xa1	//161
#define	BIG5_1HI	0xf9	//249
#define BIG5_1NUM	0x59	//89
#define	BIG5_2LO1	0x40	//64
#define	BIG5_2HI1	0x7e	//126
#define	BIG5_2NUM1	0x3f	//63
#define	BIG5_2LO2	0xa1	//161
#define	BIG5_2HI2	0xfe	//254
#define	BIG5_2NUM2	0x5e	//94
#define	BIG5_2NUM	0x9d	//157

#define	GBK_NUM		0x5e02	//24066

#define GBK_1LO		0x81	//129
#define GBK_1HI		0xfe	//254
#define	GBK_1NUM	0x7e	//126
#define	GBK_2LO		0x40	//64
#define	GBK_2HI		0xfe	//254
#define	GBK_2NUM	0xbf	//191
#define GBK_2INV	0x7f	//127

/*==== for chinese */
#define CHN_CHAR_NUM	6768
#define SYM_HIMIN		0xA1	/**< GB symbol : minmum value of first byte */
#define SYM_HIMAX		0xF7	/**< GB symbol : maxmum value of first byte */
#define SYM_LOMIN		0xA1	/**< GB symbol : minmum value of second byte */
#define SYM_LOMAX		0xFE	/**< GB symbol : maxmum value of second byte */
#define CHAR_HIMIN		0xB0	/**< GB char   : min - 1st byte */
#define CHAR_HIMAX		0xF7	/**< GB char   : max - 1st byte */
#define CHAR_LOMIN		0xA1	/**< GB char   : min - 2nd byte */
#define CHAR_LOMAX		0xFE	/**< GB char   : max - 2nd byte */
#define CHAR_HINUM      0x48	/**< (0xF7-0xB0+1) */
#define CHAR_LONUM      0x5E	/**< (0xFE-0xA1+1) */
#define MARK_HIMIN		0xA1	/**< GB mark   : min - 1st byte */
#define MARK_HIMAX		0xAF	/**< GB mark   : max - 1st byte */
#define MARK_LOMIN		0xA1	/**< GB mark   : min - 2nd byte */
#define MARK_LOMAX		0xFE	/**< GB mark   : max - 2nd byte */
#define SMARK_HIMIN     0xA3    /**< simple GB mark   : min - 1st byte */
#define SMARK_HIMAX     0xA3    /**< simple GB mark   : max - 1st byte */
#define SMARK_LOMIN     0xA1    /**< simple GB mark   : min - 2nd byte */
#define SMARK_LOMAX     0xFE    /**< simple GB mark   : max - 2nd byte */

#define DIGIT_HIMIN     0xa3    /**< GB 0---9 */
#define DIGIT_HIMAX     0xa3
#define DIGIT_LOMIN     0xb0
#define DIGIT_LOMAX     0xb9
 
#define HI_HIMIN        0xa3    /**< GB A---Z */   
#define HI_HIMAX        0xa3
#define HI_LOMIN        0xc1
#define HI_LOMAX        0xda
                        
#define LOW_HIMIN       0xa3    /**< GB a---z */ 
#define LOW_HIMAX       0xa3
#define LOW_LOMIN       0xe1
#define LOW_LOMAX       0xfa

#define GBK_HIMIN       0x81
#define GBK_HIMAX       0xfe
#define GBK_LOMIN       0x40
#define GBK_LOMAX       0xfe

/* define the range of gb */
#define IN_RANGE(ch, min, max) ( (((unsigned char)(ch))>=(min)) && (((unsigned char)(ch))<=(max)) )
#define IS_GB(cst)	( IN_RANGE((cst)[0], SYM_HIMIN, SYM_HIMAX) && IN_RANGE((cst)[1], SYM_LOMIN, SYM_LOMAX) )
#define IS_GBK(cst)      ( IN_RANGE((cst)[0], GBK_HIMIN, GBK_HIMAX) && IN_RANGE((cst)[1], GBK_LOMIN, GBK_LOMAX) )
#define IS_GB_CODE(cst)	( IN_RANGE((cst)[0], CHAR_HIMIN, CHAR_HIMAX) && IN_RANGE((cst)[1], CHAR_LOMIN, CHAR_LOMAX) )
#define IS_GB_MARK(cst)	( IN_RANGE((cst)[0], MARK_HIMIN, MARK_HIMAX) && IN_RANGE((cst)[1], MARK_LOMIN, MARK_LOMAX) )
#define IS_GB_SMAEK(cst) ( IN_RANGE((cst)[0], SMARK_HIMIN, SMARK_HIMAX) && IN_RANGE((cst)[1], SMARK_LOMIN, SMARK_LOMAX) )
#define IS_GB_SPACE(cst) ( (((unsigned char)((cst)[0]))==SYM_HIMIN) && (((unsigned char)((cst)[1]))==SYM_LOMIN) )
#define IS_GB_NOBREAK(cst) ( (unsigned char)(cst)[0]==0xa3 && ((unsigned char)(cst)[1]==0xa6 ||(unsigned char)(cst)[1]==0xad ||(unsigned char)(cst)[1]==0xdf) )
#define IS_GB_1234(cst) (IN_RANGE((cst)[0],DIGIT_HIMIN,DIGIT_HIMAX) && IN_RANGE((cst)[1],DIGIT_LOMIN,DIGIT_LOMAX) )
#define IS_GB_ABCD(cst) ( (IN_RANGE((cst)[0],HI_HIMIN,HI_HIMAX) && IN_RANGE((cst)[1],HI_LOMIN,HI_LOMAX) ) || (IN_RANGE((cst)[0],LOW_HIMIN,LOW_HIMAX) && IN_RANGE((cst)[1],LOW_LOMIN,LOW_LOMAX) ) ) 

//Other gb characters can not displayed properly:
//A9: F5--F6
//A8: EA--F6
//    96--A0
//A7: F2--F6
//    C2--D0
//A6: B9--C0
//    D9--DF
//    F6--FE
//A4: F3--F7
//A2: FE
//A2: FD--FE
#define IS_GB_UNDEFINED(cst) ( (IN_RANGE((cst)[0], 0xaa, 0xaf) && IN_RANGE((cst)[1], 0xa1, 0xfe) ) || (IN_RANGE((cst)[0], 0xf8, 0xfe) && IN_RANGE((cst)[1], 0xa1, 0xfe) ) || (IN_RANGE((cst)[0], 0xa1, 0xa7) && IN_RANGE((cst)[1], 0x40, 0xa0) ) || ( IN_RANGE((cst)[0], 0xa4, 0xa9) && IN_RANGE((cst)[1], 0xf7, 0xfe)) ) 


//LANGTYPE_NULL为 新旧coding的标识，当LANGTYPE被置为此值时说明是旧coding的识别结果
//LANGTYPE_GLOBAL 表示支持所有语言
//LANGTYOE_END 表示当前语言种类的数目

typedef enum _LANG_T {
    LANGTYPE_NULL = -3,//新旧coding的标识，当LANGTYPE被置为此值时说明是旧coding的识别结果
    LANGTYPE_NONE = -1,
    LANGTYPE_ENGLISH = 0,
    LANGTYPE_SIMP_CHINESE = 1,
    LANGTYPE_TRAD_CHINESE = 2,
    LANGTYPE_JAPANESE = 3,
    LANGTYPE_KOREAN = 4,
    LANGTYPE_TURKISH = 5,
    LANGTYPE_THAI = 6,
    LANGTYPE_VIETNAMESE = 7,
    LANGTYPE_MALAY = 8,
    LANGTYPE_GERMAN = 9,
    LANGTYPE_FRENCH = 10,
    LANGTYPE_RUSSIAN = 11,
    LANGTYPE_IRANIAN = 12,
    LANGTYPE_ARABIC = 13,
    LANGTYPE_ESTONIAN = 14,
    LANGTYPE_BELARUSIAN = 15,
    LANGTYPE_BULGARIAN = 16,
    LANGTYPE_HINDI = 17,
    LANGTYPE_ICELANDIC = 18,
    LANGTYPE_POLISH = 19,
    LANGTYPE_FARSI = 20,
    LANGTYPE_DANISH = 21,
    LANGTYPE_FILIPINO = 22,
    LANGTYPE_FINNISH = 23,
    LANGTYPE_DUTCH = 24,
    LANGTYPE_CATALAN = 25,
    LANGTYPE_CZECH = 26,
    LANGTYPE_CROATIAN = 27,
    LANGTYPE_LATVIAN = 28,
    LANGTYPE_LITHUANIAN = 29,
    LANGTYPE_ROMANIAN = 30,
    LANGTYPE_AFRIKAANS = 31,
    LANGTYPE_NORWEGIAN = 32,
    LANGTYPE_BRAZILIAN = 33,
    LANGTYPE_PORTUGUESE = 34,
    LANGTYPE_SWEDISH = 35,
    LANGTYPE_SERBIAN = 36,
    LANGTYPE_ESPERANTO = 37,
    LANGTYPE_SLOVAK = 38,
    LANGTYPE_SLOVENE = 39,
    LANGTYPE_SWAHILI = 40,
    LANGTYPE_UKRAINIAN = 41,
    LANGTYPE_SPANISH = 42,
    LANGTYPE_HEBREW = 43,
    LANGTYPE_GREEK = 44,
    LANGTYPE_HUNGARIAN = 45,
    LANGTYPE_ARMENIAN = 46,
    LANGTYPE_ITALIAN = 47,
    LANGTYPE_INDONESIAN = 48,
    LANGTYPE_ALBANIAN = 49,
    LANGTYPE_GLOBAL, // support all of the languages
    LANGTYPE_END     // current langtype num
} LANG_T;

//CODETYPE_END表示当前编码类型的数目
typedef enum _CODE_T {
    CODETYPE_UNKNOWN = -2,
    CODETYPE_NONE = -1,
    CODETYPE_ENG = 0, //mv to langtype
    CODETYPE_GB = 1,//means gb18030 now
    CODETYPE_BIG5 = 2,
    CODETYPE_UTF8 = 3,
    CODETYPE_JPKR = 4,//rm
    CODETYPE_JIS = 5,// added by jcode
    CODETYPE_SJIS = 6,// added by jcode
    CODETYPE_EUCJP = 7,// added by jcode
    CODETYPE_UTF8JP = 8,//rm
    CODETYPE_ASCII = 9,
    CODETYPE_ISO_8859_1 = 10,
    CODETYPE_EUCKR = 11,
    CODETYPE_UCS2 = 12,
    CODETYPE_GBK = 13,
    CODETYPE_ISO_8859_9 = 14,
    CODETYPE_WIN1254 = 15,
    CODETYPE_IBM857 = 16,
    CODETYPE_MACTURKISH = 17,
    CODETYPE_ISO_8859_3 = 18,
    CODETYPE_END // current codetype num
} CODE_T;

// 编码类型，用于表明某数据/策略支持的语种（语言属性）
// langmark 的每一位代表是否支持相应语种，最多64个语种
typedef struct _LangType {
    unsigned long long langmark;
} LangType;

#define PY_MAXPAGESIZE 512000
#ifndef u_char
#define u_char		unsigned char
#endif

//============== for coding_check coding_check_content exit type =================
//modified to 19 from 23, to avoid redefinition with jcode
#define	MAX_CHECK_EXIT		19
#define	CODING_CHECK_EXIT_1	1
#define	CODING_CHECK_EXIT_2	2
#define	CODING_CHECK_EXIT_3	3
#define	CODING_CHECK_EXIT_4	4
#define	CODING_CHECK_EXIT_5	5
#define	CODING_CHECK_EXIT_6	6
#define	CODING_CHECK_EXIT_7	7
#define	CODING_CHECK_EXIT_8	8
#define	CODING_CHECK_EXIT_9	9
#define	CODING_CHECK_EXIT_10	10
#define	CODING_CHECK_EXIT_11	11
#define	CODING_CHECK_EXIT_12	12
#define	CODING_CHECK_EXIT_13	13
#define	CODING_CHECK_EXIT_14	14
#define	CODING_CHECK_EXIT_15	15
#define	CODING_CHECK_EXIT_16	16
#define	CODING_CHECK_EXIT_17	17
#define	CODING_CHECK_EXIT_18	18
#define	CODING_CHECK_EXIT_19	19

#define CODING_CHECK_EXIT_20	20
#define CODING_CHECK_EXIT_21 21
#define CODING_CHECK_EXIT_22 22
#define CODING_CHECK_EXIT_23 23


#define	CODING_CONT_URLNONE		101
#define	CODING_CONT_SHORT		102
#define	CODING_CONT_ENG2GB		103
#define	CODING_CONT_GBBIG52ENG	104
#define	CODING_CONT_UTF82NONE	105
#define	CODING_CONT_NONE2ENG		106
#define	CODING_CONT_REMAIN		107
#define	CODING_CONT_UTF82ENG		108

#define CODING_CONT_NONE2GB		109

//definition for codetype/langtype detection
#define CODING_BUFFER_SIZE 1024
#define CODING_MAX_CHARSET_LEN 128
#define CODING_VALUE_NOT_COMPUTED (-1)
#define CODING_MAX_SAMPLE_LEN 512
#define CODING_MAX_DETECT_TRUST 1023
#define CODING_MIN_DETECT_TRUST 0

//#define CODING_DETECT_DEBUG

//not used
#define CODING_BYTE_FREQ_ARRAY_SIZE 94
#define CODING_BYTE_FREQ_OFFSET (0xa1)
#define CODING_BYTE_FREQ_END (0xfe)


/** 
 * @brief 编码识别接口
 * 
 * @param page 页面字符串，不可为NULL
 * @param url 页面url，不可为NULL
 * @param charset 页面charset字符串，如为NULL则将在page中查找
 * @param content 页面正文，可为NULL
 * @param[out] codetype 编码类型
 * 
 * @return 识别成功返回1，出错返回-1
*/
int coding_detect_codetype(const char *page, const char* url, const char* charset, const char* content, int *out_type);



/** 
 * @brief 编码识别接口
 * 
 * @param page 页面字符串，不可为NULL
 * @param url 页面url，不可为NULL
 * @param charset 页面charset字符串，如为NULL则将在page中查找
 * @param content 页面正文，可为NULL
 * @param content  codetype 已经辨认的编码类型
 * @param[out] langtype 语言类型
 * @param[out] trustarr  语言置信度数组
 * 
 * @return 识别成功返回1，出错返回-1
*/
int coding_detect_langtype(const char *page, const char* url, const char* charset, const char* content, int *codetype, int* langtype,int * trustarr=NULL);


//=========================================

/* You should specify the `path' to txt's files in which char-map tables are
 * allocated while using these functions.
 *
 * All tables are lying in path/code directory.
 */

/**
 * @brief coding加载时的控制flag
 * <ul>FLAG_SPECIAL_CHAR 控制coding繁简转换的行为，置此flag则会对一些词组进行特殊处理，如“瞭望”不会被转为“了望”
 * <ul>FLAG_QUERY_DETECT 控制是否加载短串编码识别的词典，如需要用到短串编码识别接口coding_query_detect，需要置此项
 * <ul>FLAG_NOT_SPECIAL_CHAR 空flag，只加载默认功能
 */
#define FLAG_SPECIAL_CHAR 1
#define FLAG_QUERY_DETECT 2
#define FLAG_NOT_SPECIAL_CHAR 0 

/**
 * @brief 加载coding data文件
 *
 * @param [in] path   : char* eg: ./code
 * @param [in] flag   : int 是否进行特殊字符的转换，例如”瞭望“,默认不进行转换。
 * @return  int 
 * @retval  成功0 失败-1
 * @see 
**/
int coding_load(char *path , int flag = FLAG_NOT_SPECIAL_CHAR) ;

/**
 * @brief	coding_conv结构全局初始化. 
 *	在不需要加载coding词典时，用这个接口替代coding_load()进行全局初始化。
 * @return  int 
 * @retval	0:初始化成功；-1:初始化失败。   
 * @see 
**/
int coding_conv_init();

/*
return codetype
currently: 
it can be 
0: english
1: GBK
2: Big5
3: UTF-8 encoded chinese
-1: Other
*/

int coding_check_realtext(const char* pstr);
int coding_check(char *pstr);
int coding_check_notetype (char *pstr, int *out_type);

/*
check whether the main content of a URL is Chinese and should be indexed
Notice: MUST call trans2gb before sending pTitle & pContent to this function
		old_codetype is the return of coding_check
return: new codetype
*/
int coding_check_content
	(const char* pTitle, const char* pContent, const char* pUrl, int old_codetype);
int coding_check_content_notetype
	(const char* pTitle, const char* pContent, const char* pUrl, int old_codetype, int *out_type);

int coding_del();

/**
 * @brief coding_conv结构全局回收函数。 
 *  在不需要用coding_load()加载coding词典时，用这个接口替代coding_del()进行全局回收。
 * @return  void 
**/
void coding_conv_del();

 /*=============================================================================
  * Function:        is_tradgbk_str       
  * 
  * Description:     judge whether a string is encoded in traditional gbk
  * 
  * Input:           const char* pstr, the string to be judged, it must be a
  * 				 
  * 				 zero ended string                
  * 
  * Return:          1, traditional gbk string; 0. simple gbk string 
  * 
  *============================================================================*/
int is_tradgbk_str(const char* pstr);

/*
trans2gb
return 0 when success
return -1 when error 
*/
int trans2gb(char *input,char *output, int codetype);


 /*=============================================================================
  * Function:        big5_gbk_str       
  * 
  * Description:     translate big5 encoding string into gbk encoding string
  * 
  * Input:           u_char* psrc, input big5 encoding string 
  *                  int len, the length of the psrc, len<=0, the the lenght 
  *                  will be calculated internally,  but must make sure that 
  *                  psrc ended with '\0';                
  * 
  * Output:          u_char* pDest, buffer to hold the gbk encoding string
  * 
  * Return:          <0, failed; >=0, succeed. the lengh of the input string; 
  * 
  * Others:          the buffer length of pdes must be larger or equal than the length
  * 				 of input string.  coding_load() must be called before
  * 				 this function.
  *============================================================================*/
int big5_gbk_str(u_char *psrc, u_char *pdes, int len);

int big5_gbi_str(u_char *psrc, u_char *pdes, int len);

 /*=============================================================================
  * Function:        gbk_big5_str       
  * 
  * Description:     translate gbk encoding string into big5 encoding string
  * 
  * Input:           u_char* psrc, input big5 encoding string 
  *                  int len, the length of the psrc, len<=0, the the lenght 
  *                  will be calculated internally,  but must make sure that 
  *                  psrc ended with '\0';                
  * 
  * Output:          u_char* pDest, buffer to hold the gbk encoding string
  * 
  * Return:          <0, failed; >=0, succeed. the lengh of the input string; 
  * 
  * Others:          the buffer length of pdes must be larger or equal than the length
  * 				 of input string.  coding_load() must be called before
  * 				 this function.
  *============================================================================*/
int gbk_big5_str(u_char *psrc, u_char *pdes, int len);

int gbk_gbi_str(u_char *psrc, u_char *pdes, int len);
int gbi_big5_str(u_char *psrc, u_char *pdes, int len);
int gbi_gbk_str(u_char *psrc, u_char *pdes, int len);


// transfer quanjiao keyboard letter/number/symbol to banjiao 
// this transformation can be made in-place, i.e. szIn == szOut
void trans2bj(const char *szIn, char *szOut);

// transfer some special chinese quanjiao symbols to its corresponding banjiao form
// this transformation can be made in-place, i.e. szIn == szOut
void trans2bj_ext(const char *szIn, char *szOut);

// transfer from uppercase to lower case, for gbk-encoded string
// this transformation can be made in-place, i.e. szIn == szOut
void trans2lower(const char *szIn, char *szOut);

/*
obsolete function, use trans2bj/trans2bj_ext/trans2lower instead
* transfer quanjiao to banjiao and upper to lower

int transfer_ch(char *input,char *output);
*/

/*=============================================================================
 * Function:		gbk_to_utf8       
 *
 * Description:		translate gbk encoding string into utf8 encoding string
 *
 * Input:			const char* pSrc, input gbk encoding string 
 * 					const unsigned nDesCnt, the length of the dest buffer
 *
 * Output:			char* pDest, buffer to hold the utf8 encoding string
 *
 * Return:			<0, failed; >0, succeed. the lengh of the utf8 string; 
 *
 * Others:			nDestCnt must be larger than 3/2*strlen(pSrc);
 *
 *============================================================================*/
int gbk_to_utf8(char* pDest, const unsigned nDesCnt, const char* pSrc);

/*=============================================================================
 * Function:		utf8_to_gbk       
 *
 * Description:		translate utf8 encoding string into gbk encoding string
 *
 * Input:			const char* pSrc, input gbk encoding string 
 * 					const unsigned nDesCnt, the length of the dest buffer
 *
 * Output:			char* pDest, buffer to hold the gbk encoding string
 *
 * Return:			<0, failed; >0, succeed. the lengh of the gbk string; 
 *
 * Others:			nDestCnt must be larger than strlen(pSrc);
 *
 *============================================================================*/
int utf8_to_gbk(char* pDest, const unsigned nDesDnt, const char* pSrc);

inline int is_big5( u_char *upstr )
{
	if ( (upstr[0]>=0xa1) && (upstr[0]<=0xf9) &&
	     ( (upstr[1]>=0x40) && (upstr[1]<=0x7e) ||
               (upstr[1]>=0xa1) && (upstr[1]<=0xfe)
	   ) ) {
		return 1;
	}
	else{
		return 0;
	}
}

inline int is_gbk( u_char *pstr )
{
	if ((pstr[0]>=GBK_1LO) && (pstr[0]<=GBK_1HI) &&
	     (pstr[1]>=GBK_2LO) && (pstr[1]<=GBK_2HI) &&
             (pstr[1]!=GBK_2INV)
	   ){
		return 1;
	} else {
		return 0;
	}
}

inline int is_euckr(u_char* pstr){
	return (pstr[0] >= 0xA1 && pstr[0] <= 0xFE && pstr[1] >= 0xA1 && pstr[1] <= 0xFE) ? 1 : 0;
}


inline int is_gb(u_char* pstr)
{
	if ( (pstr[0]>=0xB0) && (pstr[0]<=0xF7) &&
	     (pstr[1]>=0xA1) && (pstr[1]<=0xFE) ) 
	{
		return 1;
	} else {
		return 0;
	}
}

inline int is_big5_str(const char* pstr)
{
	char szContent[1024];
	char* pSrc = (char*) pstr;
	char* pDest = szContent;
	char* pEnd = szContent + 256;
	u_char ch;
	int count = 0, len, i, real_count=0;
	short former_word = 0;

	// Get out the double bite words
	while( (ch = *pSrc) != 0 && pSrc[1] ){
		if ( ch <= 0x7f ){
			++pSrc;
		} else {
			if(*(short*)pSrc != former_word)
				++real_count;
			*pDest++ = ch;
			*pDest++ = pSrc[1];
			former_word= *(short*)pSrc;
			pSrc += 2;
			++count;
		}
        if (pDest >= pEnd){
		   break;
        }
	}
	*pDest = 0;
	len = pDest - szContent;

	if (count == 0)
		return 0;

	for (i= 0;i<count; i++){
		if (!is_big5((u_char*)&szContent[i]))
			return 0;
		if (!is_gb((u_char*)&szContent[i]))
			return 1;
	}

	// can not judge from code range, make a choice of bigger probability
	return 0;
}

inline int is_gbk_str(const char* pSrc);


/**
 * @brief 初始化中日文编码转换所需的结构coding_conv_t
 * <p>该结构不是线程安全的，每个子线程需要单独维护一个</p>
 *
 * @param conv 待初始化的转换结构体，不能为NULL
 *
 * @return 成功返回0，出错返回-1
 */
int coding_conv_open(coding_conv_t* conv);

/**
 * @brief	线程内coding初始化函数，每个线程调用一次. 
 * 等同于coding_conv_open(coding_conv_t* conv)，但不需要显式传递coding_conv_t结构.
**/
int coding_conv_open();

/**
 * @brief 关闭中日文编码转换所需的结构
 * <p>该不是线程安全的，每个子线程需要单独维护一个</p>
 *
 * @param conv 待关闭的转换结构体
 * @return 返回0
 */
int coding_conv_close(coding_conv_t* conv);

/**
 * @brief	线程内coding关闭函数.
 * 等同于coding_conv_close(coding_conv_t* conv)，但不需要显式传递coding_conv_t结构.
**/
int coding_conv_close();

/** 
 * @brief 将输入字符串转换成GBI编码
 * <p>目前处理GB18030(GBK) BIG5 UTF8到GBI的转换。其它输入编码类型将直接拷贝</p>
 * 
 * @param input 输入字符串
 * @param output 输出buffer，注意：如果输入编码类型是UTF8，则input和output不可是同一buffer，output大小最好是input的2倍
 * @param outlen 输出buffer大小，当输出buffer写满时将不再继续转换
 * @param codetype 输入的编码类型
 * @param conv 用于做编码转换的结构体，用coding_conv_open和coding_conv_close打开和关闭。该结构不是线程安全的，各子线程需要维护单独的结构
 * 
 * @return 转换成功返回0，出错返回-1
 */
int trans2gbi(char *input, char *output, int outlen, int codetype, coding_conv_t* conv);

int trans2gbi(char *input, char *output, int outlen, int codetype);

/** 
 * @brief 将输入字符串转换成GB18030编码
 * <p>目前处理BIG5 UTF8 JIS SHIFT_JIS EUCJP到GB18030的转换，其它输入编码类型将直接拷贝</p>
 * 
 * @param input 输入字符串
 * @param output 输出buffer，注意：如果输入编码类型是UTF8 JIS SHIFT_JIS EUCJP，则input和output不可是同一buffer，output的大小最好是input的2倍
 * @param outlen 输出buffer大小，当输出buffer写满时将不再继续转换
 * @param codetype 输入的编码类型
 * @param cconv 用于做编码转换的结构体，用coding_conv_open和coding_conv_close打开和关闭。该结构不是线程安全的，各子线程需要维护单独的结构
 * 
 * @return 转换成功返回0，出错返回-1
 */
int trans2gb18030(char* input, char* output, int outlen, int codetype, coding_conv_t* conv);

int trans2gb18030(char* input, char* output, int outlen, int codetype);

/**
 * @brief 判断字符串前4个字节是否在gb18030码域内
 * @param s 输入字符串，可以为NULL
 * @return 是则返回true，NULL或不在则返回false
 */
inline bool is_gb18030_4bytes(const char* s){
	return (s != NULL)
		&& IN_RANGE(s[0], 0x81, 0xFE)
		&& IN_RANGE(s[1], 0x30, 0x39)
		&& IN_RANGE(s[2], 0x81, 0xFE)
		&& IN_RANGE(s[3], 0x30, 0x39);
}

/**
 * @brief 判断字符串前4个字节是否是gb18030-2005的四字节汉字
 * @param s 输入字符串，可以为NULL
 * @return 是则返回true，NULL或不是则返回false
 */
inline bool is_gb18030_4bytes_hanzi(const char* s){
	return (s != NULL)
		&& (IN_RANGE(s[0], 0x81, 0x82) || IN_RANGE(s[0], 0x95, 0x98)) //2 sections
		&& IN_RANGE(s[1], 0x30, 0x39)
		&& IN_RANGE(s[2], 0x81, 0xFE)
		&& IN_RANGE(s[3], 0x30, 0x39);
}

/** 
 * @brief 判断字符串开头是否是合法的GB18030字符，给出字符的字节数
 * 
 * @param s 输入字符串，假定是gb18030编码
 * 
 * @return 字符的字节数
 * 0: 如果输入为NULL或空串
 * 1: 如果输入首字节为ASCII字符
 * 2: 如果输入首2字节为GBK字符，即GB18030的双字节部分
 * 4: 如果输入首4字节为GB18030的四字节部分
 * -1: 如果输入不是合法GB18030字符
 */
int next_gb18030(const char *s);

/** 
 * @brief 判断字符串含GB18030字符个数，包括ascii字符、GB18030双字节、GB18030四字节。不包括非法字符
 * 
 * @param s 输入字符串，假定是GB18030编码
 * 
 * @return GB18030字符个数
 */
int gb18030_len(const char *s);

/** 
 * @brief 判断字符串首两字节是否是GB全角标点
 * <p>包括： 、 。 · ˉ ˇ ¨ 〃 — ～ ‖ … ‘ ’ “ ” 〔 〕〈 〉 《 》 「 」 『 』 〖 〗【 】
 *  ！ ＂ ＃ ￥ ％ ＆ ＇ （ ）＊ ＋ ， － ． ／： ； ＜ ＝ ＞ ？ ［ ＼］ ＾ ＿ ｀｛ ｜ ｝ ￣</p>
 * 
 * @param s 输入字符串，假定是GB18030编码
 * 
 * @return 是返回true，否则返回false
 */
inline bool is_gb_punct(const char *s){
	return (s != NULL) && (((u_char)s[0] == 0xA1 && IN_RANGE((u_char)s[1], 0xA2, 0xBF) && (u_char)s[1] != 0xA9) //punctuation in A1
			|| (IS_GB_SMAEK((u_char*)s) && !IS_GB_1234((u_char*)s) && !IS_GB_ABCD((u_char*)s)));//punctuation in A3
}

/**
 * @brief 初始化gb18030到utf8间的转换所需的iconv_t结构
 * <p>iconv不是线程安全的，每个子线程需要单独维护一个iconv_t结构</p>
 *
 * @param gb18030_utf8 gb18030到utf8的转换结构，如为NULL则不初始化
 * @param utf8_gb18030 utf8到gb18030的转换结构，如为NULL则不初始化
 *
 * @return 成功返回0，出错返回-1
 */
int gb18030_utf8_open(iconv_t *gb18030_utf8, iconv_t *utf8_gb18030);

/**
 * @brief 关闭gb18030到utf8间的转换所需的iconv_t结构
 * <p>iconv不是线程安全的，每个子线程需要单独维护一个iconv_t结构</p>
 *
 * @param gb18030_utf8 gb18030到utf8的转换结构，如为NULL则不处理
 * @param utf8_gb18030 utf8到gb18030的转换结构，如为NULL则不处理
 *
 * @return 返回0
 */
int gb18030_utf8_close(iconv_t *gb18030_utf8, iconv_t *utf8_gb18030);

/** 
 * @brief 将输入的gb18030编码的字符串转换为big5编码
 * <p>对于big5没有的gb18030字符，以两个空格代替。</p>
 * 
 * @param psrc 输入字符串
 * @param pdes 输出字符串，两者可以是同一个buffer，否则请保证输出buffer大小至少跟输入相同
 * @param slen 输入字符串长度，函数至多转换slen个字节，如slen<=0，将转换整个输入
 * 
 * @return 转换成功返回1，出错返回-1
 */
int gb18030_big5_str(u_char *psrc, u_char *pdes, int slen);

/** 
 * @brief 将输入的big5编码的字符串转换成gb18030编码
 * <p>由于big5字符在gbk中都有映射，而gbk是gb18030的子集，所以本函数实际做的是big5->gbk转换</p>
 * 
 * @param psrc 输入字符串
 * @param pdes 输出字符串，两者可以是同一个buffer，否则请保证输出buffer大小至少跟输入相同
 * @param slen 输入字符串长度，函数至多转换slen个字节，如slen<=0，将转换整个输入
 * 
 * @return 转换成功返回1，出错返回-1
 */
int big5_gb18030_str(u_char *psrc, u_char *pdes, int slen);

/** 
 * @brief 将输入的gb18030编码的字符串转换为简体
 * 
 * @param psrc 输入字符串
 * @param pdes 输出字符串，两者可以是同一个buffer，否则请保证输出buffer大小至少跟输入相同
 * @param slen 输入字符串长度，函数至多转换slen个字节，如slen<=0，将转换整个输入
 * 
 * @return 转换成功返回1，出错返回-1
 */
int gb18030_gbi_str(u_char *psrc, u_char *pdes, int slen);

/** 
 * @brief 将输入的gbi编码的字符串转换成gb18030编码
 * 
 * @param psrc 输入字符串
 * @param pdes 输出字符串，两者可以是同一个buffer，否则请保证输出buffer大小至少跟输入相同
 * @param slen 输入字符串长度，函数至多转换slen个字节，如slen<=0，将转换整个输入
 * 
 * @return 转换成功返回1，出错返回-1
 */
int gbi_gb18030_str(u_char *psrc, u_char *pdes, int slen);

/** 
 * @brief 将gb18030编码的字符串转换为utf8编码
 * <p>遇到转换失败的字符用一个空格代替</p>
 * 
 * @param psrc 输入字符串
 * @param pdes 输出字符串，不可与输入字符串相同，其大小不应小于输入的3/2
 * @param destlen 输出buffer大小，当buffer写满时转换将停止
 * @param gb18030_utf8 对应的iconv_t结构，用gb18030_utf8_open打开，用gb18030_utf8_close关闭，非线程安全
 * 
 * @return 成功返回转换失败的字符数，出错返回-1
 */
int gb18030_utf8_str(char *psrc, char *pdes, int destlen, iconv_t *gb18030_utf8);

int gb18030_utf8_str(char *psrc, char *pdes, int destlen);

/** 
 * @brief 将utf8编码的字符串转换为gb18030编码
 * <p>遇到转换失败的字符时，如果是单字节用一个空格代替，否则用两个空格代替</p>
 * 
 * @param psrc 输入字符串
 * @param pdes 输出字符串，不可与输入字符串相同，其大小不应小于输入的2倍
 * @param destlen 输出buffer大小，当buffer写满时转换将停止
 * @param utf8_gb18030 对应的iconv_t结构，用gb18030_utf8_open打开，用gb18030_utf8_close关闭，非线程安全
 * 
 * @return 成功返回转换失败的字符数，出错返回-1
 */
int utf8_gb18030_str(char *psrc, char *pdes, int destlen, iconv_t *utf8_gb18030);

int utf8_gb18030_str(char *psrc, char *pdes, int destlen);

/** 
 * @brief 编码和语言识别接口
 * 
 * @param page 页面字符串，不可为NULL
 * @param url 页面url，不可为NULL
 * @param charset 页面charset字符串，如为NULL则将在page中查找
 * @param content 页面正文，可为NULL
 * @param[out] codetype 编码类型
 * @param[out] langtype 语言类型
 * 
 * @return 识别成功返回1，出错返回-1
 */
int coding_detect(const char *page, const char *url, const char *charset, const char *content, int *codetype, int *langtype, int *trustarr=NULL);

/**
 * @brief 日文编码和语言识别接口，直接封装coding_detect
 *
 * @param page 页面字符串，不可为NULL
 * @param url 页面url，不可为NULL
 * @param charset 页面charset字符串，如为NULL则将在page中查找
 * @param content 页面正文，可为NULL
 * @param[out] out_type 编码类型
 *
 * @return 识别成功返回1，出错返回-1
 */
int jcode_detect(char *page, const char *url, const char *charset, const char *content, int *out_type);

/** 
 * @brief 检测短文本的编码类型，可以是GBK UTF8 或UCS2编码.
 * <p>可以通过配置query_detect.conf来指定某个字符串的编码类型，防止识别错误</p>
 * 
 * @param query 待检查的短文本，长度应该在(1, 102400)范围内
 * @param query_len 待检查的短文本的长度，如不大于0，则默认为strlen(query)
 * 
 * @return 短文本编码类型，CODETYPE_GB CODETYPE_UTF8 CODETYPE_NONE之一，出错也返回-2
 */
int coding_query_detect(char *query, int query_len);

/** 
 * @brief 检测短文本的编码类型，可以是GBK UTF8 或UCS2编码.
 * <p>可以通过配置query_detect.conf来指定某个字符串的编码类型，防止识别错误</p>
 * 
 * @param query 待检查的短文本，长度应该在(1, 102400)范围内，默认输入中不含有\0，将以strlen作为输入的长度
 * 
 * @return 短文本编码类型，CODETYPE_GB CODETYPE_UTF8 CODETYPE_NONE之一，出错也返回-2
 */
int coding_query_detect(char *query);

/** 
 * @brief 将输入字符串由UCS2编码转换为GBK编码
 * 
 * @param src 输入字符串，不能为空
 * @param[out] dst 输出buffer
 * @param dstlen 输出buffer大小，不能小于1
 * 
 * @return 输出字符串长度，出错返回-1
 */
int ucs2_gbk_str(char *src, char *dst, int dstlen);
	
/**
 * @brief	获取线程内全局私有的coding_conv结构 
**/
coding_conv_t *get_coding_conv();
/**
 * @brief  使用htmltree 来提高编码精确度编码识别和语言识别增加两个接口，
      主要是把传入 info结构，改为了与coding_detect接口类似。 
      把info 结构改为了线程内部数据结构，和coding_conv_t 一样，为了保持接口不变，
      初始化和销毁的函数放到了coding_conv_open()和coding_conv_close()中，不需要init。 
      语言识别增加了  trustarr 数组 用于存放各个语言置信度 。
      但只对编码判定为utf-8和 gb18030时 有用。 
      对每个网页，务必先调用coding_detect_codetype。然后调用coding_detect_langtype。
      原有接口与现接口不要混用。coding_detect可以用，没问题。
      考虑到效率，调用coding_detect_codetype后再调用coding_detect_langtype，
      如果参数url不变的话不会再重新计算一次info。
      如果要重新计算，可以传入空字符串""。注意不可传入NULL。会直接返回。
 *
 * @param page 页面字符串，不可为NULL
    @main_content 提取的主要内容，
    @page_content 提取的内容，
 * @param url 页面url，不可为NULL
 * @param charset 页面charset字符串，如为NULL则将在page中查找
 * @param content 页面正文，可为NULL
 * @param[out] codetype 编码类型
 * @param[out] langtype 语言类型
 * @param[out] trustarr    最后一个元素是检测字符编码的缓冲区长度
*  

 */
typedef struct _coding_page_info_t{
		char *page;
		char *main_content;
		char *page_content;
		char *title;
		int len_page;
		int len_main_content;
		int len_page_content;
		int len_title;
}coding_page_info_t;

int coding_page_detect(coding_page_info_t *page_info, const char *url, const char *charset, const char *content, int *codetype, int *langtype, int *trustarr);

/**
 * @brief 由输入语种ID返回其字符串描述
 *
 * @param [in] id   : int 语种ID
 * @return  const char*  输入语种的字符串描述
 * @retval   NULL: 输入不合法；其它：输入语种字符串描述
 * @see 
 * @note 返回指针指向全局字符串，必须为const char *
**/
const char *getlangtype(int id);

 
/**
 * @brief 由输入字符串返回语种ID
 *
 * @param [in] langtypeStr   : const char* 输入字符串
 * @return  int 语种ID
 * @retval   -2：输入非法；其它：输入字符串对应ID
 * @see 
 * @note 
**/
int getlangtype(const char *langtypeStr);

/**
 * @brief 清空某数据/策略的语言属性，用于初始化
 *
 * @param [in] langtype   : LangType * 某数据/策略的语言类型
 * @return  int 成功返回0，否则-1
 * @retval  -1：输入非法；0：成功清空语言属性
 * @see 
 * @note 
**/
int langtype_clean(LangType *langtype);

/**
 * @brief 判断某数据/策略的语言属性是否已清空
 *
 * @param [in] langtype   : LangType* 某数据/策略的语言类型
 * @return  int 判断是否已清空
 * @retval  -1：输入非法；1：本数据/策略已成功清空；0：本数据/策略未清空
 * @see 
 * @note 
**/
int langtype_isclean(LangType *langtype);

/**
 * @brief 用于设置本数据/策略的语言属性，输入整形ID，设置对应语言属性
 *
 * @param [in] langtype   : LangType* 本数据/策略的语言类型
 * @param [in] id   : int 输入语言属性ID
 * @return  int 成功返回0，否则-1
 * @retval  -1：输入非法；0：设置成功
 * @see 
 * @note 
**/
int langtype_set(LangType *langtype, int id);

/**
 * @brief 用于设置本数据/策略的语言属性，输入字符串，设置对应语言属性
 *
 * @param [in] langtype   : LangType* 本数据/策略的语言类型
 * @param [in] langtypeStr   : const char* 输入字符串
 * @return  int 成功返回0，否则-1
 * @retval    -1：输入非法；0：设置成功
 * @see 
 * @note 
**/
int langtype_set(LangType *langtype, const char *langtypeStr);

/**
 * @brief 用于添加本数据/策略的语言属性，输入整形ID，添加对应语言属性
 *
 * @param [in/out] langtype   : LangType*本数据/策略的语言类型
 * @param [in] id   : int 输入语言属性ID
 * @return  int 
 * @retval   成功返回0，否则-1
 * @see 
 * @note 
**/
int langtype_add(LangType *langtype, int id);

/**
 * @brief 用于添加本数据/策略的语言属性，输入字符串，添加对应语言属性
 *
 * @param [in/out] langtype   : LangType*本数据/策略的语言类型
 * @param [in] id   : int 输入语言属性ID
 * @return  int 
 * @retval   成功返回0，否则-1
 * @see 
 * @note 
**/
int langtype_add(LangType *langtype, const char *langtypeStr);

/**
 * @brief 输入ID，去除本数据/策略中对应属性
 *
 * @param [in] langtype   : LangType* 本数据/策略的语言类型
 * @param [in] id   : int 输入整形ID
 * @return  int 成功返回0，否则返回-1
 * @retval  -1:输入非法；0：成功
 * @see 
 * @note 
**/
int langtype_unset(LangType *langtype, int id);
 
/**
 * @brief 输入字符串语种，去除本数据/策略中对应属性
 *
 * @param [in] langtype   : LangType* 本数据/策略的语言类型
 * @param [in] langtypeStr   : const char* 输入语种字符串
 * @return  int 成功返回0，否则-1
 * @retval   -1:输入非法；0：成功
 * @see 
 * @note 
**/
int langtype_unset(LangType *langtype, const char *langtypeStr);

/**
 * @brief 用于获取本数据/策略的语言属性,将langtype中的语言属性赋值给ids,返回语言属性数目
 *
 * @param [in] langtype   : LangType* 本数据/策略的语言属性
 * @param [out] ids   : int* 输出语言属性列表
 * @param [in] n   : int     输出buffer长度
 * @return  int 输出的语言属性的数目
 * @retval  -1:参数非法；其它：语言属性数目
 * @see 
 * @note 
**/
int langtype_get(LangType *langtype, int *ids, int n);

/**
 * @brief 用于判断两个语言类型实例的语言属性是否相同
 *
 * @param [in] langtype1   : LangType* 输入语言类型1
 * @param [in] langtype2   : LangType* 输入语言类型2
 * @return  int 
 * @retval   -1：参数非法；1：两语言类型相同；0：两语言类型不同
 * @see 
 * @note 
**/
int langtype_equal(LangType *langtype1, LangType *langtype2);

/**
 * @brief 用于判断本数据/策略的语言属性是否与目标语言属性兼容,判断某个语言属性是否在属性集合langtype中，LANGTYPE_GLOBAL属性兼容所有id
 *
 * @param [in] langtype   : LangTypes* 本数据/策略的语言类型
 * @param [in] id   : int 目标语言属性
 * @return  int 
 * @retval  -1:参数非法；1：兼容；0：不兼容
 * @see 
 * @note 
**/
int langtype_compatible(LangType *langtype, int id);
 
/**
 * @brief 判断多个语言属性是否为langtype的子集
 *
 * @param [in] langtype   : LangType* 本数据/策略语言类型
 * @param [in] id   : int* 目标语言属性列表
 * @param [in] n   : int   语言属性数目
 * @return  int 
 * @retval   -1:参数非法；1：兼容；0：不兼容
 * @see 
 * @note 
**/
int langtype_compatible(LangType *langtype, int *id, int n);

/**
 * @brief 用于判断两个语言类型实例的语言属性是否兼容：langtype2中语言属性是否为langtype1的子集
 *
 * @param [in] langtype1   : LangType* 输入语言类型1
 * @param [in] langtype2   : LangType* 输入语言类型2
 * @return  int 
 * @retval   -1：参数非法；1：兼容；0：不兼容
 * @see 
 * @note 
**/
int langtype_compatible(LangType *langtype1, LangType *langtype2);

/**
 * @brief 用于获取本数据/策略支持的语言属性的数目
 *
 * @param [in] langtype   : LangType* 本数据/策略的语言类型
 * @return  int 返回语言属性数目
 * @retval   -1：参数非法；其它：支持的语言属性数目
 * @see 
 * @note 
**/
int langtype_num(LangType *langtype);

/**
 * @brief 识别输入网页的编码类型,支持GB18030,UTF8,ASCII,JIS,SJIS,EUCJP,ISO-8859-9,Windows-1254,ISO-8859-1
 *
 * @param [in] page   : const char* 输入网页
 * @param [in] url   : const char*  输入url
 * @param [in] charset   : const char* 输入网页charset标记，可为NULL
 * @param [in] content   : const char* 输入正文内容，可为NULL
 * @param [out] codetype   : int* 输出编码类型
 * @return  int
 * @retval -1:参数异常未识别;1:iso-8859-1英文;2:iso-8859-1德文;3:iso-8859-1法文;0:成功识别为其它类型
 * @see 
 * @note 
**/
int coding_chardet(const char *page, const char *url, const char *charset, const char *content, int *codetype);

/**
 * @brief 识别输入网页的语言类型：支持中文、日文、土耳其文、英文、德文、法文
 *
 * @param [in] page   : const char* 输入网页
 * @param [in] url   : const char* 输入网页的url
 * @param [in] charset   : const char* 输入网页的charset标记，可为NULL
 * @param [in] content   : const char* 输入网页正文，可为NULL
 * @param [in/out] codetype   : int* 若编码类型已知，请输入已知的编码类型；否则设置codetype为-2，将自动识别编码类型。
 * @param [out] langtype   : int* 输出语言类型
 * @param [out] backuplang   : int* 默认为NULL；backuplang将输出最可能的前三种语言(暂不支持)
 * @return  int
 * @retval   -1：参数异常、未准确识别；0：成功识别为某种语言类型
 * @see 
 * @note 
**/
int coding_langdet(const char *page, const char *url, const char *charset, const char *content, int *codetype, int *langtype, int *backuplang = NULL);

/**
 * @brief 返回当前线程的语言类型
 *
 * @return  int 
 * @retval  未成功获取返回-2，其它返回获取的语言类型
 * @see 
 * @note 
**/
int get_globallang();

/**
 * @brief 设置当前线程的语言类型
 *
 * @param [in] langtype   : int 有效范围[LANGTYPE_NONE, LANGTYPE_END)
 * @return  int 
 * @retval  成功设置返回0,失败返回-1
 * @see 
 * @note 
**/
int set_globallang(int langtype);

/**
 * @brief 设置当前进程的全局编码类型
 *
 * @param [in] codetype   : int 有效范围 [CODETYPE_NONE, CODETYPE_END)
 * @return  int 
 * @retval  成功返回0，失败返回-1
 * @see 
 * @note 
**/
int set_globalcode(int codetype);

/**
 * @brief 获取当前进程的全局编码类型
 *
 * @return  int 全局编码类型
 * @retval   
 * @see 
 * @note 
**/
int get_globalcode();

#endif

#ifndef _ADS_ADVERTISE_MATERIAL_H
#define _ADS_ADVERTISE_MATERIAL_H

enum class AdsMaterialType
{
	IMAGE = 1,
	VIDEO,
	NATIVE,
};

enum class AdsMaterialImageType
{
	ICON = 1,
	IMAGE,
};

class AdsMaterialImage
{
public:
	AdsMaterialImageType type;

	unsigned int width;
	unsigned int height;
	string ext;
	string mime;
	string source;
};

class AdsMaterialVideo
{
public:
	unsigned int width;
	unsigned int height;
	unsigned int duration;
	string ext;
	string mime;
	string source;
};

class AdsMaterialNative
{
public:
	string title;
	string description;
	vector<AdsMaterialImage> images;

};

class AdsMaterial
{
public:
	AdsMaterialType type;

	/* 图片素材 */
	AdsMaterialImage *image;
	
	/* 视频素材 */
	AdsMaterialVideo *video;
	
	/* 信息流素材 */
	AdsMaterialNative *native;
};


#endif
/* vim: set ts=4 sw=4 noet: */

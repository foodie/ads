#ifndef _ADS_ADVERTISE_MATERIAL_H
#define _ADS_ADVERTISE_MATERIAL_H

#include <string>
#include <vector>

using std::string;
using std::vector;

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

/**
 * @brief      Class for ads material image.
 */
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

/**
 * @brief      Class for ads material video.
 */
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

/**
 * @brief      Class for ads material native.
 */
class AdsMaterialNative
{
public:
	string title; 						// 标题
	string description; 				// 描述
	vector<AdsMaterialImage> images;	// 图片
};

class AdsMaterial
{
public:
	AdsMaterial(AdsMaterialType type) 
		: _id(0),_type(type),
		  _image(NULL),_video(NULL),_native(NULL)
	{
		if ( _type == AdsMaterialType::IMAGE ) {
			_image = new (std::nothrow) AdsMaterialImage;
		} else if ( _type == AdsMaterialType::VIDEO ) {
			_video = new (std::nothrow) AdsMaterialVideo;
		} else if ( _type == AdsMaterialType::NATIVE ) {
			_native = new (std::nothrow) AdsMaterialNative;
		}
	}

	~AdsMaterial()
	{
		if ( _type == AdsMaterialType::IMAGE ) {
			delete _image;
		} else if ( _type == AdsMaterialType::VIDEO ) {
			delete _video;
		} else if ( _type == AdsMaterialType::NATIVE ) {
			delete _native;
		}
	}

	void setId(int id) { _id = id; }
	int id() const { return _id; }

	AdsMaterialType type() const { return _type; }

	AdsMaterialImage* image() const { return _image; }
	AdsMaterialVideo* video() const { return _video; }
	AdsMaterialNative* native() const { return _native; }

private:
	int _id;

	AdsMaterialType _type;

	/* 图片素材 */
	AdsMaterialImage *_image;
	
	/* 视频素材 */
	AdsMaterialVideo *_video;
	
	/* 信息流素材 */
	AdsMaterialNative *_native;
};


#endif
/* vim: set ts=4 sw=4 noet: */

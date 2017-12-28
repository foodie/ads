#ifndef __ADS_JSON_H_
#define __ADS_JSON_H_

#include <google/protobuf/descriptor.h>  
#include <google/protobuf/message.h>  

#include "rapidjson/document.h"
#include "rapidjson/writer.h"

#include "ads_string.h"
#include "ads_map.h"

typedef ads_map_t<string, string> ads_string_map_t;

#define rapidjson_value(itr) itr->value

static inline void rapidjson_value_find(const rapidjson::Value& value, const char *key, 
	rapidjson::Value::ConstMemberIterator& itr) {
	itr = value.FindMember(key);
}

static inline bool rapidjson_value_exist(const rapidjson::Value& value, const char *key, 
	rapidjson::Value::ConstMemberIterator& itr) {
	itr = value.FindMember(key);
	return itr != value.MemberEnd();
}



// json => string
void ads_json_to_string(const rapidjson::Value& value, char *json, size_t size);
void ads_json_to_str(const rapidjson::Value& value, ads_str_t *str);

// protobuf => json string
void ads_protobuf_to_json(const ::google::protobuf::Message& msg, char *json, size_t size);
void ads_protobuf_to_json_str(const ::google::protobuf::Message& msg, ads_str_t *str);

// map => json string
void ads_string_map_to_json(ads_string_map_t *map, char *json, size_t size);
void ads_string_map_to_json_str(ads_string_map_t *map, ads_str_t *str);

#endif

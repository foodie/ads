#ifndef __ADS_JSON_H_
#define __ADS_JSON_H_

#include <google/protobuf/descriptor.h>  
#include <google/protobuf/message.h>  

#include "rapidjson/document.h"
#include "rapidjson/writer.h"

#include <string>
#include "ads_core.h"


#define rapidjson_value(itr) itr->value

inline void rapidjson_value_find(const rapidjson::Value& value, const char *key, 
	rapidjson::Value::ConstMemberIterator& itr) {
	itr = value.FindMember(key);
}

inline bool rapidjson_value_exist(const rapidjson::Value& value, const char *key, 
	rapidjson::Value::ConstMemberIterator& itr) {
	itr = value.FindMember(key);
	return itr != value.MemberEnd();
}


// json => string
string ads_json_to_string(const rapidjson::Value& value);
void ads_json_to_str(const rapidjson::Value& value, char *json, size_t size);

// protobuf => json string
string ads_protobuf_to_json(const ::google::protobuf::Message& msg);
void ads_protobuf_to_json_str(const ::google::protobuf::Message& msg, char *json, size_t size);


#endif

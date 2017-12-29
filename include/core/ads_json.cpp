#include "ads_json.h"

string ads_json_to_string(const rapidjson::Value& value)
{
	rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    value.Accept(writer);
    return buffer.GetString();
}

void ads_json_to_str(const rapidjson::Value& value, char *json, size_t size)
{
	rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    value.Accept(writer);
	snprintf(json, size, "%s", buffer.GetString());
}

/********** protobuf **********/
static int ads_protobuf_repeated_to_json(const ::google::protobuf::Message& msg, 
										  const ::google::protobuf::FieldDescriptor* field, 
										  rapidjson::Value& root,
										  rapidjson::Document::AllocatorType& allocator);
static int ads_protobuf_message_to_json(const ::google::protobuf::Message& msg,
							  			rapidjson::Value& root, 
							  			rapidjson::Document::AllocatorType& allocator);

string ads_protobuf_to_json(const ::google::protobuf::Message& msg)
{
	rapidjson::Document doc;
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	rapidjson::Value root(rapidjson::kObjectType);
	ads_protobuf_message_to_json(msg, root, allocator);
	return ads_json_to_string(root);
}

void ads_protobuf_to_json_str(const ::google::protobuf::Message& msg, char *json, size_t size)
{
	rapidjson::Document doc;
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	rapidjson::Value root(rapidjson::kObjectType);
	ads_protobuf_message_to_json(msg, root, allocator);
	
	ads_json_to_str(root, json, size);
}

static int ads_protobuf_repeated_to_json(const ::google::protobuf::Message& msg, 
							   const ::google::protobuf::FieldDescriptor* field,
							   rapidjson::Value& root,
							   rapidjson::Document::AllocatorType& allocator)
{
	char buffer[ADS_MAX_BUF_LEN];
	const ::google::protobuf::Reflection* reflection = msg.GetReflection();
	for (int i = 0; i < reflection->FieldSize(msg, field); i++) {
		rapidjson::Value value;
		switch (field->type())
		{
			case ::google::protobuf::FieldDescriptor::TYPE_MESSAGE:
				{
					const ::google::protobuf::Message& tmp_msg = reflection->GetRepeatedMessage(msg, field, i);
					if (0 != tmp_msg.ByteSize())
					{
						value.SetObject();
						ads_protobuf_message_to_json(tmp_msg, value, allocator);
					}
				}
				break;
			case ::google::protobuf::FieldDescriptor::TYPE_INT32:
				value.SetInt(reflection->GetRepeatedInt32(msg, field, i));
				break;
			case ::google::protobuf::FieldDescriptor::TYPE_UINT32:
				value.SetUint(reflection->GetRepeatedUInt32(msg, field, i));
				break;
			case ::google::protobuf::FieldDescriptor::TYPE_INT64:
				value.SetInt64(reflection->GetRepeatedInt64(msg, field, i));
				break;
			case ::google::protobuf::FieldDescriptor::TYPE_UINT64:
				value.SetUint64(reflection->GetRepeatedUInt64(msg, field, i));
				break;
			case ::google::protobuf::FieldDescriptor::TYPE_FLOAT:
				value.SetDouble(reflection->GetRepeatedFloat(msg, field, i));
				break;
			case ::google::protobuf::FieldDescriptor::TYPE_DOUBLE:
				value.SetDouble(reflection->GetRepeatedDouble(msg, field, i));
				break;
			case ::google::protobuf::FieldDescriptor::TYPE_STRING:
			case ::google::protobuf::FieldDescriptor::TYPE_BYTES:
				{
					ads_memzero(buffer, sizeof(buffer));
					int len = snprintf(buffer, sizeof(buffer), "%s", reflection->GetRepeatedString(msg, field, i).c_str());
					value.SetString(buffer, len, allocator);
				}
				break;
			default: continue;
		}
		root.PushBack(value, allocator);
	}
	return 0;
}

static int ads_protobuf_message_to_json(const ::google::protobuf::Message& msg,
							  			rapidjson::Value& root, 
							  			rapidjson::Document::AllocatorType& allocator)
{
	char buffer[ADS_MAX_BUF_LEN];
	
	const ::google::protobuf::Descriptor* descriptor = msg.GetDescriptor();
	const ::google::protobuf::Reflection* reflection = msg.GetReflection();
	
	const int count = descriptor->field_count();
	
	for (int i = 0; i < count; i++)
	{  
		rapidjson::Value value;
		
		const ::google::protobuf::FieldDescriptor* field = descriptor->field(i);
		
		// 数组
		if (field->is_repeated())
		{
			if (reflection->FieldSize(msg, field) > 0) 
			{
				value.SetArray();
				ads_protobuf_repeated_to_json(msg, field, value, allocator);
				goto ADD_MEMBER;
			}
			continue;
		}
		
		if (!reflection->HasField(msg, field))
        {
            continue;
        }
		
		switch (field->type())
		{
			case ::google::protobuf::FieldDescriptor::TYPE_MESSAGE:
				{
					const ::google::protobuf::Message& tmp_msg = reflection->GetMessage(msg, field);
					if (0 != tmp_msg.ByteSize())  
					{
						value.SetObject();
						ads_protobuf_message_to_json(tmp_msg, value, allocator);
					}
				}
				break;
			case ::google::protobuf::FieldDescriptor::TYPE_INT32:
				value.SetInt(reflection->GetInt32(msg, field));
				break;
			case ::google::protobuf::FieldDescriptor::TYPE_UINT32:
				value.SetUint(reflection->GetUInt32(msg, field));
				break;
			case ::google::protobuf::FieldDescriptor::TYPE_INT64:
				value.SetInt64(reflection->GetInt64(msg, field));
				break;
			case ::google::protobuf::FieldDescriptor::TYPE_UINT64:
				value.SetUint64(reflection->GetUInt64(msg, field));
				break;
			case ::google::protobuf::FieldDescriptor::TYPE_FLOAT:
				value.SetDouble(reflection->GetFloat(msg, field));
				break;
			case ::google::protobuf::FieldDescriptor::TYPE_DOUBLE:
				value.SetDouble(reflection->GetDouble(msg, field));
				break;
			case ::google::protobuf::FieldDescriptor::TYPE_STRING:
			case ::google::protobuf::FieldDescriptor::TYPE_BYTES:
				{	
					ads_memzero(buffer, sizeof(buffer));
					int len = snprintf(buffer, sizeof(buffer), "%s", reflection->GetString(msg, field).c_str());
					value.SetString(buffer, len, allocator);
				}
				break;
			default: continue;
		}
		
ADD_MEMBER:
		root.AddMember(rapidjson::StringRef(field->name().c_str()), value, allocator);
	}
	return 0;
}



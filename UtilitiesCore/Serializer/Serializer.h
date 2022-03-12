// 
#ifndef _SERIALIZER_H_
#define _DEBUGGER_H_

#include <nlohmann/json.hpp>

//
// macro to create to_json & from_json definitions 
// (same as NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE but without inline qualifier)
//
#define IMPLEMENT_JSON_SERIALIZATION(type_name, ...)                      \
void to_json(nlohmann::json& nlohmann_json_j, const type_name& nlohmann_json_t)   \
{                                                                         \
    NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_TO, __VA_ARGS__))      \
}                                                                                 \
void from_json(const nlohmann::json& nlohmann_json_j, type_name& nlohmann_json_t) \
{                                                                                 \
    NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_FROM, __VA_ARGS__))    \
}

#endif // _DEBUGGER_H_

// 
#ifndef _SERIALIZER_H_
#define _DEBUGGER_H_

#include <nlohmann/json.hpp>
#include "Serializer/Serializer.h"

struct JsonModelInfo
{
    std::string Name;
    std::string ModelId;
    int sceneType;
    double position[3];
    double scale[3];
    double texture[2];
};

struct JsonModel
{
    std::vector<JsonModelInfo> Models;
};

IMPLEMENT_JSON_SERIALIZATION(JsonModelInfo,
    Name,
    ModelId,
    sceneType,
    position,
    scale,
    texture
);

IMPLEMENT_JSON_SERIALIZATION(JsonModel,
    Models
);

template <typename Type>
void TryToConvert(const char* jsonBuffer, Type& out)
{
    try
    {
        auto j = nlohmann::json::parse(jsonBuffer);
        from_json(j, out);
    }
    catch (const std::exception& e)
    {
        std::cout << "Error while trying to convert json, info: " << e.what() << std::endl;
    }
}

#endif // _DEBUGGER_H_

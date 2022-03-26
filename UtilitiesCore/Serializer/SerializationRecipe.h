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
    Double position[3];
    Double scale[3];
    Double texture[2];
};

struct JsonBackgroundMusicInfo
{
    std::string Name;
    Float Volume;
};

struct JsonLevelData
{
    std::vector<JsonModelInfo> Models;
    std::vector<string> SkyBox;
    std::vector<JsonBackgroundMusicInfo> BackgroundMusic;
};

IMPLEMENT_JSON_SERIALIZATION(JsonModelInfo,
    Name,
    ModelId,
    sceneType,
    position,
    scale,
    texture
);

IMPLEMENT_JSON_SERIALIZATION(JsonBackgroundMusicInfo,
    Name,
    Volume
);

IMPLEMENT_JSON_SERIALIZATION(JsonLevelData,
    Models,
    SkyBox,
    BackgroundMusic
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

#include "algui/BackgroundType.hpp"


namespace algui {


    std::map<BackgroundType, std::string> getBackgroundTypeToStringMap() {
        std::map<BackgroundType, std::string> map;
        map[BackgroundType::None] = "None";
        map[BackgroundType::Color] = "Color";
        map[BackgroundType::ColorGradient] = "ColorGradient";
        map[BackgroundType::CenteredBitmap] = "CenteredBitmap";
        map[BackgroundType::ScaledBitmap] = "ScaledBitmap";
        map[BackgroundType::TiledBitmap] = "TiledBitmap";
        return map;
    }


    std::map<std::string, BackgroundType> getStringToBackgroundTypeMap() {
        std::map<std::string, BackgroundType> map;
        map["None"] = BackgroundType::None;
        map["Color"] = BackgroundType::Color;
        map["ColorGradient"] = BackgroundType::ColorGradient;
        map["CenteredBitmap"] = BackgroundType::CenteredBitmap;
        map["ScaledBitmap"] = BackgroundType::ScaledBitmap;
        map["TiledBitmap"] = BackgroundType::TiledBitmap;
        return map;
    }


} //namespace algui

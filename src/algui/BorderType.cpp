#include "algui/BorderType.hpp"


namespace algui {


    std::map<BorderType, std::string> getBorderTypeToStringMap() {
        std::map<BorderType, std::string> map;
        map[BorderType::None] = "None";
        map[BorderType::Color] = "Color";
        map[BorderType::ScaledBitmap] = "ScaledBitmap";
        map[BorderType::TiledBitmap] = "TiledBitmap";
        return map;
    }


    std::map<std::string, BorderType> getStringToBorderTypeMap() {
        std::map<std::string, BorderType> map;
        map["None"] = BorderType::None;
        map["Color"] = BorderType::Color;
        map["ScaledBitmap"] = BorderType::ScaledBitmap;
        map["TiledBitmap"] = BorderType::TiledBitmap;
        return map;
    }


} //namespace algui

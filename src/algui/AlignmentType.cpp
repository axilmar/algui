#include "algui/AlignmentType.hpp"


namespace algui {


    std::map<AlignmentType, std::string> getAlignmentToStringMap() {
        std::map<AlignmentType, std::string> map;
        map[AlignmentType::Start] = "Start";
        map[AlignmentType::Middle] = "Middle";
        map[AlignmentType::End] = "End";
        return map;
    }


    std::map<std::string, AlignmentType> getStringToAlignmentMap() {
        std::map<std::string, AlignmentType> map;
        map["Start"] = AlignmentType::Start;
        map["Middle"] = AlignmentType::Middle;
        map["End"] = AlignmentType::End;
        return map;
    }


} //namespace algui

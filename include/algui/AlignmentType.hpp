#ifndef ALGUI_ALIGNMENTTYPE_HPP
#define ALGUI_ALIGNMENTTYPE_HPP


#include <map>
#include <string>


namespace algui {


    /**
     * AlignmentType type.
     */
    enum class AlignmentType {
        /** Start position (either left or top). */
        Start,

        /** Middle position. */
        Middle,

        /** End position (either right or bottom). */
        End
    };


    /**
     * Returns a map of values of type AlignmentType to their respective strings.
     * @return a map of values of type AlignmentType to their respective strings.
     */
    std::map<AlignmentType, std::string> getAlignmentToStringMap();


    /**
     * Returns a map of string to value of type AlignmentType.
     * @return a map of string to value of type AlignmentType.
     */
    std::map<std::string, AlignmentType> getStringToAlignmentMap();


} //namespace algui


#endif //ALGUI_ALIGNMENTTYPE_HPP

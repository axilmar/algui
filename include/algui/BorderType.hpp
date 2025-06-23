#ifndef ALGUI_BORDERTYPE_HPP
#define ALGUI_BORDERTYPE_HPP


#include <map>
#include <string>


namespace algui {


    /**
     * Border type.
     */
    enum class BorderType {
        /** No border. */
        None,

        /** Border drawn with color. */
        Color,

        /** Border taken from bitmap and scaled. */
        ScaledBitmap,

        /** Border taken from bitmap and tiled. */
        TiledBitmap
    };


    /**
     * Returns a map of border type values to strings.
     * @return a map of border type values to strings.
     */
    std::map<BorderType, std::string> getBorderTypeToStringMap();


    /**
     * Returns a map of strings to border type values.
     * @return a map of strings to border type values.
     */
    std::map<std::string, BorderType> getStringToBorderTypeMap();


} //namespace algui


#endif //ALGUI_BORDERTYPE_HPP

#ifndef ALGUI_BACKGROUNDTYPE_HPP
#define ALGUI_BACKGROUNDTYPE_HPP


#include <map>
#include <string>


namespace algui {


    /**
     * Background type.
     */
    enum class BackgroundType {
        /** No background. */
        None,

        /**  Background filled with a single color. */
        Color,

        /**  Background filled with a gradient. */
        ColorGradient,

        /** Centered bitmap. */
        CenteredBitmap,

        /** Scaled bitmap. */
        ScaledBitmap,

        /** Tiled bitmap. */
        TiledBitmap
    };


    /**
     * Returns a map of background type values to strings.
     * @return a map of background type values to strings.
     */
    std::map<BackgroundType, std::string> getBackgroundTypeToStringMap();


    /**
     * Returns a map of strings to background type values.
     * @return a map of strings to background type values.
     */
    std::map<std::string, BackgroundType> getStringToBackgroundTypeMap();


} //namespace algui


#endif //ALGUI_BACKGROUNDTYPE_HPP

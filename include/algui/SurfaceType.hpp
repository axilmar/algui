#ifndef ALGUI_SURFACETYPE_HPP
#define ALGUI_SURFACETYPE_HPP


#include <string>


namespace algui {


    /**
     * The default surface types.
     */
    class SurfaceType {
    public:
        /**
         * An interactive surface is a surface that can be clicked, such as a push button.
         * Graphical output on interactive surfaces may be different than on a background surface.
         * For example, text on background may be black, but text on a push button may be white.
         */
        static const std::string Interactive;
    };


} //namespace algui


#endif //ALGUI_SURFACETYPE_HPP

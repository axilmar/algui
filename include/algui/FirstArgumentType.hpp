#ifndef ALGUI_FIRSTARGUMENT_HPP
#define ALGUI_FIRSTARGUMENT_HPP


namespace algui {


    /**
     * A trait that retrieves the type of the first argument of a functor or pointer to function.
     * @param T type of functor/function pointer.
     */
    template <class T> struct FirstArgument {
    private:
        template <class R, class C, class A, class... Rest> static A helper(R(C::*)(A, Rest...) const);
        template <class R, class C, class A, class... Rest> static A helper(R(C::*)(A, Rest...));
        template <class R, class A, class... Rest> static A helper(R(*)(A, Rest...));
        template <class F> static decltype(helper(&F::operator())) helper(F);

    public:
        /**
         * The type of the first argument.
         */
        using Type = decltype(helper(std::declval<T>()));
    };


    /**
     * Shortcut for getting the first argument type.
     * @param T type of functor/function pointer.
     */
    template <class T>
    using FirstArgumentType = typename FirstArgument<T>::Type;


} //namespace algui


#endif //ALGUI_FIRSTARGUMENT_HPP

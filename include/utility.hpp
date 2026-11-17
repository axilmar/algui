#ifndef ALGUI_UTILITY_HPP
#define ALGUI_UTILITY_HPP


#include <algorithm>


#ifdef min
#undef min
#undef max
#endif


namespace algui {


    template <class T>
    bool checkRectIntersection(T left1, T top1, T right1, T bottom1, T left2, T top2, T right2, T bottom2) {
        return left1 < right2 && top1 < bottom2 && right1 >= left2 && bottom1 >= top2;
    }


    template <class T>
    void calcRectIntersection(
        T left1, T top1, T right1, T bottom1,
        T left2, T top2, T right2, T bottom2,
        T& left, T& top, T& right, T& bottom)
    {
        left = std::max(left1, left2);
        top = std::max(top1, top2);
        right = std::min(right1, right2);
        bottom = std::min(bottom1, bottom2);
    }


    template <class T>
    bool isValidRect(T left, T top, T right, T bottom) {
        return left < right && top < bottom;
    }


    template <class It, class StrongPtr>
    auto findWeakPtr(const It& begin, const It& end, const StrongPtr& ptr) {
        return std::find_if(begin, end, [&](const auto& weakPtr) {
            return weakPtr.lock() == ptr;
        });
    }


} //namespace algui


#endif //ALGUI_UTILITY_HPP

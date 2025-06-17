#include "algui/UString.hpp"


namespace algui {


    template <class F>
    static auto _do(const std::string_view& s, const F& f) {
        char* endPos = const_cast<char*>(s.data() + s.size());
        char orgChar = *endPos;
        *endPos = '\0';
        const auto result = f(s.data());
        *endPos = orgChar;
        return result;
    }


    template <class R, class F>
    static auto _do(const std::string_view& s, const F& f, R falseResult) {
        return s.empty() ? falseResult : _do(s, f);
    }


    UString::UString() 
        : m_ustr(al_ustr_new(""))
    {
    }


    UString::UString(int32_t c)
        : m_ustr(al_ustr_new(""))
    {
        al_ustr_append_chr(m_ustr, c);
    }


    UString::UString(size_t count, int32_t c)
        : m_ustr(al_ustr_new(""))
    {
        for (; count; --count) {
            al_ustr_append_chr(m_ustr, c);
        }
    }


    UString::UString(const char* s)
        : m_ustr(al_ustr_new(s ? s : ""))
    {
    }


    UString::UString(const char* s, size_t size)
        : m_ustr(s && size > 0 ? al_ustr_new_from_buffer(s, size) : al_ustr_new(""))
    {
    }


    UString::UString(const std::string& s)
        : m_ustr(al_ustr_new(s.c_str()))
    {
    }


    UString::UString(const std::string_view& s)
        : m_ustr(s.empty() ? al_ustr_new("") : al_ustr_new_from_buffer(s.data(), s.size()))
    {
    }


    UString::UString(const char* format, ...)
        : m_ustr(al_ustr_new(""))
    {
        va_list ap;
        va_start(ap, format);
        al_ustr_vappendf(m_ustr, format, ap);
        va_end(ap);
    }


    UString::UString(const char* format, va_list ap)
        : m_ustr(al_ustr_new(""))
    {
        al_ustr_vappendf(m_ustr, format, ap);
    }


    UString::UString(const ALLEGRO_USTR* ustr) 
        : m_ustr(al_ustr_dup(ustr))
    {
    }


    UString::UString(const UString& str)
        : m_ustr(al_ustr_dup(str.m_ustr))
    {
    }


    UString::UString(UString&& str) noexcept
        : m_ustr(str.m_ustr)
    {
        str.m_ustr = al_ustr_new("");
    }


    UString::~UString() {
        al_ustr_free(m_ustr);
    }


    UString& UString::operator = (const char* s) {
        al_ustr_assign_cstr(m_ustr, s ? s : "");
        return *this;
    }


    UString& UString::operator = (const std::string& s) {
        al_ustr_assign_cstr(m_ustr, s.c_str());
        return *this;
    }


    UString& UString::operator = (const std::string_view& s) {
        s.empty() ?
            al_ustr_assign(m_ustr, al_ustr_empty_string()) :
            _do(s, [&](const char* s) { return al_ustr_assign_cstr(m_ustr, s); });
        return *this;
    }


    UString& UString::operator = (const ALLEGRO_USTR* str) {
        al_ustr_assign(m_ustr, str);
        return *this;
    }


    UString& UString::operator = (const UString& str) {
        al_ustr_assign(m_ustr, str.m_ustr);
        return *this;
    }


    UString& UString::operator = (UString&& str) noexcept {
        if (&str != this) {
            m_ustr = str.m_ustr;
            str.m_ustr = al_ustr_new("");
        }
        return *this;
    }

    
    bool UString::reset() {
        return al_ustr_assign_cstr(m_ustr, "");
    }


    const char* UString::data() const {
        return al_cstr(m_ustr);
    }


    bool UString::empty() const {
        return al_ustr_size(m_ustr) == 0;
    }


    size_t UString::size() const {
        return al_ustr_size(m_ustr);
    }


    size_t UString::length() const {
        return al_ustr_length(m_ustr);
    }


    int UString::getCodePointPosition(int index) const {
        return al_ustr_offset(m_ustr, index);
    }


    bool UString::moveToPreviousCodePoint(int& pos) const {
        return al_ustr_prev(m_ustr, &pos);
    }


    bool UString::moveToNextCodePoint(int& pos) const {
        return al_ustr_next(m_ustr, &pos);
    }


    int32_t UString::getCodePointAtPosition(int pos) const {
        return al_ustr_get(m_ustr, pos);
    }


    int32_t UString::operator [](int index) const {
        return al_ustr_get(m_ustr, getCodePointPosition(index));
    }


    int32_t UString::getCodePointAndMoveToNext(int& pos) const {
        return al_ustr_get_next(m_ustr, &pos);
    }


    int32_t UString::moveToPreviousCodePointAndGet(int& pos) const {
        return al_ustr_prev_get(m_ustr, &pos);
    }


    bool UString::insert(int pos, int32_t c) {
        return al_ustr_insert_chr(m_ustr, pos, c);
    }


    bool UString::insert(int pos, const char* s) {
        return s ? al_ustr_insert_cstr(m_ustr, pos, s) : false;
    }


    bool UString::insert(int pos, const std::string& s) {
        return al_ustr_insert_cstr(m_ustr, pos, s.c_str());
    }


    bool UString::insert(int pos, const std::string_view& s) {
        return _do(s, [&](const char* s) { return al_ustr_insert_cstr(m_ustr, pos, s); }, false);
    }


    bool UString::insert(int pos, const ALLEGRO_USTR* str) {
        return al_ustr_insert(m_ustr, pos, str);
    }


    bool UString::insert(int pos, const char* format, ...) {
        va_list ap;
        va_start(format, ap);
        const bool result = insert(pos, format, ap);
        va_end(ap);
        return result;
    }


    bool UString::insert(int pos, const char* format, va_list ap) {
        return insert(pos, UString(format, ap));
    }


    bool UString::prepend(int32_t c) {
        return insert(0, c);
    }


    bool UString::prepend(const char* s) {
        return s ? al_ustr_insert_cstr(m_ustr, 0, s) : false;
    }


    bool UString::prepend(const std::string& s) {
        return insert(0, s);
    }


    bool UString::prepend(const std::string_view& s) {
        return insert(0, s);
    }


    bool UString::prepend(const ALLEGRO_USTR* str) {
        return insert(0, str);
    }


    bool UString::prepend(const char* format, ...) {
        va_list ap;
        va_start(format, ap);
        const bool result = prepend(format, ap);
        va_end(ap);
        return result;
    }


    bool UString::prepend(const char* format, va_list ap) {
        return insert(0, format, ap);
    }


    bool UString::append(int32_t c) {
        return al_ustr_append_chr(m_ustr, c);
    }


    bool UString::append(const char* s) {
        return s ? al_ustr_append_cstr(m_ustr, s) : false;
    }


    bool UString::append(const std::string& s) {
        return al_ustr_append_cstr(m_ustr, s.c_str());
    }


    bool UString::append(const std::string_view& s) {
        return _do(s, [&](const char* s) { return al_ustr_append_cstr(m_ustr, s); }, false);
    }


    bool UString::append(const ALLEGRO_USTR* str) {
        return al_ustr_append(m_ustr, str);
    }


    bool UString::append(const char* format, ...) {
        va_list ap;
        va_start(format, ap);
        const bool result = append(format, ap);
        va_end(ap);
        return result;
    }


    bool UString::append(const char* format, va_list ap) {
        return al_ustr_vappendf(m_ustr, format, ap);
    }


    bool UString::remove(int pos) {
        return al_ustr_remove_chr(m_ustr, pos);
    }


    bool UString::removeRange(int startPos, int endPos) {
        return al_ustr_remove_range(m_ustr, startPos, endPos);
    }


    bool UString::truncate(int startPos) {
        return al_ustr_truncate(m_ustr, startPos);
    }


    bool UString::trimLeadingWhitespace() {
        return al_ustr_ltrim_ws(m_ustr);
    }


    bool UString::trimTrailingWhitespace() {
        return al_ustr_rtrim_ws(m_ustr);
    }


    bool UString::trimWhitespace() {
        return al_ustr_trim_ws(m_ustr);
    }


    int UString::find(int startPos, int32_t c) const {
        return al_ustr_find_chr(m_ustr, startPos, c);
    }


    int UString::find(int startPos, const char* s) const {
        return al_ustr_find_cstr(m_ustr, startPos, s);
    }


    int UString::find(int startPos, const std::string& s) const {
        return al_ustr_find_cstr(m_ustr, startPos, s.c_str());
    }


    int UString::find(int startPos, const std::string_view& s) const {
        return _do(s, [&](const char* s) { return al_ustr_find_cstr(m_ustr, startPos, s); }, -1);
    }


    int UString::find(int startPos, const ALLEGRO_USTR* str) const {
        return al_ustr_find_str(m_ustr, startPos, str);
    }


    int UString::findReverse(int endPos, int32_t c) const {
        return al_ustr_rfind_chr(m_ustr, endPos, c);
    }


    int UString::findReverse(int endPos, const char* s) const {
        return al_ustr_rfind_cstr(m_ustr, endPos, s);
    }


    int UString::findReverse(int endPos, const std::string& s) const {
        return al_ustr_rfind_cstr(m_ustr, endPos, s.c_str());
    }


    int UString::findReverse(int endPos, const std::string_view& s) const {
        return _do(s, [&](const char* s) { return al_ustr_rfind_cstr(m_ustr, endPos, s); }, -1);
    }


    int UString::findReverse(int endPos, const ALLEGRO_USTR* str) const {
        return al_ustr_rfind_str(m_ustr, endPos, str);
    }


    int UString::findInSet(int startPos, const char* s) const {
        return al_ustr_find_set_cstr(m_ustr, startPos, s);
    }


    int UString::findInSet(int startPos, const std::string& s) const {
        return al_ustr_find_set_cstr(m_ustr, startPos, s.c_str());
    }


    int UString::findInSet(int startPos, const std::string_view& s) const {
        return _do(s, [&](const char* s) { return al_ustr_find_set_cstr(m_ustr, startPos, s); }, -1);
    }


    int UString::findInSet(int startPos, const ALLEGRO_USTR* str) const {
        return al_ustr_find_set(m_ustr, startPos, str);
    }


    int UString::findNotInSet(int startPos, const char* s) const {
        return al_ustr_find_cset_cstr(m_ustr, startPos, s);
    }


    int UString::findNotInSet(int startPos, const std::string& s) const {
        return al_ustr_find_cset_cstr(m_ustr, startPos, s.c_str());
    }


    int UString::findNotInSet(int startPos, const std::string_view& s) const {
        return _do(s, [&](const char* s) { return al_ustr_find_cset_cstr(m_ustr, startPos, s); }, -1);
    }


    int UString::findNotInSet(int startPos, const ALLEGRO_USTR* str) const {
        return al_ustr_find_cset(m_ustr, startPos, str);
    }


    bool UString::replaceRange(int startPos, int endPos, int32_t c) {
        if (startPos >= 0 && endPos > startPos && (size_t)endPos <= size()) {
            for (; startPos < endPos; ) {
                startPos += al_ustr_set_chr(m_ustr, startPos, c);
            }
            return true;
        }
        return false;
    }


    bool UString::replaceRange(int startPos, int endPos, const char* s) {
        ALLEGRO_USTR_INFO info;
        const ALLEGRO_USTR* str = al_ref_cstr(&info, s);
        return al_ustr_replace_range(m_ustr, startPos, endPos, str);
    }


    bool UString::replaceRange(int startPos, int endPos, const std::string& s) {
        return replaceRange(startPos, endPos, s.c_str());
    }


    bool UString::replaceRange(int startPos, int endPos, const std::string_view& s) {
        return _do(s, [&](const char* s) { return replaceRange(startPos, endPos, s); }, false);
    }


    bool UString::replaceRange(int startPos, int endPos, const ALLEGRO_USTR* str) {
        return al_ustr_replace_range(m_ustr, startPos, endPos, str);
    }


    int UString::replace(int startPos, int32_t find, int32_t replace) {
        int pos = al_ustr_find_chr(m_ustr, startPos, find);
        if (pos >= 0) {
            return pos + al_ustr_set_chr(m_ustr, pos, replace);
        }
        return pos;
    }


    int UString::replace(int startPos, const char* find, const char* replace) {
        ALLEGRO_USTR_INFO findInfo;
        const ALLEGRO_USTR* findStr = al_ref_cstr(&findInfo, find);
        ALLEGRO_USTR_INFO replaceInfo;
        const ALLEGRO_USTR* replaceStr = al_ref_cstr(&replaceInfo, replace);
        return this->replace(startPos, findStr, replaceStr);
    }


    int UString::replace(int startPos, const std::string& find, const std::string& replace) {
        return this->replace(startPos, find.c_str(), replace.c_str());
    }


    int UString::replace(int startPos, const std::string_view& find, const std::string_view& replace) {
        return _do(find, [&](const char* find) {
            return replace.empty() ?
                this->replace(startPos, find, "") :
                _do(replace, [&](const char* replace) { return this->replace(startPos, find, replace); });
        }, false);
    }


    int UString::replace(int startPos, const ALLEGRO_USTR* find, const ALLEGRO_USTR* replace) {
        int pos = al_ustr_find_str(m_ustr, startPos, find);
        if (pos >= 0) {
            if (al_ustr_replace_range(m_ustr, pos, pos + al_ustr_size(find), replace)) {
                return pos + al_ustr_size(replace);
            }
        }
        return -1;
    }


    int UString::replaceReverse(int endPos, int32_t find, int32_t replace) {
        const int pos = al_ustr_rfind_chr(m_ustr, endPos, find);
        if (pos >= 0) {
            al_ustr_set_chr(m_ustr, pos, replace);
        }
        return pos;
    }


    int UString::replaceReverse(int endPos, const char* find, const char* replace) {
        ALLEGRO_USTR_INFO findInfo;
        const ALLEGRO_USTR* findStr = al_ref_cstr(&findInfo, find);
        ALLEGRO_USTR_INFO replaceInfo;
        const ALLEGRO_USTR* replaceStr = al_ref_cstr(&replaceInfo, replace);
        return replaceReverse(endPos, findStr, replaceStr);
    }


    int UString::replaceReverse(int endPos, const std::string& find, const std::string& replace) {
        return replaceReverse(endPos, find.c_str(), replace.c_str());
    }


    int UString::replaceReverse(int endPos, const std::string_view& find, const std::string_view& replace) {
        return _do(find, [&](const char* find) {
            return replace.empty() ?
                replaceReverse(endPos, find, "") :
                _do(replace, [&](const char* replace) { return replaceReverse(endPos, find, replace); });
        }, false);
    }


    int UString::replaceReverse(int endPos, const ALLEGRO_USTR* find, const ALLEGRO_USTR* replace) {
        const int pos = al_ustr_rfind_str(m_ustr, endPos, find);
        if (pos >= 0) {
            al_ustr_replace_range(m_ustr, pos, pos + al_ustr_size(find), replace);
        }
        return pos;
    }


    int UString::replaceAll(int startPos, int32_t find, int32_t replace) {
        for (;;) {
            int pos = al_ustr_find_chr(m_ustr, startPos, find);
            if (pos >= 0) {                
                startPos = pos + al_ustr_set_chr(m_ustr, pos, replace);
            }
            else {
                break;
            }
        }
        return startPos;
    }


    int UString::replaceAll(int startPos, const char* find, const char* replace) {
        ALLEGRO_USTR_INFO findInfo;
        const ALLEGRO_USTR* findStr = al_ref_cstr(&findInfo, find);
        ALLEGRO_USTR_INFO replaceInfo;
        const ALLEGRO_USTR* replaceStr = al_ref_cstr(&replaceInfo, replace);
        return replaceAll(startPos, findStr, replaceStr);
    }


    int UString::replaceAll(int startPos, const std::string& find, const std::string& replace) {
        return al_ustr_find_replace_cstr(m_ustr, startPos, find.c_str(), replace.c_str());
    }


    int UString::replaceAll(int startPos, const std::string_view& find, const std::string_view& replace) {
        return _do(find, [&](const char* find) {
            return replace.empty() ?
                replaceAll(startPos, find, "") :
                _do(replace, [&](const char* replace) { return replaceAll(startPos, find, replace); });
        }, false);
    }


    int UString::replaceAll(int startPos, const ALLEGRO_USTR* find, const ALLEGRO_USTR* replace) {
        for (;;) {
            int pos = al_ustr_find_str(m_ustr, startPos, find);
            if (pos >= 0) {
                al_ustr_replace_range(m_ustr, pos, pos + al_ustr_size(find), replace);
                startPos = pos + al_ustr_size(replace);
            }
            else {
                break;
            }
        }
        return startPos;
    }


    int UString::replaceAllReverse(int endPos, int32_t find, int32_t replace) {
        for (;;) {
            int pos = al_ustr_rfind_chr(m_ustr, endPos, find);
            if (pos >= 0) {
                al_ustr_set_chr(m_ustr, pos, replace);
                endPos = pos;
            }
            else {
                break;
            }
        }
        return endPos;
    }


    int UString::replaceAllReverse(int endPos, const char* find, const char* replace) {
        ALLEGRO_USTR_INFO findInfo;
        const ALLEGRO_USTR* findStr = al_ref_cstr(&findInfo, find);
        ALLEGRO_USTR_INFO replaceInfo;
        const ALLEGRO_USTR* replaceStr = al_ref_cstr(&replaceInfo, replace);
        return replaceAllReverse(endPos, findStr, replaceStr);
    }


    int UString::replaceAllReverse(int endPos, const std::string& find, const std::string& replace) {
        return replaceAllReverse(endPos, find.c_str(), replace.c_str());
    }


    int UString::replaceAllReverse(int endPos, const std::string_view& find, const std::string_view& replace) {
        return _do(find, [&](const char* find) {
            return replace.empty() ?
                replaceAllReverse(endPos, find, "") :
                _do(replace, [&](const char* replace) { return replaceAllReverse(endPos, find, replace); });
        }, false);
    }


    int UString::replaceAllReverse(int endPos, const ALLEGRO_USTR* find, const ALLEGRO_USTR* replace) {
        if (find && replace) {
            for (;;) {
                int pos = al_ustr_rfind_str(m_ustr, endPos, find);
                if (pos >= 0) {
                    al_ustr_replace_range(m_ustr, pos, pos + al_ustr_size(find), replace);
                    endPos = pos;
                }
                else {
                    break;
                }
            }
            return endPos;
        }
        return -1;
    }


    bool UString::operator == (int32_t c) const {
        return length() == 1 && compare(c) == 0;
    }


    bool UString::operator != (int32_t c) const {
        return length() != 1 || compare(c) != 0;
    }


    bool UString::operator < (int32_t c) const {
        return compare(c) < 0;
    }


    bool UString::operator <= (int32_t c) const {
        return operator < (c) || operator == (c);
    }


    bool UString::operator > (int32_t c) const {
        return compare(c) > 0;
    }


    bool UString::operator >= (int32_t c) const {
        return operator > (c) || operator == (c);
    }


    int UString::compare(int32_t c) const {
        return empty() ? -1 : operator [](0) - c;
    }


    bool UString::operator == (const char* str) const {
        ALLEGRO_USTR_INFO info;
        const ALLEGRO_USTR* ustr = al_ref_cstr(&info, str);
        return al_ustr_equal(m_ustr, ustr);
    }


    bool UString::operator != (const char* str) const {
        ALLEGRO_USTR_INFO info;
        const ALLEGRO_USTR* ustr = al_ref_cstr(&info, str);
        return !al_ustr_equal(m_ustr, ustr);
    }


    bool UString::operator < (const char* str) const {
        return compare(str) < 0;
    }


    bool UString::operator <= (const char* str) const {
        return compare(str) <= 0;
    }


    bool UString::operator > (const char* str) const {
        return compare(str) > 0;
    }


    bool UString::operator >= (const char* str) const {
        return compare(str) >= 0;
    }


    int UString::compare(const char* str) const {
        ALLEGRO_USTR_INFO info;
        const ALLEGRO_USTR* ustr = al_ref_cstr(&info, str);
        return al_ustr_compare(m_ustr, ustr);
    }


    bool UString::operator == (const std::string& str) const {
        return operator == (str.c_str());
    }


    bool UString::operator != (const std::string& str) const {
        return operator != (str.c_str());
    }


    bool UString::operator < (const std::string& str) const {
        return compare(str) < 0;
    }


    bool UString::operator <= (const std::string& str) const {
        return compare(str) <= 0;
    }


    bool UString::operator > (const std::string& str) const {
        return compare(str) > 0;
    }


    bool UString::operator >= (const std::string& str) const {
        return compare(str) >= 0;
    }


    int UString::compare(const std::string& str) const {
        return compare(str.c_str());
    }


    bool UString::operator == (const std::string_view& str) const {
        return compare(str) == 0;
    }


    bool UString::operator != (const std::string_view& str) const {
        return compare(str) != 0;
    }


    bool UString::operator < (const std::string_view& str) const {
        return compare(str) < 0;
    }


    bool UString::operator <= (const std::string_view& str) const {
        return compare(str) <= 0;
    }


    bool UString::operator > (const std::string_view& str) const {
        return compare(str) > 0;
    }


    bool UString::operator >= (const std::string_view& str) const {
        return compare(str) >= 0;
    }


    int UString::compare(const std::string_view& str) const {
        return str.empty() ?
            compare(al_ustr_empty_string()) :
            _do(str, [&](const char* str) { return compare(str); });
    }


    bool UString::operator == (const ALLEGRO_USTR* str) const {
        return al_ustr_equal(m_ustr, str);
    }


    bool UString::operator != (const ALLEGRO_USTR* str) const {
        return !al_ustr_equal(m_ustr, str);
    }


    bool UString::operator < (const ALLEGRO_USTR* str) const {
        return compare(str) < 0;
    }


    bool UString::operator <= (const ALLEGRO_USTR* str) const {
        return compare(str) <= 0;
    }


    bool UString::operator > (const ALLEGRO_USTR* str) const {
        return compare(str) > 0;
    }


    bool UString::operator >= (const ALLEGRO_USTR* str) const {
        return compare(str) >= 0;
    }


    int UString::compare(const ALLEGRO_USTR* str) const {
        return al_ustr_compare(m_ustr, str);
    }


    bool UString::startsWith(int32_t c) const {
        return al_ustr_get(m_ustr, 0) == c;
    }


    bool UString::startsWith(const char* s) const {
        return al_ustr_has_prefix_cstr(m_ustr, s);
    }


    bool UString::startsWith(const std::string& s) const {
        return startsWith(s.c_str());
    }


    bool UString::startsWith(const std::string_view& s) const {
        return _do(s, [&](const char* s) { return startsWith(s); }, false);
    }


    bool UString::startsWith(const ALLEGRO_USTR* str) const {
        return al_ustr_has_prefix(m_ustr, str);
    }


    bool UString::endsWith(int32_t c) const {
        return length() > 0 ? operator [](length() - 1) == c : false;
    }


    bool UString::endsWith(const char* s) const {
        return al_ustr_has_suffix_cstr(m_ustr, s);
    }


    bool UString::endsWith(const std::string& s) const {
        return endsWith(s.c_str());
    }


    bool UString::endsWith(const std::string_view& s) const {
        return _do(s, [&](const char* s) { return endsWith(s); }, false);
    }


    bool UString::endsWith(const ALLEGRO_USTR* str) const {
        return al_ustr_has_suffix(m_ustr, str);
    }


    UString UString::substring(int startPos, int endPos) const {
        return al_ustr_dup_substr(m_ustr, startPos, endPos);
    }


    UString UString::substring(int startPos) const {
        return al_ustr_dup_substr(m_ustr, startPos, al_ustr_size(m_ustr));
    }


} //namespace algui

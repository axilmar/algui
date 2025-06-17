#ifndef ALGUI_USTRING_HPP
#define ALGUI_USTRING_HPP


#include <string>
#include <string_view>
#include <allegro5/allegro.h>


namespace algui {


    /**
     * A wrapper around ALLEGRO_USTR.
     */
    class UString {
    public:
        /**
         * The default constructor.
         * Constructs an empty string.
         */
        UString();

        /**
         * Constructor from character.
         * @param c character.
         */
        UString(int32_t c);

        /**
         * Constructor from character and count.
         * @param count character count.
         * @param c character.
         */
        UString(size_t count, int32_t c);

        /**
         * Constructor from null-terminated string.
         * @param s null-terminated string; can be null.
         */
        UString(const char* s);

        /**
         * Constructor from string buffer.
         * @param s string; can be null.
         * @param size string size; can be 0.
         */
        UString(const char* s, size_t size);

        /**
         * Constructor from std::string.
         * @param s string.
         */
        UString(const std::string& s);

        /**
         * Constructor from std::string_view.
         * @param s string view.
         */
        UString(const std::string_view& s);

        /**
         * Constructor from allegro ustring.
         * @param ustr string.
         */
        UString(const ALLEGRO_USTR* ustr);

        /**
         * Constructor from format and arguments.
         * @param format printf-style format.
         * @param ... variable arguments to print.
         */
        UString(const char* format, ...);

        /**
         * Constructor from format and variable-argument list.
         * @param format printf-style format.
         * @param ap variable argument list with arguments to print.
         */
        UString(const char* format, va_list ap);

        /**
         * The copy constructor.
         * @param str the string.
         */
        UString(const UString& str);

        /**
         * The move constructor.
         * @param str the string.
         */
        UString(UString&& str) noexcept;

        /**
         * The destructor.
         * Frees the internal allegro string.
         */
        ~UString();

        /**
         * Assignment from null-terminated string.
         * The input string is copied.
         * @param s the string to copy.
         * @return reference to this.
         */
        UString& operator = (const char* s);

        /**
         * Assignment from std::string.
         * The input string is copied.
         * @param s the string to copy.
         * @return reference to this.
         */
        UString& operator = (const std::string& s);

        /**
         * Assignment from std::string_view.
         * The input string is copied.
         * @param s the string to copy.
         * @return reference to this.
         */
        UString& operator = (const std::string_view& s);

        /**
         * Assignment from allegro string.
         * The input string is copied.
         * @param s the string to copy.
         * @return reference to this.
         */
        UString& operator = (const ALLEGRO_USTR* str);

        /**
         * The copy assignment operator.
         * @param s the string to copy.
         * @return reference to this.
         */
        UString& operator = (const UString& str);

        /**
         * The move assignment operator.
         * @param s the string to move.
         * @return reference to this.
         */
        UString& operator = (UString&& str) noexcept;

        /**
         * The conversion to allegro ustr pointer.
         * @return pointer to the internal allegro ustr; never null.
         */
        operator const ALLEGRO_USTR* () const { return m_ustr; }

        /**
         * Returns the internal allegro ustr pointer.
         * @return pointer to the internal allegro ustr; never null.
         */
        const ALLEGRO_USTR* get() const { return m_ustr; }

        /**
         * Sets the string to the empty string.
         * @return true on success, false on failure.
         */
        bool reset();

        /**
         * Returns the pointer to the character data.
         * @return the pointer to the character data; never null.
         */
        const char* data() const;

        /**
         * Checks if the string is empty.
         * @return true if the string is empty, false otherwise.
         */
        bool empty() const;

        /**
         * Returns the size of the character data buffer, in bytes.
         * @return the size of the character data buffer, in bytes.
         */
        size_t size() const;

        /**
         * Returns the length of the string, in code points.
         * @return the length of the string, in code points.
         */
        size_t length() const;

        /**
         * Returns byte offset (a.k.a. position) from code point index.
         * @param index code point idnex.
         * @return byte offset (a.k.a. position) of code point index.
         */
        int getCodePointPosition(int index) const;

        /**
         * Moves the given position to the start of the previous code point.
         * @param pos current position; it can be in the middle of a code point; 
         *  on success, it contains the position of the previous code point.
         * @return true if the position was changed successfully, false otherwise.
         */
        bool moveToPreviousCodePoint(int& pos) const;

        /**
         * Moves the given position to the start of the next code point.
         * @param pos current position; it can be in the middle of a code point;
         *  on success, it contains the position of the next code point.
         * @return true if the position was changed successfully, false otherwise.
         */
        bool moveToNextCodePoint(int& pos) const;

        /**
         * Returns the code point at the given position.
         * @param pos position.
         * @return code point at the given position.
         */
        int32_t getCodePointAtPosition(int pos) const;

        /**
         * Returns a code point from code point index.
         * @param index code point index.
         * @return code point at specified index or -1 if the index was invalid.
         */
        int32_t operator [](int index) const;

        /**
         * Moves the given position to the previous code point and returns it.
         * @param pos current position; on success, the position of the previous code point.
         * @return the code point at the current position or -1/-2 if there was an error.
         */
        int32_t moveToPreviousCodePointAndGet(int& pos) const;

        /**
         * Returns the code point at the given position, then moves the given position to the next code point.
         * @param pos current position; on success, the position of the next code point.
         * @return the code point at the current position or -1/-2 if there was an error.
         */
        int32_t getCodePointAndMoveToNext(int& pos) const;

        /**
         * Inserts a character at the specified position.
         * @param pos position.
         * @param c character to insert.
         * @return true on success, false on failure.
         */
        bool insert(int pos, int32_t c);

        /**
         * Inserts a null-terminated string at the specified position.
         * @param pos position.
         * @param s null-terminated string to insert.
         * @return true on success, false on failure.
         */
        bool insert(int pos, const char* s);

        /**
         * Inserts an std::string at the specified position.
         * @param pos position.
         * @param s std::string to insert.
         * @return true on success, false on failure.
         */
        bool insert(int pos, const std::string& s);

        /**
         * Inserts an std::string_view at the specified position.
         * @param pos position.
         * @param s std::string_view to insert.
         * @return true on success, false on failure.
         */
        bool insert(int pos, const std::string_view& s);

        /**
         * Inserts an allegro ustr at the specified position.
         * @param pos position.
         * @param str allegro ustr to insert.
         * @return true on success, false on failure.
         */
        bool insert(int pos, const ALLEGRO_USTR* str);

        /**
         * Inserts a printf style string at the specified position.
         * @param pos position.
         * @param format printf-style format.
         * @param ... variable arguments to print.
         * @return true on success, false on failure.
         */
        bool insert(int pos, const char* format, ...);

        /**
         * Inserts a printf style string at the specified position.
         * @param pos position.
         * @param format printf-style format.
         * @param ap variable argument list with arguments to print.
         * @return true on success, false on failure.
         */
        bool insert(int pos, const char* format, va_list ap);

        /**
         * Inserts a character at the beginning of the string.
         * @param c character to insert.
         * @return true on success, false otherwise.
         */
        bool prepend(int32_t c);

        /**
         * Inserts a null-terminated string at the beginning of the string.
         * @param s string to insert.
         * @return true on success, false otherwise.
         */
        bool prepend(const char* s);

        /**
         * Inserts an std::string at the beginning of the string.
         * @param s std::string to insert.
         * @return true on success, false otherwise.
         */
        bool prepend(const std::string& s);

        /**
         * Inserts an std::string_view at the beginning of the string.
         * @param s std::string_view to insert.
         * @return true on success, false otherwise.
         */
        bool prepend(const std::string_view& s);

        /**
         * Inserts an allegro ustr at the beginning of the string.
         * @param str the allegro ustr to insert.
         * @return true on success, false otherwise.
         */
        bool prepend(const ALLEGRO_USTR* str);

        /**
         * Inserts a printf-style string at the beginning of the string.
         * @param format printf-style format.
         * @param ... variable arguments to print.
         * @return true on success, false otherwise.
         */
        bool prepend(const char* format, ...);

        /**
         * Inserts a printf-style string at the beginning of the string.
         * @param format printf-style format.
         * @param ap variable argument list with arguments to print.
         * @return true on success, false otherwise.
         */
        bool prepend(const char* format, va_list ap);

        /**
         * Inserts a character at the end of the string.
         * @param c character to insert.
         * @return true on success, false otherwise.
         */
        bool append(int32_t c);

        /**
         * Inserts a null-terminated string at the end of the string.
         * @param s string to insert.
         * @return true on success, false otherwise.
         */
        bool append(const char* s);

        /**
         * Inserts an std::string at the end of the string.
         * @param s std::string to insert.
         * @return true on success, false otherwise.
         */
        bool append(const std::string& s);

        /**
         * Inserts an std::string_view at the end of the string.
         * @param s std::string_view to insert.
         * @return true on success, false otherwise.
         */
        bool append(const std::string_view& s);

        /**
         * Inserts an allegro ustr at the end of the string.
         * @param str the allegro ustr to insert.
         * @return true on success, false otherwise.
         */
        bool append(const ALLEGRO_USTR* str);

        /**
         * Inserts a printf-style string at the end of the string.
         * @param format printf-style format.
         * @param ... variable arguments to print.
         * @return true on success, false otherwise.
         */
        bool append(const char* format, ...);

        /**
         * Inserts a printf-style string at the end of the string.
         * @param format printf-style format.
         * @param ap variable argument list with arguments to print.
         * @return true on success, false otherwise.
         */
        bool append(const char* format, va_list ap);

        /**
         * Removes a code point at the specified position.
         * @param pos position to remove a code point at.
         * @return true on success, false otherwise.
         */
        bool remove(int pos);

        /**
         * Removes the code points at the specified positions.
         * @param startPos start position of range.
         * @param endPos start position of range.
         * @return true on success, false otherwise.
         */
        bool removeRange(int startPos, int endPos);

        /**
         * Removes all the code points from the given position to the end of the string.
         * @param startPos start position.
         * @return true on success, false otherwise.
         */
        bool truncate(int startPos);

        /**
         * Trims the leading whitespace.
         * @return true on success, false otherwise.
         */
        bool trimLeadingWhitespace();

        /**
         * Trims the trailing whitespace.
         * @return true on success, false otherwise.
         */
        bool trimTrailingWhitespace();

        /**
         * Trims the leading and trailing whitespace.
         * @return true on success, false otherwise.
         */
        bool trimWhitespace();

        /**
         * Finds the position of a character.
         * @param startPos position to start the search from.
         * @param c character to find.
         * @return the found position or -1 if not found.
         */
        int find(int startPos, int32_t c) const;

        /**
         * Finds the position of a null-terminated string.
         * @param startPos position to start the search from.
         * @param s string to find.
         * @return the found position or -1 if not found.
         */
        int find(int startPos, const char* s) const;

        /**
         * Finds the position of an std::string.
         * @param startPos position to start the search from.
         * @param s string to find.
         * @return the found position or -1 if not found.
         */
        int find(int startPos, const std::string& s) const;

        /**
         * Finds the position of an std::string_view.
         * @param startPos position to start the search from.
         * @param s string to find.
         * @return the found position or -1 if not found.
         */
        int find(int startPos, const std::string_view& s) const;

        /**
         * Finds the position of an allegro ustr.
         * @param startPos position to start the search from.
         * @param str string to find.
         * @return the found position or -1 if not found.
         */
        int find(int startPos, const ALLEGRO_USTR* str) const;

        /**
         * Finds the position of a character in reverse direction.
         * @param startPos position to start the search from.
         * @param c character to find.
         * @return the found position or -1 if not found.
         */
        int findReverse(int endPos, int32_t c) const;

        /**
         * Finds the position of a null-terminated string in reverse direction.
         * @param startPos position to start the search from.
         * @param s string to find.
         * @return the found position or -1 if not found.
         */
        int findReverse(int endPos, const char* s) const;

        /**
         * Finds the position of an std::string in reverse direction.
         * @param startPos position to start the search from.
         * @param s string to find.
         * @return the found position or -1 if not found.
         */
        int findReverse(int endPos, const std::string& s) const;

        /**
         * Finds the position of an std::string_view in reverse direction.
         * @param startPos position to start the search from.
         * @param s string to find.
         * @return the found position or -1 if not found.
         */
        int findReverse(int endPos, const std::string_view& s) const;

        /**
         * Finds the position of an allegro ustr in reverse direction.
         * @param startPos position to start the search from.
         * @param str string to find.
         * @return the found position or -1 if not found.
         */
        int findReverse(int endPos, const ALLEGRO_USTR* str) const;

        /**
         * Finds the position of a character from a set of characters in a null-terminated string.
         * @param startPos position to start the search from.
         * @param s set of characters.
         * @return the found position or -1 if not found.
         */
        int findInSet(int startPos, const char* s) const;

        /**
         * Finds the position of a character from a set of characters in an std::string.
         * @param startPos position to start the search from.
         * @param s set of characters.
         * @return the found position or -1 if not found.
         */
        int findInSet(int startPos, const std::string& s) const;

        /**
         * Finds the position of a character from a set of characters in an std::string_view.
         * @param startPos position to start the search from.
         * @param s set of characters.
         * @return the found position or -1 if not found.
         */
        int findInSet(int startPos, const std::string_view& s) const;

        /**
         * Finds the position of a character from a set of characters in an allegro ustr.
         * @param startPos position to start the search from.
         * @param str set of characters.
         * @return the found position or -1 if not found.
         */
        int findInSet(int startPos, const ALLEGRO_USTR* str) const;

        /**
         * Finds the position of a character that is not in a set of characters in a null-terminated string.
         * @param startPos position to start the search from.
         * @param s set of characters.
         * @return the found position or -1 if not found.
         */
        int findNotInSet(int startPos, const char* s) const;

        /**
         * Finds the position of a character that is not in a set of characters in an std::string.
         * @param startPos position to start the search from.
         * @param s set of characters.
         * @return the found position or -1 if not found.
         */
        int findNotInSet(int startPos, const std::string& s) const;

        /**
         * Finds the position of a character that is not in a set of characters in an std::string_view.
         * @param startPos position to start the search from.
         * @param s set of characters.
         * @return the found position or -1 if not found.
         */
        int findNotInSet(int startPos, const std::string_view& s) const;

        /**
         * Finds the position of a character that is not in a set of characters in an std::string_view.
         * @param startPos position to start the search from.
         * @param str set of characters.
         * @return the found position or -1 if not found.
         */
        int findNotInSet(int startPos, const ALLEGRO_USTR* str) const;

        /**
         * Replaces the given range of code points with the given code point.
         * @param startPos start of range.
         * @param endPos end of range.
         * @param c code point to replace the code points in the above range.
         * @return true on success, false otherwise.
         */
        bool replaceRange(int startPos, int endPos, int32_t c);

        /**
         * Replaces the given range of code points with the given string.
         * @param startPos start of range.
         * @param endPos end of range.
         * @param s null-terminated string to replace the code points in the above range.
         * @return true on success, false otherwise.
         */
        bool replaceRange(int startPos, int endPos, const char* s);

        /**
         * Replaces the given range of code points with the given string.
         * @param startPos start of range.
         * @param endPos end of range.
         * @param s std::string to replace the code points in the above range.
         * @return true on success, false otherwise.
         */
        bool replaceRange(int startPos, int endPos, const std::string& s);

        /**
         * Replaces the given range of code points with the given string.
         * @param startPos start of range.
         * @param endPos end of range.
         * @param s std::string_view to replace the code points in the above range.
         * @return true on success, false otherwise.
         */
        bool replaceRange(int startPos, int endPos, const std::string_view& s);

        /**
         * Replaces the given range of code points with the given string.
         * @param startPos start of range.
         * @param endPos end of range.
         * @param str allegro ustr to replace the code points in the above range.
         * @return true on success, false otherwise.
         */
        bool replaceRange(int startPos, int endPos, const ALLEGRO_USTR* str);

        /**
         * Replaces the first occurrence of the given code point after the given start position.
         * @param startPos position to start the search from.
         * @param find the code point to replace.
         * @param replace the code point to replace the above code point with.
         * @return position after the replacement or -1 if not found.
         */
        int replace(int startPos, int32_t find, int32_t replace);

        /**
         * Replaces the first occurrence of the given null-terminated string after the given start position.
         * @param startPos position to start the search from.
         * @param find the string to replace.
         * @param replace the string to replace the above string with.
         * @return position after the replacement or -1 if not found.
         */
        int replace(int startPos, const char* find, const char* replace);

        /**
         * Replaces the first occurrence of the given std::string after the given start position.
         * @param startPos position to start the search from.
         * @param find the string to replace.
         * @param replace the string to replace the above string with.
         * @return position after the replacement or -1 if not found.
         */
        int replace(int startPos, const std::string& find, const std::string& replace);

        /**
         * Replaces the first occurrence of the given std::string_view after the given start position.
         * @param startPos position to start the search from.
         * @param find the string to replace.
         * @param replace the string to replace the above string with.
         * @return position after the replacement or -1 if not found.
         */
        int replace(int startPos, const std::string_view& find, const std::string_view& replace);

        /**
         * Replaces the first occurrence of the given allegro ustr after the given start position.
         * @param startPos position to start the search from.
         * @param find the string to replace.
         * @param replace the string to replace the above string with.
         * @return position after the replacement or -1 if not found.
         */
        int replace(int startPos, const ALLEGRO_USTR* find, const ALLEGRO_USTR* replace);

        /**
         * Replaces the last occurrence of the given code point before the given end position.
         * @param endPos position to start the search from.
         * @param find the code point to replace.
         * @param replace the code point to replace the above code point with.
         * @return position after the replacement or -1 if not found.
         */
        int replaceReverse(int endPos, int32_t find, int32_t replace);

        /**
         * Replaces the last occurrence of the given null-terminated string before the given end position.
         * @param endPos position to start the search from.
         * @param find the string to replace.
         * @param replace the string to replace the above string with.
         * @return position after the replacement or -1 if not found.
         */
        int replaceReverse(int endPos, const char* find, const char* replace);

        /**
         * Replaces the last occurrence of the given std::string before the given end position.
         * @param endPos position to start the search from.
         * @param find the string to replace.
         * @param replace the string to replace the above string with.
         * @return position after the replacement or -1 if not found.
         */
        int replaceReverse(int endPos, const std::string& find, const std::string& replace);

        /**
         * Replaces the last occurrence of the given std::string_view before the given end position.
         * @param endPos position to start the search from.
         * @param find the string to replace.
         * @param replace the string to replace the above string with.
         * @return position after the replacement or -1 if not found.
         */
        int replaceReverse(int endPos, const std::string_view& find, const std::string_view& replace);

        /**
         * Replaces the last occurrence of the given allegro ustr before the given end position.
         * @param endPos position to start the search from.
         * @param find the string to replace.
         * @param replace the string to replace the above string with.
         * @return position after the replacement or -1 if not found.
         */
        int replaceReverse(int endPos, const ALLEGRO_USTR* find, const ALLEGRO_USTR* replace);

        /**
         * Replaces all the occurrences of the given code point after the given start position.
         * @param startPos position to start the search from.
         * @param find the code point to replace.
         * @param replace the code point to replace the above code point with.
         * @return position after the replacement or -1 if not found.
         */
        int replaceAll(int startPos, int32_t find, int32_t replace);

        /**
         * Replaces all the occurrences of the given null-terminated string after the given start position.
         * @param startPos position to start the search from.
         * @param find the string to replace.
         * @param replace the string to replace the above string with.
         * @return position after the replacement or -1 if not found.
         */
        int replaceAll(int startPos, const char* find, const char* replace);

        /**
         * Replaces all the occurrences of the given std::string after the given start position.
         * @param startPos position to start the search from.
         * @param find the string to replace.
         * @param replace the string to replace the above string with.
         * @return position after the replacement or -1 if not found.
         */
        int replaceAll(int startPos, const std::string& find, const std::string& replace);

        /**
         * Replaces all the occurrences of the given std::string_view after the given start position.
         * @param startPos position to start the search from.
         * @param find the string to replace.
         * @param replace the string to replace the above string with.
         * @return position after the replacement or -1 if not found.
         */
        int replaceAll(int startPos, const std::string_view& find, const std::string_view& replace);

        /**
         * Replaces all the occurrences of the given allegro ustr after the given start position.
         * @param startPos position to start the search from.
         * @param find the string to replace.
         * @param replace the string to replace the above string with.
         * @return position after the replacement or -1 if not found.
         */
        int replaceAll(int startPos, const ALLEGRO_USTR* find, const ALLEGRO_USTR* replace);

        /**
         * Replaces all the occurrences of the given code point before the given end position.
         * @param endPos position to start the search from.
         * @param find the code point to replace.
         * @param replace the code point to replace the above code point with.
         * @return position after the replacement or -1 if not found.
         */
        int replaceAllReverse(int endPos, int32_t find, int32_t replace);

        /**
         * Replaces all the occurrences of the given null-terminated string before the given end position.
         * @param endPos position to start the search from.
         * @param find the string to replace.
         * @param replace the string to replace the above string with.
         * @return position after the replacement or -1 if not found.
         */
        int replaceAllReverse(int endPos, const char* find, const char* replace);

        /**
         * Replaces all the occurrences of the given std::string before the given end position.
         * @param endPos position to start the search from.
         * @param find the string to replace.
         * @param replace the string to replace the above string with.
         * @return position after the replacement or -1 if not found.
         */
        int replaceAllReverse(int endPos, const std::string& find, const std::string& replace);

        /**
         * Replaces all the occurrences of the given std::string_view before the given end position.
         * @param endPos position to start the search from.
         * @param find the string to replace.
         * @param replace the string to replace the above string with.
         * @return position after the replacement or -1 if not found.
         */
        int replaceAllReverse(int endPos, const std::string_view& find, const std::string_view& replace);

        /**
         * Replaces all the occurrences of the given allegro ustr before the given end position.
         * @param endPos position to start the search from.
         * @param find the string to replace.
         * @param replace the string to replace the above string with.
         * @return position after the replacement or -1 if not found.
         */
        int replaceAllReverse(int endPos, const ALLEGRO_USTR* find, const ALLEGRO_USTR* replace);

        /**
         * Checks if the string represents a single code point.
         * @param c code point to check against this string.
         * @return true on success, false on failure.
         */
        bool operator == (int32_t c) const;

        /**
         * Checks if the string does not represent a single code point.
         * @param c code point to check against this string.
         * @return true on success, false on failure.
         */
        bool operator != (int32_t c) const;

        /**
         * Checks if the string comes before a single code point.
         * @param c code point to check against this string.
         * @return true on success, false on failure.
         */
        bool operator < (int32_t c) const;

        /**
         * Checks if the string comes before a single code point or it is equal to the code point.
         * @param c code point to check against this string.
         * @return true on success, false on failure.
         */
        bool operator <= (int32_t c) const;

        /**
         * Checks if the string comes after a single code point.
         * @param c code point to check against this string.
         * @return true on success, false on failure.
         */
        bool operator > (int32_t c) const;

        /**
         * Checks if the string comes after a single code point or it is equal to the code point.
         * @param c code point to check against this string.
         * @return true on success, false on failure.
         */
        bool operator >= (int32_t c) const;

        /**
         * Computes the delta between the first code point of the string and the given code point.
         * @param c code point to check against this string.
         * @return the delta between the first code point of the string and the given code point;
         *  if the string is empty, then it returns -1.
         */
        int compare(int32_t c) const;

        /**
         * Checks if this and the given string are equal.
         * @param s the string to check against this.
         * @return true on success, false on failure.
         */
        bool operator == (const char* s) const;

        /**
         * Checks if this and the given string are different.
         * @param s the string to check against this.
         * @return true on success, false on failure.
         */
        bool operator != (const char* s) const;

        /**
         * Checks if this comes before the given string.
         * @param s the string to check against this.
         * @return true on success, false on failure.
         */
        bool operator < (const char* s) const;

        /**
         * Checks if this comes before the given string or they are equal.
         * @param s the string to check against this.
         * @return true on success, false on failure.
         */
        bool operator <= (const char* s) const;

        /**
         * Checks if this comes after the given string.
         * @param s the string to check against this.
         * @return true on success, false on failure.
         */
        bool operator > (const char* s) const;

        /**
         * Checks if this comes after the given string or they are equal.
         * @param s the string to check against this.
         * @return true on success, false on failure.
         */
        bool operator >= (const char* s) const;

        /**
         * Computes the delta between this and the given string.
         * @param s the string to check against this.
         * @return the delta between the two strings.
         */
        int compare(const char* s) const;

        /**
         * Checks if this and the given string are equal.
         * @param s the string to check against this.
         * @return true on success, false on failure.
         */
        bool operator == (const std::string& s) const;

        /**
         * Checks if this and the given string are different.
         * @param s the string to check against this.
         * @return true on success, false on failure.
         */
        bool operator != (const std::string& s) const;

        /**
         * Checks if this comes before the given string.
         * @param s the string to check against this.
         * @return true on success, false on failure.
         */
        bool operator < (const std::string& s) const;

        /**
         * Checks if this comes before the given string or they are equal.
         * @param s the string to check against this.
         * @return true on success, false on failure.
         */
        bool operator <= (const std::string& s) const;

        /**
         * Checks if this comes after the given string.
         * @param s the string to check against this.
         * @return true on success, false on failure.
         */
        bool operator > (const std::string& s) const;

        /**
         * Checks if this comes after the given string or they are equal.
         * @param s the string to check against this.
         * @return true on success, false on failure.
         */
        bool operator >= (const std::string& s) const;

        /**
         * Computes the delta between this and the given string.
         * @param s the string to check against this.
         * @return the delta between the two strings.
         */
        int compare(const std::string& s) const;

        /**
         * Checks if this and the given string are equal.
         * @param s the string to check against this.
         * @return true on success, false on failure.
         */
        bool operator == (const std::string_view& s) const;

        /**
         * Checks if this and the given string are different.
         * @param s the string to check against this.
         * @return true on success, false on failure.
         */
        bool operator != (const std::string_view& s) const;

        /**
         * Checks if this comes before the given string.
         * @param s the string to check against this.
         * @return true on success, false on failure.
         */
        bool operator < (const std::string_view& s) const;

        /**
         * Checks if this comes before the given string or they are equal.
         * @param s the string to check against this.
         * @return true on success, false on failure.
         */
        bool operator <= (const std::string_view& s) const;

        /**
         * Checks if this comes after the given string.
         * @param s the string to check against this.
         * @return true on success, false on failure.
         */
        bool operator > (const std::string_view& s) const;

        /**
         * Checks if this comes after the given string or they are equal.
         * @param s the string to check against this.
         * @return true on success, false on failure.
         */
        bool operator >= (const std::string_view& s) const;

        /**
         * Computes the delta between this and the given string.
         * @param s the string to check against this.
         * @return the delta between the two strings.
         */
        int compare(const std::string_view& s) const;

        /**
         * Checks if this and the given string are equal.
         * @param s the string to check against this.
         * @return true on success, false on failure.
         */
        bool operator == (const ALLEGRO_USTR* str) const;

        /**
         * Checks if this and the given string are different.
         * @param s the string to check against this.
         * @return true on success, false on failure.
         */
        bool operator != (const ALLEGRO_USTR* str) const;

        /**
         * Checks if this comes before the given string.
         * @param s the string to check against this.
         * @return true on success, false on failure.
         */
        bool operator < (const ALLEGRO_USTR* str) const;

        /**
         * Checks if this comes before the given string or they are equal.
         * @param s the string to check against this.
         * @return true on success, false on failure.
         */
        bool operator <= (const ALLEGRO_USTR* str) const;

        /**
         * Checks if this comes after the given string.
         * @param s the string to check against this.
         * @return true on success, false on failure.
         */
        bool operator > (const ALLEGRO_USTR* str) const;

        /**
         * Checks if this comes after the given string or they are equal.
         * @param s the string to check against this.
         * @return true on success, false on failure.
         */
        bool operator >= (const ALLEGRO_USTR* str) const;

        /**
         * Computes the delta between this and the given string.
         * @param s the string to check against this.
         * @return the delta between the two strings.
         */
        int compare(const ALLEGRO_USTR* str) const;

        /**
         * Checks if the given string starts with the given code point.
         * @param c code point to check.
         * @return true on success, false otherwise.
         */
        bool startsWith(int32_t c) const;

        /**
         * Checks if the given string starts with the given string.
         * @param s string to check.
         * @return true on success, false otherwise.
         */
        bool startsWith(const char* s) const;

        /**
         * Checks if the given string starts with the given string.
         * @param s string to check.
         * @return true on success, false otherwise.
         */
        bool startsWith(const std::string& s) const;

        /**
         * Checks if the given string starts with the given string.
         * @param s string to check.
         * @return true on success, false otherwise.
         */
        bool startsWith(const std::string_view& s) const;

        /**
         * Checks if the given string starts with the given string.
         * @param s string to check.
         * @return true on success, false otherwise.
         */
        bool startsWith(const ALLEGRO_USTR* str) const;

        /**
         * Checks if the given string ends with the given code point.
         * @param c code point to check.
         * @return true on success, false otherwise.
         */
        bool endsWith(int32_t c) const;

        /**
         * Checks if the given string ends with the given string.
         * @param s string to check.
         * @return true on success, false otherwise.
         */
        bool endsWith(const char* s) const;

        /**
         * Checks if the given string ends with the given string.
         * @param s string to check.
         * @return true on success, false otherwise.
         */
        bool endsWith(const std::string& s) const;

        /**
         * Checks if the given string ends with the given string.
         * @param s string to check.
         * @return true on success, false otherwise.
         */
        bool endsWith(const std::string_view& s) const;

        /**
         * Checks if the given string ends with the given string.
         * @param s string to check.
         * @return true on success, false otherwise.
         */
        bool endsWith(const ALLEGRO_USTR* str) const;

        /**
         * Returns a substring from the given code point range.
         * @param startPos start of range.
         * @param endPos end of range.
         * @return substring at the given range.
         */
        UString substring(int startPos, int endPos) const;

        /**
         * Returns a substring from the given start position up to the end of the string.
         * @param startPos start of range.
         * @return substring from the given position up to the end of the string.
         */
        UString substring(int startPos) const;

    private:
        ALLEGRO_USTR* m_ustr;

        UString(ALLEGRO_USTR* str) : m_ustr(str) {}
    };


} //namespace algui


#endif //ALGUI_USTRING_HPP

#ifndef UTIL_FUNCS_HPP
#define UTIL_FUNCS_HPP

// serialization helper
template <typename T>
static void copyIntoBytes(const T &val, char **dst) {
    const char *data = reinterpret_cast<const char *>(&val);
    for (int i = 0; i < sizeof(T); ++i)
        *((*dst)++) = data[i];
}

// deserialization helper
template <typename T>
static void copyFromBytes(T &val, const char **src) {
    char *data = reinterpret_cast<char *>(&val);
    for (int i = 0; i < sizeof(T); ++i)
        data[i] = *((*src)++);
}

#endif  // UTIL_FUNCS_HPP

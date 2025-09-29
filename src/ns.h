#ifdef YORU_STRIP_NAMESPACE
    #define YORU_NS(x) x

#else
    #define YORU_NS(x) yoru_##x

#endif

#ifdef YORU_STRIP_NAMESPACE
    #define ns(x) x

#else
    #define ns(x) yoru_##x

#endif

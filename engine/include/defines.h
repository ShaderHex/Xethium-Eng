#ifdef _WIN32
    #ifdef BUILDING_XETHIUMLIB
        #define XAPI __declspec(dllexport)
    #else
        #define XAPI __declspec(dllimport)
    #endif
#else
    #define XAPI __attribute__((visibility("default")))
#endif

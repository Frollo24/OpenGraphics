#pragma once

#ifdef OG_BUILD_SHARED
    #define OPEN_API __declspec(dllexport)
#else
    #define OPEN_API __declspec(dllimport)
#endif
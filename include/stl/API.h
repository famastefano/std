#pragma once

#ifdef STL_API_EXPORTS
#  define STL_API __declspec(dllexport)
#else
#  define STL_API __declspec(dllimport)
#endif

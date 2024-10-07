#pragma once

#ifdef STD_API_EXPORTS
#  define STD_API __declspec(dllexport)
#else
#  define STD_API __declspec(dllimport)
#endif

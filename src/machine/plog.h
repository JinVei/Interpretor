#ifndef  _PLOG_H_
#define  _PLOG_H_
#include <stdio.h>
#include <time.h>

inline void GetTime(char* pszTimebuf, int nBuflen)
{
    time_t tmTime;
    time(&tmTime);
    strftime(pszTimebuf, nBuflen, "%Y-%m-%d %H:%M:%S", localtime(&tmTime));
}

inline void GetTime_s(char* pszTimebuf, int nBuflen)
{
    time_t tmTime;
    time(&tmTime);
    strftime(pszTimebuf, nBuflen, "%Y%m%d%H%M%S", localtime(&tmTime));
}

inline void GetTime_h(char* pszTimebuf, int nBuflen)
{
    time_t tmTime;
    time(&tmTime);
    strftime(pszTimebuf, nBuflen, "%Y%m%d%H", localtime(&tmTime));
}
#define _INTEGER_TO_CHARS1_(X) #X
#define _INTEGER_TO_CHARS_(X) _INTEGER_TO_CHARS1_(X)
#ifdef __GNUC__
#define PLOG_FUNCTION_LOCATION_INFO __FILE__ " " _LINR_TO_CHARS_(__LINE__) " in " __PRETTY_FUNCTION__
#define Plog(format, ...)\
    do{\
        char szTime[64];\
        GetTime(szTime, 64);\
        fprintf(stderr, "%s " PLOG_HEADER_STRING "\n" format "\n", szTime, ##__VA_ARGS__);\
    }while(0)
#elif _WINDOWS
#define PLOG_FUNCTION_LOCATION_INFO __FILE__ " " _INTEGER_TO_CHARS_(__LINE__) " in " __FUNCTION__
#define Plog(format, ...)\
    do{\
        char szTime[64];\
        GetTime(szTime, 64);\
        fprintf(stderr, "%s " PLOG_FUNCTION_LOCATION_INFO "\n" format "\n", szTime, ##__VA_ARGS__);\
    }while(0)
#endif //__GNUC__

#endif//_PLOG_H_
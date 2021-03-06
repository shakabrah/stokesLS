#ifndef __MISCIO_H__
#define __MISCIO_H__

#include <string>

namespace levelset {
        
    const char* FileName(const char* base, const int num, const char* suffix, char* result);
    std::string FileName(std::string base, const int num, std::string suffix);
    const char* dtoa(const double a, char* result);

}
#endif //__MISCIO_H__

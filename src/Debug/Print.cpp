/**
    Cucca Game Engine - Debug - Print.cpp

    This file is subject to the terms and conditions defined in
    file 'LICENSE.txt', which is part of this source code package.

    @version    0.1
    @author     Miika Lehtim�ki
    @date       2015-08-12
**/


#include <Cucca/Debug/Print.hpp>

#include <thread>
#include <ctime>
#include <iomanip>
#include <chrono>


std::recursive_mutex Cucca::debugPrintMutex;


/// Function definitions
std::string Cucca::debugPrintPrefix(void) {
    std::time_t now_c = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm* t = std::localtime(&now_c);

    std::stringstream ss;
    ss  << std::setfill('0') << std::setw(2)
        << t->tm_hour << '-' << t->tm_min << '-' << t->tm_sec   //  time
        << " THR#" << std::this_thread::get_id() << " | ";       //  thread id
    return ss.str();
}


/// Template specializations
namespace Cucca {

    template<>
    void debugPrintFormat(const char* format, bool first) {
        std::lock_guard<std::recursive_mutex> lock(debugPrintMutex);

        if (first)
            printf(debugPrintPrefix().c_str());

        printf(format);
        printf("\n");
    }

    //  FormatString values
    template<> const char* FormatString<int>::value(void)                       { return "%d"; }
    template<> const char* FormatString<long int>::value(void)                  { return "%ld"; }
    template<> const char* FormatString<long long int>::value(void)             { return "%lld"; }
    template<> const char* FormatString<unsigned int>::value(void)              { return "%u"; }
    template<> const char* FormatString<long unsigned int>::value(void)         { return "%lu"; }
    template<> const char* FormatString<long long unsigned int>::value(void)    { return "%llu"; }
    template<> const char* FormatString<float>::value(void)                     { return "%0.4f"; }
    template<> const char* FormatString<double>::value(void)                    { return "%g"; }
    template<> const char* FormatString<char>::value(void)                      { return "%c"; }
    template<> const char* FormatString<bool>::value(void)                      { return "%d"; }

}   //  namespace Cucca

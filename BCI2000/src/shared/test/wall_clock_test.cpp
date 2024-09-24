#include <iostream>

#include "TimeUtils.h"
#include "ThreadUtils.h"
#include <ctime>

int main(int argc, char* argv[])
{
    while (true)
    {
        Time t = TimeUtils::SystemTimeUTC();
        ::time_t now = t.Seconds();
        tm* tm_ = ::localtime(&now);
        std::cout << ::asctime(tm_) << std::flush;
        ThreadUtils::SleepFor(Time::Seconds(1));
    }
    return 0;
}

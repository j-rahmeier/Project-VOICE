#include <iostream>

#include "TimeUtils.h"
#include "ThreadUtils.h"

int main(int argc, char* argv[])
{
    while (true)
    {
        Time t = TimeUtils::MonotonicTime();
        double d = t.Seconds();
        std::cout << ::floor(d) << std::endl;
        ThreadUtils::SleepFor(Time::Seconds(1));
    }
    return 0;
}

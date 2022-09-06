//-------------------------------------------------------------------------
//
// The MIT License (MIT)
//
// Copyright (c) 2022 Andrew Duncan
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------------

#include <cstdint>
#include <fstream>
#include <regex>

#include "system.h"

//-------------------------------------------------------------------------

int16_t
ogsinf::
getTemperature()
{
    int millidegrees = 0;

    std::ifstream ifs{"/sys/class/thermal/thermal_zone0/temp"};

    if (ifs.is_open())
    {
        ifs >> millidegrees;
    }

    return static_cast<int16_t>((millidegrees + 500) / 1000);
}

//-------------------------------------------------------------------------

ogsinf::BatteryInfo
ogsinf::
getBatteryInfo()
{
    int percent = 0;
    bool isCharging = false;

    {
        std::ifstream ifs{"/sys/class/power_supply/ac/uevent"};

        if (ifs.is_open())
        {
            std::string line;
            while (getline(ifs, line))
            {
                try
                {
                    if (line.rfind("POWER_SUPPLY_STATUS=", 0) == 0)
                    {
                        std::regex pattern{R"(POWER_SUPPLY_STATUS=([a-zA-Z]+))"};
                        std::smatch match;
       
                        if (std::regex_search(line, match, pattern) &&
                            (match.size() == 2))
                        {
                            isCharging = (match[1].str() != "Discharging");
                        }
                    }
                }
                catch (std::exception&)
                {
                    // ignore
                }
            }
        }
    }

    {
        std::ifstream ifs{"/sys/class/power_supply/battery/uevent"};

        if (ifs.is_open())
        {
            std::string line;
            while (getline(ifs, line))
            {
                try
                {
                    if (line.rfind("POWER_SUPPLY_CAPACITY=", 0) == 0)
                    {
                        std::regex pattern{R"(POWER_SUPPLY_CAPACITY=(\d+))"};
                        std::smatch match;
       
                        if (std::regex_search(line, match, pattern) &&
                            (match.size() == 2))
                        {
                            percent = std::stoi(match[1].str());
                        }
                    }
                }
                catch (std::exception&)
                {
                    // ignore
                }
            }
        }
    }

    return BatteryInfo{static_cast<int16_t>(percent), isCharging};
}


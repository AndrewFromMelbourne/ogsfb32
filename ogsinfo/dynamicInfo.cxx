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

#include <cmath>
#include <cstdint>
#include <iomanip>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>

#include <ifaddrs.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>

#include "dynamicInfo.h"
#include "system.h"

//-------------------------------------------------------------------------

std::string
DynamicInfo::
getIpAddress(
    char& interface)
{
    struct ifaddrs *ifaddr = nullptr;
    struct ifaddrs *ifa = nullptr;
    interface = 'X';

    std::string address = "   .   .   .   ";

    ::getifaddrs(&ifaddr);

    for (ifa = ifaddr ; ifa != nullptr ; ifa = ifa->ifa_next)
    {
        if (ifa ->ifa_addr->sa_family == AF_INET)
        {
            void *addr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;

            if (strcmp(ifa->ifa_name, "lo") != 0)
            {
                char buffer[INET_ADDRSTRLEN];
                ::inet_ntop(AF_INET, addr, buffer, sizeof(buffer));
                address = buffer;
                interface = ifa->ifa_name[0];
                break;
            }
        }
    }

    if (ifaddr != nullptr)
    {
        ::freeifaddrs(ifaddr);
    }

    return address;
}

//-------------------------------------------------------------------------

void
DynamicInfo::
drawIpAddress(
    ogsfb32::FontPoint& position)
{
    position = drawString(position,
                          "ip(",
                          m_heading,
                          getImage());

    char interface = ' ';
    std::string ipaddress = getIpAddress(interface);

    position = drawChar(position,
                        interface,
                        m_foreground,
                        getImage());

    position = drawString(position,
                          ") ",
                          m_heading,
                          getImage());

    position = drawString(position,
                          ipaddress + " ",
                          m_foreground,
                          getImage());

}

//-------------------------------------------------------------------------

std::string
DynamicInfo::
getTemperature()
{
    return std::to_string(ogsinf::getTemperature());
}

//-------------------------------------------------------------------------

void
DynamicInfo::
drawTemperature(
    ogsfb32::FontPoint& position)
{
    position = drawString(position,
                          "temperature ",
                          m_heading,
                          getImage());

    std::string temperatureString = getTemperature();

    position = drawString(position,
                          temperatureString,
                          m_foreground,
                          getImage());

    uint8_t degreeSymbol = 0xF8;

    position = drawChar(position,
                        degreeSymbol,
                        m_foreground,
                        getImage());

    position = drawString(position,
                          "C ",
                          m_foreground,
                          getImage());

}

//-------------------------------------------------------------------------

std::string
DynamicInfo::
getTime(
    time_t now)
{
    char buffer[128];

    struct tm result;
    struct tm *lt = ::localtime_r(&now, &result);
    std::strftime(buffer, sizeof(buffer), "%T", lt);

    return buffer;
}

//-------------------------------------------------------------------------

void
DynamicInfo::
drawTime(
    ogsfb32::FontPoint& position,
    time_t now)
{
    position = drawString(position,
                          "time ",
                          m_heading,
                          getImage());

    std::string timeString = getTime(now);

    position = drawString(position,
                          timeString + " ",
                          m_foreground,
                          getImage());
}

//-------------------------------------------------------------------------

void
DynamicInfo::
drawBatteryCharge(
    ogsfb32::FontPoint& position)
{
    auto bi = ogsinf::getBatteryInfo();

    position = drawString(position,
                          " battery ",
                          m_heading,
                          getImage());

    position = drawString(position,
                          std::to_string(bi.charge) + "% ",
                          m_foreground,
                          getImage());

    if (bi.isCharging)
    {
        position = drawBattery(position,
                               bi.charge,
                               m_heading.get8880(),
                               getImage());
   }
   else if (bi.charge > 10)
   {

        position = drawBattery(position,
                               bi.charge,
                               m_foreground.get8880(),
                               getImage());
   }
   else
   {

        position = drawBattery(position,
                               bi.charge,
                               m_warning.get8880(),
                               getImage());
   }

    position = drawString(position,
                          " ",
                          m_foreground,
                          getImage());
}

//-------------------------------------------------------------------------

DynamicInfo::
DynamicInfo(
    int16_t width,
    int16_t yPosition)
:
    Panel{width, ogsfb32::sc_fontHeight + 4, yPosition},
    m_heading(255, 255, 0),
    m_foreground(255, 255, 255),
    m_warning(255, 0, 0),
    m_background(0, 0, 0)
{
}

//-------------------------------------------------------------------------

void
DynamicInfo::
update(
    time_t now)
{
    getImage().clear(m_background);

    //---------------------------------------------------------------------

    ogsfb32::FontPoint position = { 0, 0 };
    drawIpAddress(position);
    drawTime(position, now);
    drawTemperature(position);
    drawBatteryCharge(position);
}


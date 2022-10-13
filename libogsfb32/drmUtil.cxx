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

#include "drmUtil.h"

//-------------------------------------------------------------------------

drm::drmModeConnector_ptr
drm::drmModeGetConnector(
    ogsfb32::FileDescriptor& fd,
    uint32_t connId)
{
    return drmModeConnector_ptr(::drmModeGetConnector(fd.fd(), connId),
                                &drmModeFreeConnector);
}

//-------------------------------------------------------------------------

drm::drmModeCrtc_ptr
drm::drmModeGetCrtc(
    ogsfb32::FileDescriptor& fd,
    uint32_t crtcId)
{
    return drmModeCrtc_ptr(::drmModeGetCrtc(fd.fd(), crtcId),
                           &drmModeFreeCrtc);
}

//-------------------------------------------------------------------------

drm::drmModeEncoder_ptr
drm::drmModeGetEncoder(
    ogsfb32::FileDescriptor& fd,
    uint32_t encoderId)
{
    return drmModeEncoder_ptr(::drmModeGetEncoder(fd.fd(), encoderId),
                              &drmModeFreeEncoder);
}

//-------------------------------------------------------------------------

drm::drmModeObjectProperties_ptr
drm::drmModeObjectGetProperties(
    ogsfb32::FileDescriptor& fd,
    uint32_t objectId,
    uint32_t objectType)
{
    return drmModeObjectProperties_ptr(::drmModeObjectGetProperties(fd.fd(), objectId, objectType),
                                       &drmModeFreeObjectProperties);
}

//-------------------------------------------------------------------------

drm::drmModePlane_ptr
drm::drmModeGetPlane(
    ogsfb32::FileDescriptor& fd,
    uint32_t planeId)
{
    return drmModePlane_ptr(::drmModeGetPlane(fd.fd(), planeId),
                            &drmModeFreePlane);
}

//-------------------------------------------------------------------------

drm::drmModePlaneRes_ptr
drm::drmModeGetPlaneResources(
    ogsfb32::FileDescriptor& fd)
{
    return drmModePlaneRes_ptr(::drmModeGetPlaneResources(fd.fd()),
                               &drmModeFreePlaneResources);
}

//-------------------------------------------------------------------------

drm::drmModePropertyRes_ptr
drm::drmModeGetProperty(
    ogsfb32::FileDescriptor& fd,
    uint32_t propertyId)
{
    return drmModePropertyRes_ptr(::drmModeGetProperty(fd.fd(), propertyId),
                                  &drmModeFreeProperty);
}

//-------------------------------------------------------------------------

drm::drmModeRes_ptr
drm::drmModeGetResources(
    ogsfb32::FileDescriptor& fd)
{
    return drmModeRes_ptr(::drmModeGetResources(fd.fd()),
                          &drmModeFreeResources);
}

//-------------------------------------------------------------------------

uint64_t
drm::drmGetPropertyValue(
    ogsfb32::FileDescriptor& fd,
    uint32_t objectId,
    uint32_t objectType,
    const std::string& name)
{
    auto properties = drmModeObjectGetProperties(fd, objectId, objectType);

    for (uint32_t i = 0; i < properties->count_props; ++i)
    {
        auto property = drmModeGetProperty(fd, properties->props[i]);

        if (name == property->name)
        {
            return properties->prop_values[i];
        }
    }

    return ~0;
}


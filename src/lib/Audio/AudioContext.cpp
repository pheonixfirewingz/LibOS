#include "../InternalRefractile.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <RefractileAPI.h>
#include <cstring>
#include <memory>
#include <vector>

struct refAudioDevice_T
{
    const char *name;
    ALCdevice *device;
    bool initialized{false};
    refAudioDevice_T() = default;
    refAudioDevice_T(const char *name_in)
        : name(name_in){};
};

bool device_is_set{false};
refAudioDevice_T device;

losResult refAppendAudioContext(refHandle)
{
    return LOS_SUCCESS;
}

losResult refGetAudioDeviceList(refHandle, refAudioDevice *devices_list)
{
    if (alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT"))
    {
        const ALchar *devices = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
        const ALCchar *device = devices, *next = devices + 1;
        uint64 device_count = 0;
        while (device && *device != '\0' && next && *next != '\0')
        {
            size len = strlen(device);
            device += (len + 1);
            next += (len + 2);
            device_count++;
        }
        device = devices, next = devices + 1;
        (*devices_list) = new refAudioDevice_T[device_count];
        uint64 i = 0;
        while (device && *device != '\0' && next && *next != '\0')
        {
            size len = strlen(device);
            (*devices_list)[i].name = std::string(device, len).c_str();
            device += (len + 1);
            next += (len + 2);
            i++;
        }

        (void)alGetError();
        [[unlikely]] if (!(*devices_list)) return LOS_ERROR_COULD_NOT_INIT;
        return LOS_SUCCESS;
    }
    else
    {
        (*devices_list) = new refAudioDevice_T(alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER));
        (void)alGetError();
        return LOS_SUCCESS;
    }
}

losResult refSetAudioDevice(refHandle handle, refAudioDevice dev, uint8 offset)
{
    if (device_is_set)
    {
        printf("LibOS: Open AL Info - audio device is already set a new device cannot be set until after "
               "refUnsetAudioDevice is called");
        return LOS_SUCCESS;
    }
    device_is_set = true;
    auto &name = dev[offset].name;
    dev[offset].device = alcOpenDevice(name);
    if (dev[offset].device == NULL)
    {
        ALCenum error = alGetError();
        printf("LibOS: Open AL Error - %x\n", error);
        return LOS_ERROR_COULD_NOT_INIT;
    }
    handle->audio_context = alcCreateContext(dev[offset].device, NULL);
    if (handle->audio_context == NULL)
    {
        ALCenum error = alGetError();
        printf("LibOS: Open AL Error - %x\n", error);
        return LOS_ERROR_COULD_NOT_INIT;
    }
    alcMakeContextCurrent(handle->audio_context);
    alutInit(NULL,NULL);
    dev[offset].initialized = true;

    device = dev[offset];
    (void)alGetError();
    return LOS_SUCCESS;
}

losResult refUnsetAudioDevice(refHandle handle)
{
    if (!device_is_set)
    {
        printf("LibOS: Open AL Info - audio device is already unset a new device cannot be unset until after "
               "refSetAudioDevice is called");
        return LOS_SUCCESS;
    }
    device_is_set = false;
    alutExit();
    alcMakeContextCurrent(NULL);
    alcDestroyContext((*handle).audio_context);
    alcCloseDevice(device.device);
    device.initialized = false;
    device = NULL;
    return LOS_SUCCESS;
}

losResult refUnAppendAudioContext(refHandle)
{
    return LOS_SUCCESS;
}
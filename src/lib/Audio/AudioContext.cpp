#include <AL/al.h>
#include <AL/alc.h>
#include <RefractileAPI.h>
#include "../InternalRefractile.h"
#include <cstring>
#include <memory>
#include <vector>


struct refAudioDevice_T
{
    const char *name;
    ALCdevice *device;
    bool initialized{false};
    explicit refAudioDevice_T(const char *name_in)
        : name(name_in){};
};

bool device_is_set{false};

losResult refAppendAudioContext(refHandle)
{
    return LOS_SUCCESS;
}

losResult refGetAudioDeviceList(refHandle, refAudioDevice *devices_list, uint8 count)
{
    std::vector<refAudioDevice_T> v_devices;
    const ALchar *devices = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
    const ALCchar *device = devices, *next = devices + 1;
    while (device && *device != '\0' && next && *next != '\0')
    {
        size len = strlen(device);
        v_devices.push_back(refAudioDevice_T(device));
        device += (len + 1);
        next += (len + 2);
    }

    count = v_devices.size();

    if (count == 0)
        return LOS_ERROR_COULD_NOT_INIT;

    *devices_list = std::move(v_devices.data());
    return LOS_SUCCESS;
}

losResult refSetAudioDevice(refHandle handle, refAudioDevice dev)
{
    if (device_is_set)
    {
        printf("LibOS: Open AL Info - audio device is already set a new device cannot be set until after "
               "refUnsetAudioDevice is called");
        return LOS_SUCCESS;
    }
    device_is_set = true;
    dev->device = alcOpenDevice(dev->name);
    ALCenum error = alGetError();
    if (error != AL_NO_ERROR)
    {
        printf("LibOS: Open AL Error - %x", error);
        return LOS_ERROR_COULD_NOT_INIT;
    }
    handle->audio_context = alcCreateContext(dev->device,NULL);
    error = alGetError();
    if (error != AL_NO_ERROR)
    {
        printf("LibOS: Open AL Error - %x", error);
        return LOS_ERROR_COULD_NOT_INIT;
    }
    dev->initialized = true;
    return LOS_SUCCESS;
}

losResult refUnsetAudioDevice(refHandle handle, refAudioDevice dev)
{
    if (!device_is_set)
    {
        printf("LibOS: Open AL Info - audio device is already unset a new device cannot be unset until after "
               "refSetAudioDevice is called");
        return LOS_SUCCESS;
    }
    device_is_set = false;
    alcDestroyContext((*handle).audio_context);
    ALCenum error = alGetError();
    if (error != AL_NO_ERROR)
    {
        printf("LibOS: Open AL Error - %x", error);
        return LOS_ERROR_COULD_NOT_DESTORY;
    }
    alcCloseDevice(dev->device);
    error = alGetError();
    if (error != AL_NO_ERROR)
    {
        printf("LibOS: Open AL Error - %x", error);
        return LOS_ERROR_COULD_NOT_DESTORY;
    }
    dev->initialized = false;
    return LOS_SUCCESS;
}

losResult refSetAudioDeviceSoundLevel(refAudioDevice, float64)
{
    return LOS_SUCCESS;
}

losResult refUnAppendAudioContext(refHandle)
{
    return LOS_SUCCESS;
}
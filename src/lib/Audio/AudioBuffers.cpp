#include "../IFileIO.h"
#include "../InternalRefractile.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <RefractileAPI.h>

struct refAudioBuffer_T
{
    ALuint buffer = 0;
};

struct refAudioPlayer_T
{
    ALuint source = 0;
    bool paused = false;
    bool initialized = false;
};
#define TEST_ERROR(_msg, x)                                  \
    error = alGetError();                                    \
    if (error != AL_NO_ERROR)                                \
    {                                                        \
        fprintf(stderr, "LIBOS - OpenAL Error: %s\n", _msg); \
        return x;                                            \
    }

losResult refCreateAudioBuffer(refAudioBuffer *buffer, refCreateAudioBufferInfo info)
{
    ALCenum error;
    *buffer = new refAudioBuffer_T();
    alGenBuffers(1, &(*buffer)->buffer);
    TEST_ERROR("buffer generation", LOS_ERROR_COULD_NOT_INIT);
    ALsizei v_size;
    ALfloat freq;
    ALenum format;
    ALvoid *data;

    auto path = getCorrectPath(info.audioFile);
    data = alutLoadMemoryFromFile(path.c_str(), &format, &v_size, &freq);
    TEST_ERROR("loading wav file", LOS_ERROR_COULD_NOT_INIT);
    alBufferData((*buffer)->buffer, format, data, v_size, freq);
    TEST_ERROR("buffer copy", LOS_ERROR_COULD_NOT_INIT);

    return LOS_SUCCESS;
}

losResult refDestroyAudioBuffer(refAudioBuffer buffer)
{
    alDeleteBuffers(1, &buffer->buffer);
    return LOS_SUCCESS;
}

losResult refPlay(refAudioPlayer *player, refAudioBuffer, float64 x, float64 y, float64 z, uint8 pitch)
{
    ALCenum error;
    if (!(*player))
        *player = new refAudioPlayer_T();

    [[likely]] if ((*player)->paused)
    {
        (*player)->paused = false;
        alSourcePlay((*player)->source);
    }
    else
    {
        [[likely]] if ((*player)->initialized)
        {
            return LOS_SUCCESS;
        }
        else
        {
            alGenSources((ALuint)1, &(*player)->source);
            TEST_ERROR("source generation", LOS_ERROR_COULD_NOT_INIT);
            alSourcef((*player)->source, AL_PITCH, pitch);
            TEST_ERROR("(*player)->source pitch", LOS_ERROR_COULD_NOT_INIT);
            alSourcef((*player)->source, AL_GAIN, 1);
            TEST_ERROR("source gain", LOS_ERROR_COULD_NOT_INIT);
            alSource3f((*player)->source, AL_POSITION, x, y, z);
            TEST_ERROR("(*player)->source position", LOS_ERROR_COULD_NOT_INIT);
            alSource3f((*player)->source, AL_VELOCITY, 0, 0, 0);
            TEST_ERROR("source velocity", LOS_ERROR_COULD_NOT_INIT);
            alSourcei((*player)->source, AL_LOOPING, AL_FALSE);
            TEST_ERROR("source looping", LOS_ERROR_COULD_NOT_INIT);
            alSourcePlay((*player)->source);
            (*player)->initialized = true;
        }
    }
    return LOS_SUCCESS;
}

losResult refPause(refAudioPlayer player)
{
    if (!player->paused)
        return LOS_SUCCESS;
    player->paused = true;
    alSourcePause(player->source);
    return LOS_SUCCESS;
}

losResult refResume(refAudioPlayer player)
{
    return refPlay(&player, nullptr, 0, 0, 0, 0);
}

losResult refStop(refAudioPlayer player)
{
    if(!player->initialized)
       return LOS_SUCCESS;
    
    player->initialized = false;
    player->paused = false;

    alDeleteSources(1, &player->source);

    delete player;
    return LOS_SUCCESS;
}
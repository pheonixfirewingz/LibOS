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
#define TEST_ERROR(_msg, x)                                                    \
    error = alGetError();                                                      \
    if (error != AL_NO_ERROR)                                                  \
    {                                                                          \
        fprintf(stderr, "LIBOS - OpenAL Error: %s - code: %x\n", _msg, error); \
        return x;                                                              \
    }

losResult refCreateAudioBuffer(refAudioBuffer *buffer, const char * audioFile)
{
    *buffer = new refAudioBuffer_T();
    (*buffer)->buffer = alutCreateBufferFromFile(getCorrectPath(audioFile).c_str());
    if ((*buffer)->buffer == AL_NONE)
    {
        fprintf(stderr, "LIBOS - OpenAL WARNING: %s - ALUT ERROR CODE: %X\n", "loading audio file failed",
                alutGetError());
    }
    return LOS_SUCCESS;
}

losResult refDestroyAudioBuffer(refAudioBuffer buffer)
{
    alDeleteBuffers(1, &buffer->buffer);
    return LOS_SUCCESS;
}

losResult refPrepPlayer(refAudioPlayer* player)
{
    if(!(player))
        return LOS_ERROR_HANDLE_IN_USE;
    (*player) = new refAudioPlayer_T();
    return LOS_SUCCESS;
}

losResult refPlay(refAudioPlayer player, refAudioBuffer buffer, float64 x, float64 y, float64 z, uint8 pitch)
{
    ALCenum error;

    [[likely]] if (player->paused)
    {
        player->paused = false;
        alSourcePlay(player->source);
    }
    else
    {
        [[likely]] if (player->initialized)
        {
            return LOS_SUCCESS;
        }
        else
        {
            alGenSources((ALuint)1, &player->source);
            TEST_ERROR("source generation", LOS_ERROR_COULD_NOT_INIT);
            alSourcef(player->source, AL_PITCH, pitch);
            TEST_ERROR("player->source pitch", LOS_ERROR_COULD_NOT_INIT);
            alSourcef(player->source, AL_GAIN, 1);
            TEST_ERROR("source gain", LOS_ERROR_COULD_NOT_INIT);
            alSource3f(player->source, AL_POSITION, static_cast<ALfloat>(x), static_cast<ALfloat>(y), static_cast<ALfloat>(z));
            TEST_ERROR("player->source position", LOS_ERROR_COULD_NOT_INIT);
            alSource3f(player->source, AL_VELOCITY, 0, 0, 0);
            TEST_ERROR("source velocity", LOS_ERROR_COULD_NOT_INIT);
            alSourcei(player->source, AL_LOOPING, AL_FALSE);
            TEST_ERROR("source looping", LOS_ERROR_COULD_NOT_INIT);
            alSourcei(player->source,AL_BUFFER,buffer->buffer);
            TEST_ERROR("attach buffer", LOS_ERROR_COULD_NOT_INIT);
            alSourcePlay(player->source);
            player->initialized = true;
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
    return refPlay(player, nullptr, 0, 0, 0, 0);
}

losResult refStop(refAudioPlayer player)
{
    if (!player->initialized)
        return LOS_SUCCESS;

    player->initialized = false;
    player->paused = false;

    alDeleteSources(1, &player->source);

    delete player;
    return LOS_SUCCESS;
}
#include "../InternalRefractile.h"
#include <AL/alut.h>
#include <RefractileAPI.h>

losResult refAppendAudioContext(refHandle)
{
    alutInit(NULL,NULL);
    return LOS_SUCCESS;
}

losResult refUnAppendAudioContext(refHandle)
{
    alutExit();
    return LOS_SUCCESS;
}
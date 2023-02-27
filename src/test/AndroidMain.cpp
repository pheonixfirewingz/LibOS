#include <jni.h>
#include <game-activity/GameActivity.h>
#include <game-text-input/gametextinput.h>
extern "C" {
#include <game-activity/native_app_glue/android_native_app_glue.h>

void handle_cmd(android_app *pApp, int32_t cmd) {
    switch (cmd) {
        default:
            break;
    }
}

void android_main(struct android_app *pApp) {
    pApp->onAppCmd = handle_cmd;
    int events;
    android_poll_source *pSource;
    do {
        if (ALooper_pollAll(0, nullptr, &events, (void **) &pSource) >= 0) {
            if (pSource)
                pSource->process(pApp, pSource);
        }
    } while (!pApp->destroyRequested);
}
}
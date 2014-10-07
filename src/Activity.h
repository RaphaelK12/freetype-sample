// -*- C++ -*-
#pragma once

#include <jni.h>
#include <errno.h>

#include <android_native_app_glue.h>

#include <memory>

#include <EGLWindow.h>
#include <Dimension.h>

using std::unique_ptr;

struct android_app;
struct AInputEvent;

class AndroidInputListener;

class Game;

class Activity
{
public:

    //!
    //!
    //!
    Activity(const Dimension viewportSize, android_app *app);

    //!
    //!
    //!
    int32_t handleInput(AInputEvent *event);

    //!
    //!
    //!
    void handleCmd(int32_t cmd);

    //!
    //!
    //!
    void run();

private:

    //!
    android_app *app;

    //!
    unique_ptr<EGLWindow> window;

    //!
    Dimension viewportSize;
};
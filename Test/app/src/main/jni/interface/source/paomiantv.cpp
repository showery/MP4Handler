/*******************************************************************************
 *        Module: interface
 *          File: paomiantv.cpp
 * Functionality: load all jni modules.
 *       Related:
 *        System: android
 *      Language: C++
 *        Author: huangxuefeng
 *       Version: V1.0 Copyright(C) 2017 paomiantv, All rights reserved.
 * -----------------------------------------------------------------------------
 * Revisions:
 * Date        Version     Reviser       Description
 * 2017-07-20  v1.0        huangxuefeng  created
 ******************************************************************************/

#include <jniloader.h>
#include "paomiantv.h"
#include "jniloader.h"
jint JNI_OnLoad(JavaVM * vm, void * reserved)
{
    LOGI("version: %u", LIB_PAOMIANTV_VERSION);
    JNIEnv * env = NULL;
    jint result = -1;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        goto bail;
    }
    paomiantv::CJNILoader::getInstance()->loadModules(env);
    result = JNI_VERSION_1_4;
bail:
    return result;
}
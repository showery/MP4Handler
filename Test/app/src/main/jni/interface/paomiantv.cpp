/*******************************************************************************
 *        Module: paomiantv
 *          File: paomiantv.cpp
 * Functionality: load all jni modules.
 *       Related: jni
 *        System: android
 *      Language: C++
 *        Author: huangxuefeng
 *       Version: V1.0 Copyright(C) 2017 PAOMIANTV, All rights reserved.
 * -----------------------------------------------------------------------------
 * Revisions:
 * Date        Version     Reviser       Description
 * 2017-07-20  v1.0        huangxuefeng  created
 ******************************************************************************/

#include "paomiantv.h"
#include "modulemanager.h"
jint JNI_OnLoad(JavaVM * vm, void * reserved)
{
    JNIEnv * env = NULL;
    jint result = -1;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        goto bail;
    }
    paomiantv::CModuleManger::getInstance()->loadModules(env);
    result = JNI_VERSION_1_4;
bail:
    return result;
}
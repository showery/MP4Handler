/*******************************************************************************
 *        Module: interface
 *          File: jniloader.cpp
 * Functionality: load all jni modules.
 *       Related:
 *        System: android
 *      Language: C++
 *        Author: huangxuefeng
 *       Version: V1.0 Copyright(C) 2017 paomiantv, All rights reserved.
 * -----------------------------------------------------------------------------
 * Revisions:
 * Date        Version     Reviser       Description
 * 2017-08-01  v1.0        huangxuefeng  created
 ******************************************************************************/
#include "jniloader.h"

namespace paomiantv {
    CJNILoader::Garbo CJNILoader::garbo; // 一定要初始化，不然程序结束时不会析构garbo

    CJNILoader *CJNILoader::m_pInstance = NULL;

    CJNILoader *CJNILoader::getInstance() {
        if (m_pInstance == NULL)
            m_pInstance = new CJNILoader();
        return m_pInstance;
    }

    CJNILoader::CJNILoader() {

    }

    CJNILoader::~CJNILoader() {

    }

    void CJNILoader::loadModule(JNIEnv *env, TJavaClazzParam *param) {
        if (param == NULL) {
            LOGE("load module: %s error!", param->m_pchClazzName);
            return;
        }
        LOGI("load module: %s", param->m_pchClazzName);
        jclass clazz = env->FindClass(param->m_pchClazzName);
        if (clazz == NULL) {
            s8 *msg;
            asprintf(&msg, "Native registration unable to find class '%s', aborting",
                     param->m_pchClazzName);
            env->FatalError(msg);
            free(msg);
        }

        if (env->RegisterNatives(clazz, param->m_ptMethods, param->m_nMtdCount) < 0) {
            s8 *msg;
            asprintf(&msg, "RegisterNatives failed for '%s', aborting", param->m_pchClazzName);
            env->FatalError(msg);
            free(msg);
        }
        free(param->m_ptMethods);
        delete param;
    }

    s32 CJNILoader::loadModules(JNIEnv *env) {
        loadModule(env, CJNIModuleEngine::GetJavaClazzParam());
        loadModule(env, CJNIModuleStoryboard::GetJavaClazzParam());
        loadModule(env, CJNIModuleClip::GetJavaClazzParam());
        loadModule(env, CJNIModuleFilter::GetJavaClazzParam());
        loadModule(env, CJNIModuleTransition::GetJavaClazzParam());
        loadModule(env, CJNIModuleRenderer::GetJavaClazzParam());
        return 0;
    }
}
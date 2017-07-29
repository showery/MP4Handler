/*******************************************************************************
 *        Module: paomiantv
 *          File: 
 * Functionality: load jni modules
 *       Related: 
 *        System: android
 *      Language: C++
 *        Author: huangxuefeng
 *       Version: V1.0 Copyright(C) 2017 PAOMIANTV, All rights reserved.
 * -----------------------------------------------------------------------------
 * Revisions:
 * Date        Version     Reviser       Description
 * 2017-07-20  v1.0        huangxuefeng  created
 ******************************************************************************/
#ifndef _PAOMIANTV_LOADMODULE_H_
#define _PAOMIANTV_LOADMODULE_H_

#include "typedef.h"
#include "autolock.h"
#include "version.h"
#include "autolog.h"
#include "jnimodule.h"
#include "stdlib.h"
#include "jnimoduleengine.h"
#include "jnimodulestoryboard.h"
#include "jnimoduleclip.h"

namespace paomiantv
{

class CJNIModuleManger
{
  private:
    CJNIModuleManger(){};
    CJNIModuleManger(const CJNIModuleManger &);

    CJNIModuleManger &operator=(const CJNIModuleManger &);
    static CJNIModuleManger *m_pInstance;

    class Garbo
    {
      public:
        ~Garbo()
        {
            if (CJNIModuleManger::m_pInstance)
            {
                delete CJNIModuleManger::m_pInstance;
            }
        }
    };
    void loadModule(JNIEnv *env, TJavaClazzParam *param)
    {
        if (param == NULL)
        {
            LOGE("module: %s", LIB_PAOMIANTV_VERSION, param->m_pchClazzName);
            return;
        }
        LOGE("load module: %s", LIB_PAOMIANTV_VERSION, param->m_pchClazzName);
        jclass clazz = env->FindClass(param->m_pchClazzName);
        if (clazz == NULL)
        {
            s8 *msg;
            asprintf(&msg, "Native registration unable to find class '%s', aborting", param->m_pchClazzName);
            env->FatalError(msg);
            free(msg);
        }

        if (env->RegisterNatives(clazz, param->m_ptMethods, param->m_nMtdCount) < 0)
        {
            s8 *msg;
            asprintf(&msg, "RegisterNatives failed for '%s', aborting", param->m_pchClazzName);
            env->FatalError(msg);
            free(msg);
        }
        free(param->m_ptMethods);
        delete param;
    }
    static Garbo garbo;

  public:
    static CJNIModuleManger *getInstance();
    s32 loadModules(JNIEnv *env)
    {
        loadModule(env, CJNIModuleEngine::getJavaClazzParam());
        loadModule(env, CJNIModuleStoryboard::getJavaClazzParam());
        loadModule(env, CJNIModuleClip::getJavaClazzParam());
        return 0;
    }
};

CJNIModuleManger::Garbo CJNIModuleManger::garbo; // 一定要初始化，不然程序结束时不会析构garbo

CJNIModuleManger *CJNIModuleManger::m_pInstance = NULL;

CJNIModuleManger *CJNIModuleManger::getInstance()
{
    if (m_pInstance == NULL)
        m_pInstance = new CJNIModuleManger();
    return m_pInstance;
}
}

#endif /* _PAOMIANTV_LOADMODULE_H_ */

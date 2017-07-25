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
#include "module.h"
#include "stdlib.h"
#include "mp4v2module.h"

namespace paomiantv
{

class CModuleManger
{
  private:
    CModuleManger(){};
    CModuleManger(const CModuleManger &);

    CModuleManger &operator=(const CModuleManger &);
    static CModuleManger *m_pInstance;

    class Garbo
    {
      public:
        ~Garbo()
        {
            if (CModuleManger::m_pInstance)
            {
                delete CModuleManger::m_pInstance;
            }
        }
    };
    void loadModule(JNIEnv *env, CModule *module)
    {
        ALOG(LOG_INFO, "paomiantv", "version: %u, module: %s", LIB_PAOMIANTV_VERSION, module->getClazzName());
        ALOG(LOG_INFO, "paomiantv", "name: %s, sign: %s, ptr: %p", module->getMethods()[0].name,module->getMethods()[0].signature,module->getMethods()[0].fnPtr);
        jclass clazz;
        clazz = env->FindClass(module->getClazzName());
        if (clazz == NULL)
        {
            s8 *msg;
            asprintf(&msg, "Native registration unable to find class '%s', aborting", module->getClazzName());
            env->FatalError(msg);
            free(msg);
        }

        if (env->RegisterNatives(clazz, module->getMethods(),module->getMethodsCount()) < 0)
        {
            s8 *msg;
            asprintf(&msg, "RegisterNatives failed for '%s', aborting", module->getClazzName());
            env->FatalError(msg);
            free(msg);
        }
        delete module;
    }
    static Garbo garbo;

  public:
    static CModuleManger *getInstance();
    s32 loadModules(JNIEnv *env)
    {
        loadModule(env, new CMp4v2Module());
        return 0;
    }
};

CModuleManger::Garbo CModuleManger::garbo; // 一定要初始化，不然程序结束时不会析构garbo

CModuleManger* CModuleManger::m_pInstance = NULL;

CModuleManger *CModuleManger::getInstance()
{
    if (m_pInstance == NULL)
        m_pInstance = new CModuleManger();
    return m_pInstance;
}

}

#endif /* _PAOMIANTV_LOADMODULE_H_ */

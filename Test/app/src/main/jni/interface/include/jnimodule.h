/*******************************************************************************
 *        Module: paomiantv
 *          File: 
 * Functionality: jni modules
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
#ifndef _PAOMIANTV_MODULE_H_
#define _PAOMIANTV_MODULE_H_

#include <jni.h>
#include "typedef.h"

#ifndef CLASS_NAME_MAX
#define CLASS_NAME_MAX 100
#endif

#ifndef NELEM
#define NELEM(x) ((int)(sizeof(x) / sizeof((x)[0])))
#endif

namespace paomiantv
{
/*!
 * \brief    used in Cutter.
 * \author  huangxuefeng
 * \date    2017-07-21
 */
typedef struct tagJavaClazzParam
{
    //! count of java native method
    s32 m_nMtdCount;
    //! java native methods
    JNINativeMethod *m_ptMethods;
    //! class name
    s8 m_pchClazzName[CLASS_NAME_MAX];
    //! constructure
    tagJavaClazzParam()
    {
        memset(this, 0, sizeof(tagJavaClazzParam));
    }

} TJavaClazzParam;

class CJNIModule
{
  protected:
    JavaVM *m_jvm;
    jobject m_jObject;
    jfieldID m_jfldNativeAddr;
    static CResidentReg m_Registry;
    CJNIModule* m_pHolder;

  public:
    CJNIModule()
    {
    }
    virtual ~CJNIModule()
    {
    }
};
}

#endif /* _PAOMIANTV_JNIMODULE_H_ */
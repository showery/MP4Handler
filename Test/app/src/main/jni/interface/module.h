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

namespace paomiantv
{

class CModule
{
  protected:
    const s8 *m_pbyClazzName[];
    JNINativeMethod *m_pMethods[];
    s32 m_nCount;

  public:
    CModule()
    {
    }
    CModule( s8 *pbyClazzName, JNINativeMethod *pMethods)
        : m_pbyClazzName(pbyClazzName),
          m_pMethods(pMethods)
    {
    }
    ~CModule()
    {
    }
    const s8 *getClazzName()
    {
        return m_pbyClazzName;
    }
    const JNINativeMethod *getMethods()
    {
        return m_pMethods;
    }
    s32 getMethodsCount(){
        return m_nCount;
    }

};
}

#endif /* _PAOMIANTV_JNIMODULE_H_ */
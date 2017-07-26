/*******************************************************************************
 *        Module: paomiantv
 *          File: 
 * Functionality: jni PMEngine module
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
#ifndef _PAOMIANTV_JNIMODULEENGINE_H_
#define _PAOMIANTV_JNIMODULEENGINE_H_

#include "jnimodule.h"

namespace paomiantv
{

class CJNIModuleEngine : public CJNIModule
{
public:
  CJNIModuleEngine();
  virtual ~CJNIModuleEngine();
  static TJavaClazzParam *GetJavaClazzParam();
  static void init(JNIEnv *env, jobject jengine);
  static void uninit(JNIEnv *env, jobject jengine);
  static void log_cb(s32 loglevel, const s8 *fmt, va_list ap);
};
}

#endif /* _PAOMIANTV_JNIMODULEENGINE_H_ */
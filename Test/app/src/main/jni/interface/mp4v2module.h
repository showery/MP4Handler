/*******************************************************************************
 *        Module: paomiantv
 *          File: 
 * Functionality: mp4v2 modules
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
#ifndef _PAOMIANTV_MP4V2MODULE_H_
#define _PAOMIANTV_MP4V2MODULE_H_

#include "module.h"

namespace paomiantv
{

class CMp4v2Module : public CModule
{
public:
  CMp4v2Module();
  virtual ~CMp4v2Module();
  BOOL32 hasSubModule();
  static jboolean jni_clipMp4(JNIEnv *env, jobject jclip, jstring src, jstring dst, jlong start, jlong duration);
  static jboolean jni_joinMp4(JNIEnv *env, jobject jjoin, jstring src, jstring dst);
};
}

#endif /* _PAOMIANTV_MP4V2MODULE_H_ */
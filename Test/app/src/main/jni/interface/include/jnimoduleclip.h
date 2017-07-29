/*******************************************************************************
 *        Module: paomiantv
 *          File: 
 * Functionality: jni clip module
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

#include "jnimodule.h"

//field name
#define CLIP_FIELD_NATIVE_ADDRESS_NAME "mNativeClipAddress"
//field signiture
#define CLIP_FIELD_NATIVE_ADDRESS_SIG "I"

namespace paomiantv
{

class CJNIModuleClip : public CJNIModule
{
private:
  CClip *m_pClip;

public:
  static TJavaClazzParam *GetJavaClazzParam();
  static CJNIModuleClip *GetJniClip(JNIEnv *env, jobject jClip);
  static CJNIModuleClip *ExistJniClip(JNIEnv *env, jobject jClip);
  static void DestroyJniClip(CJNIModuleClip *&p);
  static bool IsValid(CJNIModuleClip *p);
  inline CClip *getCClip();

private:
  CJNIModuleClip();
  virtual ~CJNIModuleClip();
  static CJNIModuleClip *CreateJniClip(JNIEnv *env, jobject jClip);

  static jboolean init(JNIEnv *env, jobject jclip, jstring jsrc, jlong jstart, jlong jduration);

  static jboolean uninit(JNIEnv *env, jobject jclip);

  static jstring getSrc(JNIEnv *env, jobject jclip);

  static void setSrc(JNIEnv *env, jobject jclip, jstring jsrc);

  static jlong getStart(JNIEnv *env, jobject jclip);

  static void setStart(JNIEnv *env, jobject jclip, jlong jstart);

  static jlong getDuration(JNIEnv *env, jobject jclip);

  static void setDuration(JNIEnv *env, jobject jclip, jlong jduration);

  static jstring getFilter(JNIEnv *env, jobject jclip, jint jindex);

  static jboolean addFilter(JNIEnv *env, jobject jclip, jstring jfilter);

  static jboolean removeFilter(JNIEnv *env, jobject jclip, jstring jfilter);

  static jint getFilterCount(JNIEnv *env, jobject jclip);

  static jboolean addTransition(JNIEnv *env, jobject jclip, jstring jtransition);

  static jstring getTransition(JNIEnv *env, jobject jclip, jint jindex);

  static jboolean removeTransition(JNIEnv *env, jobject jclip, jstring);

  static jint getTransitionCount(JNIEnv *env, jobject jclip);
};
inline CClip *CJNIModuleClip::getCClip()
{
  return m_pClip;
}
}

#endif /* _PAOMIANTV_MP4V2MODULE_H_ */
/*******************************************************************************
 *        Module: paomiantv
 *          File: 
 * Functionality: jni PMStoryboard module
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
#ifndef _PAOMIANTV_JNIMODULESTORYBOARD_H_
#define _PAOMIANTV_JNIMODULESTORYBOARD_H_

#include "jnimodule.h"

#define ERROR_SOURCE 10000
#define ERROR_SOURCE_DIST "Soure file is null or not existed!"

//method name
#define STORYBOARD_METHOD_FAILED_NAME "fireOnFailed"
#define STORYBOARD_METHOD_PROGRESS_NAME "fireOnProgress"
#define STORYBOARD_METHOD_SUCCESS_NAME "fireOnSuccess"
//method signiture
#define STORYBOARD_METHOD_FAILED_SIG "(ILjava/lang/String;)V"
#define STORYBOARD_METHOD_PROGRESS_SIG "(I)V"
#define STORYBOARD_METHOD_SUCCESS_SIG "(Ljava/lang/String;)V"

//field name
#define STORYBOARD_FIELD_NATIVE_ADDRESS_NAME "mNativeStoryboardAddress"
//field signiture
#define STORYBOARD_FIELD_NATIVE_ADDRESS_SIG "I"

namespace paomiantv
{

typedef struct tagJAVAMethod
{
  const s8 *m_pchName;
  const s8 *m_pchSigniture;
} TJavaMemeber;

class CJNIModuleStoryboard : public CJNIModule
{
private:
  CStoryboard *m_pStoryboard;
  jmethodID m_ajmtd[3];

private:
  CJNIModuleStoryboard(JNIEnv *env, jobject jStoryboard);
  virtual ~CJNIModuleStoryboard();
  static CJNIModuleStoryboard *CreateJniStoryboard(JNIEnv *env, jobject jStoryboard);

  static jboolean init(JNIEnv *env, jobject jstoryboard, jstring jdstPath);

  static jboolean uninit(JNIEnv *env, jobject jstoryboard);

  static jboolean setBGM(JNIEnv *env, jobject jstoryboard, jstring jsrcpath, jlong jstartCutTm, jlong jdurationCutTm, jlong jstartTm, jlong jendTm);

  static jboolean addClip(JNIEnv *env, jobject jstoryboard, jobject jclip);

  static jobject removeClip(JNIEnv *env, jobject jstoryboard, jint jindex);

  static jobject getClip(JNIEnv *env, jobject jstoryboard, jint jindex);

  static jboolean swapClip(JNIEnv *env, jobject jstoryboard, jint jindexA, jint jindexB);

  static jboolean procsss(JNIEnv *env, jobject jstoryboard);

  static jboolean cancel(JNIEnv *env, jobject jstoryboard);

public:
  inline CStoryboard *getCStoryboard();
  static CJNIModuleStoryboard *GetJniStoryboard(JNIEnv *env, jobject jStoryboard);
  static void DestroyJniStoryboard(CJNIModuleStoryboard *&p);
  static bool IsValid(CJNIModuleStoryboard *p);
  static TJavaClazzParam *GetJavaClazzParam();
};
inline CStoryboard *CJNIModuleStoryboard::getCStoryboard()
{
  return m_pStoryboard;
}
}

#endif /* _PAOMIANTV_JNIMODULESTORYBOARD_H_ */
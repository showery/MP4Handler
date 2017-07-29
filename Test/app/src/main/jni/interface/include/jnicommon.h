#ifndef _PAOMIANTV_JNICOMMON_H_
#define _PAOMIANTV_JNICOMMON_H_

#include <jni.h>
#include "typedef.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

namespace paomiantv
{

BOOL32 setIntField(JNIEnv *env, jobject obj, const s8 *fieldName, s32 value);
BOOL32 getIntField(JNIEnv *env, jobject obj, const s8 *fieldName, jint *value);
s32 setByteArrayField(JNIEnv *env, jobject obj, const s8 *fieldName, const s8 *value, s32 size);
BOOL32 setIntArrayField(JNIEnv *env, jobject obj, const s8 *fieldName, const s32 *value, s32 size);
BOOL32 set2DInt2ArrayField(JNIEnv *env, jobject obj, const s8 *fieldName, const s32 (*value)[2], s32 size);
s32 getStringBytesFromObjFld(JNIEnv *env, jobject obj, const s8 *fieldName, u8 *buffer, s32 size);
s32 getStringBytes(JNIEnv *env, jstring obj, s8 *buffer, s32 size);
jstring stringToJString(JNIEnv *env, s8 *pchPat);

} // namespace paomiantv

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // _PAOMIANTV_JNICOMMON_H_
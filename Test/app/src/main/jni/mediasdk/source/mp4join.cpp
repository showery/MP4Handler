#include <stdlib.h>
#include "mp4clip.h"
#include "autolog.h"
#include "autofile.h"
#include "autolock.h"
#include "common.h"

//method name
#define CUTTER_METHOD_FAILED_NAME "fireOnFailed"
#define CUTTER_METHOD_PROGRESS_NAME "fireOnProgress"
#define CUTTER_METHOD_SUCCESS_NAME "fireOnSuccess"
//method signiture
#define CUTTER_METHOD_FAILED_SIG "(ILjava/lang/String;)V"
#define CUTTER_METHOD_PROGRESS_SIG "(I)V"
#define CUTTER_METHOD_SUCCESS_SIG "(Ljava/lang/String;)V"

namespace paomiantv
{

CMp4Clip::CMp4Clip(JNIEnv *env, jobject jcutter,
                   TMp4CutterParam *pCutterParam)
    : m_jvm(NULL),
      m_ptMp4CutterParam(pCutterParam)
{
    USE_LOG;

    if (env == NULL || jcutter == NULL || pCutterParam == NULL)
    {
        LOGE("invalid parameters");
        return;
    }

    if (env->GetJavaVM(&m_jvm) != 0)
    {
        LOGE("get java vm failed");
        return;
    }

    env->PushLocalFrame(10);

    jclass jcls = env->GetObjectClass(jcutter);
    if (jcls == NULL)
    {
        LOGE("get java class failed");
        return;
    }
    m_jCutter = env->NewGlobalRef(jcutter);
    m_jmtdFailed = env->GetMethodID(jcls, CUTTER_METHOD_FAILED_NAME, CUTTER_METHOD_FAILED_SIG);
    if (m_jmtdFailed == NULL)
    {
        LOGE("java method 'void %s()' is not defined", CUTTER_METHOD_FAILED_NAME);
        return;
    }
    m_jmtdPrgress = env->GetMethodID(jcls, CUTTER_METHOD_PROGRESS_NAME, CUTTER_METHOD_PROGRESS_SIG);
    if (m_jmtdPrgress == NULL)
    {
        LOGE("java method 'void %s()' is not defined", CUTTER_METHOD_PROGRESS_NAME);
        return;
    }
    m_jmtdSuccess = env->GetMethodID(jcls, CUTTER_METHOD_SUCCESS_NAME, CUTTER_METHOD_SUCCESS_SIG);
    if (m_jmtdSuccess == NULL)
    {
        LOGE("java method 'void %s()' is not defined", CUTTER_METHOD_SUCCESS_NAME);
        return;
    }
    env->PopLocalFrame(NULL);
}

// FIXME: when exit the application, it will sometimes prints this error log:
// E/libEGL(19049): call to OpenGL ES API with no current context (logged once per thread)
// -- noted by huangxuefeng, 2013-07-29
CMp4Clip::~CMp4Clip()
{
    USE_LOG;
    JNIEnv *env = NULL;
    if (m_jvm->GetEnv((void **)&env, JNI_VERSION_1_4) != JNI_OK)
    {
        LOGE("get JNIEnv failed");
        return;
    }

    if (m_jCutter != NULL)
    {
        env->DeleteGlobalRef(m_jCutter);
        m_jCutter = NULL;
    }
}

void CMp4Clip::failedCallback(s32 nErr, s8 *pchDescription)
{
    if (!pchDescription || !m_jvm || !m_jCutter || !m_jmtdFailed)
        return;
    JNIEnv *env = NULL;
    bool isAttacked = false;
    int status = (m_jvm)->GetEnv((void **)&env, JNI_VERSION_1_4);
    if (status != JNI_OK)
    {

        status = m_jvm->AttachCurrentThread(&env, NULL);
        if (status != JNI_OK)
        {
            LOGE("failed to attach current thread");
            return;
        }
        isAttacked = true;
    }

    env->PushLocalFrame(10);
    jstring jnistr = stringToJString(env, pchDescription);
    env->CallVoidMethod(m_jCutter, m_jmtdFailed, nErr, jnistr);
    env->DeleteLocalRef(jnistr);
    env->PopLocalFrame(NULL);

    if (isAttacked)
    {
        m_jvm->DetachCurrentThread();
    }
}

void CMp4Clip::progressCallback(s32 nProgress)
{

    if (!m_jvm || !m_jCutter || !m_jmtdPrgress)
        return;
    JNIEnv *env = NULL;
    bool isAttacked = false;
    int status = (m_jvm)->GetEnv((void **)&env, JNI_VERSION_1_4);
    if (status != JNI_OK)
    {

        status = m_jvm->AttachCurrentThread(&env, NULL);
        if (status != JNI_OK)
        {
            LOGE("failed to attach current thread");
            return;
        }
        isAttacked = true;
    }

    env->PushLocalFrame(10);
    env->CallVoidMethod(m_jCutter, m_jmtdPrgress, nProgress);
    env->PopLocalFrame(NULL);

    if (isAttacked)
    {
        m_jvm->DetachCurrentThread();
    }
}

void CMp4Clip::successCallback()
{
    if (!m_jvm || !m_jCutter || !m_jmtdSuccess)
        return;
    JNIEnv *env = NULL;
    bool isAttacked = false;
    int status = (m_jvm)->GetEnv((void **)&env, JNI_VERSION_1_4);
    if (status != JNI_OK)
    {

        status = m_jvm->AttachCurrentThread(&env, NULL);
        if (status != JNI_OK)
        {
            LOGE("failed to attach current thread");
            return;
        }
        isAttacked = true;
    }

    env->PushLocalFrame(10);
    jstring jnistr = stringToJString(env, m_ptMp4CutterParam->pchDstPath);
    env->CallVoidMethod(m_jCutter, m_jmtdSuccess, jnistr);
    env->DeleteLocalRef(jnistr);
    env->PopLocalFrame(NULL);

    if (isAttacked)
    {
        m_jvm->DetachCurrentThread();
    }
}

// void CMp4Clip::clipTrack(
//     MP4FileHandle srcFile,
//     MP4TrackId trackId,
//     MP4FileHandle dstFile,
//     s64 startTime,
//     s64 duration)
// {
//     MP4Timestamp trackStartTime =
//         MP4ConvertToTrackTimestamp(
//             srcFile,
//             trackId,
//             startTime,
//             MP4_MSECS_TIME_SCALE);

//     MP4Duration trackDuration =
//         MP4ConvertToTrackDuration(
//             srcFile,
//             trackId,
//             duration,
//             MP4_MSECS_TIME_SCALE);

//     MP4EditId editId =
//         MP4AddTrackEdit(
//             srcFile,
//             trackId,
//             MP4_INVALID_EDIT_ID,
//             trackStartTime,
//             trackDuration);

//     if (editId == MP4_INVALID_EDIT_ID)
//     {
//         LOGE("can't create track edit");
//         return;
//     }
//     if (dstFile)
//     {
//         MP4CopyTrack(
//             srcFile,
//             trackId,
//             dstFile,
//             true);
//         LOGE("start delete edit");
//         MP4DeleteTrackEdit(
//             srcFile,
//             trackId,
//             editId);
//     }
// }

MP4TrackId CMp4Clip::clipTrack(
    MP4FileHandle srcFile,
    MP4TrackId trackId,
    MP4FileHandle dstFile,
    s64 startTime,
    s64 duration)
{
    MP4Timestamp trackStartTime = MP4ConvertToTrackTimestamp(
        srcFile,
        trackId,
        startTime,
        MP4_MSECS_TIME_SCALE);

    MP4Duration trackDuration = MP4ConvertToTrackDuration(
        srcFile,
        trackId,
        duration,
        MP4_MSECS_TIME_SCALE);

    MP4TrackId dstTrackId = MP4CloneTrack(srcFile, trackId, dstFile);

    if (dstTrackId == MP4_INVALID_TRACK_ID)
    {
        return dstTrackId;
    }

    MP4Timestamp when = 0;
    const MP4SampleId numSamples = MP4GetTrackNumberOfSamples(srcFile, trackId);
    MP4SampleId sampleId = MP4GetSampleIdFromTime(srcFile, trackId, trackStartTime, true);

    while (true)
    {
        if (when >= trackDuration)
        {
            break;
        }
        if (sampleId > numSamples)
        {
            break;
        }
        MP4Duration sampleDuration = MP4GetSampleDuration(srcFile, trackId, sampleId);
        bool rc = MP4CopySample(
            srcFile,
            trackId,
            sampleId,
            dstFile,
            dstTrackId,
            sampleDuration);
        if (!rc)
        {
            MP4DeleteTrack(dstFile, dstTrackId);
            return MP4_INVALID_TRACK_ID;
        }
        sampleId++;
        when += sampleDuration;
    }

    return dstTrackId;
}
void CMp4Clip::clip()
{
    MP4TrackId trackId = MP4_INVALID_TRACK_ID;
    MP4FileHandle srcFile = MP4Modify(m_ptMp4CutterParam->pchSrcPath, 0);
    LOGE("Clip: src,dst,start,duration = %s,%s,%lld,%lld,", m_ptMp4CutterParam->pchSrcPath,
         m_ptMp4CutterParam->pchDstPath,
         m_ptMp4CutterParam->sllStart,
         m_ptMp4CutterParam->sllDuration);
    if (!srcFile)
    {
        LOGE(ERROR_SOURCE_DIST);
        failedCallback(ERROR_SOURCE, ERROR_SOURCE_DIST);
    }

    MP4FileHandle dstFile = MP4_INVALID_FILE_HANDLE;

    if (m_ptMp4CutterParam->pchDstPath)
    {
        dstFile = MP4Create(m_ptMp4CutterParam->pchDstPath, MP4_CREATE_64BIT_DATA | MP4_CREATE_64BIT_TIME);
        MP4SetTimeScale(dstFile, MP4GetTimeScale(srcFile));
    }

    u32 numTracks = MP4GetNumberOfTracks(srcFile);
    for (u32 i = 0; i < numTracks; i++)
    {
        trackId = MP4FindTrackId(srcFile, i);
        clipTrack(srcFile, trackId, dstFile, m_ptMp4CutterParam->sllStart, m_ptMp4CutterParam->sllDuration);
    }
    MP4Close(srcFile);
    if (dstFile != MP4_INVALID_FILE_HANDLE)
    {
        MP4Close(dstFile);
    }
    successCallback();
}

} // namespace paomiantv

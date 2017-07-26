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

CStoryboard::CStoryboard(JNIEnv *env, jobject jcutter,
                   TMp4CutterParam *pCutterParam)
    : m_jvm(NULL),
      m_ptMp4CutterParam(pCutterParam)
{
    
}

CStoryboard::~CStoryboard()
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

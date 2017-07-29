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

CClip::CClip(JNIEnv *env, jobject jcutter,
                   TMp4CutterParam *pCutterParam)
{
    USE_LOG;  
    m_vFilters.clear();
    m_vTransitions.clear();
    m_pLock = new CLock;
}

CClip::~CClip()
{
    USE_LOG;
    delete m_pLock;
}

MP4TrackId CClip::clipTrack(
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
void CClip::clip()
{
    MP4TrackId trackId = MP4_INVALID_TRACK_ID;
    MP4FileHandle srcFile = MP4Modify(m_ptMp4CutterParam->pchSrcPath, 0);
    if (!srcFile)
    {
        return;
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
}

} // namespace paomiantv

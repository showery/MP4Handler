LOCAL_PATH:=$(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= \
    mSrc/3gp.cpp \
    mSrc/atom_ac3.cpp \
    mSrc/atom_amr.cpp \
    mSrc/atom_avc1.cpp \
    mSrc/atom_avcC.cpp \
    mSrc/atom_chpl.cpp \
    mSrc/atom_colr.cpp \
    mSrc/atom_d263.cpp \
    mSrc/atom_dac3.cpp \
    mSrc/atom_damr.cpp \
    mSrc/atom_dref.cpp \
    mSrc/atom_elst.cpp \
    mSrc/atom_enca.cpp \
    mSrc/atom_encv.cpp \
    mSrc/atom_free.cpp \
    mSrc/atom_ftyp.cpp \
    mSrc/atom_ftab.cpp \
    mSrc/atom_gmin.cpp \
    mSrc/atom_hdlr.cpp \
    mSrc/atom_hinf.cpp \
    mSrc/atom_hnti.cpp \
    mSrc/atom_href.cpp \
    mSrc/atom_mdat.cpp \
    mSrc/atom_mdhd.cpp \
    mSrc/atom_meta.cpp \
    mSrc/atom_mp4s.cpp \
    mSrc/atom_mp4v.cpp \
    mSrc/atom_mvhd.cpp \
    mSrc/atom_nmhd.cpp \
    mSrc/atom_ohdr.cpp \
    mSrc/atom_pasp.cpp \
    mSrc/atom_root.cpp \
    mSrc/atom_rtp.cpp \
    mSrc/atom_s263.cpp \
    mSrc/atom_sdp.cpp \
    mSrc/atom_sdtp.cpp \
    mSrc/atom_smi.cpp \
    mSrc/atom_sound.cpp \
    mSrc/atom_standard.cpp \
    mSrc/atom_stbl.cpp \
    mSrc/atom_stdp.cpp \
    mSrc/atom_stsc.cpp \
    mSrc/atom_stsd.cpp \
    mSrc/atom_stsz.cpp \
    mSrc/atom_stz2.cpp \
    mSrc/atom_text.cpp \
    mSrc/atom_tfhd.cpp \
    mSrc/atom_tkhd.cpp \
    mSrc/atom_treftype.cpp \
    mSrc/atom_trun.cpp \
    mSrc/atom_tx3g.cpp \
    mSrc/atom_udta.cpp \
    mSrc/atom_url.cpp \
    mSrc/atom_urn.cpp \
    mSrc/atom_uuid.cpp \
    mSrc/atom_video.cpp \
    mSrc/atom_vmhd.cpp \
    mSrc/cmeta.cpp \
    mSrc/descriptors.cpp \
    mSrc/exception.cpp \
    mSrc/isma.cpp \
    mSrc/log.cpp \
    mSrc/mp4.cpp \
    mSrc/mp4atom.cpp \
    mSrc/mp4container.cpp \
    mSrc/mp4descriptor.cpp \
    mSrc/mp4file.cpp \
    mSrc/mp4file_io.cpp \
    mSrc/mp4info.cpp \
    mSrc/mp4property.cpp \
    mSrc/mp4track.cpp \
    mSrc/mp4util.cpp \
    mSrc/ocidescriptors.cpp \
    mSrc/odcommands.cpp \
    mSrc/qosqualifiers.cpp \
    mSrc/rtphint.cpp \
    mSrc/text.cpp \
    mSrc/bmff/typebmff.cpp \
    mSrc/itmf/CoverArtBox.cpp \
    mSrc/itmf/Tags.cpp \
    mSrc/itmf/generic.cpp \
    mSrc/itmf/type.cpp \
    mSrc/qtff/ColorParameterBox.cpp \
    mSrc/qtff/PictureAspectRatioBox.cpp \
    mSrc/qtff/coding.cpp \
    libplatform/io/File.cpp \
    libplatform/io/FileSystem.cpp \
    libplatform/prog/option.cpp \
    libplatform/sys/error.cpp \
    libplatform/time/time.cpp \
    libplatform/io/File_posix.cpp \
    libplatform/io/FileSystem_posix.cpp \
    libplatform/number/random_posix.cpp \
    libplatform/process/process_posix.cpp \
    libplatform/time/time_posix.cpp \
    libutil/Database.cpp \
    libutil/Timecode.cpp \
    libutil/TrackModifier.cpp \
    libutil/Utility.cpp \
    libutil/crc.cpp \
    libutil/other.cpp \
    interface/common.cpp \
    interface/mp4clip.cpp \
    interface/mp4extract.cpp \
    interface/mp4v2module.cpp \
    interface/paomiantv.cpp \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include \
    $(LOCAL_PATH)/common \

LOCAL_LDLIBS := -llog
#
#LOCAL_SHARED_LIBRARIES := \
#    libutils \
#    libbinder \
#    libcutils
#
LOCAL_MODULE:= mp4v2
LOCAL_CPPFLAGS := -O0 -fpic -fexceptions -DHAVE_SOCKLEN_T -DHAVE_STRUCT_IOVEC -Wno-write-strings
LOCAL_MODULE_TAGS := optional
include $(BUILD_SHARED_LIBRARY)
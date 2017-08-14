LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE:= codec16
LOCAL_SRC_FILES:= libnative_codec16.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE:= codec17
LOCAL_SRC_FILES:= libnative_codec17.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE:= codec18
LOCAL_SRC_FILES:= libnative_codec18.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE:= codec19
LOCAL_SRC_FILES:= libnative_codec19.so
include $(PREBUILT_SHARED_LIBRARY)
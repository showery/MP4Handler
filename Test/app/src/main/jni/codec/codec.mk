LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE:= codec

CODEC_SRC_DIR    := source
CODEC_INC_DIR    := include

INC_PATH    	 := $(CODEC_INC_DIR)

CODEC_SRC_CFILES		:= $(wildcard $(LOCAL_PATH)/$(CODEC_SRC_DIR)/*.c)
CODEC_SRC_CPPFILES	:= $(wildcard $(LOCAL_PATH)/$(CODEC_SRC_DIR)/*.cpp)
CODEC_SRC_CCFILES	:= $(wildcard $(LOCAL_PATH)/$(CODEC_SRC_DIR)/*.cc)

LOCAL_C_INCLUDES:=$(addprefix $(LOCAL_PATH)/,$(INC_PATH))

LOCAL_SRC_FILES := $(subst $(LOCAL_PATH)/,,$(CODEC_SRC_CFILES))\
		$(subst $(LOCAL_PATH)/,,$(CODEC_SRC_CPPFILES))\
		$(subst $(LOCAL_PATH)/,,$(CODEC_SRC_CCFILES))

LOCAL_CFLAGS += -D__ANDROID__
LOCAL_CFLAGS += -fpic
LOCAL_CFLAGS += -fexceptions

ifeq ($(PWLIB_SUPPORT),1)
   LOCAL_C_INCLUDES += $(PWLIBDIR)/include/ptlib/unix $(PWLIBDIR)/include
endif

LOCAL_PRELINK_MODULE:= false
#LOCAL_SHARED_LIBRARIES := native_codec16 native_codec17 native_codec18 native_codec19
LOCAL_LDLIBS += -lz -llog -ldl  -lGLESv2 -lEGL

include $(BUILD_SHARED_LIBRARY)
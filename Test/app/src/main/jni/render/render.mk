LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE:= render
OPENGLES_SRC_DIR	:= source
OPENGLES_INC_DIR	:= include
COMM_DIR    		:= ../common/include

INC_PATH            := $(OPENGLES_INC_DIR)\
                $(COMM_DIR)

OPENGLES_SRC_CFILES	:= $(wildcard $(LOCAL_PATH)/$(OPENGLES_SRC_DIR)/*.c)
OPENGLES_SRC_CPPFILES	:= $(wildcard $(LOCAL_PATH)/$(OPENGLES_SRC_DIR)/*.cpp)
OPENGLES_SRC_CCFILES	:= $(wildcard $(LOCAL_PATH)/$(OPENGLES_SRC_DIR)/*.cc)

LOCAL_C_INCLUDES:=$(addprefix $(LOCAL_PATH)/,$(INC_PATH))

LOCAL_SRC_FILES := $(subst $(LOCAL_PATH)/,,$(OPENGLES_SRC_CFILES))\
		$(subst $(LOCAL_PATH)/,,$(OPENGLES_SRC_CPPFILES))\
		$(subst $(LOCAL_PATH)/,,$(OPENGLES_SRC_CCFILES))

LOCAL_CPPFLAGS += \
	-DHAVE_PTHREADS=1 \
	-DLOG_NDEBUG=0

LOCAL_CFLAGS := \
	-D__ANDROID__ \
	-fpic \
  -fno-exceptions

LOCAL_PRELINK_MODULE:= false
LOCAL_LDLIBS += -lz -llog -ldl -lGLESv2 -lEGL
include $(BUILD_SHARED_LIBRARY)



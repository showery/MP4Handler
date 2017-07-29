LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE:= mediasdk
MEDIASDK_SRC_DIR    := source
MEDIASDK_INC_DIR    := include
COMM_DIR    		 := ../common/include
INC_PATH    	 := $(MEDIASDK_INC_DIR)\
			$(COMM_DIR)

MEDIASDK_SRC_CFILES		:= $(wildcard $(LOCAL_PATH)/$(MEDIASDK_SRC_DIR)/*.c)
MEDIASDK_SRC_CPPFILES	:= $(wildcard $(LOCAL_PATH)/$(MEDIASDK_SRC_DIR)/*.cpp)
MEDIASDK_SRC_CCFILES	:= $(wildcard $(LOCAL_PATH)/$(MEDIASDK_SRC_DIR)/*.cc)
		
LOCAL_C_INCLUDES:=$(addprefix $(LOCAL_PATH)/,$(INC_PATH))

LOCAL_SRC_FILES := $(subst $(LOCAL_PATH)/,,$(MEDIASDK_SRC_CFILES))\
		$(subst $(LOCAL_PATH)/,,$(MEDIASDK_SRC_CPPFILES))\
		$(subst $(LOCAL_PATH)/,,$(MEDIASDK_SRC_CCFILES))
		
LOCAL_CFLAGS += -D_ANDROID_                               
LOCAL_CFLAGS += -fpic
LOCAL_CFLAGS += -fexceptions

ifeq ($(PWLIB_SUPPORT),1)
   LOCAL_C_INCLUDES += $(PWLIBDIR)/include/ptlib/unix $(PWLIBDIR)/include
endif

LOCAL_PRELINK_MODULE:= false
LOCAL_SHARED_LIBRARIES := mp4v2
LOCAL_LDLIBS += -lz -llog -ldl

include $(BUILD_SHARED_LIBRARY)
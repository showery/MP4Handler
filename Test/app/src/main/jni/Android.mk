LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

subdirs += $(LOCAL_PATH)/mp4v2/mp4v2.mk

subdirs += $(LOCAL_PATH)/render/render.mk

subdirs += $(LOCAL_PATH)/codec/codec.mk

subdirs += $(LOCAL_PATH)/codec//lib/precodec.mk

subdirs += $(LOCAL_PATH)/mediasdk/mediasdk.mk

subdirs += $(LOCAL_PATH)/interface/interface.mk

include $(subdirs)

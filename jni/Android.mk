LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := ndk_file_patch
LOCAL_SRC_FILES := ndk_file_patch.c
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)

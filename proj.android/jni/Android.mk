LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := hellocpp_shared

LOCAL_MODULE_FILENAME := libhellocpp

LOCAL_CFLAGS += -std=c++11

LOCAL_SRC_FILES := hellocpp/main.cpp \
 				   ../../Classes/Game/definitions.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/AcquireScene.cpp \
                   ../../Classes/ATileSprite.cpp \
                   ../../Classes/Popup.cpp \
                   ../../Classes/Game/Board.cpp \
                   ../../Classes/Game/GameStage.cpp \
                   ../../Classes/Game/order.cpp \
                   ../../Classes/Game/PlayerAI.cpp \

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes 
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/Game

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
        
include $(BUILD_SHARED_LIBRARY)
   
$(call import-module,cocos2dx)
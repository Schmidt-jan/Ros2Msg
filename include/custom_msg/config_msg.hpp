#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdexcept>
#include <string.h>

enum class CfgMsgType {
    INIT,
    RESET,
    SLEEP,
    AWAKE,

    GET_PRODUCT_DESCRIPTION,
    GET_MOTION_STATUS,
    GET_DELTA_X,
    GET_DELTA_Y,
    GET_CONFIGURATION,
    GET_RESOLUTION_X,
    GET_RESOLUTION_Y,
    GET_RESOLUTION_X_RAW,
    GET_RESOLUTION_Y_RAW,
    GET_SHUTTER_TIME,
    GET_AVG_FRAME_BRIGHTNESS,
    GET_LASER_DRIVE_CURRENT_RAW,
    GET_LASER_DRIVE_CURRENT,
    GET_IMAGE_QUALITY,
    GET_OPERATING_MODE,

    SET_OPERATING_MODE,
    SET_RESOLUTION_X,
    SET_RESOLUTION_Y,
    SET_RESOLUTION_X_RAW,
    SET_RESOLUTION_Y_RAW,
    SET_LASER_DRIVE_CURRENT_RAW,
    SET_LASER_DRIVE_CURRENT,

    IS_SLEEPING,
    IS_WRITE_PROTECTED,
    ENABLE_WRITE_PROTECTION,
    DISABLE_WRITE_PROTECTION,
};

size_t get_msg_type_req_length(CfgMsgType type) {
    switch(type) {
        case CfgMsgType::INIT: return 0;
        case CfgMsgType::RESET: return 0;
        case CfgMsgType::SLEEP: return 0;
        case CfgMsgType::AWAKE: return 0;

        case CfgMsgType::IS_SLEEPING: return 0;
        case CfgMsgType::IS_WRITE_PROTECTED: return 0;
        case CfgMsgType::ENABLE_WRITE_PROTECTION: return 0;
        case CfgMsgType::DISABLE_WRITE_PROTECTION: return 0;

        case CfgMsgType::GET_PRODUCT_DESCRIPTION: return 0;
        case CfgMsgType::GET_MOTION_STATUS: return 0;
        case CfgMsgType::GET_DELTA_X: return 0;
        case CfgMsgType::GET_DELTA_Y: return 0;
        case CfgMsgType::GET_CONFIGURATION: return 0;
        case CfgMsgType::GET_RESOLUTION_X: return 0;
        case CfgMsgType::GET_RESOLUTION_Y: return 0;
        case CfgMsgType::GET_LASER_DRIVE_CURRENT: return 0;
        case CfgMsgType::GET_RESOLUTION_X_RAW: return 0;
        case CfgMsgType::GET_RESOLUTION_Y_RAW: return 0;
        case CfgMsgType::GET_LASER_DRIVE_CURRENT_RAW: return 0;
        case CfgMsgType::GET_SHUTTER_TIME: return 0;
        case CfgMsgType::GET_AVG_FRAME_BRIGHTNESS: return 0;
        case CfgMsgType::GET_IMAGE_QUALITY: return 0;
        case CfgMsgType::GET_OPERATING_MODE: return 0;

        case CfgMsgType::SET_OPERATING_MODE: return sizeof(uint8_t);
        case CfgMsgType::SET_RESOLUTION_X: return sizeof(float);
        case CfgMsgType::SET_RESOLUTION_Y: return sizeof(float);
        case CfgMsgType::SET_LASER_DRIVE_CURRENT: return sizeof(float);
        case CfgMsgType::SET_RESOLUTION_X_RAW: return sizeof(uint8_t);
        case CfgMsgType::SET_RESOLUTION_Y_RAW: return sizeof(uint8_t);
        case CfgMsgType::SET_LASER_DRIVE_CURRENT_RAW: return sizeof(uint8_t);
        default:
            throw std::invalid_argument("Message type '" + std::to_string(static_cast<int>(type)) + "' is not defined");
    }
}

size_t get_msg_type_res_length(CfgMsgType type) {
    switch(type) {
        case CfgMsgType::INIT:  return sizeof(bool);
        case CfgMsgType::RESET: return 0;
        case CfgMsgType::SLEEP: return 0;
        case CfgMsgType::AWAKE: return 0;

        case CfgMsgType::IS_SLEEPING: return sizeof(bool);
        case CfgMsgType::IS_WRITE_PROTECTED: return sizeof(bool);
        case CfgMsgType::ENABLE_WRITE_PROTECTION: return sizeof(bool);
        case CfgMsgType::DISABLE_WRITE_PROTECTION: return sizeof(bool);

        case CfgMsgType::GET_PRODUCT_DESCRIPTION: return sizeof(uint16_t);
        case CfgMsgType::GET_MOTION_STATUS: return sizeof(uint8_t);
        case CfgMsgType::GET_DELTA_X: return sizeof(int16_t);
        case CfgMsgType::GET_DELTA_Y: return sizeof(int16_t);
        case CfgMsgType::GET_CONFIGURATION: return sizeof(uint8_t);
        case CfgMsgType::GET_RESOLUTION_X: return sizeof(float);
        case CfgMsgType::GET_RESOLUTION_Y: return sizeof(float);
        case CfgMsgType::GET_RESOLUTION_X_RAW: return sizeof(uint8_t);
        case CfgMsgType::GET_RESOLUTION_Y_RAW: return sizeof(uint8_t);
        case CfgMsgType::GET_SHUTTER_TIME: return sizeof(uint8_t);
        case CfgMsgType::GET_AVG_FRAME_BRIGHTNESS: return sizeof(uint8_t);
        case CfgMsgType::GET_LASER_DRIVE_CURRENT_RAW: return sizeof(uint8_t);
        case CfgMsgType::GET_LASER_DRIVE_CURRENT: return sizeof(float);
        case CfgMsgType::GET_IMAGE_QUALITY: return sizeof(uint8_t);
        case CfgMsgType::GET_OPERATING_MODE: return sizeof(uint8_t);

        case CfgMsgType::SET_OPERATING_MODE: return sizeof(bool);
        case CfgMsgType::SET_RESOLUTION_X: return 0;
        case CfgMsgType::SET_RESOLUTION_Y: return 0;
        case CfgMsgType::SET_RESOLUTION_X_RAW: return 0;
        case CfgMsgType::SET_RESOLUTION_Y_RAW: return 0;
        case CfgMsgType::SET_LASER_DRIVE_CURRENT_RAW: return 0;
        case CfgMsgType::SET_LASER_DRIVE_CURRENT: return 0;
        default:
            throw std::invalid_argument("Message type '" + std::to_string(static_cast<int>(type)) + "' is not defined");
    }
}

typedef struct {
    CfgMsgType type;
    size_t payload_size = 0;
    uint8_t* payload = nullptr;
} setting_msg;

setting_msg* create_setting_msg(CfgMsgType type, bool req)  {
    setting_msg* msg = (setting_msg*)malloc(sizeof(setting_msg));
    if (msg == NULL) {
        return NULL;
    }
    msg->type = type;

    if (req) {
        msg->payload_size = get_msg_type_req_length(type);
    } else {
        msg->payload_size = get_msg_type_res_length(type);
    }
    if (msg->payload_size > 0) {
        msg->payload = (uint8_t*)malloc(msg->payload_size);
        if (msg->payload == NULL) {
            free(msg);
            return NULL;
        }
    } else {
        msg->payload = NULL;
    }

    return msg;
}

setting_msg* create_setting_msg_req(CfgMsgType type) {
    return create_setting_msg(type, true);
}

setting_msg* create_setting_msg_res(CfgMsgType type) {
    return create_setting_msg(type, false);
}


void free_setting_msg(setting_msg* msg) {
    if (msg == NULL) {
        return;
    }
    if (msg->payload != NULL) {
        free(msg->payload);
        msg->payload = nullptr;
    }
    
    free(msg);
    msg = nullptr;
}

uint8_t* serialize_setting_msg(setting_msg* msg, size_t* serialized_size) {
    size_t type_size = sizeof(CfgMsgType);
    *serialized_size = type_size + msg->payload_size;
    uint8_t* buffer = (uint8_t*)malloc(*serialized_size);
    if (buffer == NULL) {
        return NULL;
    }
    memcpy(buffer, &msg->type, type_size);
    memcpy(buffer + type_size, msg->payload, msg->payload_size);
    return buffer;
}

setting_msg* deserialize_setting_msg(const uint8_t* buffer, size_t buffer_len) {
    if (buffer_len < sizeof(CfgMsgType)) {
        return NULL;
    }
    setting_msg* msg = (setting_msg*)malloc(sizeof(setting_msg));
    if (msg == NULL) {
        return NULL;
    }
    memcpy(&msg->type, buffer, sizeof(CfgMsgType));
    size_t payload_size = get_msg_type_req_length(msg->type);
    if (buffer_len < sizeof(CfgMsgType) + payload_size) {
        free(msg);
        return NULL;
    }
    msg->payload = (uint8_t*)malloc(payload_size);
    if (msg->payload == NULL) {
        free(msg);
        return NULL;
    }
    memcpy(msg->payload, buffer + sizeof(CfgMsgType), payload_size);
    return msg;
}

setting_msg* deserialize_setting_msg(z_bytes_slice_iterator_t it, bool req) {
    size_t msg_type_size = sizeof(CfgMsgType::AWAKE);

    // get the type
    z_view_slice_t slice;
    if (!z_bytes_slice_iterator_next(&it, &slice)) {
        Serial.println("Error: unable to get slice from iterator");
        return NULL;
    }
    if (slice._val.len < msg_type_size) {
        Serial.println("Error: slice length is less than message type size");
        return NULL;
    }

    CfgMsgType msg_type;
    memcpy(&msg_type, slice._val.start, msg_type_size);

    auto setting_msg = create_setting_msg(msg_type, req);
    if (setting_msg == NULL) {
        Serial.println("Error: unable to create setting message");
        return NULL;
    }

    setting_msg->type = msg_type;
    setting_msg->payload_size = slice._val.len - msg_type_size;
    if (setting_msg->payload_size > 0) {
        setting_msg->payload = (uint8_t*)malloc(setting_msg->payload_size);
        if (setting_msg->payload == NULL) {
            free_setting_msg(setting_msg);
            return NULL;
        }
        memcpy(setting_msg->payload, slice._val.start + msg_type_size, setting_msg->payload_size);
    } else {
        setting_msg->payload = NULL;
    }

    return setting_msg;
}
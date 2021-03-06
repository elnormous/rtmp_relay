//
//  rtmp_relay
//

#pragma once

#include <cstdint>
#include <vector>
#include <map>

namespace relay
{
    namespace rtmp
    {
        enum Channel: uint32_t
        {
            NONE = 0,

            NETWORK = 2, // channel for network-related messages (bandwidth report, ping, etc)
            SYSTEM = 3, // channel for sending server control messages
            AUDIO = 4, // channel for audio data
            VIDEO   = 6, // channel for video data
            SOURCE  = 8, // channel for a/v invokes
        };

        enum class MessageType: uint8_t
        {
            NONE = 0,

            SET_CHUNK_SIZE = 1, // chunk size change
            ABORT = 2, // abort message
            BYTES_READ = 3, // number of bytes read
            USER_CONTROL = 4, // user control message
            SERVER_BANDWIDTH = 5, // server bandwidth
            CLIENT_BANDWIDTH = 6, // client bandwidth
            AUDIO_PACKET = 8, // audio packet
            VIDEO_PACKET = 9, // video packet
            AMF3_DATA = 15, // AMF3 data
            AMF3_SHARED_OBJECT = 16, // AMF3 shared object
            AMF3_INVOKE = 17, // AMF3 data
            AMF0_DATA = 18, // AMF0 notification
            AMF0_SHARED_OBJECT = 19, // AMF0 shared object
            AMF0_INVOKE = 20, // AMF0 invoke
            AGGREGATE = 22 // Aggregated messages
        };

        enum class UserControlType: uint16_t
        {
            CLEAR_STREAM = 0, // stream begin
            CLEAR_BUFFER = 1,
            CLIENT_BUFFER_TIME = 3,
            RESET_STREAM = 4,
            PING = 6, // from server to client
            PONG = 7 // from client
        };

        struct Header
        {
            enum class Type: uint8_t
            {
                NONE = 0xff,

                TWELVE_BYTE = 0x00, // bits 00, 12-byte header, type 0
                EIGHT_BYTE = 0x01,  // bits 01, 8-byte header, type 1
                FOUR_BYTE = 0x02,   // bits 10, 4-byte header, type 2
                ONE_BYTE = 0x03     // bits 11, 1-byte header, type 3

            };

            Type type = Type::NONE;
            uint32_t channel = Channel::NONE;
            uint32_t ts = 0; // 3-btye timestamp field
            uint32_t length = 0;
            MessageType messageType = MessageType::NONE;
            uint32_t messageStreamId = 0;
            uint64_t timestamp = 0; // final timestamp (either from 3-byte timestamp or extended timestamp fields)
        };

        struct Packet
        {
            uint32_t channel = Channel::NONE;
            MessageType messageType = MessageType::NONE;
            uint32_t messageStreamId = 0;
            uint64_t timestamp = 0;

            std::vector<uint8_t> data;

            uint32_t decode(const std::vector<uint8_t>& data, uint32_t offset, uint32_t chunkSize, std::map<uint32_t, rtmp::Header>& previousPackets);
            uint32_t encode(std::vector<uint8_t>& data, uint32_t chunkSize, std::map<uint32_t, rtmp::Header>& previousPackets) const;
        };

        struct Challenge
        {
            uint32_t time;
            uint8_t version[4];
            uint8_t randomBytes[1528];
        };

        struct Ack
        {
            uint32_t time;
            uint8_t version[4];
            uint8_t randomBytes[1528];
        };
    }
}

/*
 * SPDX-License-Identifier: Unlicense
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
 * software, either in source code form or as a compiled binary, for any purpose,
 * commercial or non-commercial, and by any means.
 *
 * For more information, please refer to <http://unlicense.org/>
 */

#pragma once
#include <vector>
#include <string>
#include <thread>

#include "rtspcallback.h"

class Rtsp2WsStream
{
    public:
        Rtsp2WsStream(HttpServerRequestHandler &httpServer, const std::string & wsurl, const std::string & rtspurl, int rtptransport, int verbose) :
            m_stop(0),
            m_cb(httpServer, wsurl),
            m_thread(std::thread([this, rtspurl, rtptransport, verbose]() {
                Environment env(m_stop);
                RTSPConnection rtspClient(env, &m_cb, rtspurl.c_str(), 10, rtptransport, verbose);
                
                env.mainloop();	
            })) {
        }

        Json::Value toJSON() {
            return m_cb.toJSON();
        }

        virtual ~Rtsp2WsStream() {
            m_stop = 1;
            m_thread.join();
        }

    private:
        char                                                          m_stop;    
        RTSPCallback                                                  m_cb;
        std::thread                                                   m_thread;
};
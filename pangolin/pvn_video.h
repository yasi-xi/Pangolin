/* This file is part of the Pangolin Project.
 * http://github.com/stevenlovegrove/Pangolin
 *
 * Copyright (c) 2011 Steven Lovegrove
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef PANGOLIN_PVN_H
#define PANGOLIN_PVN_H

#include "pangolin.h"
#include "video.h"
#include "fstream"
#include "timer.h"

namespace pangolin
{

struct VideoStream
{
    std::string name;
    VideoPixelFormat fmt;
    int w,h;
    size_t frame_size_bytes;
};

class PvnVideo : public VideoInterface
{
public:
    PvnVideo(const char* filename, bool realtime = false);
    ~PvnVideo();

    // Implement VideoInterface
    void Start();
    void Stop();
    unsigned Width() const;
    unsigned Height() const;
    size_t SizeBytes() const;
    std::string PixFormat() const;
    bool GrabNext( unsigned char* image, bool wait = true );
    bool GrabNewest( unsigned char* image, bool wait = true );

protected:
    int frames;
    std::ifstream file;
    std::vector<VideoStream> stream_info;


    bool realtime;
    pangolin::basetime frame_interval;
    pangolin::basetime last_frame;

    void ReadFileHeader();
};

}


#endif //PANGOLIN_PVN_H
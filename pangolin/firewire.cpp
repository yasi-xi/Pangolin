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

#include "firewire.h"
#ifdef HAVE_DC1394

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>

using namespace std;

namespace pangolin
{

void FirewireVideo::init_camera(
    uint64_t guid, int dma_frames,
    dc1394speed_t iso_speed,
    dc1394video_mode_t video_mode,
    dc1394framerate_t framerate
    ) {
    camera = dc1394_camera_new (d, guid);
    if (!camera)
        throw VideoException("Failed to initialize camera");

    // Attempt to stop camera if it is already running
    dc1394switch_t is_iso_on = DC1394_OFF;
    dc1394_video_get_transmission(camera, &is_iso_on);
    if (is_iso_on==DC1394_ON) {
        dc1394_video_set_transmission(camera, DC1394_OFF);
    }


    cout << "Using camera with GUID " << camera->guid << endl;

    /*-----------------------------------------------------------------------
   *  setup capture
   *-----------------------------------------------------------------------*/

    if( iso_speed >= DC1394_ISO_SPEED_800)
    {
        err=dc1394_video_set_operation_mode(camera, DC1394_OPERATION_MODE_1394B);
        if( err != DC1394_SUCCESS )
            throw VideoException("Could not set DC1394_OPERATION_MODE_1394B");
    }

    err=dc1394_video_set_iso_speed(camera, iso_speed);
    if( err != DC1394_SUCCESS )
        throw VideoException("Could not set iso speed");

    err=dc1394_video_set_mode(camera, video_mode);
    if( err != DC1394_SUCCESS )
        throw VideoException("Could not set video mode");

    err=dc1394_video_set_framerate(camera, framerate);
    if( err != DC1394_SUCCESS )
        throw VideoException("Could not set framerate");

    err=dc1394_capture_setup(camera,dma_frames, DC1394_CAPTURE_FLAGS_DEFAULT);
    if( err != DC1394_SUCCESS )
        throw VideoException("Could not setup camera - check settings");

    /*-----------------------------------------------------------------------
   *  initialise width and height from mode
   *-----------------------------------------------------------------------*/
    dc1394_get_image_size_from_video_mode(camera, video_mode, &width, &height);

    Start();
}

std::string Dc1394ColorCodingToString(dc1394color_coding_t coding)
{
    switch(coding)
    {
    case DC1394_COLOR_CODING_MONO8 : return "MONO8";
    case DC1394_COLOR_CODING_YUV411 : return "YUV411";
    case DC1394_COLOR_CODING_YUV422 : return "YUV422";
    case DC1394_COLOR_CODING_YUV444 : return "YUV444";
    case DC1394_COLOR_CODING_RGB8 : return "RGB8";
    case DC1394_COLOR_CODING_MONO16 : return "MONO16";
    case DC1394_COLOR_CODING_RGB16 : return "RGB16";
    case DC1394_COLOR_CODING_MONO16S : return "MONO16S";
    case DC1394_COLOR_CODING_RGB16S : return "RGB16S";
    case DC1394_COLOR_CODING_RAW8 : return "RAW8";
    case DC1394_COLOR_CODING_RAW16 : return "RAW16";
    default: return "";
    }
}

dc1394color_coding_t Dc1394ColorCodingFromString(std::string coding)
{
    if( !coding.compare("MONO8")) return DC1394_COLOR_CODING_MONO8;
    else if(!coding.compare("YUV411")) return DC1394_COLOR_CODING_YUV411;
    else if(!coding.compare("YUV422")) return DC1394_COLOR_CODING_YUV422;
    else if(!coding.compare("YUV444")) return DC1394_COLOR_CODING_YUV444;
    else if(!coding.compare("RGB8")) return DC1394_COLOR_CODING_RGB8;
    else if(!coding.compare("MONO16")) return DC1394_COLOR_CODING_MONO16;
    else if(!coding.compare("RGB16")) return DC1394_COLOR_CODING_RGB16;
    else if(!coding.compare("MONO16S")) return DC1394_COLOR_CODING_MONO16S;
    else if(!coding.compare("RGB16S")) return DC1394_COLOR_CODING_RGB16S;
    else if(!coding.compare("RAW8")) return DC1394_COLOR_CODING_RAW8;
    else if(!coding.compare("RAW16")) return DC1394_COLOR_CODING_RAW16;
    throw VideoException("Unknown Colour coding");
}

std::string FirewireVideo::PixFormat() const
{
    dc1394video_mode_t video_mode;
    dc1394color_coding_t color_coding;
    dc1394_video_get_mode(camera,&video_mode);
    dc1394_get_color_coding_from_video_mode(camera,video_mode,&color_coding);
    return Dc1394ColorCodingToString(color_coding);
}

void FirewireVideo::Start()
{
    if( !running )
    {
        err=dc1394_video_set_transmission(camera, DC1394_ON);
        if( err != DC1394_SUCCESS )
            throw VideoException("Could not start camera iso transmission");
        running = true;
    }
}

void FirewireVideo::Stop()
{
    if( running )
    {
        // Stop transmission
        err=dc1394_video_set_transmission(camera,DC1394_OFF);
        if( err != DC1394_SUCCESS )
            throw VideoException("Could not stop the camera");
        running = false;
    }
}

FirewireVideo::FirewireVideo(
    Guid guid,
    dc1394video_mode_t video_mode,
    dc1394framerate_t framerate,
    dc1394speed_t iso_speed,
    int dma_buffers
) :running(false)
{
    d = dc1394_new ();
    if (!d)
        throw VideoException("");

    init_camera(guid.guid,dma_buffers,iso_speed,video_mode,framerate);
}

FirewireVideo::FirewireVideo(
    unsigned deviceid,
    dc1394video_mode_t video_mode,
    dc1394framerate_t framerate,
    dc1394speed_t iso_speed,
    int dma_buffers
) :running(false)
{
    d = dc1394_new ();
    if (!d)
        throw VideoException("");

    err=dc1394_camera_enumerate (d, &list);
    if( err != DC1394_SUCCESS )
        throw VideoException("Failed to enumerate cameras");

    if (list->num == 0)
        throw VideoException("No cameras found");

    if( deviceid >= list->num )
        throw VideoException("Invalid camera index");

    const uint64_t guid = list->ids[deviceid].guid;

    dc1394_camera_free_list (list);

    init_camera(guid,dma_buffers,iso_speed,video_mode,framerate);

}

bool FirewireVideo::GrabNext( unsigned char* image, bool wait )
{
    const dc1394capture_policy_t policy =
            wait ? DC1394_CAPTURE_POLICY_WAIT : DC1394_CAPTURE_POLICY_POLL;

    dc1394video_frame_t *frame;
    dc1394_capture_dequeue(camera, policy, &frame);
    if( frame )
    {
        memcpy(image,frame->image,frame->image_bytes);
        dc1394_capture_enqueue(camera,frame);
        return true;
    }
    return false;
}

bool FirewireVideo::GrabNewest( unsigned char* image, bool wait )
{
    dc1394video_frame_t *f;
    dc1394_capture_dequeue(camera, DC1394_CAPTURE_POLICY_POLL, &f);

    if( f ) {
        while( true )
        {
            dc1394video_frame_t *nf;
            dc1394_capture_dequeue(camera, DC1394_CAPTURE_POLICY_POLL, &nf);
            if( nf )
            {
                err=dc1394_capture_enqueue(camera,f);
                f = nf;
            }else{
                break;
            }
        }
        memcpy(image,f->image,f->image_bytes);
        err=dc1394_capture_enqueue(camera,f);
        return true;
    }else if(wait){
        return GrabNext(image,true);
    }
    return false;
}

FirewireFrame FirewireVideo::GetNext(bool wait)
{
    const dc1394capture_policy_t policy =
            wait ? DC1394_CAPTURE_POLICY_WAIT : DC1394_CAPTURE_POLICY_POLL;

    dc1394video_frame_t *frame;
    dc1394_capture_dequeue(camera, policy, &frame);
    return FirewireFrame(frame);
}

FirewireFrame FirewireVideo::GetNewest(bool wait)
{
    dc1394video_frame_t *f;
    dc1394_capture_dequeue(camera, DC1394_CAPTURE_POLICY_POLL, &f);

    if( f ) {
        while( true )
        {
            dc1394video_frame_t *nf;
            dc1394_capture_dequeue(camera, DC1394_CAPTURE_POLICY_POLL, &nf);
            if( nf )
            {
                err=dc1394_capture_enqueue(camera,f);
                f = nf;
            }else{
                break;
            }
        }
        return FirewireFrame(f);
    }else if(wait){
        return GetNext(true);
    }
    return FirewireFrame(0);
}

void FirewireVideo::PutFrame(FirewireFrame& f)
{
    if( f.frame )
    {
        dc1394_capture_enqueue(camera,f.frame);
        f.frame = 0;
    }
}

float FirewireVideo::GetShutterTime() const
{
    float shutter;
    err = dc1394_feature_get_absolute_value(camera,DC1394_FEATURE_SHUTTER,&shutter);
    if( err != DC1394_SUCCESS )
        throw VideoException("Failed to read shutter");

    return shutter;

}

float FirewireVideo::GetGamma() const
{
    float gamma;
    err = dc1394_feature_get_absolute_value(camera,DC1394_FEATURE_GAMMA,&gamma);
    if( err != DC1394_SUCCESS )
        throw VideoException("Failed to read gamma");
    return gamma;
}


FirewireVideo::~FirewireVideo()
{
    Stop();

    // Close camera
    dc1394_video_set_transmission(camera, DC1394_OFF);
    dc1394_capture_stop(camera);
    dc1394_camera_free(camera);
    dc1394_free (d);
}

}

#endif

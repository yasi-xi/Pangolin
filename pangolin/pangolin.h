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

#ifndef PANGOLIN_H
#define PANGOLIN_H

#include <pangolin/config.h>
#include <pangolin/platform.h>
#include <pangolin/gl.h>
#include <pangolin/gldraw.h>
#include <pangolin/display.h>
#include <pangolin/plotter.h>

#ifdef BUILD_PANGOLIN_VARS
#include <pangolin/vars.h>
#include <pangolin/widgets.h>
#endif

#ifdef BUILD_PANGOLIN_VIDEO
#include <pangolin/video.h>
#endif // BUILD_PANGOLIN_VIDEO

// Let other libraries headers know about Pangolin
#define HAVE_PANGOLIN

#endif // PANGOLIN_H


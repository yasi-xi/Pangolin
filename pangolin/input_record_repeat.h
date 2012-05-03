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

#ifndef PANGOLIN_INPUT_RECORD_REPEAT_H
#define PANGOLIN_INPUT_RECORD_REPEAT_H

#include "pangolin.h"

#include "video.h"
#include "video_recorder.h"

#include <list>

namespace pangolin
{

struct FrameInput
{
    int index;
    std::string var;
    std::string val;
};

struct LIBRARY_API InputRecordRepeat
{
    InputRecordRepeat(const std::string& var_record_prefix);
    ~InputRecordRepeat();

    void SetIndex(int id);

    void Record();
    void Stop();

    void LoadBuffer(const std::string& filename);
    void SaveBuffer(const std::string& filename);
    void ClearBuffer();

    void PlayBuffer();
    void PlayBuffer(int start, int end);

    int Size();

protected:
    bool record;
    bool play;

    int index;
    std::ofstream file;
    std::string filename;

    std::list<FrameInput> play_queue;
    std::list<FrameInput> record_queue;

    static void GuiVarChanged(void* data, const std::string& name, _Var& var);
};

}

#endif // PANGOLIN_INPUT_RECORD_REPEAT_H

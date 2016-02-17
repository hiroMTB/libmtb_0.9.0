#pragma once

#include <sndfile.hh>
#include <vector>

using namespace std;

class SoundWriter{
    
public:
    
    static bool writeWav24f( const vector<float>& data, int nCh, int samplingRate, int totalFrames, string path ){
        writeWav24f(&data[0], nCh, samplingRate, totalFrames, path);
    }

    static bool writeWav24f( const float * data, int nCh, int samplingRate, int totalFrames, string path ){
        
        SNDFILE * file;
        SF_INFO sfinfo;
        sfinfo.samplerate = samplingRate;
        sfinfo.frames = totalFrames;
        sfinfo.channels = nCh;
        sfinfo.format = (SF_FORMAT_WAV | SF_FORMAT_PCM_24); // SF_FORMAT_FLOAT  /* 32 bit float data */
        file = sf_open(path.c_str(), SFM_WRITE, &sfinfo);
        sf_count_t frameWrote = sf_write_float(file, data, totalFrames);

        bool success = frameWrote == totalFrames;
        if( !success)
            puts(sf_strerror(file));

        sf_close(file);
        return success;
    }
};
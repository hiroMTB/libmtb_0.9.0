#include "cinder/audio/Buffer.h"
#include "cinder/audio/dsp/Dsp.h"
#include "cinder/Cinder.h"
#include <vector>

#include <Accelerate/Accelerate.h>

using namespace cinder;

class Dft{
    
public:
    
    Dft(){}
    
    void forward( const audio::Buffer*waveform, audio::BufferSpectral * spectral, size_t dftSize ){
        
        dftSize = findBiggestDftSize( dftSize );
        
        vDSP_DFT_Setup setup = vDSP_DFT_zrop_CreateSetup(0, dftSize, vDSP_DFT_FORWARD );
        if( setup == 0 ){
            cout << "Error, unable to create DFT setup." << endl;
            exit(EXIT_FAILURE);
        }
        
        ::DSPSplitComplex mSplitComplexSignal;
        
        mSplitComplexSignal.realp = spectral->getReal();
        mSplitComplexSignal.imagp = spectral->getImag();
        
        vDSP_ctoz((::DSPComplex*)waveform->getData(), 2, &mSplitComplexSignal, 1, dftSize/2 );
        vDSP_DFT_Execute(setup,
                         mSplitComplexSignal.realp, mSplitComplexSignal.imagp,	 // Input
                          mSplitComplexSignal.realp, mSplitComplexSignal.imagp); // Output
        
    }
    
    void inverse( const audio::BufferSpectral *spectral, audio::Buffer *waveform, size_t dftSize ){
        
        dftSize = findBiggestDftSize( dftSize );

        vDSP_DFT_Setup setup = vDSP_DFT_zrop_CreateSetup(0, dftSize, vDSP_DFT_INVERSE );
        if( setup == 0 ){
            cout << "Error, unable to create DFT setup." << endl;
            exit(EXIT_FAILURE);
        }

        ::DSPSplitComplex mSplitComplexSignal, mSplitComplexResult;
        
        mSplitComplexSignal.realp = const_cast<float *>( spectral->getReal() );
        mSplitComplexSignal.imagp = const_cast<float *>( spectral->getImag() );
        
        mSplitComplexResult.realp = (float *)malloc( dftSize/2 * sizeof( float ) );
        mSplitComplexResult.imagp = (float *)malloc( dftSize/2 * sizeof( float ) );

        float * data = waveform->getData();
        
        vDSP_DFT_Execute(setup,
                         mSplitComplexSignal.realp, mSplitComplexSignal.imagp,	// Input
                         mSplitComplexResult.realp, mSplitComplexResult.imagp);	// Output
        
        vDSP_ztoc( &mSplitComplexResult, 1, (::DSPComplex *)data, 2, dftSize/2 );
        
        float scale = 1.0f / float( 2 * dftSize );
        vDSP_vsmul( data, 1, &scale, data, 1, dftSize );
        
        free( mSplitComplexResult.realp );
        free( mSplitComplexResult.imagp );
    }

    static int findBiggestDftSize( int size ){
        /*
            Valid lengths for vDSP_DFT_zrop_CreateSetup in Mac OS X 10.7
            are f * 2**n, where f is 3, 5, or 15, and 5 <= n.
         */
        int  s3 = size - size%3;
        int  s5 = size - size%5;
        int s15 = size - size%15;
        
        if( !isPowerOf2(s3/3) ) s3 = nextPowerOf2(s3/3)/2*3;
        if( !isPowerOf2(s5/5) ) s5 = nextPowerOf2(s5/5)/2*5;
        if( !isPowerOf2(s15/15)) s15 = nextPowerOf2(s15/15)/2*15;
        
        size = max(s3, s5);
        size = max(size, s15);
        return size;
    }
    
};
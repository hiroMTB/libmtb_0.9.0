#pragma once

#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "cinder/Camera.h"
#include "mtUtil.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class Exporter{
    
public:
    
    bool bRender;
    bool bSnap;
    int mFrame;
    int mExitFrame;
    string filename_prefix;
    gl::FboRef mFbo;
    fs::path mRenderPath;
    ImageTarget::Options mImgWOption;
    string snapFileName;
    Exporter(){}
    
    void clear(){
        //mFbo->reset();
    }
    
    void setup( int width, int height, int startFrame, int exitFrame, GLenum colorInternalFormat, fs::path path, int aaSample, string prefix="f_", bool aFlip=false ){
        bRender = false;
        bSnap = false;
        mFrame = startFrame;
        mExitFrame = exitFrame;
        filename_prefix = prefix;
        
        gl::Texture::Format tFormat;
        tFormat.internalFormat( colorInternalFormat );
        tFormat.mipmap( false );
        tFormat.target( GL_TEXTURE_RECTANGLE_ARB );
        tFormat.wrap(GL_CLAMP_TO_EDGE );
        tFormat.minFilter( GL_NEAREST );
        tFormat.magFilter( GL_NEAREST );
        gl::Fbo::Format fFormat;
        fFormat.setSamples( aaSample );
        fFormat.colorTexture( tFormat );
        mFbo = gl::Fbo::create( width, height, fFormat );
        mFbo->getColorTexture()->setTopDown( aFlip );
        mRenderPath = path;
        
        mImgWOption.quality(1);
        
        stringstream ss;
        ss << "setup FBO Renderer " << width << " x " << height << "px\n";
        ss << "exitFrame " << exitFrame << ", AntiAlias " << aaSample << "\n";
        ss << "renderPath " << mRenderPath;
        cout << ss.str() << endl;
    }

    void bind(){
        int w = mFbo->getWidth();
        int h = mFbo->getHeight();
        gl::pushMatrices();
        gl::viewport( 0, 0, w, h );
        mFbo->bindFramebuffer();        
    }
    
    void beginPersp( float fov=60.0f, float near=1.0f, float far=10000.0f){
        int w = mFbo->getWidth();
        int h = mFbo->getHeight();
        gl::pushMatrices();
        gl::viewport( 0, 0, w, h );
        mFbo->bindFramebuffer();
        gl::setMatricesWindowPersp( w, h, fov, near, far, true );
    }

    void beginOrtho( bool center=false, bool ydown=false){
        int w = mFbo->getWidth();
        int h = mFbo->getHeight();
        gl::pushMatrices();
        glViewport( 0, 0, w, h );
        mFbo->bindFramebuffer();
        mt::setMatricesWindow( w, h, center, ydown, -1000.0f, 1000.0f);
    }
    
    void begin( const Camera & cam){
        int w = mFbo->getWidth();
        int h = mFbo->getHeight();
        gl::pushMatrices();
        gl::viewport( 0,0,w,h);
        mFbo->bindFramebuffer();
        gl::setMatrices( cam );
    }
    
    void end(){
        mFbo->unbindFramebuffer();
        gl::popMatrices();
        
        if( bRender || bSnap ){
            
            stringstream ss;
            ss << std::setfill('0') << std::setw(5) << mFrame;
            string frame_name = filename_prefix + ss.str() + ".png";

            if( bSnap && snapFileName!=""){
                frame_name = snapFileName;
            }
            writeImage( mRenderPath/frame_name,  mFbo->getColorTexture()->createSource() );
            cout << "Render Image : " << frame_name << endl;
            
            if( mExitFrame <= mFrame ){
                if( bSnap ){
                    cout << "Finish Snapshot " << frame_name << endl;
                }else{
                    cout << "Finish Rendering at " << mFrame << " frames" << endl;
                    exit(1);
                }
            }
            
            bSnap = false;
            mFrame++;
        }
    }
    
    void setFrame( int aFrame ){
        mFrame = aFrame;
    }
    
    void startRender(){
        bRender = true;
    }

    void startRenderFrom( int aFrame ){
        bRender = true;
        mFrame = aFrame;
    }

    void stopRender(){
        bRender = false;
        cout << "Stop Render at " << mFrame << endl;
    }
    
    void snapShot( string fileName=""){
        bSnap = true;
        snapFileName = fileName;
    }
    
    void draw(){
        int w = getWindowWidth();
        int h = getWindowHeight();

        //int w = mFbo->getWidth();
        //int h = mFbo->getHeight();

        gl::clear();
        gl::color(1, 1, 1);
        gl::pushMatrices();
        gl::setMatricesWindow( mFbo->getSize() );
        gl::viewport( 0,0,w,h );
        gl::draw( mFbo->getColorTexture() );
        gl::popMatrices();        
    }
};


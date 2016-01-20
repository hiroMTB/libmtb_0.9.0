#pragma once
#include <fstream>

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Vbo.h"
#include "cinder/Utilities.h"

using namespace ci;
using namespace ci::app;
using namespace std;

typedef std::function<float (float)> EaseFunc;

namespace mt {
    

    long getSeed(){
        time_t curr;
        tm local;
        time(&curr);
        local =*(localtime(&curr));
        //return local.tm_gmtoff;
        return local.tm_sec + local.tm_min;
    }
    
    string getTimeStamp(){        
        time_t curr;
        tm local;
        time(&curr);
        local =*(localtime(&curr));
        int month = local.tm_mon;
        int day = local.tm_mday;
        int hours = local.tm_hour;
        int min = local.tm_min;
        int sec = local.tm_sec;
        
        char stamp[16];
        sprintf(stamp, "%02d%02d_%02d%02d_%02d", month+1, day, hours, min, sec);
        return string(stamp);
    }
    
    string getTimeStampU(){
        return toString( time(NULL) );
    }
    
    void renderScreen( fs::path path, int frame ){
        string frame_name = "f_" + toString( frame ) + ".png";
        writeImage( path/frame_name, copyWindowSurface() );
        cout << "Render Image : " << frame << endl;
    }
    
    fs::path getAssetPath(){
        return expandPath("../../../../../assets");
    }
    
    fs::path getRenderPath( string subdir_name="" ){
        if(subdir_name!="")
            return expandPath("../../../../_rtmp") / getTimeStamp() / subdir_name ;
        else
            return expandPath("../../../../_rtmp") / getTimeStamp();
    }
    
    fs::path getProjectPath(){
        return expandPath("../../../");
    }
    
    string getProjectName(){
        
    }
    
    void saveString( string str, fs::path path ){
        ofstream ost( path.string() );
        ost << str;
        ost.close();
    }
    
    float distanceToLine( const Ray &ray, const vec3 &point){
        //return cross(ray.getDirection(), point - ray.getOrigin()).length();
    }
    
    vec3 dirToLine( const vec3 p0, const vec3 &p1, const vec3 &p2 ){
        //vec3 v = p2-p1;
        //vec3 w = p0-p1;
        //double b = v.dot(w) / v.dot(v);
        //return -w + b * v;
    }
    
    void fillSurface( Surface16u & sur, const ColorAf & col){

        Surface16u::Iter itr = sur.getIter();
        while (itr.line() ) {
            while( itr.pixel()){
                //setColorToItr( itr, col );
                sur.setPixel(itr.getPos(), col);
            }
        }
    }
    
    void drawCoordinate( float length=100 ){
//        glBegin( GL_LINES ); {
//            glColor3f( 1, 0, 0 );
//            glVertex3f( 0, 0, 0 );
//            glVertex3f( length, 0, 0 );
//            glColor3f( 0, 1, 0 );
//            glVertex3f( 0, 0, 0 );
//            glVertex3f( 0, length, 0 );
//            glColor3f(  0, 0, 1 );
//            glVertex3f( 0, 0, 0 );
//            glVertex3f( 0, 0, length );
//        } glEnd();
    }
  
    void drawScreenGuide(){
//        float w = getWindowWidth();
//        float h = getWindowHeight();
//        gl::pushMatrices();
//        gl::setMatricesWindow( getWindowSize() );
//        glBegin( GL_LINES ); {
//            glColor3f( 1,1,1 );
//            glVertex3f( w*0.5, 0, 0 );
//            glVertex3f( w*0.5, h, 0 );
//            glVertex3f( 0, h*0.5, 0 );
//            glVertex3f( w, h*0.5, 0 );
//        } glEnd();
//        gl::popMatrices();
    }

    void loadColorSample( string fileName, vector<vector<Colorf>>& col){
        Surface sur( loadImage( getAssetPath()/fileName) );
        Surface::Iter itr = sur.getIter();
        
        int w = sur.getWidth();
        int h = sur.getHeight();
        
        col.assign( w, vector<Colorf>(h) );
        
        while ( itr.line() ) {
            
            while ( itr.pixel() ) {
                float r = itr.r() / 255.0f;
                float g = itr.g() / 255.0f;
                float b = itr.b() / 255.0f;
                
                vec2 pos = itr.getPos();
                col[pos.x][pos.y].r = r;
                col[pos.x][pos.y].g = g;
                col[pos.x][pos.y].b = b;
            }
        }
        cout << "ColorSample Load Success: w:" << w << ", h " << h << endl;
    }

    clock_t timer;
    void timer_start(){
        timer = clock();
    }
    
    void timer_end(){
        double elapsedSec = clock() - timer;
        elapsedSec /= CLOCKS_PER_SEC;
        cout << "Elapsed time(sec) : "  <<  elapsedSec << endl;
    }
}
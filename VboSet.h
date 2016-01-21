#pragma once

#include "cinder/app/App.h"
#include "cinder/gl/Vbo.h"

using namespace ci;
using namespace ci::app;
using namespace std;

/*
 
    VboSet
 
    Data continaer :
        gl::VboMeshRef  vbo;
        vector<Vec3f>   pos;
        vector<ColorAf> col;
        vector<int>     ind;
 
    Usage :
        1. addPos, addCol, addInd
        2. init
        3. updateVboPos, updateVboCol, updateVboInd
 
 */
class VboSet{

private:
    vector<vec3>       pos;
    vector<ColorAf>     col;
    vector<uint32_t>    ind;

    gl::BatchRef	mPointsBatch;
    
public:
    
    gl::VboMeshRef      vbo;
    gl::VboMesh::Layout play, clay, ilay;

    inline void addPos( const vec3 & p )   { pos.push_back( p ); }
    inline void addCol( const ColorAf & c ) { col.push_back( c ); }
    inline void addInd( const uint32_t i )  { ind.push_back( i ); }
    inline void addPos( const vector<vec3> & ps )   { pos.insert(pos.end(), ps.begin(), ps.end() );  }
    inline void addCol( const vector<ColorAf> & cs ) { col.insert(col.end(), cs.begin(), cs.end() ); }
    inline void addInd( const vector<uint32_t> &is ) { ind.insert(ind.end(), is.begin(), is.end() ); }
    
    inline const vector<vec3>& getPos()    { return pos; };
    inline const vector<ColorAf>& getCol()  { return col; };
    inline const vector<uint32_t>& getInd() { return ind; };
    
    inline void writePos( int where, const vec3 & p)    { pos[where] = p; }
    inline void writeCol( int where, const ColorAf & c)  { col[where] = c; }
    inline void writeInd( int where, const uint32_t & i) { ind[where] = i; }
    
    void init( GLenum primitiveType, bool bStaticPos=true, bool bStaticCol=true ){
        play.usage(bStaticPos?GL_STATIC_DRAW:GL_DYNAMIC_DRAW).attrib(geom::POSITION,3);
        clay.usage(bStaticCol?GL_STATIC_DRAW:GL_DYNAMIC_DRAW).attrib(geom::COLOR,4);

        vbo = gl::VboMesh::create( pos.size(), ind.size(), {play, clay, ilay} );
        updateVboPos();
        updateVboCol();
        updateVboInd();
    }
    
    void updateVboPos(){
        if( vbo ){
            auto itr = vbo->mapAttrib3f( geom::POSITION );
            for( int i=0; i<pos.size(); i++ ){
                *itr++ = pos[i];
            }
            itr.unmap();
        }
    }
    
    void updateVboCol(){
        if( vbo ){
            auto itr = vbo->mapAttrib4f( geom::COLOR);
            for( int i=0; i<col.size(); i++ ){
                *itr++ = col[i];
            }
            itr.unmap();
        }
    }
    
    void updateVboInd(){
        if( vbo ){
            vbo->bufferIndices( ind.size(), &ind );
        }
    }
    
    void draw(){
        if(vbo) gl::draw( vbo );
    }
    
    void drawShader(){
        if(vbo){
            if( !mPointsBatch ){
                cout << "start making shader for VBO";
                mPointsBatch = gl::Batch::create( vbo, gl::getStockShader( gl::ShaderDef().color() ) );
                cout << " ... done" << endl;
            }
            mPointsBatch->draw();
        }
    }
    
    // does not work
    void drawRange( int start, int end){
        //if(vbo) gl::drawRange( vbo, 0, ind.size(), start, end );
    }
    
    void resetVbo(){
        vbo.reset();
    }
    
    void resetPos(){
        pos.clear();
    }
    
    void resetCol(){
        col.clear();
    }
    
    void resetInd(){
        ind.clear();
    }
};
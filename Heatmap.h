//
//  http://www.andrewnoske.com/wiki/Code_-_heatmaps_and_color_gradients
//

#include "cinder/app/App.h"

namespace mt {
    
    struct ColorPoint
    {
        float r,g,b;
        float val;
        ColorPoint(float red, float green, float blue, float value)
        : r(red), g(green), b(blue), val(value) {}
    };
    
    const vector<ColorPoint> color{
        ColorPoint(0, 0, 1,   0.0f),    // Blue
        ColorPoint(0, 1, 1,   0.25f),   // Cyan
        ColorPoint(0, 1, 0,   0.5f),   // Green.
        ColorPoint(1, 1, 0,   0.75f),   // Yellow.
        ColorPoint(1, 0, 0,   1.0f)     // Red.
    };
    
    Colorf getHeatmap(const float value){
        Colorf c;
        
        for(int i=0; i<color.size(); i++){
            const ColorPoint &currC = color[i];
            if(value < currC.val){
                const ColorPoint &prevC  = color[ max(0,i-1) ];
                float valueDiff    = (prevC.val - currC.val);
                float fractBetween = (valueDiff==0) ? 0 : (value - currC.val) / valueDiff;
                
                c.r   = (prevC.r - currC.r)*fractBetween + currC.r;
                c.g = (prevC.g - currC.g)*fractBetween + currC.g;
                c.b  = (prevC.b - currC.b)*fractBetween + currC.b;
                return c;
            }
        }
        c.r   = color.back().r;
        c.g = color.back().g;
        c.b  = color.back().b;
        return c;
    }
    
    
    void getValue( const float red, const float green, const float blue, float &value){
        //
        //  ????
        //        
    }
    
};
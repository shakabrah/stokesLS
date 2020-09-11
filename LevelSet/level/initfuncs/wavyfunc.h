#ifndef __WAVYFUNC_H__
#define __WAVYFUNC_H__

#include <math.h>
#include "initialfunc.h"

namespace levelset {
        
    class WavyFunc : public InitialFunc 
    {
        enum {Wavenum=0,Amplitude,Low};
   
    public:

        WavyFunc(const double wv = 4., const double a = 1., const double lo = 0.01);
    WavyFunc(InputParams *params) : InitialFunc() {SetParams(params);} 

        virtual double X(const double t) const;
        virtual double Y(const double t) const;
        virtual double XY(const double s, const double t) const;

    protected:
        virtual void SetParams(InputParams *params);
    };

}
#endif

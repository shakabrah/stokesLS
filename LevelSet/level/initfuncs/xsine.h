/*************************************************
    xsine.h

    $Header: xsine.h,v 1.1 2000/05/31 11:08:59 chopp Exp $

    $Log:       xsine.h,v $
    * Revision 1.1  2000/05/31  11:08:59  11:08:59  chopp (David Chopp)
    * Initial revision
    * 
    *************************************************/

#ifndef __XSINE_H__
#define __XSINE_H__

        
#include <math.h>
#include "initialfunc.h"

namespace levelset {

    class XSinusoidal : public InitialFunc 
    {
        enum {Amplitude=0, Height, Frequency, Offset};
   
    public:

        XSinusoidal(const double amp = 1., const double ht = 0., const double freq = 1., const double off = 0.);
    XSinusoidal(InputParams *params) : InitialFunc() {SetParams(params);} 

        virtual double X(const double t) const;
        virtual double Y(const double t) const;
        virtual double XY(const double s, const double t) const;
  
    protected:
        virtual void SetParams(InputParams *params);
    };

}
#endif

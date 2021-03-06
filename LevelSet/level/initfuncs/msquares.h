/*************************************************
    msquares.h

    $Header: msquares.h,v 1.1 99/02/04 14:23:03 chopp Exp $

    $Log:       msquares.h,v $
    * Revision 1.1  99/02/04  14:23:03  14:23:03  chopp (David Chopp)
    * Initial revision
    * 
    * Revision 1.1  98/03/04  15:00:22  15:00:22  chopp (David Chopp)
    * Initial revision
    * 
    * Revision 1.1  98/03/02  12:59:20  12:59:20  chopp (David Chopp)
    * Initial revision
    * 
    *************************************************/

#ifndef __MSQUARES_H__
#define __MSQUARES_H__

#include <math.h>
#include "initialfunc.h"

namespace levelset {
        
    class Msquares : public InitialFunc 
    {
        int num;
   
    public:

    Msquares(const double p, const double w, const double l)
        : num(1), InitialFunc(p,w,l) {}
    Msquares(InputParams *params) : InitialFunc() {SetParams(params);}

        virtual double XY(const double s, const double t) const;

    protected:

        virtual void SetParams(InputParams *params);
    };

}
#endif

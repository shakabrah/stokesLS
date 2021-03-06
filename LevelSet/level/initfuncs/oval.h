/*************************************************
    oval.h

    $Header: /stash/chopp/cvsroot/extend/oval.h,v 1.1.1.1 2003/05/09 17:30:39 chopp Exp $

    $Log: oval.h,v $
    Revision 1.1.1.1  2003/05/09 17:30:39  chopp
    Base code, this code may or may not work, but it did at one time.


    Revision 1.1  2003/05/08 19:15:16  chopp
    Base code, may or may not work

    * Revision 1.1  2000/06/01  11:59:54  11:59:54  chopp (David Chopp)
    * Initial revision
    * 
    *************************************************/

#ifndef __OVAL_H__
#define __OVAL_H__

#include <math.h>
#include "initialfunc.h"

namespace levelset {
        
    class Oval : public InitialFunc 
    {
        enum {XCenter=0, YCenter, XWidth, YWidth, Angle};
   
    public:

        Oval(const double xc, const double yc, const double xw, const double yw,
             const double angle = 0);
    Oval(InputParams *params) : InitialFunc() {SetParams(params);} 

        virtual double X(const double t) const;
        virtual double Y(const double t) const;
        virtual double XY(const double s, const double t) const;

    protected:
        virtual void SetParams(InputParams *params);
    };

}
#endif

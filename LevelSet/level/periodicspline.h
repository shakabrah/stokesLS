/*************************************************
    periodicspline.h

    $Header: periodicspline.h,v 1.1 99/02/04 14:37:31 chopp Exp $

    $Log:       periodicspline.h,v $
    * Revision 1.1  99/02/04  14:37:31  14:37:31  chopp (David Chopp)
    * Initial revision
    * 
    *************************************************/

#ifndef __PERIODICSPLINE_H__
#define __PERIODICSPLINE_H__

#include "spline.h"

namespace levelset {
        
    class PeriodicSpline : public class Spline {
    protected:

        double* coeff[4];
        
        void Build(const int num = 0, const double* nodes);

    public:

        PeriodicSpline(const int num = 0, const double* nodes);
        
        ~PeriodicSpline(void);
        
        void ChangeNodes(const int num, const double* nodes);
        
        virtual double operator()(const double s) const;
        virtual double D(const double s) const;
        virtual double D2(const double s) const;
        virtual double D3(const double s) const;
        virtual double D4(const double s) const {return 0;}
    };

}
#endif // __PERIODICSPLINE_H__

/*************************************************
    um2xperiodic.h

    $Header: um2periodic.h,v 2.2 99/01/06 13:59:44 chopp Exp $

    $Log:       um2periodic.h,v $
    * Revision 2.2  99/01/06  13:59:44  13:59:44  chopp (David Chopp)
    * *** none ***
    * 
    * Revision 1.1  97/12/04  10:30:27  10:30:27  chopp (David Chopp)
    * Initial revision
    * 
    *************************************************/

#ifndef __UM2XPERIODIC_H__
#define __UM2XPERIODIC_H__

#include "um2boundary.h"

namespace levelset {
        
    class UM2_XPeriodicBdry : public UM2_Boundary 
    {
    public:

    UM2_XPeriodicBdry(const int wxlo = 1, const int wxhi = 1,
                      const int wylo = 1, const int wyhi = 1)
        : UM2_Boundary(wxlo, wxhi, wylo, wyhi) {} 
        virtual void Apply(const int k);
        virtual void Apply(const int i, const int j, const int k);
        virtual int Nabor(const int i, const int j, const BCDir d, int& ni, int& nj);
    };

}
#endif // __PERIODIC_H__

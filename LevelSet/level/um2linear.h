/*************************************************
    um2linear.h

    $Header: um2linear.h,v 2.2 99/01/06 13:59:43 chopp Exp $

    $Log:       um2linear.h,v $
    * Revision 2.2  99/01/06  13:59:43  13:59:43  chopp (David Chopp)
    * *** none ***
    * 
    * Revision 1.1  97/12/04  10:30:17  10:30:17  chopp (David Chopp)
    * Initial revision
    * 
    *************************************************/

#ifndef __UM2LINEAR_H__
#define __UM2LINEAR_H__

#include "um2boundary.h"

namespace levelset {
        
    class UM2_LinearBdry : public UM2_Boundary 
    {
    public:

    UM2_LinearBdry(const int wxlo = 1, const int wxhi = 1,
                   const int wylo = 1, const int wyhi = 1)
        : UM2_Boundary(wxlo, wxhi, wylo, wyhi) {} 
        void Apply(const int k);
        void Apply(const int i, const int j, const int k);
    };

}
#endif // __LINEAR_H__



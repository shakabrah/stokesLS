/*************************************************
    boundary2d.h

    $Header: boundary2d.h,v 2.2 99/01/06 13:59:32 chopp Exp $

    $Log:       boundary2d.h,v $
    * Revision 2.2  99/01/06  13:59:32  13:59:32  chopp (David Chopp)
    * *** none ***
    * 
    * Revision 1.1  97/12/04  10:28:40  10:28:40  chopp (David Chopp)
    * Initial revision
    * 
    *************************************************/

#ifndef __BOUNDARY2D_H__
#define __BOUNDARY2D_H__

namespace levelset {

    enum {BDRY2D_XLO = 0, BDRY2D_XHI, BDRY2D_YLO, BDRY2D_YHI};

    class Boundary2D 
    {
    protected:
   
        int  width[4];
   
    public:

        Boundary2D(const int wxlo = 1, const int wxhi = 1,
                   const int wylo = 1, const int wyhi = 1);
   
        inline int Width(const int k) {return width[k];}
   
        virtual void Apply(const int k) = 0;
        virtual void Apply(const int i, const int j, const int k) =0;
    };

}
#endif // __BOUNDARY_H__

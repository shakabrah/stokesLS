/*************************************************
    boundary3d.h

    $Header: boundary3d.h,v 1.1 99/02/04 14:30:14 chopp Exp $

    $Log:       boundary3d.h,v $
    * Revision 1.1  99/02/04  14:30:14  14:30:14  chopp (David Chopp)
    * Initial revision
    * 
    *************************************************/

/*************************************************
    boundary3d.h

    $Header: boundary3d.h,v 1.1 99/02/04 14:30:14 chopp Exp $

    $Log:       boundary3d.h,v $
    * Revision 1.1  99/02/04  14:30:14  14:30:14  chopp (David Chopp)
    * Initial revision
    * 
    * Revision 1.1  97/12/04  10:28:40  10:28:40  chopp (David Chopp)
    * Initial revision
    * 
    *************************************************/

#ifndef __BOUNDARY3D_H__
#define __BOUNDARY3D_H__

namespace levelset {

    enum {BDRY3D_XLO = 0, BDRY3D_XHI,
          BDRY3D_YLO, BDRY3D_YHI,
          BDRY3D_ZLO, BDRY3D_ZHI};

    class Boundary3D 
    {
    protected:
   
        int  width[6];
   
    public:

        Boundary3D(const int wxlo = 1, const int wxhi = 1,
                   const int wylo = 1, const int wyhi = 1,
                   const int wzlo = 1, const int wzhi = 1);
   
        inline int Width(const int k) {return width[k];}
   
        virtual void Apply(const int k) = 0;
        virtual void Apply(const int i, const int j, const int k, const int l) =0;
    };

}

#endif // __BOUNDARY_H__

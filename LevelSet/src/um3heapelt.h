/*************************************************
    um3heapelt.h

    $Header: um3heapelt.h,v 1.1 99/02/04 14:39:41 chopp Exp $

    $Log:       um3heapelt.h,v $
    * Revision 1.1  99/02/04  14:39:41  14:39:41  chopp (David Chopp)
    * Initial revision
    * 
    *************************************************/

#ifndef __UM3HEAPELT_H__
#define __UM3HEAPELT_H__

#include "heapelt3d.h"
#include "uniformmesh3d.h"

namespace levelset {
        
    class UM3_HeapElement : public HeapElement3D {

        UniformMesh3D* mesh;
        int ikheap;
   
    public:

    UM3_HeapElement(UniformMesh3D* m, const int i, const int j, const int k,
                    const int iktemp, const double val) 
        : HeapElement3D(i,j,k,val), mesh(m), ikheap(iktemp) {;}
    UM3_HeapElement(void) : HeapElement3D(), mesh(NULL) {;}
   
        inline UM3_HeapElement& operator=(const UM3_HeapElement& a)
            {mesh->idata_(ind[0],ind[1],ind[2],ikheap)
             = a.mesh->idata_(a.ind[0],a.ind[1],a.ind[2],ikheap);
             HeapElement3D::operator=(a);
             return *this;}

#ifdef LEVEL_DEBUG
        friend class UniformMesh3D;
#endif
    };

}
#endif // __HEAPELT_H__

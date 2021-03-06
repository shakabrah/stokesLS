/*************************************************
    heapelt3d.h

    $Header: heapelt3d.h,v 1.1 99/02/04 14:33:08 chopp Exp $

    $Log:       heapelt3d.h,v $
    * Revision 1.1  99/02/04  14:33:08  14:33:08  chopp (David Chopp)
    * Initial revision
    * 
    *************************************************/

#ifndef __HEAPELT3D_H__
#define __HEAPELT3D_H__

namespace levelset {

    class HeapElement3D {
    protected:
   
        int ind[3];
        double   value;

    public:

        HeapElement3D(const int i, const int j, const int k, const double val); 
        HeapElement3D(void) {;}
   
        inline HeapElement3D& operator=(const HeapElement3D& a)
            {for (int i=0; i<3; ++i) ind[i] = a.ind[i]; 
             value = a.value; return *this;}

        inline HeapElement3D& operator=(const double& val)
            {value = val; return *this;}

        inline int Index(const int i) {return ind[i];}
        inline double Value(void) const {return value;}
        inline double& Value(void) {return value;} 

        friend inline int operator<(const HeapElement3D& a, const HeapElement3D& b)
        { return a.value < b.value;}
        friend inline int operator>(const HeapElement3D& a, const HeapElement3D& b)
        { return a.value > b.value;}
        friend inline int operator<=(const HeapElement3D& a, const HeapElement3D& b)
        { return a.value <= b.value;}
        friend inline int operator>=(const HeapElement3D& a, const HeapElement3D& b)
        { return a.value >= b.value;}

#ifdef LEVEL_DEBUG
        friend class UniformMesh3D;
#endif
    };

}
#endif // __HEAPELT3D_H__

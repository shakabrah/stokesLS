#ifndef __HEAPELT_H__
#define __HEAPELT_H__

namespace levelset {
        
    class HeapElement {
    protected:
   
        int ind[2];
        double   value;

    public:

        HeapElement(const int i, const int j, const double val); 
        HeapElement(void) {;}
   
        inline HeapElement& operator=(const HeapElement& a)
            {for (int i=0; i<2; ++i) ind[i] = a.ind[i]; 
             value = a.value; return *this;}

        inline HeapElement& operator=(const double& val)
            {value = val; return *this;}

        inline int Index(const int i) {return ind[i];}
        inline double Value(void) const {return value;}
        inline double& Value(void) {return value;} 

        friend inline int operator<(const HeapElement& a, const HeapElement& b)
        { return a.value < b.value;}
        friend inline int operator>(const HeapElement& a, const HeapElement& b)
        { return a.value > b.value;}
        friend inline int operator<=(const HeapElement& a, const HeapElement& b)
        { return a.value <= b.value;}
        friend inline int operator>=(const HeapElement& a, const HeapElement& b)
        { return a.value >= b.value;}

#ifdef LEVEL_DEBUG
        friend class UniformMesh2D;
#endif
    };

}

#endif // __HEAPELT_H__

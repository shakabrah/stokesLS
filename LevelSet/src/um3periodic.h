#ifndef __UM3PERIODIC_H__
#define __UM3PERIODIC_H__

#include "um3boundary.h"

namespace levelset {
        
/// Periodic boundary conditions class
    class UM3_PeriodicBdry : public UM3_Boundary 
    {
    public:

        /// Constructor for boundary condition class\n
        /// Inputs:\n
        /// (wxlo,wxhi): width of ghost point band used on the low-x and high-x sides\n
        /// (wylo,wyhi): width of ghost point band used on the low-y and high-y sides\n
        /// (wzlo,wzhi): width of ghost point band used on the low-z and high-z sides   
    UM3_PeriodicBdry(const int wxlo = 1, const int wxhi = 1,
                     const int wylo = 1, const int wyhi = 1,
                     const int wzlo = 1, const int wzhi = 1)
        : UM3_Boundary(wxlo, wxhi, wylo, wyhi, wzlo, wzhi) {} 

        /// Apply the boundary conditions to a whole slice\n
        /// Input:\n
        /// l: slice to have the boundary conditions applied
        virtual void Apply(const int l);

        /// Apply the boundary conditions to a single ghost point\n
        /// Inputs:\n
        /// (i,j,k): coordinates of the ghost point\n
        /// l: slice where the boundary conditions are being applied
        virtual void Apply(const int i, const int j, const int k, const int l);
    };

}
#endif // __PERIODIC_H__

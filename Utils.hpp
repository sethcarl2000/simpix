#ifndef Utils_HPP
#define Utils_HPP

#include "TASImage.h"
#include "TROOT.h"
#include <random> 


double Pixel_difference(const UInt_t& p1, const UInt_t& p2); 

class Annealer {
public: 
    Annealer(TASImage *_target, TASImage *_source); 

    //anneals the target to match the source, with temp 'T', for 'N' iterations. 
    //returns the fraction of total updates 'accepted' (fraction of N trials resulting in accepted swap)
    double Swap_update(double T, unsigned long N); 

    TASImage* Get_target() { return fImage_target; } 
    TASImage* Get_source() { return fImage_source; }

    //return the total square 'distance' per pixel 
    double Difference_per_pixel() const;

private: 
    TASImage *fImage_target, *fImage_source; 
    const long int f_height, f_width; 
    const long int n_pixels; 
    UInt_t *fTarget, *fSource; 

    //these are our random number generators
    std::random_device fRd; 
    std::mt19937 fGenerator; 
    std::uniform_int_distribution<long int> fRand_index_gen;
    std::uniform_real_distribution<double> fRand_uniform;  

    inline long int rand_index()   { return fRand_index_gen(fGenerator); }
    inline long int rand_uniform() { return fRand_uniform(fGenerator); }
}; 

#endif
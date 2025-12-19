
#include "Utils.hpp"
#include <stdexcept> 
#include <sstream> 
#include <cmath> 

namespace {
    constexpr UInt_t bit8_line = (1<<8) - 1; 
}

//compute the 'difference' between two pixels. 
// basically, its the squared difference between their Red, Green, and Blue values, respectivley. 
// just as if they were euclidian vectors, and we were computing their distances. 
//
// With the annealing, we will seek to minimize this value summed over all pixels. 
double Pixel_difference(const UInt_t& p1, const UInt_t& p2) 
{   
    //compute the difference with red 
    int diff_blue   = ((p1)     & bit8_line)  - ((p2)     & bit8_line);  
    int diff_green  = ((p1>>8)  & bit8_line)  - ((p2>>8)  & bit8_line);  
    int diff_red    = ((p1>>16) & bit8_line)  - ((p2>>16) & bit8_line);  

    return (double)(
        diff_blue*diff_blue + 
        diff_green*diff_green + 
        diff_red*diff_red
    ); 
}

Annealer::Annealer(TASImage* _target, TASImage* _source)
    : fImage_target{_target},
    fImage_source{_source},
    f_height{_target->GetHeight()},
    f_width{_target->GetWidth()},
    n_pixels{f_height*f_width}
{
    using namespace std; 

    //check to make sure target and source images do not match 
    if (_source->GetHeight() != f_height || _source->GetWidth() != f_width) {
        ostringstream oss; oss << "in <" << __func__ << ">: Target "
        "[" << _target->GetWidth() << "-w, " << _target->GetHeight() << "-h] and Source "
        "[" << _source->GetWidth() << "-w, " << _source->GetHeight() << "-h] dimensions do not match"; 

        throw invalid_argument(oss.str()); 
        return; 
    }

    //get the pixel arrays
    fTarget = fImage_target->GetArgbArray(); 
    fSource = fImage_source->GetArgbArray(); 

    //initialize the random number generators
    fGenerator = mt19937{fRd()}; 
    fRand_index_gen = std::uniform_int_distribution<long int>(0, n_pixels-1); 
    fRand_uniform = std::uniform_real_distribution<double>(0., 1.); 

    return; 
}


//___________________________________________________________________________________________________________________
double Annealer::Swap_update(double T, unsigned long N)
{
    unsigned long n_pass=0; 
    
    for (unsigned long i=0; i<N; i++) {

        //consider a swap of two random pixels
        long int ind1 = rand_index(); 
        long int ind2; do { ind2 = rand_index(); } while (ind1==ind2); 

        UInt_t& p1_src = fSource[ind1]; 
        UInt_t& p1_tgt = fTarget[ind1]; 
        
        UInt_t& p2_src = fSource[ind2]; 
        UInt_t& p2_tgt = fTarget[ind2]; 

        //check the 'energy change' of swapping these pixels 
        // since we're trying to minimize the pixel difference, we do [d_energy = new - old] 
        double d_energy = 
            (Pixel_difference(p1_src, p2_tgt) + Pixel_difference(p2_src, p1_tgt)) -
            (Pixel_difference(p1_src, p1_tgt) + Pixel_difference(p2_src, p2_tgt)); 

        if ( d_energy < 0. || exp( -d_energy/T ) > rand_uniform() ) {

            //do update. 
            UInt_t transp=p1_src; p1_src=p2_src; p2_src=transp;  
            n_pass++; 
        }
    }
    
    return ((double)n_pass)/((double)N); 
}

//___________________________________________________________________________________________________________________
double Annealer::Difference_per_pixel() const
{
    double total_diff =0.; 

    for (long int i=0; i<n_pixels; i++) {
        total_diff += Pixel_difference( fTarget[i], fSource[i] ); 
    }
    return total_diff/((double)n_pixels); 
}

//___________________________________________________________________________________________________________________
//___________________________________________________________________________________________________________________
//___________________________________________________________________________________________________________________
//___________________________________________________________________________________________________________________
//___________________________________________________________________________________________________________________
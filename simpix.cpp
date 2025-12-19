// simple example of using ROOT libraries in a C++ program with graphics
// and use of TASImage class

#include "TROOT.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TASImage.h"
#include "TApplication.h"
#include "TSystem.h"
#include <vector> 
#include <chrono> 
#include <cstdio> 
#include <cmath> 
#include <TGraph.h> 
#include <TPad.h> 
#include "Utils.hpp"
#include <iostream>
#include <stdio.h>
#include <string> 
using namespace std;

void Usage() 
{
  printf(
    "\n"
    "Usage: \n"
    " -h    print this message and exit\n"
    " -t    specify path to target image\n"
    " -s    specify path to source image\n"
    " -o    specify path to output image\n"
    " -c    specify path to output 'collage' of all three images\n"
  ); 
}

int main(int argc, char **argv)
{

  string path_target  = "rotunda_north_facade.png"; 
  string path_source  = "frisbe_scott_stadium.png"; 
  string path_output  = "out.png"; 
  string path_collage = "collage.png"; 

  //get options
  char opt_flag; 
  while ((opt_flag = getopt(argc, argv, "ht:s:o:c:")) != -1) {

    switch (opt_flag) {
      case 'h' : 
        Usage(); return 0; 
      case 't' : //path to target image
        path_target  = string(optarg); break; 
      case 's' : //path to source image
        path_source  = string(optarg); break; 
      case 'o' : //path to output image
        path_output  = string(optarg); break; 
      case 'c' : //path to collage
        path_collage = string(optarg); break; 
      default  : //unrecognized option
        std::cerr  << "Unrecognized option '" << opt_flag << "'." << endl;
        Usage();  
        return -1;  
    }
  }

  printf(
    "Transforming images.\n"
    "  target:  %s\n"
    "  source:  %s\n"
    "  output:  %s\n"
    "  collage: %s\n",
    path_target.data(), path_source.data(), path_output.data(), path_collage.data()
  );  

  TApplication theApp("App", &argc, argv);

  // create image objects
  TASImage *src = new TASImage(path_source.data());
  TASImage *tgt = new TASImage(path_target.data());
  TASImage *out = new TASImage(path_source.data()); // start with copy of source

  //create the Annealer class. this is defined in 'Utils.hpp' and 'Utils.cpp'
  // the annealer class will automatically check that the 'target' and 'source' images have the same dimensions.
  // it saves a copy of the pointers to the images, as well as their arrays of ARGB values for each pixel. 
  Annealer annealer(tgt, out);  

  //get the width and height of our images
  const int width  = src->GetWidth(); 
  const int height = src->GetHeight(); 

  //this is the starting and ending 'temperature'. 
  //Keep in mind that the way I define the energy difference for a swap of pixels is computing the 
  // 'square error' between the new and old arrangement of pixels; i.e., 
  // (red1-red2)^2 + (blue1-blue2)^2 + (green1-green2)^2
  const double T_start = 400.; 
  const double T_end   = 0.1;

  cout << endl; 
  unsigned long swaps_per_T = width*height*8; //gives, on average, 50 swaps-per-pixel 

  //this is the number of times temperature will be decreased. 
  // the temperature decreases exponentially, so that the final temperature will be 'T_end'. 
  int n_T_steps = 30; 
  const double T_inflation = pow( T_end/T_start, 1./((double)n_T_steps-1)); 

  //track the annealing temperature + total 'difference' per pixel 
  //vector<double> pts_T, pts_diff; 

  //starting temperature 
  double T = T_start;
  
  //start the timer clock 
  auto t_start = chrono::steady_clock::now(); 
  for (int step=0; step<n_T_steps; step++) {
    
    //do the alloted number of steps for this T-value, and measure the time it takes 
    auto t_step_start = chrono::steady_clock::now(); 
    double fraction_accepted = annealer.Swap_update(T, swaps_per_T); 
    //double diff = annealer.Difference_per_pixel(); 
    auto t_step_end   = chrono::steady_clock::now(); 

    chrono::duration<double, std::milli> duration_step = t_step_end - t_step_start; 

    //pts_T.push_back(T); pts_diff.push_back(diff); 

    printf(
      "\rStep %4i,  temp: %.2e  Fraction of proposed updates accepted: %.6f  time: %.1f ms", 
      step+1, T, fraction_accepted, duration_step.count()
    ); 
    cout << flush; 
    T *= T_inflation; 
  } 
  auto t_end = chrono::steady_clock::now(); 
  cout << endl; 

  chrono::duration<double, std::milli> total_duration = t_end - t_start;  

  printf("Total time elapsed: %.2f s", total_duration.count()/1.e3); cout << endl; 

  // print the results
  TCanvas *c1 = new TCanvas("c1", "images", 1920, 1080);
  c1->Divide(2,2);

  c1->cd(1);
  c1->Draw();
  src->Draw("X");
  c1->cd(2);
  tgt->Draw("X");
  c1->cd(3);
  out->Draw("X");
  c1->Print(path_collage.data());
  
  /*TCanvas *c2 = new TCanvas("c2", "annealing schedule", 640, 480); 
  auto g = new TGraph(pts_T.size(), pts_T.data(), pts_diff.data()); 
  g->SetTitle("Annealing schedule;T;square-error per pixel"); 
  gPad->SetLogx(1); 
  g->Draw(); */ 

  //use this little hack to get the output image to be saved as the proper size
  TCanvas *c_output = new TCanvas("c_img", "image", width, height); 
  gPad->SetMargin(0.,0.,0.,0.); 
  out->Draw("X"); 
  c_output->SaveAs(path_output.data()); 
  delete c_output; 

  // coment out the lines for running in batch mode
  cout << "Press ^c to exit" << endl;
  theApp.Run();

  return 0; 
}

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
using namespace std;

int main(int argc, char **argv)
{

  if (argc<3) {
    cout << "Usage: simpix_start image1 image2 <output=out.png>" << endl;
    return 0; 
  }
  TString fsrc=argv[1];
  TString ftgt=argv[2];
  TString fout;
  argc>3 ? fout = argv[3] : fout="out.png";
  cout << "Reading images: source= " << fsrc << " target= " << ftgt << endl;
  cout << "Output= " << fout << endl;


  TApplication theApp("App", &argc, argv);

  // create image objects
  TASImage *src = new TASImage(fsrc.Data());
  TASImage *tgt = new TASImage(ftgt.Data());
  TASImage *out = new TASImage(fsrc.Data()); // start with copy of source

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
  const double T_end   = 0.1; //T_start/1000.; 

  cout << endl; 
  unsigned long swaps_per_T = width*height*8; //gives, on average, 50 swaps-per-pixel 

  int n_T_steps = 40; 
  const double T_inflation = pow( T_end/T_start, 1./((double)n_T_steps)); 

  vector<double> pts_T, pts_diff; 

  auto t_start = chrono::steady_clock::now(); 
  double T = T_start;
  for (int step=0; step<n_T_steps; step++) {
  
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
  TCanvas *c1 = new TCanvas("c1", "images", 640, 480);
  c1->Divide(2,2);

  c1->cd(1);
  c1->Draw();
  src->Draw("X");
  c1->cd(2);
  tgt->Draw("X");
  c1->cd(3);
  out->Draw("X");
  c1->Print("collage.png");
  
  /*TCanvas *c2 = new TCanvas("c2", "annealing schedule", 640, 480); 
  auto g = new TGraph(pts_T.size(), pts_T.data(), pts_diff.data()); 
  g->SetTitle("Annealing schedule;T;square-error per pixel"); 
  gPad->SetLogx(1); 
  g->Draw(); */ 

  //use this little hack to get the output image to be saved as the proper size
  TCanvas *c_output = new TCanvas("c_img", "image", 1920, 1080); 
  gPad->SetMargin(0.,0.,0.,0.); 
  out->Draw("X"); 
  c_output->SaveAs(fout.Data()); 
  delete c_output; 


  // coment out the lines for running in batch mode
  cout << "Press ^c to exit" << endl;
  theApp.Run();

  return 0; 
}

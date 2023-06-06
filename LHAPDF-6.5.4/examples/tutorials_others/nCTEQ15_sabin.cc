//including (needed) available C++ and LHAPDF header files
#include <iostream>
#include <dlfcn.h>
#include <vector>
#include <string>
#include <random>


 //LHAPDF header files
 #include "LHAPDF/LHAPDF.h" //Main header file that includes all LHAPDF functionality
 #include "LHAPDF/GridPDF.h" //Contains classes for representing PDF grids
#include "LHAPDF/Info.h"
#include "LHAPDF/Config.h"
#include "LHAPDF/PDFInfo.h"
#include "LHAPDF/Paths.h" //paths of PDF sets and data directory
#include "LHAPDF/PDFSet.h" //Header file for PDFSet class
#include "LHAPDF/Factories.h"
#include "LHAPDF/AlphaS.h" //Contains functions for computing the strong coupling constant
#include "LHAPDF/BicubicInterpolator.h" //Contains classes for bicubic interpolation of PDF grids
#include "LHAPDF/BilinearInterpolator.h"
#include "LHAPDF/Config.h"
#include "LHAPDF/ContinuationExtrapolator.h"
#include "LHAPDF/ErrExtrapolator.h" //PDF extrapolation using error-based methods
#include "LHAPDF/Exceptions.h"
#include "LHAPDF/Extrapolator.h"
#include "LHAPDF/Factories.h"
#include "LHAPDF/FileIO.h"
#include "LHAPDF/Info.h"
#include "LHAPDF/Interpolator.h"
#include "LHAPDF/KnotArray.h"
#include "LHAPDF/LHAGlue.h"
#include "LHAPDF/LogBicubicInterpolator.h"
#include "LHAPDF/LogBilinearInterpolator.h"
#include "LHAPDF/NearestPointExtrapolator.h"
#include "LHAPDF/PDF.h"
#include "LHAPDF/PDFIndex.h"
#include "LHAPDF/Reweighting.h"
#include "LHAPDF/Utils.h"
#include "LHAPDF/Version.h"

using namespace std;
using namespace LHAPDF;


// Global params; these default values are overridden by the params file


// Helper function for computing and printing errors from the given vals vector
void printUncs(const LHAPDF::PDFSet& set, const vector<double>& vals, double cl, const string& varname, bool alt=false) {
  if (cl == 0) cl = LHAPDF::CL1SIGMA;
  cout << "PDF uncertainties on " << varname << " computed with " << set.name() << " at CL=" << cl << "%" << endl;
  const LHAPDF::PDFErrInfo errinfo = set.errorInfo();
  const LHAPDF::PDFUncertainty err = set.uncertainty(vals, cl, alt);
  if (cl >= 0) cout << "Scaled PDF uncertainties using scale = " << err.scale << endl;
  // Print summary numbers
  cout.precision(5);
  cout << varname << " = " << err.central << " +" << err.errplus << " -" << err.errminus << " (+-" << err.errsymm << ")" << endl;
  // Break down into quadrature-combined uncertainty components
  for (size_t i = 0; i < errinfo.qparts.size(); ++i) {
    //cout << "  " << errinfo.qpartName(i) << endl;
    cout << "  " << setw(12) << err.errparts[i].first << setw(12) << err.errparts[i].second << "  " << errinfo.qpartName(i) << endl;
  }
  
  

int main(){

// Set the LD_LIBRARY_PATH environment variable
setenv("LD_LIBRARY_PATH", "/home/stricklandlab/LHAPDF/LHAPDF_install/lib", 1);

//For LHAPDF = nCTEQ15WZSIH_FullNuc_208_82
LHAPDF::initPDFByName("nCTEQ15WZSIH_FullNuc_208_82", LHAPDF::LHAPDF);
LHAPDF::PDF* pdf = LHAPDF::mkPDF("CTEQ15WZSIH_FullNuc_208_82", 0);
int memset = pdf->size();
cout << "Memset value is: " << memset << endl;

LHAPDF::initPDF(memset);	

//Let's only take 4 total members out of 40, 0 being central
// Initialize PDF set with 0th member and 3 other members
const int nset = 1;  // PDF set number
const int member0 = 0;  // 0th member index
const int member1 = 15; // member index
const int member2 = 25; // member index
const int member3 = 35; // member index
const size_t nmem = 4; // number of members
vector<int> members{nset, member0, member1, member2, member3};
LHAPDF::PDFSet set("nCTEQ15WZSIH_FullNuc_208_82", LHAPDF::LHAPDF);
set.loadMembers(members);


//const size_t nmem = set.size()-1;
 
// momentum fraction; For nCTEQ14, x_min = 5e-6 and x_max = 1.0
int n;
double x_i = x_values[]; 
double xmin = getXmin();
x_i[0] = 0.0001; //xmin
double xmax = getXmax();
x_i[n] = 1; //xmax

// Q_i = Q[i] factorisation scale in GeV, here i runs from 0 to 'n=1', 
//where n is the size of the vector
double Q_i = Q_values[]; 
double Q_i[0]

// Get information about the loaded PDF set
const LHAPDF::PDFSetInfo & pdfSetInfo = pdf->info();
cout << "PDF Set Name: " << pdfSetInfo.name() << endl;
cout << "Description: " << pdfSetInfo.description() << endl;
cout << "Number of flavors: " << pdfSetInfo.numFlavors() << endl;



  for (int pid : pids) {
    const string spid = lexical_cast<string>(pid);
    const string filename = setname + "_" + smem + "_" + spid + ".dat";
    ofstream f(filename.c_str());
    for (int ix = 0; ix < NX; ++ix) {
      const double log10x = (MINLOGX + ix*DX < -1e-3) ? MINLOGX + ix*DX : 0;
      const double x = pow(10, log10x);
      for (int iq2 = 0; iq2 < NQ2; ++iq2) {
        const double log10q2 = MINLOGQ2 + iq2*DQ2;
        const double q2 = pow(10, log10q2);
        const double xf = pdf->xfxQ2(pid, x, q2);
        f << x << " " << q2 << " " << xf << endl;
      }
    }
    f.close();
  }


// Calculate PDFs at x and Q^2
double gluonpdf = pdf->xfxQ(21, x, Q2);
double protonpdf = pdf->xfxQ(2212, x, Q2);
double neutronpdf = pdf->xfxQ(2112, x, Q2);
double totalpdf = 0.0;
for (int i = -5; i <= 5; i++){
totalpdf += pdf->xfxQ(i, x, Q2);
}

// Print the results
cout << "Gluon PDF at x = " << x << " and Q^2 = " << Q2 << ": " << gluonpdf << endl;
cout << "Proton PDF at x = " << x << " and Q^2 = " << Q2 << ": " << protonpdf << endl;
cout << "Neutron PDF at x = " << x << " and Q^2 = " << Q2 << ": " << neutronpdf << endl;
cout << "Total PDF at x = " << x << " and Q^2 = " << Q2 << ": " << totalpdf << endl;

// Set parameters for the calculation
  const double Q = 10.0;  // GeV
  const double xmin = 0.01;
  const double xmax = 0.99;
  const double step = 0.01;

  // Loop over x and Q^2
  for (double x = xmin; x <= xmax; x += step) {
    for (double Q2 = 1.0; Q2 <= 100.0; Q2 += 1.0) {
      // Evaluate PDFs for each member of the set
      const vector<LHAPDF::PDF*> pdfs = set.mkPDFs();
      vector<double> xfx(pdfs.size());
      for (size_t i = 0; i < pdfs.size(); i++) {
        xfx[i] = pdfs[i]->xfxQ(21, x, Q2);
      }
      
      // Compute and print uncertainties
      cout << "x = " << x << ", Q^2 = " << Q2 << endl;
      printUncs(pdf_set, xfx, -1, "gluon PDF");
    }
  
}
return 0;


}





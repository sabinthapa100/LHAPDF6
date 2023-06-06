#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include "LHAPDF/LHAPDF.h" 
#include "LHAPDF/GridPDF.h" 
#include "LHAPDF/Config.h"
#include "LHAPDF/PDFInfo.h"
#include "LHAPDF/Info.h"
#include "LHAPDF/PDF.h"

using namespace std;

int main(){

//subfolder for the output files
string foldername = "gluonpdf_epps21";

//For EPPS21nlo_CT18Anlo_Pb208, total 107 members
//LHAPDFname_<nnnn>.dat; member 0 => LHAPDFname_<0000>.dat file, 
//member 39 => LHAPDFname_<0039>.dat file. 
// NumMembers: 107 (so we take from 0 to 106; 0 being central).
int n_mem; //pdf_member number


//(later we can upgrade this to show all the available pdfsets: including loop over different flavors)//


const string LHAPDFname = "EPPS21nlo_CT18Anlo_Pb208"; //pdf

//loop: pdf_members
for (n_mem = 0; n_mem <= 106; n_mem++) { 

//pdf_members:name
stringstream filename;
filename << "LHAPDFname" << "_" << setfill('0') << setw(4) << n_mem << ".dat";

//Load the PDF data for the current member from the file
// mkPDF(..) returns a heap-allocated PDF*: return type is a pointer
//while LHAPDF::getPDFSet(..) is not a function, 
//but a lightweight object shared between the set members; 
//and getPDFSet() is available to the users via a reference handle (not a pointer)
const LHAPDF::PDF* pdf = LHAPDF::mkPDF(LHAPDFname, n_mem); cout << pdf << endl;

//Minimum and Maximum values of (x;Q) for the pdf of memeber n_mem.
const double Xmin = 1.000000e-07; //LHAPDF::getXmin(0); n_mem =0 (central).
const double Xmax = 0.05; //taking shadowing region (x<=0.05).
int NX = 10000; //no. of steps
const double Qmin = 1.300000e+00;//LHAPDF::qMin();
const double Qmax = 10000; //no. of steps
int NQ = 10000; //no. of steps, start with less points

//the step sizes
double dX = (Xmax - Xmin)/(NX-1);
double dQ = (Qmax-Qmin)/(NQ-1);

//available flavors in a pdf
vector<int> pids = pdf->flavors();
cout << "Available flavors pids:" << " " ;
for (int i = 0; i < pids.size(); i++){
cout << pids[i] << ", ";
}
cout<<endl;

//flavors IDs available, one can directly enter that as an argument to pdf functions
//int flav = 21; //21 for gluon, 2 for up quark, and so on.

//Fix x values for which we want pdf vs Q^2
vector<double> xvalues = {1.0e-07,1.0e-06,1.0e-05,1.0e-04,0.0005, 0.001, 0.002, 0.003, 0.004,0.005,0.006,0.007,0.008,0.009,0.01,0.02,0.03, 0.04, 0.05};

//loop over NX x points
for (int ix = 0; ix < xvalues.size(); ix++){
double x = xvalues[ix];

//output
string output_pdfvsQ = foldername + "/output_x_" + to_string(x) + ".txt";
ofstream outfile(output_pdfvsQ);
//loop over Qmin to Qmax
for (int iQ = 0; iQ < NQ; iQ++) {
    double Q = Qmin + iQ*dQ;
    double Q2 = Q*Q;
    //pdf
    double xfg = pdf->xfxQ(21, x, Q); //gluon pdf
    double xfu = pdf->xfxQ(2, x, Q); //up quark pdf

     outfile << Q2 << " " << xfg << " " << xfu << std::endl;
   
  } 
 outfile.close(); //closing file
}


//fixed Q's
vector<double> qvalues = {100, 200, 500, 1000, 2000, 5000, 10000};

//loop over NQ Q values
for (int iq = 0; iq < qvalues.size(); iq++){
double q = qvalues[iq];
double q2 = q*q;

//output
string output_pdfvsx = foldername + "/output_Q_" + to_string(q) + ".txt";
ofstream outfile(output_pdfvsx);

// Loop over xmin to xmax
for (int iX = 0; iX < NX; iX++) {
    double X = Xmin + iX * dX;
    //pdf
    double Xfg = pdf->xfxQ(21, X, q); //gluon pdf
    double Xfu = pdf->xfxQ(2, X, q); //up quark pdf

    outfile << X << " " << Xfg << " " << Xfu << std::endl;

    }
    
    outfile.close(); //closing file
}


delete pdf;


}

return 0;


}

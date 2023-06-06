#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include<math.h>
#include "LHAPDF/LHAPDF.h" 
#include "LHAPDF/GridPDF.h" 
#include "LHAPDF/Config.h"
#include "LHAPDF/PDFInfo.h"
#include "LHAPDF/Info.h"
#include "LHAPDF/PDF.h"

using namespace std;

int main(){

//subfolder for the output files
string foldername = "gpdf_proton_m1S";

//Our Parameters (fixed)
//Fix x values for which we want pdf vs Q^2
vector<double> xvalues = {1.0e-05,1.0e-04,0.001, 0.01, 0.1, 1};

//fixed Q2's
vector<double> q2values = {89.4916,100};


//member number of the PDF Set, 
//LHAPDFname_<nnnn>.dat; member 0 => LHAPDFname_<0000>.dat file, 
//member 39 => LHAPDFname_<0039>.dat file. 
// NumMembers: 39 (so we take from 0 to 38; 0 being central).
int n_mem; //pdf_member number

// Get the available PDF sets
    const std::vector<string>& sets = LHAPDF::availablePDFSets();
    
    // Print out the names of all available PDF sets
    cout << "Available PDF sets:" << endl;
    for (const auto& set: sets) {
        std::cout << " - " << set << std::endl;
    }
    
    // Get paths to PDF sets
    const std::vector< std::string >& paths = LHAPDF::paths();
    std::cout << "Paths to PDF sets:" << std::endl;
    for (const auto& path : paths) {
        std::cout << " - " << path << std::endl;
    }

//download and unpack/unzip the pdf in the installation library  
// my case: /home/stricklandlab/LHAPDF/LHAPDF_install/share/LHAPDF
//terminal: $ wget http://lhapdfsets.web.cern.ch/lhapdfsets/current/NNPDF23_lo_as_0119_qed.tar.gz
// $ tar -xvzf NNPDF23_lo_as_0119_qed.tar.gz 

const string LHAPDFname = "NNPDF23_lo_as_0119_qed"; 

//loop: pdf_members, NumMembers = 101 (found in .info).
for (n_mem = 0; n_mem <= 100; n_mem++) { 

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
const double Xmin = 1.000000e-05; //LHAPDF::getXmin(0); n_mem =0 (central).
const double Xmax = 1;
int NX = 100000; //no. of steps
const double Qmin = 1.5;//LHAPDF::qMin();
const double Qmax = 100; //no. of steps
int NQ = 10; //no. of steps, start with less points

//the step sizes
double dX = (Xmax - Xmin)/(NX-1);
double dQ = (Qmax-Qmin)/(NQ-1);

//available flavors in a pdf
vector<int> pids = pdf->flavors();
cout << "Available flavors pids:" << " " ;
for (int i = 0; i < pids.size(); i++){
cout << pids[i] << ", ";
}
//flavors IDs available, one can directly enter that as an argument to pdf functions
//int flav = 21; //21 for gluon, 2 for up quark, and so on.

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

//loop over NQ Q values
for (int iq = 0; iq < q2values.size(); iq++){
double q2 = q2values[iq];
double q = sqrt(q2);

//output
string output_pdfvsx = foldername + "/output_Q2_" + to_string(q) + ".txt";
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

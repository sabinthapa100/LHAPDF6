#include "LHAPDF/LHAPDF.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>


using namespace std;


int main(){

//$LD_LIBRARY_PATH=/home/stricklandlab/LHAPDF/LHAPDF_install/lib:$LD_LIBRARY_PATH

//subfolder for the output files
string foldername = "gluonpdf_nCTEQ15";

//member number of the PDF Set, 
//LHAPDFname_<nnnn>.dat; member 0 => LHAPDFname_<0000>.dat file, 
//member 39 => LHAPDFname_<0039>.dat file. 
// NumMembers: 39 (so we take from 0 to 38; 0 being central)
int n_mem; 


//(later we can upgrade this to show all the available pdfsets: including loop over different flavors)//

//provide the name of the LHAPDF
const string LHAPDFname = "nCTEQ15WZSIH_FullNuc_208_82";

//Loop over for the member PDF sets from 0 to 38 
//for this particular nPDF- can be found in the LHAPDFname.info file

for (n_mem = 0; n_mem <= 38; n_mem++) {

//Construct the file name for the current member
stringstream filename;
filename << "LHAPDFname" << "_" << setfill('0') << setw(4) << n_mem << ".dat";

//Load the PDF data for the current member from the file
// mkPDF(..) returns a heap-allocated PDF*: return type is a pointer
//while LHAPDF::getPDFSet(..) is not a function, 
//but a lightweight object shared between the set members; 
//and getPDFSet() is available to the users via a reference handle (not a pointer)
const LHAPDF::PDF* pdf = LHAPDF::mkPDF(LHAPDFname, n_mem); //pointer_used
cout << pdf << endl;

//available flavors in a pdf
vector<int> pids = pdf->flavors();
cout << "Available flavors pids:" << " " ;
for (int i = 0; i < pids.size(); i++){
cout << pids[i] << ", ";
}
//flavors IDs available, one can directly enter that as an argument to pdf functions
//int flav = 21; //21 for gluon, 2 for up quark, and so on.


//Minimum and Maximum values of (x;Q) for the pdf of memeber n_mem.
const double xmin = LHAPDF::getXmin(0); // n_mem =0 (central).
const double xmax = LHAPDF::getXmax(0); // n_mem =0 (central).
int Nx = 1000; //step size (for now)
const double Qmin = 100; //LHAPDF::qMin();
const double Qmax = 10000; //LHAPDF::qMax();
int NQ = 1000; //step size (for now)
//calculate the step sizes
double dx = (xmax - xmin)/(Nx-1);
double dQ = (Qmax-Qmin)/(NQ-1);

// Set Q values for which we want pdf vs x; loop over these values
vector<double> Qvalues = {200, 500, 1000, 2000};

// Set x values for which we want pdf vs Q^2
vector<double> Xvalues = {0.001, 0.02, 0.05, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1};





// Loop over Q (Qmin to Qmax) for different values of x

for (int ix = 0; ix < Xvalues.size(); ix++){
    double x = Xvalues[ix];
    
 // create a new file for outputing pdf vs Q for different x
string output_pdfvsQ = foldername + "/output_x_" + to_string(x) + ".txt";
ofstream outfile(output_pdfvsQ);
 
for (int iQ = 0; iQ < NQ; iQ++) {
    double Q = Qmin + iQ*dQ;
    double Q2 = Q*Q;
    // pdf for the flavor at (x;Q)
    double xfg = pdf->xfxQ(21, x, Q); //21 for gluon

    //proton pid = 2212
    double xfu = pdf->xfxQ(2, x, Q); //2 for up quark

    // write the output to the file: xfg for gluon pdf, xfu for up quark, and so on.
    //first column is Q^2, second column is xfg and third column is xfu in the output file.
     outfile << Q2 << " " << xfg << " " << xfu << std::endl;

   
  } 
 outfile.close(); // close the file
}


for (int iQ = 0; iQ < Qvalues.size(); iQ++){
    double Q = Qvalues[iQ];

// create a new file for pdf vs x for different Q 
string output_pdfvsx = foldername + "/output_Q_" + to_string(Q) + ".txt";
ofstream outfile(output_pdfvsx);


// Loop over x values: from xmin to xmax
for (int ix = 0; ix < Nx; ix++) {
    double x = xmin + ix * dx;
       
    // pdf for the flavor at (x;Q)
    double xfg = pdf->xfxQ(21, x, Q); //21 for gluon

    //proton pid = 2212
    double xfu = pdf->xfxQ(2, x, Q); //2 for up quark

    // write the output to the file: xfg for gluon pdf, xfu for up quark, and so on.
    //first column is x, second column is xfg and third column is xfu in the output file.
     outfile << x << " " << xfg << " " << xfu << std::endl;

    }
    
    outfile.close(); // close the file
}


//clean up memory
delete pdf;


}

return 0;


}

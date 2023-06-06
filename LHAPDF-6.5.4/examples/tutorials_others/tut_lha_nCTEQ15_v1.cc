#include "LHAPDF/LHAPDF.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>

using namespace LHAPDF;
using namespace std;

int main(){


 //subfolder for the output files
string foldername = "gluon_pdf_nCTEQ15";
string command = "mkdir " + foldername;


//member number of the PDF Set
int n_mem; 

const double xmin = 0.001; 
const double xmax = 0.9; 
int Nx = 100; //step 
const double Qmin = 200; 
const double Qmax = 5000;
int NQ = 100; //step 

//provide the name of the LHAPDF
const string LHAPDFname = "nCTEQ15WZSIH_FullNuc_208_82";

for (n_mem = 0; n_mem <= 39; n_mem++) {

//Construct the file name for the current member
stringstream filename;
filename << "LHAPDFname" << "_" << setfill('0') << setw(4) << n_mem << ".dat";

//Load the PDF data for the current member from the file
const LHAPDF::PDF* pdf = LHAPDF::mkPDF(LHAPDFname, n_mem); //pointer_used

//flavor
int pid = 21;
cout << "Particle ID = " << pid << endl; //gluon

//calculate the step sizes
double dx = (xmax - xmin)/(Nx-1);
double dQ = (Qmax-Qmin)/(NQ-1);


// Open output file for writing PDF values
//std::ofstream f("gluon_pdf.dat");

//Loop over x (from xmin to xmax) and Q (from Qmin to QMax) values
for (int ix = 0; ix < Nx; ix++) {
    double x = xmin + ix * dx;
    cout << "x = " << x << endl;
    for (int iQ = 0; iQ < NQ; iQ++) {
        double Q = Qmin + iQ * dQ;

        //pdf for the flavor (pid) at (x;Q)
        double xf = pdf -> xfxQ(pid, x, Q);

        //create a new file with a unique name based on the x and Q
        string filename = foldername + "/output_x_" + to_string(x) + "_Q_" + to_string(Q) + ".txt";
        ofstream outfile(filename);

        //write the output to the file
        cout << Q << " " << xf << std::endl;

        outfile.close(); //close the file
    }
}



//clean up memory
delete pdf;
}

return 0;


}

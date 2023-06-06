#include "LHAPDF/LHAPDF.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>

using namespace LHAPDF;
using namespace std;

int main(){

//Remember to specify the env variable path (terminal): 
//$export LD_LIBRARY_PATH=/home/stricklandlab/LHAPDF/LHAPDF_install/lib:$LD_LIBRARY_PATH 
//member number of the PDF Set, 
//LHAPDFname_<nnnn>.dat; member 0 => LHAPDFname_<0000>.dat file, 
//member 39 => LHAPDFname_<0039>.dat file. 
// NumMembers: 39 (so we take from 0 to 39; 0 being central)
int n_mem; 

//(later we can upgrade this to show all the available pdfsets)
//provide the name of the LHAPDF
string LHAPDFname = "nCTEQ15WZSIH_FullNuc_208_82";

//Loop over for the member PDF sets from 0 to 39 
//for this particular nPDF- can be found in the LHAPDFname.info file

for (n_mem = 0; n_mem <= 39; n_mem++) {

//Construct the file name for the current member
stringstream filename;
filename << "LHAPDFname" << "_" << setfill('0') << setw(4) << n_mem << ".dat";

//Load the PDF data for the current member from the file
// getPDFSet() returns a reference handle to the entire PDF set
// getPDF() returns a reference to the specific PDF member
const LHAPDF::PDFSet& pdfSet = LHAPDF::getPDFSet("LHAPDFname");
const LHAPDF::PDF& pdf = pdfSet.getPDF(n_mem);

double xmin = LHAPDF::xMin();
double xmax = LHAPDF::xMax();
double Qmin = LHAPDF::qMin();
double Qmax = LHAPDF::qMax();



cout << pdf.xfxQ(21, 1e-4, 100.);

}

return 0;

}


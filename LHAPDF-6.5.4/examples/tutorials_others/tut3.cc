//including (needed) available C++ and LHAPDF header files
#include <iostream>
#include <dlfcn.h>
#include <vector>
#include <string>

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

int main(){
// Get a function pointer for availablePDFSets
const char* availablePDFSets_name = "LHAPDF::availablePDFSets()";

auto availablePDFSets = reinterpret_cast<std::vector<std::string>(*)()>(dlsym(lhapdf_library, availablePDFSets_name));

if (availablePDFSets == nullptr) {
    std::cerr << "Failed to get availablePDFSets function pointer." << std::endl;
    dlclose(lhapdf_library);
    return 1;
}

// Print out the names of all available PDF sets
std::vector<std::string> sets = availablePDFSets();
std::cout << "Available PDF sets:" << std::endl;
for (const auto& set: sets) {
    std::cout << " - " << set << std::endl;
}

// Get a function pointer for mkPDF
const char* mkPDF_name = "LHAPDF::mkPDF(std::string const&, int)";
auto mkPDF = reinterpret_cast<LHAPDF::PDF*(*)(std::string const&, int)>(dlsym(lhapdf_library, mkPDF_name));

if (mkPDF == nullptr) {
    std::cerr << "Failed to get mkPDF function pointer." << std::endl;
    dlclose(lhapdf_library);
    return 1;
}

// Prompt user to select a PDF set and load it
std::string pdfSetName;
std::cout << "Enter the name of a PDF set to load: ";
std::getline(std::cin, pdfSetName);
LHAPDF::PDF* pdf = mkPDF(pdfSetName, 0);

// Get information about the loaded PDF set
const LHAPDF::PDFSetInfo& pdfSetInfo = pdf->info();
std::cout << "PDF Set Name: " << pdfSetInfo.name() << std::endl;
std::cout << "Description: " << pdfSetInfo.description() << std::endl;
std::cout << "Number of flavors: " << pdfSetInfo.numFlavors() << std::endl;

// Set x and Q^2 values
double x, Q2;
std::cout << "Enter a value for x: ";
std::cin >> x;
std::cout << "Enter a value for Q^2: ";
std::cin >> Q2;

// Get a function pointer for xfxQ
const char* xfxQ_name = "LHAPDF::PDF::xfxQ(int, double, double) const";
auto xfxQ = reinterpret_cast<double(*)(int, double, double)>(dlsym(lhapdf_library, xfxQ_name));

if (xfxQ == nullptr) {
    std::cerr << "Failed to get xfxQ function pointer." << std::endl;
    delete pdf;
    dlclose(lhapdf_library);
    return 1;
}

// Calculate PDFs at x and Q^2
double gluonpdf = xfxQ(21, x, Q2);
double protonpdf = x;


}


//including (needed) available C++ and LHAPDF header files
#include <iostream>
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

int main(){

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

    // Prompt user to select a PDF set and load it
    string pdfSetName;
    cout << "Enter the name of a PDF set to load: ";
    getline(cin, pdfSetName);
    LHAPDF::PDF* pdf = LHAPDF::mkPDF(pdfSetName, 0);

    // Get information about the loaded PDF set
    const LHAPDF::PDFSetInfo & pdfSetInfo = pdf->info();
    cout << "PDF Set Name: " << pdfSetInfo.name() << endl;
    cout << "Description: " << pdfSetInfo.description() << endl;
    cout << "Number of flavors: " << pdfSetInfo.numFlavors() << endl;

    // Set x and Q^2 values
    double x, Q2;
    cout << "Enter a value for x: ";
    cin >> x;
    cout << "Enter a value for Q^2: ";
    cin >> Q2;

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

    // Delete the PDF object
    delete pdf;

    return 0;
}


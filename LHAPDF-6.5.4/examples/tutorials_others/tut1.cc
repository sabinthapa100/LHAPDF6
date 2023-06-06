#include <iostream>
#include <string>
#include "LHAPDF/LHAPDF.h"

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
    
    // Load the nCTEQ15WZSIH_FullNuc_208_82 PDF set
    LHAPDF::PDF* pdf = LHAPDF::mkPDF("nCTEQ15WZSIH_FullNuc_208_82", 0);

    // Set x and Q^2 values
    double x = 0.01;
    double Q2 = 10.0;

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


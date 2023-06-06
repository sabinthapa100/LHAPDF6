#include <iostream>
#include <fstream>
#include "LHAPDF/LHAPDF.h"

using namespace std;

int main() {
    // create a PDF object for a proton beam
    LHAPDF::PDF* pdf = LHAPDF::mkPDF("nCTEQ15WZSIH_FullNuc_208_82", 0);

    // set the values of x and Q^2
    double x[5] = {0.01, 0.1, 0.3, 0.5, 0.8};
    double Q2[5] = {10.0, 20.0, 50.0, 100.0, 200.0};

    // open output file for writing
    ofstream outfile("output.txt");

    // write column headers to output file
    outfile << "x" << "\t" << "Q^2" << "\t" << "gluon pdf" << "\t" << "valence pdf" << "\t" << "sea pdf" << "\t" << "total pdf" << endl;

    // loop over x and Q^2 values
    for (int i=0; i<5; i++) {
        for (int j=0; j<5; j++) {
            // extract the PDF values
            double gluonPDF = pdf->xfxQ2(21, x[i], Q2[j]);
            double valencePDF = pdf->xfxQ2(2, x[i], Q2[j]) + pdf->xfxQ2(-2, x[i], Q2[j]);
            double seaPDF = pdf->xfxQ2(1, x[i], Q2[j]) + pdf->xfxQ2(-1, x[i], Q2[j]) + pdf->xfxQ2(3, x[i], Q2[j]) + pdf->xfxQ2(-3, x[i], Q2[j]);
            double totalPDF = pdf->xfxQ2(0, x[i], Q2[j]);

            // write the results to the output file
            outfile << x[i] << "\t" << Q2[j] << "\t" << gluonPDF << "\t" << valencePDF << "\t" << seaPDF << "\t" << totalPDF << endl;
        }
    }

    // close the output file and delete the PDF object
    outfile.close();
    delete pdf;

    return 0;
}


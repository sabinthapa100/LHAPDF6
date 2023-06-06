#include <iostream>
#include "LHAPDF/LHAPDF.h"

using namespace std;

int main() {
    // Load the PDF set
    LHAPDF::PDF* pdf = LHAPDF::mkPDF("nCTEQ15nlo", 103100);

    // Set the kinematics (x and Q^2)
    double x = 0.1;
    double Q2 = 100.0;

    // Calculate the PDFs
    map<int, double> pdfs = pdf->xfxQ2(x, Q2);

    // Extract the gluon PDF
    double gluon_pdf = pdfs[21];

    // Extract the total PDF
    double total_pdf = 0;
    for (int i = -5; i <= 5; i++) {
        total_pdf += pdfs[i];
    }

    // Extract the other PDFs
    double u_pdf = pdfs[2];
    double d_pdf = pdfs[1];
    double s_pdf = pdfs[3];
    double c_pdf = pdfs[4];
    double b_pdf = pdfs[5];

    // Print the results
    cout << "x = " << x << endl;
    cout << "Q^2 = " << Q2 << endl;
    cout << "Gluon PDF = " << gluon_pdf << endl;
    cout << "Total PDF = " << total_pdf << endl;
    cout << "u PDF = " << u_pdf << endl;
    cout << "d PDF = " << d_pdf << endl;
    cout << "s PDF = " << s_pdf << endl;
    cout << "c PDF = " << c_pdf << endl;
    cout << "b PDF = " << b_pdf << endl;

    // Delete the PDF object
    delete pdf;

    return 0;
}


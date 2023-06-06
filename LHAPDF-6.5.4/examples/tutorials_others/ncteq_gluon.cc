#include <iostream>
#include "LHAPDF/LHAPDF.h"

int main() {

    LHAPDF::PDF* pdf = LHAPDF::mkPDF("nCTEQ15WZSIH_FullNuc_208_82", 0);
    double Q = 10.0;
    
    std::cout << "x\tPDF_gluon" << std::endl;
    for (double x = 1e-5; x < 1.0; x += 0.01) {
        double pdf_gluon = pdf->xfxQ(21, x, Q);
        std::cout << x << "\t" << pdf_gluon << std::endl;
    }
    
    delete pdf;
    return 0;
}


#include "LHAPDF/PDF.h"
#include "LHAPDF/GridPDF.h"

// Load PDF data from disk
GridPDF::mypdf("CT10nlo");

// Load PDF data from memory
std::string data = "..."; // PDF data as a string
GridPDF mypdf(data);

double x = 0.1;
double Q2 = 100.0;
int flavor = 1; // up quark
double pdfvalue = mypdf.xfxQ2(flavor, x, Q2);

mypdf.setInterpolator("linear");
mypdf.setExtrapolator("polynomial");



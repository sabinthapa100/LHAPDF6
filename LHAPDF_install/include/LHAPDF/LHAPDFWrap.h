#ifndef LHAPDFWRAP_H
#define LHAPDFWRAP_H
 
#include <string>
#include <vector>
 
// WARNING! This C++ interface is deprecated in favour of the 
// interface declared in LHAPDF/LHAPDF.h

// This class is a wrapper around the LHAPDF package for parton
// distribution functions of the proton.
//
// Originally by Stefan Gieseke.
// Adapted for LHAPDFv4 by Mike Whalley.
// Adapted for LHAPDFv5 by Craig Group/Mike Whalley.
// Fortran portability and interface improvements by Andy Buckley.


class LHAPDFWrap {

public:
LHAPDFWrap();
   
LHAPDFWrap(const std::string& name);

LHAPDFWrap(const std::string& name, int memset);

LHAPDFWrap(int nset, const std::string& name);

LHAPDFWrap(int nset, const std::string& name, int memset);

std::vector<double> xfx(double x, double Q);

double xfx(double x, double Q, int fl);
 
std::vector<double> xfxp(double x, double Q, double P2, int ip);
double xfxp(double x, double Q, double P2, int ip, int fl);

std::vector<double> xfxa(double x, double Q, double a);
double xfxa(double x, double Q, double a, int fl);
 
std::vector<double> xfxphoton(double x, double Q);
double xfxphoton(double x, double Q, int fl);

void initPDFSet(const std::string& name);

void initPDFSetByName(const std::string& name);

void initPDF(int memset);

void getDescription();

int numberPDF();
 
double alphasPDF(double Q);

int getOrderPDF();

int getOrderAlphaS();

double getQMass(int f);

double getThreshold(int f);

int getNf();

double getLam4(int m);

double getLam5(int m);

double getXmin(int m);
double getXmax(int m);
double getQ2min(int m);
double getQ2max(int m);
 
void extrapolate();

// Additional functions for when more than 1 PDF set is being stored in memory

// Returns a vector xf(x, Q) with index 0 < i < 12.
// 0..5 = tbar, ..., ubar, dbar; 
// 6 = g; 
// 7..12 = d, u, ..., t
std::vector<double> xfxM(int nset, double x, double Q);
00142 
// Returns xf(x, Q) for flavour fl - this time the flavour encoding
00144   // is as in the LHAPDF manual...
00145   // -6..-1 = tbar,...,ubar, dbar
00146   // 1..6 = duscbt
00147   // 0 = g
 double xfxM(int nset, double x, double Q, int fl);

std::vector<double> xfxpM(int nset, double x, double Q, double P2, int ip);
double xfxpM(int nset, double x, double Q, double P2, int ip, int fl);

std::vector<double> xfxaM(int nset, double x, double Q, double a);
double xfxaM(int nset, double x, double Q, double a, int fl);

std::vector<double> xfxphotonM(int nset, double x, double Q);
double xfxphotonM(int nset, double x, double Q, int fl);

void initPDFSetM(int nset, const std::string& name);

void initPDFSetByNameM(int nset, const std::string& name);
 
void initPDFM(int nset, int memset);
 
void getDescriptionM(int nset);

int numberPDFM(int nset);

double alphasPDFM(int nset, double Q);

int getOrderPDFM(int nset);

int getOrderAlphaSM(int nset);

double getQMassM(int nset, int f);

double getThresholdM(int nset, int f);

int getNfM(int nset);

double getLam4M(int nset, int m);

 double getLam5M(int nset, int m);

double getXminM(int nset, int m);
double getXmaxM(int nset, int m);
double getQ2minM(int nset, int m);
double getQ2maxM(int nset, int m);

};
 
#endif

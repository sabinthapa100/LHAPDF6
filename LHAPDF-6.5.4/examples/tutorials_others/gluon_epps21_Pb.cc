// Program to test LHAPDF6 PDF CPU/memory performance by sampling PDFs in several ways

#include "LHAPDF/LHAPDF.h"
#include <iostream>
#include <cmath>
#include <ctime>
#ifdef HAVE_MPI
#include <mpi.h>
#endif
using namespace std;

int main(int argc, char* argv[]) {

#ifdef HAVE_MPI
  MPI_Init(&argc, &argv);
#endif

  const double MINLOGX = -7.5;
  const double MINLOGQ = 1;
  const double MAXLOGQ = 3;
  const double dx = 0.005;
  const double dq = 0.005;

  const clock_t start = clock();

  const LHAPDF::PDF* pdf = LHAPDF::mkPDF(argv[1], std::stoi(argv[2]));

  const clock_t init = clock();

  vector<double> xfs; xfs.resize(pdf->nFlavors());
  for (double log10x = MINLOGX; log10x <= 0.0; log10x += dx) {
    for (double log10q = MINLOGQ; log10q <= MAXLOGQ; log10q += dq) {
      pdf->xfxQ2(0, pow(10, log10x), pow(10, log10q), xfs);
    }
  }

  const clock_t end = clock();

  std::cout << "Init  = " << (init - start) << std::endl;
  std::cout << "Query = " << (end - init) << std::endl;
  std::cout << "Total = " << (end - start) << std::endl;

  delete pdf;

#ifdef HAVE_MPI
  MPI_Finalize();
#endif
  return 0;
}


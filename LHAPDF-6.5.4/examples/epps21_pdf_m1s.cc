#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <math.h>
#include <vector>
#include "LHAPDF/LHAPDF.h"
#include "LHAPDF/Config.h"
#include "LHAPDF/PDFInfo.h"
#include "LHAPDF/Info.h"
#include "LHAPDF/PDF.h"

using namespace std;

int main() {

    // Subfolder for the output files
    string foldername = "q2=m1s2";

    // For EPPS21nlo_CT18Anlo_Pb208, total 107 members
    // LHAPDFname_<nnnn>.dat; member 0 => LHAPDFname_<0000>.dat file,
    // member 39 => LHAPDFname_<0039>.dat file.
    // NumMembers: 107 (so we take from 0 to 106; 0 being central).
    int n_mem; // PDF member number

    //(later we can upgrade this to show all the available pdfsets: including loop over different flavors)//

    const string LHAPDFname = "EPPS21nlo_CT18Anlo_Pb208"; // PDF

    // Our Parameters (fixed)
    // Fix x values for which we want pdf vs Q^2
    vector<double> xvalues = {1.0e-05, 1.0e-04, 0.001, 0.01, 0.1, 1};

    // Fixed Q2's
    vector<double> q2values = {89.4916, 100};

    // Loop: PDF members
    for (n_mem = 0; n_mem <= 106; n_mem++) {

        // PDF member name
        stringstream filename;
        filename << "LHAPDFname" << "_" << setfill('0') << setw(4) << n_mem << ".dat";

        // Load the PDF data for the current member from the file
        // mkPDF(..) returns a heap-allocated PDF*: return type is a pointer
        // while LHAPDF::getPDFSet(..) is not a function,
        // but a lightweight object shared between the set members;
        // and getPDFSet() is available to the users via a reference handle (not a pointer)
        const LHAPDF::PDF* pdf = LHAPDF::mkPDF(LHAPDFname, n_mem);
        cout << pdf << endl;

        // Minimum and Maximum values of (x;Q) for the pdf of member n_mem.
        const double Xmin = 1.000000e-05; // LHAPDF::getXmin(0); n_mem =0 (central).
        const double Xmax = 1; // whole x for fixed Qs.
        int NX = 100000; // Number of steps
        const double Qmin = 1.5; // LHAPDF::qMin();
        const double Qmax = 100; // qMax, i.e., Q^2max = 10000 GeV
        int NQ = 10; // Number of steps, start with less points

        // The step sizes
        double dX = (Xmax - Xmin) / (NX - 1);
        double dQ = (Qmax - Qmin) / (NQ - 1);

        // Available flavors in a PDF
        vector<int> pids = pdf->flavors();
        cout << "Available flavors pids: ";
        for (int i = 0; i < pids.size(); i++) {
            cout << pids[i] << ", ";
        }
        cout << endl;

        // Flavors IDs available, one can directly enter that as an argument to PDF functions
        // int flav = 21; // 21 for gluon, 2 for up quark, and so on.

        // Loop over NX x points
        for (int ix = 0; ix < xvalues.size(); ix++) {
            double x = xvalues[ix];

            // Output
            string output_pdfvsQ = foldername + "/output_x_" + "_" + to_string(n_mem) + to_string(x) + ".txt";
            ofstream outfile(output_pdfvsQ);

            // Loop over Qmin to Qmax
            for (int iQ = 0; iQ < NQ; iQ++) {
                double Q = Qmin + iQ * dQ;
                double Q2 = Q * Q;

                // PDF
                double xfg = pdf->xfxQ(21, x, Q); // Gluon PDF
                double xfu = pdf->xfxQ(2, x, Q); // Up quark PDF

                outfile << Q2 << " " << xfg << " " << xfu << std::endl;
            }

            outfile.close(); // Closing file
        }

        // Loop over NQ Q values
        for (int iq = 0; iq < q2values.size(); iq++) {
            double q2 = q2values[iq];
            double q = sqrt(q2);

            // Output
            string output_pdfvsx = foldername + "/output_Q2_" + "_" + to_string(n_mem) + to_string(q2) + ".txt";
            ofstream outfile(output_pdfvsx);

            // Loop over xmin to xmax
            for (int iX = 0; iX < NX; iX++) {
                double X = Xmin + iX * dX;

                // PDF
                double Xfg = pdf->xfxQ(21, X, q); // Gluon PDF
                double Xfu = pdf->xfxQ(2, X, q); // Up quark PDF

                outfile << X << " " << Xfg << " " << Xfu << std::endl;
            }

            outfile.close(); // Closing file
        }

        delete pdf;
    }

    return 0;
}


#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <math.h>
#include <sys/stat.h> // for mkdir
#include <vector>
#include "LHAPDF/LHAPDF.h" 
#include "LHAPDF/Config.h"
#include "LHAPDF/PDFInfo.h"
#include "LHAPDF/Info.h"
#include "LHAPDF/PDF.h"

using namespace std;

int main() {
    int pdfChoiceAttempts = 0; // Number of attempts for PDF choice
    const int maxAttempts = 3; // Maximum number of attempts for PDF choice
    string LHAPDFname; // pdf_name

    // Show all the available PDF sets
    cout << "List of available PDF sets:" << endl;
    vector<string> availablePDFSets = LHAPDF::availablePDFSets();
    for (int i = 0; i < availablePDFSets.size(); i++) {
        cout << i + 1 << ". " << availablePDFSets[i] << endl;
    }
    cout << "==============================" << endl;

    // Choose one PDF set
    while (pdfChoiceAttempts < maxAttempts) {
        cout << "Choose a PDF set (enter the corresponding number): ";
        int pdfChoice;
        cin >> pdfChoice;
        if (pdfChoice >= 1 && pdfChoice <= availablePDFSets.size()) {
            LHAPDFname = availablePDFSets[pdfChoice - 1];
            cout << "Selected PDF set: " << LHAPDFname << endl;
            break; // Valid choice, exit the loop
        }
        else {
            cout << "Invalid choice. Please choose a number from the list." << endl;
            pdfChoiceAttempts++;
        }
    }

    // Check if maximum attempts reached
    if (pdfChoiceAttempts == maxAttempts) {
        cout << "Maximum attempts reached. Exiting..." << endl;
        return 0; // Exit the program
    }

    // Find the number of PDF members in the selected PDF set
    const std::string setname = LHAPDFname;
    const LHAPDF::PDFSet set(setname);
    const size_t n = set.size();
    cout << "Number of PDF members in set '" << setname << "': " << n << std::endl;
	cout << "==============================" << endl;


    // Our Parameters (fixed)
    // Fix x values for which we want pdf vs Q^2
    vector<double> xvalues = {0.001};
    // Fixed Q2's
    vector<double> q2values = {100};

    // Create output folder
    string foldername = "output_" + LHAPDFname;
    int check = mkdir(foldername.c_str(), 0777);
    if (check == 0)
        cout << "Output folder created: " << foldername << endl;
    else {
        cout << "Unable to create output folder. Exiting..." << endl;
        return 0;
    }

    // Loop: pdf_members
    for (int n_mem = 0; n_mem <= n; n_mem++) {

        // PDF members:
        stringstream filename;
        filename << LHAPDFname << "_" << setfill('0') << setw(4) << n_mem << ".dat";

        // Load the PDF data for the current member from the file
        const LHAPDF::PDF* pdf = LHAPDF::mkPDF(LHAPDFname, n_mem);
        cout << pdf << endl;

        // Minimum and Maximum values of (x;Q) for the pdf of member n_mem.
        const double Xmin = 0.00001;
        const double Xmax = 1;
        int NX = 1000;
        const double Qmin = 1.5;
        const double Qmax = 100;
        int NQ = 1000;

        // The step sizes
        double dX = (Xmax - Xmin) / (NX - 1);
        double dQ = (Qmax - Qmin) / (NQ - 1);

        // Available flavors in a pdf
        vector<int> pids = pdf->flavors();
        cout << "Available flavors pids: ";
        for (int i = 0; i < pids.size(); i++) {
            cout << pids[i] << ", ";
        }
        cout << endl;

        // Loop over NX x points
        for (int ix = 0; ix < xvalues.size(); ix++) {
            double x = xvalues[ix];

            // Output
            string output_pdfvsQ = foldername + "/output_x_" + to_string(x) + "_" + to_string(n_mem) + ".txt";
            ofstream outfile(output_pdfvsQ);

            // Loop over Qmin to Qmax
            for (int iQ = 0; iQ < NQ; iQ++) {
                double Q = Qmin + iQ * dQ;
                double Q2 = Q * Q;
                // PDF
                double xfg = pdf->xfxQ(21, x, Q); // Gluon pdf
                double xfu = pdf->xfxQ(2, x, Q);  // Up quark pdf

                outfile << Q << " " << xfg << " " << xfu << std::endl;
            }

            outfile.close(); // Closing file
        }

        // Loop over NQ Q values
        for (int iq = 0; iq < q2values.size(); iq++) {
            double q2 = q2values[iq];
            double q = sqrt(q2);

            // Output
            string output_pdfvsx = foldername + "/output_Q_" + to_string(q) + "_" + to_string(n_mem) + ".txt";
            ofstream outfile(output_pdfvsx);

            // Loop over xmin to xmax
            for (int iX = 0; iX < NX; iX++) {
                double X = Xmin + iX * dX;
                // PDF
                double Xfg = pdf->xfxQ(21, X, q); // Gluon pdf
                double Xfu = pdf->xfxQ(2, X, q);  // Up quark pdf

                outfile << X << " " << Xfg << " " << Xfu << std::endl;
            }

            outfile.close(); // Closing file
        }

        delete pdf;
    }

    return 0;
}


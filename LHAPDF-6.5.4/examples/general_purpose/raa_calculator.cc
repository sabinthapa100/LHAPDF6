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
    string LHAPDFname; // name of the pdf

    // Show all the available PDF sets
    cout << "==============================" << endl;
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
        } else {
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
    const LHAPDF::PDFSet set(LHAPDFname);
    const size_t n = set.size();
    cout << "Number of PDF members in set '" << setname << "': " << n - 1 << std::endl;
    cout << "==============================" << endl;

    // Our Parameters (fixed)
    // Fix x values for which we want pdf vs Q^2
    vector<double> xvalues = {0.001, 0.01, 0.1, 1};
    // Fixed Q2's
    vector<double> q2values = {100, 200, 400, 500};

    // Get the number of X points from the user
    cout << "------------------" << endl;
    int NX;
    cout << "Enter the number of X points: ";
    cin >> NX;

    // Get the number of Q points from the user
    int NQ;
    cout << "Enter the number of Q points: ";
    cin >> NQ;
    cout << "------------------" << endl;

    // Minimum and Maximum values of (x;Q) for the pdf of member n_mem.
    LHAPDF::initPDFSet(LHAPDFname);
    const LHAPDF::PDF* basepdf = LHAPDF::mkPDF(LHAPDFname);
    double Xmin = LHAPDF::getXmin(0);
    cout << "Xmin = " << Xmin << endl;
    double Xmax = LHAPDF::getXmax(0);
    cout << "Xmax = " << Xmax << endl;
    double q2min = LHAPDF::getQ2min(0);
    double Qmin = sqrt(q2min);
    cout << "Q2_min = " << q2min << endl;
    double q2max = LHAPDF::getQ2max(0);
    double Qmax = sqrt(q2max);
    cout << "Q2_max = " << q2max << endl;

    // The step sizes
    double dX = (Xmax - Xmin) / NX;
    double dQ = (Qmax - Qmin) / (NQ - 1);

    // Create an output folder
        std::string foldername = "output_" + LHAPDFname;

    // Create a folder/directory
    int status = mkdir(foldername.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    if (status == 0) {
        std::cout << "Folder created successfully!" << std::endl;
    } else {
        std::cout << "Failed to create the folder." << std::endl;
    }


    // Loop: pdf_members
    for (int n_mem = 0; n_mem < n; n_mem++) {
        // PDF members:
        // Load the PDF data for the current member from the file
        LHAPDF::initPDFSet(LHAPDFname);
        const LHAPDF::PDF* pdf = LHAPDF::mkPDF(LHAPDFname, n_mem);

        // each_dat_files
        stringstream filename;
        filename << LHAPDFname << "_" << setfill('0') << setw(4) << n_mem << ".dat";
        // cout << filename.str() << endl;

        // Available flavors in a pdf
        vector<int> pids = pdf->flavors();
        cout << "--------+++++++++++++++++" << endl;
        cout << "Available flavors pids: ";
        for (int i = 0; i < pids.size(); i++) {
            cout << pids[i] << ", ";
        }
        cout << endl;
        cout << "--------++++++++++++++++++" << endl;

        // Loop over NX x-points
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
                // double xfu = pdf->xfxQ(2, x, Q);  // Up quark pdf

                outfile << Q << " " << xfg << " " << endl;
            }

            outfile.close(); // Closing file
        }

        // Loop over NQ Q-values
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
                // double Xfu = pdf->xfxQ(2, X, q);  // Up quark pdf

                outfile << X << " " << Xfg << " " << endl;
            }

            outfile.close(); // Closing file
        }

        delete pdf;
    }

    return 0;
}


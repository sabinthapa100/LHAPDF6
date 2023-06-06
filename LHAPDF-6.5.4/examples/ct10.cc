#include "LHAPDF/LHAPDF.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace LHAPDF;
using namespace std;

int main(){
LHAPDF::PDF* p = LHAPDF::mkPDF(“CT10/0”);
cout << p->xfxQ(21, 1e-4, 100.);
delete p;
vector<unique_ptr<LHAPDF::PDF∗ > > ps = LHAPDF::mkPDFs(“CT10nlo”);

delete p;

return 0;

}

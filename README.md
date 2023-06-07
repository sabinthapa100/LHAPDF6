# LHAPDF6: for nPDF suppression calculation! 

(For my purpose, I use C++, but you should be able to do it in Python following the references provided)

#https://lhapdf.hepforge.org

#**installation**: https://lhapdf.hepforge.org/install.html (official)

**LHAPDF6 References:**
1. arXiv:1509.00792 [hep-ph]
2. arXiv:2112.12462 [hep-ph]
3. arXiv:1412.7420 [hep-ph]

**LHAPDF5 Manual**
https://lhapdf.hepforge.org/lhapdf5/manual#tthFtNtAAB

#tutorials are done with tut.cc name (one example) located inside "examples" subfolder of the /LHAPDF6_sabin/LHAPDF-6-5-4
#installation is done in the folder /LHAPDF6_sabin/LHAPDF_install.

To compile:
$g++ -o ncteq15_Pb tut1.cc -L$HOME/stricklandlab/LHAPDF/LHAPDF_install/lib -lLHAPDF -I$HOME/stricklandlab/LHAPDF/LHAPDF_install/local/include

or
make tut1 [but to make it errorless, we need to manually change makefile inside - need to work on it]

%%%%%%%%%%%%
Nano-editor:
%%%%%%%%%%%%
Using Ctrl+O will save your changes to the file, Enter confirms the filename, and Ctrl+X exits nano.



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
**LHAPDF6 with Python/C++**
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Reference(along with the official website): https://confluence.its.virginia.edu/pages/viewpage.action?pageId=46553452

**Installed using above reference in my local directory:******************************
/home/username/LHAPDF/LHAPDF_install
**
#LHAPDF location in the local machine**
Unpackaging/Unzipping of LHAPDFs [it -- along with the environment setup -- important and should be properly done to get LHAPDFs loaded in the code properly):
Into the /LHAPDF_install/share/LHAPDF directory (where LHAPDF is installed using those prefix variable while configuring)
(unzipped)

%%%%%%%%%%%%%%%%%%
**LHAPDF Important Note after installing there:**
%%%%%%%%%%%%%%%%%%
(after the installation in my local Ubuntu 22.04.2 LTS machine)

IMPORTANT INFORMATION ABOUT PDF SETS

LHAPDF no longer bundles PDF set data in the package tarball.

The sets are instead all stored online at:

  http://lhapdfsets.web.cern.ch/lhapdfsets/current/
and you should install those that you wish to use into
  /home/stricklandlab/LHAPDF/LHAPDF_install/share/LHAPDF

The downloadable PDF sets are packaged as tarballs, which
must be expanded to be used. The simplest way to do this is with
the 'lhapdf' script, e.g. to install the CT10nlo PDF set:
  lhapdf install CT10nlo
  
The same effect can be achieved manually with, e.g.:
  $wget http://lhapdfsets.web.cern.ch/lhapdfsets/current/CT10nlo.tar.gz -O- | tar xz -C /home/username/LHAPDF/LHAPDF_install/share/LHAPDF



%%%%%%
**LHAPDFs downloaded & unpackaged in the: (available in the website: unpack and follow a previous unzip step)**
http://lhapdfsets.web.cern.ch/lhapdfsets/current/
%%%%%%
(in the directory: /home/stricklandlab/LHAPDF/LHAPDF_install/share/LHAPDF
%%%
wget http://lhapdfsets.web.cern.ch/lhapdfsets/current/CT10nlo.tar.gz -O- | tar xz -C /home/stricklandlab/LHAPDF/LHAPDF_install/share/LHAPDF


%%%%%%%%%%%
For EPPS21NLO: (some are available in the LHAPDF official website: https://lhapdf.hepforge.org/pdfsets)
%%%%%%%
https://www.jyu.fi/science/en/physics/research/highenergy/urhic/npdfs/epps21-nuclear-pdfs


%%%%%%%%%%%%%%%%%%%%%%%%%%%
NOTE:https://confluence.its.virginia.edu/pages/viewpage.action?pageId=46553452
%%%%%%%%%%%%%%%%%%%%%%%%%%
4. Using LHAPDF

All input and outputs are defined in DOUBLE PRECISION

There are three options to make a single PDF dataset:

PDF * lhapdf.mkPDF(int lhaid)

Below is an example:

“variable name” = lhapdf.mkPDF(“filename”)

            Returns PDFs as vector<PDF*>

PDF * lhapdf.mkPDF(const std.string &setname, int member)

Ex: “variable name” = lhapdf.mkPDF(setname,imem)

 

PDF * lhapdf.mkPDF(const str.string &setname_nmen)

Where “variable name” is whatever name you want to give to the assigned variable.

For making whole PDF sets:

“variable name” = lhapdf.getPDFSet(‘filename’)

            lhapdf.getPDFSet() will allow you to use the raw data from a dataset.

                        à “Info” is a class of function. In this case, Info() is a default constructor.





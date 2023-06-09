// -*- C++ -*-
//
// This file is part of LHAPDF
// Copyright (C) 2012-2023 The LHAPDF collaboration (see AUTHORS for details)
//
#include "LHAPDF/PDFIndex.h"
#include "LHAPDF/Paths.h"
#include "LHAPDF/Exceptions.h"
#include "LHAPDF/FileIO.h"

namespace LHAPDF {


  std::map<int, std::string>& getPDFIndex() {
    static thread_local map<int, string> _lhaindex;
    if (_lhaindex.empty()) { // The map needs to be populated first
      vector<string> indexpaths = findFiles("pdfsets.index");
      if (indexpaths.empty()) throw ReadError("Could not find any pdfsets.index files");
      for (const string& indexpath : indexpaths) {
        try {
          IFile file(indexpath.c_str());
          string line;
          while (getline(*file, line)) {
            line = trim(line);
            if (line.empty() || line.find("#") == 0) continue;
            istringstream tokens(line);
            int id; string setname;
            tokens >> id;
            tokens >> setname;
            // cout << id << " -> " << _lhaindex[id] << endl;
            // Insert this line into the map iff it doesn't already exist: allows index merging & overriding
            if (_lhaindex.count(id) == 0) {
              _lhaindex[id] = setname;
            }
          }
        } catch (const std::exception& ex) {
          throw ReadError("Trouble when reading " + indexpath + ": " + ex.what());
        }
      }
    }
    return _lhaindex;
  }


  std::pair<std::string, int> lookupPDF(int lhaid) {
    map<int, string>::iterator it = getPDFIndex().upper_bound(lhaid);
    string rtnname = "";
    int rtnmem = -1;
    if (it != getPDFIndex().begin()) {
      --it; // upper_bound (and lower_bound) return the entry *above* lhaid: we need to step back
      rtnname = it->second; // name of the set that contains this ID
      rtnmem = lhaid - it->first; // the member ID is the offset from the lookup ID
    }
    return make_pair(rtnname, rtnmem);
  }


  std::pair<std::string,int> lookupPDF(const std::string& pdfstr) {
    int nmem = 0;
    const size_t slashpos = pdfstr.find("/");
    const string setname = trim(pdfstr.substr(0, slashpos));
    try {
      if (slashpos != string::npos) {
        const string smem = pdfstr.substr(slashpos+1);
        nmem = lexical_cast<int>(smem);
      }
    } catch (...) {
      throw UserError("Could not parse PDF identity string " + pdfstr);
    }
    return make_pair(setname, nmem);
  }


  int lookupLHAPDFID(const std::string& setname, int nmem) {
    for (const auto& id_name : getPDFIndex()) { //< auto to make clang happy re. key constness
      if (id_name.second == setname) return id_name.first + nmem;
    }
    return -1; //< failure value
  }


}

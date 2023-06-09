// -*- C++ -*-
//
// This file is part of LHAPDF
// Copyright (C) 2012-2023 The LHAPDF collaboration (see AUTHORS for details)
//
#include "LHAPDF/FileIO.h"

#include <typeinfo>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <map>

// Include MPI features if requested, to avoid expensive repeated data-file reads
// on MPI clusters, and avoid making the FileIO thread-local in MPI mode
#include <sys/stat.h>
#ifdef HAVE_MPI
#include <mpi.h>
#define thread_local
#endif

namespace LHAPDF {


  typedef std::map<std::string,std::string> FileContentMap;
  static thread_local FileContentMap lhapdf_filecontents;


  template <class FILETYPE>
  bool File<FILETYPE>::open() {
      close();
      _fileptr = new FILETYPE();
      std::ifstream* is = dynamic_cast<std::ifstream*>(&*_fileptr);
      std::ofstream* os = dynamic_cast<std::ofstream*>(&*_fileptr);
      if (is) {
        _streamptr = new std::stringstream();
        FileContentMap::const_iterator it = lhapdf_filecontents.find(_name);
        if (it != lhapdf_filecontents.end()) {
          (*_streamptr) << it->second;
        } else {
          #ifdef HAVE_MPI
          int world_rank = -1;
          MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
          if (world_rank) {
            int fsize;
            MPI_Bcast(&fsize, 1, MPI_INT, 0,MPI_COMM_WORLD);
            if (fsize < 0) return false;
            std::string content(fsize, ' ');
            MPI_Bcast(&content[0], fsize, MPI_CHAR, 0,MPI_COMM_WORLD);
            (*_streamptr) << content;
            lhapdf_filecontents[_name] = content;
          } else {
            std::ifstream infile(_name.c_str());
            int fsize = infile.good() ? 1 : -1;
            if (fsize < 0) {
              MPI_Bcast(&fsize, 1, MPI_INT, 0,MPI_COMM_WORLD);
              return false;
            }
            (*_streamptr) << infile.rdbuf();
            std::string content(_streamptr->str());
            fsize = content.length();
            MPI_Bcast(&fsize, 1, MPI_INT, 0,MPI_COMM_WORLD);
            MPI_Bcast(&content[0], fsize, MPI_CHAR, 0,MPI_COMM_WORLD);
            lhapdf_filecontents[_name] = content;
          }
          #else
          std::ifstream infile(_name.c_str());
          int fsize = infile.good() ? 1 : -1;
          if (fsize < 0) return false;
          (*_streamptr) << infile.rdbuf();
          #endif
        }
        _fileptr->copyfmt(*_streamptr);
        _fileptr->clear(_streamptr->rdstate());
        is->std::ios::rdbuf(_streamptr->rdbuf());
        is->seekg(0);
        return true;
      }
      if (os) {
        _streamptr = new std::stringstream();
        os->std::ios::rdbuf(_streamptr->rdbuf());
        os->seekp(0);
        return true;
      }
      return false;
    }


    /// Close file
  template <class FILETYPE>
  bool File<FILETYPE>::close() {
      if (_fileptr == nullptr) return false;
      std::ofstream* os = dynamic_cast<std::ofstream*>(&*_fileptr); ///< @todo Is this valid if not an ostream??
      do {
        #ifdef HAVE_MPI
        int world_rank = -1;
        MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
        if (world_rank != 0) break;
        #endif
        if (os) {
          std::ofstream file(_name.c_str());
          file << _streamptr->str();
        }
      } while (false);
      _fileptr->close();
      delete _streamptr;
      delete _fileptr;
      _streamptr = nullptr;
      _fileptr = nullptr;
      return true;
    }


  template class IFile;
  template class OFile;


  void flushFileCache() {
    lhapdf_filecontents.clear();
  }


}

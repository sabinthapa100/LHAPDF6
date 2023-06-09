// -*- C++ -*-
//
// This file is part of LHAPDF
// Copyright (C) 2012-2023 The LHAPDF collaboration (see AUTHORS for details)
//
#pragma once
#ifndef LHAPDF_Config_H
#define LHAPDF_Config_H

#include "LHAPDF/Info.h"

namespace LHAPDF {


  /// Class for PDF set metadata and manipulation
  class Config : public Info {
  public:

    /// @name Fetching/creation
    /// @{

    /// Get the global configuration object
    ///
    /// The global config is populated by reading from lhapdf.conf if it is
    /// found in the search paths. It is a singleton, hence the 'get' accessor
    /// rather than a constructor.
    ///
    /// @note The LHAPDF system is responsible for deletion of the returned
    /// object. Do NOT delete it yourself!
    static Config& get();
    /// @}


    /// Config destructor, used for end-of-run banner printing
    ~Config();


  private:

    /// Hide the default constructor
    Config() {
      // std::cout << "CONFIG CONSTRUCTION" << std::endl;
    }

  };


  /// @defgroup verb Verbosity control
  /// @{

  /// Convenient way to get the current verbosity level
  ///
  /// Levels: 0=silent, 1=standard, 2=debug
  ///
  /// @note Verbosity is actually managed via the Info class hierarchy and can also be obtained from there.
  inline int verbosity() {
    return Config::get().get_entry_as<int>("Verbosity", 1);
  }

  /// Convenient way to set the verbosity level
  ///
  /// Levels: 0=silent, 1=standard, 2=debug
  ///
  /// @note Verbosity is actually managed via the Info class hierarchy and can also be set there.
  inline void setVerbosity(int v) {
    Config::get().set_entry("Verbosity", v);
  }

  /// @}


}
#endif

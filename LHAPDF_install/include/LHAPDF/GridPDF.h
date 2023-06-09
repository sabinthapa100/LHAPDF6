// -*- C++ -*-
//
// This file is part of LHAPDF
// Copyright (C) 2012-2023 The LHAPDF collaboration (see AUTHORS for details)
//
#pragma once
#ifndef LHAPDF_GridPDF_H
#define LHAPDF_GridPDF_H

#include "LHAPDF/PDF.h"
#include "LHAPDF/Interpolator.h"
#include "LHAPDF/Extrapolator.h"
#include "LHAPDF/KnotArray.h"

namespace LHAPDF {


  /// @brief A PDF defined via an interpolation grid
  class GridPDF : public PDF {
  public:

    /// Default constructor, making an empty PDF to be populated by hand.
    GridPDF() {
      _mempath = "";
      _info = PDFInfo();
      _forcePos = -1;
    }

    /// @brief Constructor from a file path
    ///
    /// We allow this to exist and be user-callable for testing and other
    /// special case uses, since if you are explicitly instantiating a GridPDF
    /// rather than acquiring it via a pointer/reference of PDF type, then you
    /// probably (hopefully) know what you're doing and aren't putting it into
    /// public production code!
    GridPDF(const std::string& path) {
      _loadInfo(path); // Sets _mempath
      _loadPlugins();
      _loadData(_mempath);
      _forcePos = -1;
    }

    /// Constructor from a set name and member ID
    GridPDF(const std::string& setname, int member) {
      _loadInfo(setname, member); // Sets _mempath
      _loadData(_mempath);
      _loadPlugins();
      _forcePos = -1;
    }

    /// Constructor from an LHAPDF ID
    GridPDF(int lhaid) {
      _loadInfo(lhaid); // Sets _mempath
      _loadData(_mempath); 
      _loadPlugins();
      _forcePos = -1;
    }

    /// Virtual destructor to allow inheritance
    virtual ~GridPDF() { }


  protected:

    /// Load the interpolator, based on current metadata
    void _loadInterpolator();

    /// Load the PDF grid data block, based on current metadata
    void _loadExtrapolator();

    /// Load the alphaS, interpolator, and extrapolator based on current metadata
    void _loadPlugins() {
      _loadAlphaS();
      _loadInterpolator();
      _loadExtrapolator();
    }

    /// Load the PDF grid data block (not the metadata) from the given PDF member file
    void _loadData(const std::string& mempath);

    /// Precompute polynomial coefficients and approximate derivatives at knot positions
    void _computePolynomialCoefficients(bool logspace);


  public:

    /// @name Interpolators and extrapolators
    ///@{

    /// @brief Set the interpolator by pointer
    ///
    /// The provided Interpolator must have been new'd, as it will not be copied
    /// and ownership passes to this GridPDF: delete will be called on this ptr
    /// when this GridPDF goes out of scope or another setInterpolator call is made.
    void setInterpolator(Interpolator* ipol);

    /// @brief Set the interpolator by value
    ///
    /// The passed value must be a concrete instantiation of the Interpolator
    /// interface. It will be copied and heap-assigned for use inside this GridPDF.
    ///
    /// @todo Use SFINAE magic to restrict INTERPOLATOR to subclasses of Interpolator?
    template <typename INTERPOLATOR>
    void setInterpolator(INTERPOLATOR ipol) {
      setInterpolator(new INTERPOLATOR(ipol));
    }

    /// @brief Set the interpolator by name
    ///
    /// Use the interpolator specified by the given name, as passed to the
    /// createInterpolator factory function.
    void setInterpolator(const std::string& ipolname);

    /// Find whether an extrapolator has been set on this PDF
    bool hasInterpolator() const { return bool(_interpolator); }

    /// Get the current interpolator
    const Interpolator& interpolator() const;


    /// @brief Set the extrapolator by pointer
    ///
    /// The provided Extrapolator must have been new'd, as it will not be copied
    /// and ownership passes to this GridPDF: delete will be called on this ptr
    /// when this GridPDF goes out of scope or another setExtrapolator call is made.
    void setExtrapolator(Extrapolator* xpol);

    /// @brief Set the extrapolator by value
    ///
    /// The passed value must be a concrete instantiation of the Extrapolator
    /// interface. It will be copied and heap-assigned for use inside this GridPDF.
    ///
    /// @todo Use SFINAE magic to restrict EXTRAPOLATOR to subclasses of Extrapolator?
    template <typename EXTRAPOLATOR>
    void setExtrapolator(EXTRAPOLATOR xpol) {
      setExtrapolator(new EXTRAPOLATOR(xpol));
    }

    /// @brief Set the extrapolator by name
    ///
    /// Use the extrapolator specified by the given name, as passed to the
    /// createExtrapolator factory function.
    void setExtrapolator(const std::string& xpolname);

    /// Find whether an extrapolator has been set on this PDF
    bool hasExtrapolator() const { return bool(_extrapolator); }

    /// Get the current extrapolator
    const Extrapolator& extrapolator() const;

    ///@}


  protected:

    /// @brief Get PDF xf(x,Q2) value (via grid inter/extrapolators)
    double _xfxQ2(int id, double x, double q2) const;

    void   _xfxQ2(double x, double q2, std::vector<double>& ret) const;


  public:

    /// @name Info about the grid, and access to the raw data points
    ///@{

    // accerror to new data structure
    const KnotArray& knotarray() const {
      return data;
    }

    KnotArray& Data() {
      return data;
    }

    /// @brief Return a representative list of interpolation knots in x
    ///
    /// The x knot array for the first flavor grid of the lowest-Q2 subgrid is returned.
    const vector<double>& xKnots() const;

    /// @brief Return a representative list of interpolation knots in Q2
    ///
    /// Constructed and cached by walking over all subgrids and concatenating their Q2 lists: expensive!
    const vector<double>& q2Knots() const;

  public:

    /// Check if x is in the grid range
    bool inRangeX(double x) const {
      return data.inRangeX(x);
    }

    /// Check if q2 is in the grid range
    bool inRangeQ2(double q2) const {
      return data.inRangeQ2(q2);
    }
    ///@}

  private:
    // *new* memory object, to handle basically everything
    // name?
    KnotArray data;
	
    /// Typedef of smart pointer for ipol memory handling
    typedef unique_ptr<Interpolator> InterpolatorPtr;

    /// Typedef of smart pointer for xpol memory handling
    typedef unique_ptr<Extrapolator> ExtrapolatorPtr;

    /// Associated interpolator (mutable to allow laziness)
    mutable InterpolatorPtr _interpolator;

    /// Associated extrapolator (mutable to allow laziness)
    mutable ExtrapolatorPtr _extrapolator;

  };


}
#endif

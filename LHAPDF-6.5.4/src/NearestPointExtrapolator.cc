// -*- C++ -*-
//
// This file is part of LHAPDF
// Copyright (C) 2012-2023 The LHAPDF collaboration (see AUTHORS for details)
//
#include "LHAPDF/NearestPointExtrapolator.h"
#include "LHAPDF/GridPDF.h"

namespace LHAPDF {


  namespace { // Unnamed namespace
    
    // Return the value in the given list that best matches the target value
    double _findClosestMatch(const vector<double>& cands, double target) {
      vector<double>::const_iterator it = lower_bound(cands.begin(), cands.end(), target);
      const double upper = *it;
      const double lower = (it == cands.begin()) ? upper : *(--it); //< Avoid decrementing the first entry
      /// @todo Closeness in linear or log space? Hmm...
      if (fabs(target - upper) < fabs(target - lower)) return upper;
      return lower;
    }

  }


  double NearestPointExtrapolator::extrapolateXQ2(int id, double x, double q2) const {
    /// Find the closest valid x and Q2 points, either on- or off-grid, and use the current interpolator
    /// @todo raise error for x > 1 ?
    const KnotArray data = pdf().knotarray();
    const double closestX  = (pdf().inRangeX(x))   ? x  : _findClosestMatch(data.xs(), x);
    const double closestQ2 = (pdf().inRangeQ2(q2)) ? q2 : _findClosestMatch(data.q2s(), q2);
    return pdf().interpolator().interpolateXQ2(id, closestX, closestQ2);
  }


}

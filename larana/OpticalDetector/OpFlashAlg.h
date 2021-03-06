// -*- mode: c++; c-basic-offset: 2; -*-
#ifndef OPFLASHALG_H
#define OPFLASHALG_H
/*!
 * Title:   OpFlash Algorithims
 * Author:  Ben Jones, MIT (Edited by wketchum@lanl.gov and gleb.sinev@duke.edu)
 *
 * Description:
 * These are the algorithms used by OpFlashFinder to produce flashes.
 */

#include "lardataobj/RecoBase/OpHit.h"
#include "lardataobj/RecoBase/OpFlash.h"
#include "larcorealg/Geometry/GeometryCore.h"
namespace detinfo { class DetectorClocks; }

#include <functional>
#include <vector>
#include <map>

namespace opdet{

  void RunFlashFinder(std::vector< recob::OpHit > const&,
                      std::vector< recob::OpFlash >&,
                      std::vector< std::vector< int > >&,
                      double const&,
                      geo::GeometryCore const&,
                      float const&,
                      float const&,
                      detinfo::DetectorClocks const&,
                      float const&);

  unsigned int GetAccumIndex(double const& PeakTime,
                             double const& MinTime,
                             double const& BinWidth,
                             double const& BinOffset);

  void FillAccumulator(unsigned int const&                AccumIndex,
                       unsigned int const&                HitIndex,
                       double const&                      PE,
                       float const&                       FlashThreshold,
                       std::vector< double >&             Binned,
                       std::vector< std::vector< int > >& Contributors,
                       std::vector< int >&        FlashesInAccumulator);

  void AssignHitsToFlash(std::vector< int > const&,
                         std::vector< int > const&,
                         std::vector< double > const&,
                         std::vector< double > const&,
                         std::vector< std::vector< int > > const&,
                         std::vector< std::vector< int > > const&,
                         std::vector< recob::OpHit > const&,
                         std::vector< std::vector< int > >&,
                         float const&);

  void FillFlashesBySizeMap(std::vector< int > const&     FlashesInAccumulator,
                            std::vector< double > const&  BinnedPE,
                            int const&                    Accumulator,
                            std::map< double,
                              std::map< int, std::vector< int > >,
                                std::greater< double > >& FlashesBySize);

  void FillHitsThisFlash(std::vector< std::vector< int > > const&
                                                             Contributors,
                         int const&                          Bin,
                         std::vector< int > const&           HitClaimedByFlash,
                         std::vector< int >&                 HitsThisFlash);

  void ClaimHits(std::vector< recob::OpHit > const& HitVector,
                 std::vector< int > const&          HitsThisFlash,
                 float const&                       FlashThreshold,
                 std::vector< std::vector< int > >& HitsPerFlash,
                 std::vector< int >&                HitClaimedByFlash);

  void RefineHitsInFlash(std::vector< int > const&          HitsThisFlash,
                         std::vector< recob::OpHit > const& HitVector,
                         std::vector< std::vector< int > >&
                                                       RefinedHitsPerFlash,
                         float const&                       WidthTolerance,
                         float const&                       FlashThreshold);

  void FindSeedHit(std::map< double, std::vector< int >,
                     std::greater< double > > const&  HitsBySize,
                   std::vector< bool >&               HitsUsed,
                   std::vector< recob::OpHit > const& HitVector,
                   std::vector< int >&                HitsThisRefinedFlash,
                   double&                            PEAccumulated,
                   double&                            FlashMaxTime,
                   double&                            FlashMinTime);

  void AddHitToFlash(int const&           HitID,
                     std::vector< bool >& HitsUsed,
                     recob::OpHit const&  currentHit,
                     double const&        WidthTolerance,
                     std::vector< int >&  HitsThisRefinedFlash,
                     double&              PEAccumulated,
                     double&              FlashMaxTime,
                     double&              FlashMinTime);

  void CheckAndStoreFlash(std::vector< std::vector< int > >&
                                                    RefinedHitsPerFlash,
                          std::vector< int > const& HitsThisRefinedFlash,
                          double const&             PEAccumulated,
                          float const&              FlashThreshold,
                          std::vector< bool >&      HitsUsed);

  void ConstructFlash(std::vector< int > const&          HitsPerFlashVec,
                      std::vector< recob::OpHit > const& HitVector,
                      std::vector< recob::OpFlash >&     FlashVector,
                      geo::GeometryCore const&           geom,
                      detinfo::DetectorClocks const&     ts,
                      float const&                       TrigCoinc);

  void AddHitContribution(recob::OpHit const&    currentHit,
                          double&                MaxTime,
                          double&                MinTime,
                          double&                AveTime,
                          double&                FastToTotal,
                          double&                AveAbsTime,
                          double&                TotalPE,
                          std::vector< double >& PEs);

  void GetHitGeometryInfo(recob::OpHit const&      currentHit,
                          geo::GeometryCore const& geom,
                          std::vector< double >&   sumw,
                          std::vector< double >&   sumw2,
                          double&                  sumy,
                          double&                  sumy2,
                          double&                  sumz,
                          double&                  sumz2);

  void RemoveLateLight(std::vector< recob::OpFlash >&,
                       std::vector< std::vector< int > >&);

  double GetLikelihoodLateLight(double const& iPE,
                                double const& iTime,
                                double const& iWidth,
                                double const& jPE,
                                double const& jTime,
                                double const& jWidth);

  void MarkFlashesForRemoval(std::vector< recob::OpFlash > const& FlashVector,
                             size_t const&                        BeginFlash,
                             std::vector< bool >&            MarkedForRemoval);

  void RemoveFlashesFromVectors(std::vector< bool > const&
                                                          MarkedForRemoval,
                                std::vector< recob::OpFlash >& FlashVector,
                                size_t const&                  BeginFlash,
                                std::vector< std::vector< int > >&
                                                       RefinedHitsPerFlash);

  template < typename T, typename Compare >
    std::vector< int > sort_permutation(std::vector< T > const& vec,
                                        int offset, Compare compare);

  template < typename T >
    void apply_permutation(std::vector< T >& vec, std::vector< int > const& p);

} // End opdet namespace

#endif

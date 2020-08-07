// Kerfuffle (hierarchical state machine)
// Created by 方泓睿(chfanghr@gmail.com)
//
// Licensed under the MIT License;
// you may not use this file except in compliance with the License.
//
//
// MIT License
//
// Copyright (c) 2020
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// Have fun playing with it.

#pragma once

#if defined(__clang__)
#  pragma clang diagnostic push
#  pragma ide diagnostic   ignored "modernize-deprecated-headers"
#endif

#include <stdint.h>  // uint32_t, uint64_t
#include <string.h>  // memcpy_s()

#if defined(__clang__)
#  pragma clang diagnostic pop
#endif

#include <new>
#include <typeindex>
#include <utility>  // std::conditional<>, move(), forward()

#if defined _DEBUG && _MSC_VER
#  include <intrin.h>  // __debugbreak()
#endif

//------------------------------------------------------------------------------

// clang-format off

#include "detail/shared/macros_on.h"

#include "version.h"

#include "detail/shared/utility.h"
#include "detail/shared/iterator.h"
#include "detail/shared/array.h"
#include "detail/shared/bit_array.h"
#include "detail/shared/bit_stream.h"
#include "detail/shared/list.h"
#include "detail/shared/random.h"
#include "detail/shared/type_list.h"

#include "detail/debug/shared.h"
#include "detail/debug/logger_interface.h"

#include "detail/root/plan_data.h"
#include "detail/root/plan.h"
#include "detail/root/registry.h"
#include "detail/root/control.h"
#include "detail/debug/structure_report.h"

#include "detail/structure/injections.h"
#include "detail/structure/state_box.h"
#include "detail/structure/state.h"
#include "detail/structure/forward.h"
#include "detail/structure/composite_sub.h"
#include "detail/structure/composite.h"
#include "detail/structure/orthogonal_sub.h"
#include "detail/structure/orthogonal.h"
#include "detail/root/state_access.h"

// clang-format on

namespace kerfuffle {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <FeatureTag NFeatureTag,
          typename TContext
#ifdef KERFUFFLE_ENABLE_UTILITY_THEORY
          ,
          typename TRank,
          typename TUtility,
          typename TRNG
#endif

          ,
          LongIndex NSubstitutionLimit KERFUFFLE_IF_PLANS(
              , LongIndex NTaskCapacity)>
struct G_ {
  static constexpr FeatureTag FEATURE_TAG = NFeatureTag;

  using Context = TContext;

#ifdef KERFUFFLE_ENABLE_UTILITY_THEORY
  using Rank    = TRank;
  using Utility = TUtility;
  using RNG     = TRNG;
#endif

#ifdef KERFUFFLE_ENABLE_LOG_INTERFACE
  using LoggerInterface =
      LoggerInterfaceT<Context KERFUFFLE_IF_UTILITY_THEORY(, Utility),
                       FEATURE_TAG>;
#endif

  static constexpr LongIndex SUBSTITUTION_LIMIT = NSubstitutionLimit;

#ifdef KERFUFFLE_ENABLE_PLANS
  static constexpr LongIndex TASK_CAPACITY = NTaskCapacity;
#endif

  /// @brief Set Context type
  /// @tparam T Context type for data shared between states and/or data
  /// interface between FSM and external code
  template <typename T>
  using ContextT = G_<FEATURE_TAG,
                      T KERFUFFLE_IF_UTILITY_THEORY(, Rank, Utility, RNG),
                      SUBSTITUTION_LIMIT KERFUFFLE_IF_PLANS(, NTaskCapacity)>;

#ifdef KERFUFFLE_ENABLE_UTILITY_THEORY

  /// @brief Set Rank type
  /// @tparam T Rank type for 'TRank State::rank() const' method
  template <typename T>
  using RankT = G_<FEATURE_TAG,
                   Context,
                   T,
                   Utility,
                   RNG,
                   SUBSTITUTION_LIMIT KERFUFFLE_IF_PLANS(, NTaskCapacity)>;

  /// @brief Set Utility type
  /// @tparam T Utility type for 'TUtility State::utility() const' method
  template <typename T>
  using UtilityT = G_<FEATURE_TAG,
                      Context,
                      Rank,
                      T,
                      RNG,
                      SUBSTITUTION_LIMIT KERFUFFLE_IF_PLANS(, NTaskCapacity)>;

  /// @brief Set RNG type
  /// @tparam T RNG type used in 'Random' regions
  template <typename T>
  using RandomT = G_<FEATURE_TAG,
                     Context,
                     Rank,
                     Utility,
                     T,
                     SUBSTITUTION_LIMIT KERFUFFLE_IF_PLANS(, NTaskCapacity)>;

#endif

  /// @brief Set Substitution limit
  /// @tparam N Maximum number times 'guard()' methods can substitute their
  /// states for others
  template <LongIndex N>
  using SubstitutionLimitN =
      G_<FEATURE_TAG,
         Context KERFUFFLE_IF_UTILITY_THEORY(, Rank, Utility, RNG),
         N       KERFUFFLE_IF_PLANS(, NTaskCapacity)>;

#ifdef KERFUFFLE_ENABLE_PLANS

  /// @brief Set Task capacity
  /// @tparam N Maximum number of tasks across all plans
  template <LongIndex N>
  using TaskCapacityN =
      G_<FEATURE_TAG,
         Context KERFUFFLE_IF_UTILITY_THEORY(, Rank, Utility, RNG),
         SUBSTITUTION_LIMIT,
         N>;

#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef KERFUFFLE_ENABLE_UTILITY_THEORY

  struct UP {
    KERFUFFLE_INLINE UP(const Utility    utility_ = Utility{1.0f},
                        const ShortIndex prong_   = INVALID_SHORT_INDEX)
        : utility{utility_}, prong{prong_} {}

    Utility    utility;
    ShortIndex prong;
  };

#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
};

////////////////////////////////////////////////////////////////////////////////

template <typename TConfig>
struct M_;

template <FeatureTag NFeatureTag,
          typename TContext
#ifdef KERFUFFLE_ENABLE_UTILITY_THEORY
          ,
          typename TRank,
          typename TUtility,
          typename TRNG
#endif

          ,
          LongIndex NSubstitutionLimit KERFUFFLE_IF_PLANS(
              , LongIndex NTaskCapacity)>
struct M_<G_<NFeatureTag,
             TContext KERFUFFLE_IF_UTILITY_THEORY(, TRank, TUtility, TRNG),
             NSubstitutionLimit KERFUFFLE_IF_PLANS(, NTaskCapacity)>> {
  using Cfg = G_<NFeatureTag,
                 TContext KERFUFFLE_IF_UTILITY_THEORY(, TRank, TUtility, TRNG),
                 NSubstitutionLimit KERFUFFLE_IF_PLANS(, NTaskCapacity)>;

  static constexpr FeatureTag FEATURE_TAG = NFeatureTag;

  using Context = TContext;

#ifdef KERFUFFLE_ENABLE_UTILITY_THEORY
  using Utility = TUtility;
#endif

#ifdef KERFUFFLE_ENABLE_LOG_INTERFACE
  using LoggerInterface = typename Cfg::LoggerInterface;
#endif

  //----------------------------------------------------------------------

  /// @brief Composite region ('changeTo<>()' into the region acts as
  /// 'restart<>()')
  /// @tparam THead Head state
  /// @tparam TSubStates Sub-states
  template <typename THead, typename... TSubStates>
  using Composite = CI_<Strategy::Composite, THead, TSubStates...>;

  /// @brief Headless composite region ('changeTo<>()' into the region acts as
  /// 'restart<>()')
  /// @tparam TSubStates Sub-states
  template <typename... TSubStates>
  using CompositePeers = CI_<Strategy::Composite, void, TSubStates...>;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  /// @brief Resumable region ('changeTo<>()' into the region acts as
  /// 'resume<>()')
  /// @tparam THead Head state
  /// @tparam TSubStates Sub-states
  template <typename THead, typename... TSubStates>
  using Resumable = CI_<Strategy::Resumable, THead, TSubStates...>;

  /// @brief Headless resumable region ('changeTo<>()' into the region acts as
  /// 'resume<>()')
  /// @tparam TSubStates Sub-states
  template <typename... TSubStates>
  using ResumablePeers = CI_<Strategy::Resumable, void, TSubStates...>;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef KERFUFFLE_ENABLE_UTILITY_THEORY

  /// @brief Utilitarian region ('changeTo<>()' into the region acts as
  /// 'utilize<>()')
  /// @tparam THead Head state
  /// @tparam TSubStates Sub-states
  template <typename THead, typename... TSubStates>
  using Utilitarian = CI_<Strategy::Utilitarian, THead, TSubStates...>;

  /// @brief Headless utilitarian region ('changeTo<>()' into the region acts as
  /// 'utilize<>()')
  /// @tparam TSubStates Sub-states
  template <typename... TSubStates>
  using UtilitarianPeers = CI_<Strategy::Utilitarian, void, TSubStates...>;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  /// @brief Random region ('changeTo<>()' into the region acts as
  /// 'randomize<>()')
  /// @tparam THead Head state
  /// @tparam TSubStates Sub-states
  template <typename THead, typename... TSubStates>
  using Random = CI_<Strategy::RandomUtil, THead, TSubStates...>;

  /// @brief Headless random region ('changeTo<>()' into the region acts as
  /// 'randomize<>()')
  /// @tparam TSubStates Sub-states
  template <typename... TSubStates>
  using RandomPeers = CI_<Strategy::RandomUtil, void, TSubStates...>;

#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  /// @brief Orthogonal region (when activated, activates all sub-states)
  /// @tparam THead Head state
  /// @tparam TSubStates Sub-states
  template <typename THead, typename... TSubStates>
  using Orthogonal = OI_<THead, TSubStates...>;

  /// @brief Headless orthogonal region (when activated, activates all
  /// sub-states)
  /// @tparam TSubStates Sub-states
  template <typename... TSubStates>
  using OrthogonalPeers = OI_<void, TSubStates...>;

  //----------------------------------------------------------------------

  /// @brief Root ('changeTo<>()' into the root region acts as 'restart<>()')
  /// @tparam THead Head state
  /// @tparam TSubStates Sub-states
  template <typename THead, typename... TSubStates>
  using Root = RF_<Cfg, Composite<THead, TSubStates...>>;

  /// @brief Headless root ('changeTo<>()' into the root region acts as
  /// 'restart<>()')
  /// @tparam TSubStates Sub-states
  template <typename... TSubStates>
  using PeerRoot = RF_<Cfg, CompositePeers<TSubStates...>>;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  /// @brief Resumable root ('changeTo<>()' into the root region acts as
  /// 'resume<>()')
  /// @tparam THead Head state
  /// @tparam TSubStates Sub-states
  template <typename THead, typename... TSubStates>
  using ResumableRoot = RF_<Cfg, Resumable<THead, TSubStates...>>;

  /// @brief Headless resumable root ('changeTo<>()' into the root region acts
  /// as 'resume<>()')
  /// @tparam TSubStates Sub-states
  template <typename... TSubStates>
  using ResumablePeerRoot = RF_<Cfg, ResumablePeers<TSubStates...>>;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef KERFUFFLE_ENABLE_UTILITY_THEORY

  /// @brief Utilitarian root ('changeTo<>()' into the root region acts as
  /// 'utilize<>()')
  /// @tparam THead Head state
  /// @tparam TSubStates Sub-states
  template <typename THead, typename... TSubStates>
  using UtilitarianRoot = RF_<Cfg, Utilitarian<THead, TSubStates...>>;

  /// @brief Headless utilitarian root ('changeTo<>()' into the root region acts
  /// as 'utilize<>()')
  /// @tparam TSubStates Sub-states
  template <typename... TSubStates>
  using UtilitarianPeerRoot = RF_<Cfg, UtilitarianPeers<TSubStates...>>;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  /// @brief Random root ('changeTo<>()' into the root region acts as
  /// 'randomize<>()')
  /// @tparam THead Head state
  /// @tparam TSubStates Sub-states
  template <typename THead, typename... TSubStates>
  using RandomRoot = RF_<Cfg, Random<THead, TSubStates...>>;

  /// @brief Headless random root ('changeTo<>()' into the root region acts as
  /// 'randomize<>()')
  /// @tparam TSubStates Sub-states
  template <typename... TSubStates>
  using RandomPeerRoot = RF_<Cfg, RandomPeers<TSubStates...>>;

#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  /// @brief Orthogonal root (all sub-states are active at all times)
  /// @tparam THead Head state
  /// @tparam TSubStates Sub-states
  template <typename THead, typename... TSubStates>
  using OrthogonalRoot = RF_<Cfg, Orthogonal<THead, TSubStates...>>;

  /// @brief Headless orthogonal root (all sub-states are active at all times)
  /// @tparam TSubStates Sub-states
  template <typename... TSubStates>
  using OrthogonalPeerRoot = RF_<Cfg, OrthogonalPeers<TSubStates...>>;

  //----------------------------------------------------------------------
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace detail

/// @brief Type configuration for MachineT<>
/// @tparam TContext Context type for data shared between states and/or data
/// interface between FSM and external code
/// @tparam TRank Rank type for 'TRank State::rank() const' method
/// @tparam TUtility Utility type for 'TUtility State::utility() const' method
/// @tparam TRNG RNG type used in 'Random' regions
/// @tparam NSubstitutionLimit Maximum number times 'guard()' methods can
/// substitute their states for others
/// @tparam NTaskCapacity Maximum number of tasks across all plans
template <typename TContext = EmptyContext

#ifdef KERFUFFLE_ENABLE_UTILITY_THEORY
          ,
          typename TRank    = char,
          typename TUtility = float,
          typename TRNG     = ::kerfuffle::RandomT<TUtility>
#endif

          ,
          LongIndex NSubstitutionLimit = 4 KERFUFFLE_IF_PLANS(
              , LongIndex NTaskCapacity = INVALID_LONG_INDEX)>
using ConfigT =
    detail::G_<FEATURE_TAG,
               TContext KERFUFFLE_IF_UTILITY_THEORY(, TRank, TUtility, TRNG),
               NSubstitutionLimit KERFUFFLE_IF_PLANS(, NTaskCapacity)>;

/// @brief Type configuration for MachineT<>
using Config = ConfigT<>;

/// @brief 'Template namespace' for FSM classes
/// @tparam TConfig 'ConfigT<>' type configuration for MachineT<>
/// @see ConfigT<>
template <typename TConfig = Config>
using MachineT = detail::M_<TConfig>;

/// @brief 'Template namespace' for FSM classes parametrized with default types
using Machine = MachineT<>;

////////////////////////////////////////////////////////////////////////////////

}  // namespace kerfuffle

#include "detail/root.h"

#include "detail/shared/macros_off.h"

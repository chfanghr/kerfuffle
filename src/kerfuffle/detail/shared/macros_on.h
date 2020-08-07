//------------------------------------------------------------------------------

#ifdef __clang__
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored \
      "-Wextra-semi"  // error : extra ';' inside a class
#endif

#if defined(__GNUC__) || defined(__GNUG__)
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wpedantic"  // error : extra ';'
#endif

////////////////////////////////////////////////////////////////////////////////

#define KERFUFFLE_INLINE  // inline

//------------------------------------------------------------------------------

#if INTPTR_MAX == INT64_MAX
#  define KERFUFFLE_64BIT_OR_32BIT(p64, p32) p64
#else
#  define KERFUFFLE_64BIT_OR_32BIT(p64, p32) p32
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if defined _DEBUG
#  if defined _MSC_VER
#    define KERFUFFLE_BREAK() __debugbreak()
#  elif __has_builtin(__builtin_trap)
#    define KERFUFFLE_BREAK() __builtin_trap();
#  endif
#else
#  define KERFUFFLE_BREAK() ((void)0)
#endif
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef _DEBUG
#  define KERFUFFLE_IF_DEBUG(...) __VA_ARGS__
#  define KERFUFFLE_UNLESS_DEBUG(...)
#  define KERFUFFLE_DEBUG_OR(y, n) y
#else
#  define KERFUFFLE_IF_DEBUG(...)
#  define KERFUFFLE_UNLESS_DEBUG(...) __VA_ARGS__
#  define KERFUFFLE_DEBUG_OR(y, n)    n
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef KERFUFFLE_ENABLE_ASSERT
#  define KERFUFFLE_IF_ASSERT(...)  __VA_ARGS__
#  define KERFUFFLE_CHECKED(x)      (!!(x) || (KERFUFFLE_BREAK(), 0))
#  define KERFUFFLE_ASSERT(x)       (!!(x) || (KERFUFFLE_BREAK(), 0))
#  define KERFUFFLE_ASSERT_OR(y, n) y
#else
#  define KERFUFFLE_IF_ASSERT(...)
#  define KERFUFFLE_CHECKED(x)      x
#  define KERFUFFLE_ASSERT(x)       ((void)0)
#  define KERFUFFLE_ASSERT_OR(y, n) n
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if defined _MSC_VER || defined __clang_major__ && __clang_major__ >= 7
#  define KERFUFFLE_EXPLICIT_MEMBER_SPECIALIZATION
#endif

//------------------------------------------------------------------------------

#ifdef KERFUFFLE_ENABLE_ALL
#  define KERFUFFLE_ENABLE_UTILITY_THEORY
#  define KERFUFFLE_ENABLE_PLANS
#  define KERFUFFLE_ENABLE_SERIALIZATION
#  define KERFUFFLE_ENABLE_TRANSITION_HISTORY
#  define KERFUFFLE_ENABLE_STRUCTURE_REPORT
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef KERFUFFLE_ENABLE_UTILITY_THEORY
#  define KERFUFFLE_IF_UTILITY_THEORY(...) __VA_ARGS__
#  define KERFUFFLE_UTILITY_THEORY_MASK    (1 << 0)
#else
#  define KERFUFFLE_IF_UTILITY_THEORY(...)
#  define KERFUFFLE_UTILITY_THEORY_MASK (0 << 0)
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef KERFUFFLE_ENABLE_PLANS
#  define KERFUFFLE_IF_PLANS(...) __VA_ARGS__
#  define KERFUFFLE_PLANS_MASK    (1 << 1)
#else
#  define KERFUFFLE_IF_PLANS(...)
#  define KERFUFFLE_PLANS_MASK (0 << 1)
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef KERFUFFLE_ENABLE_SERIALIZATION
#  define KERFUFFLE_IF_SERIALIZATION(...) __VA_ARGS__
#  define KERFUFFLE_SERIALIZATION_MASK    (1 << 2)
#else
#  define KERFUFFLE_IF_SERIALIZATION(...)
#  define KERFUFFLE_SERIALIZATION_MASK (0 << 2)
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef KERFUFFLE_ENABLE_TRANSITION_HISTORY
#  define KERFUFFLE_IF_TRANSITION_HISTORY(...) __VA_ARGS__
#  define KERFUFFLE_TRANSITION_HISTORY_MASK    (1 << 3)
#else
#  define KERFUFFLE_IF_TRANSITION_HISTORY(...)
#  define KERFUFFLE_TRANSITION_HISTORY_MASK (0 << 3)
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef KERFUFFLE_ENABLE_STRUCTURE_REPORT
#  define KERFUFFLE_IF_STRUCTURE_REPORT(...) __VA_ARGS__
#  define KERFUFFLE_STRUCTURE_REPORT_MASK    (1 << 4)
#else
#  define KERFUFFLE_IF_STRUCTURE_REPORT(...)
#  define KERFUFFLE_STRUCTURE_REPORT_MASK (0 << 4)
#endif

//------------------------------------------------------------------------------

#ifdef KERFUFFLE_ENABLE_VERBOSE_DEBUG_LOG
#  define KERFUFFLE_ENABLE_LOG_INTERFACE
#endif

#ifdef KERFUFFLE_ENABLE_LOG_INTERFACE

#  define KERFUFFLE_IF_LOG_INTERFACE(...) __VA_ARGS__
#  define KERFUFFLE_LOG_INTERFACE_MASK    (1 << 5)

#  define KERFUFFLE_LOG_TRANSITION(CONTEXT, ORIGIN, TYPE, DESTINATION) \
    if (_logger) _logger->recordTransition(CONTEXT, ORIGIN, TYPE, DESTINATION)

#  ifdef KERFUFFLE_ENABLE_PLANS

#    define KERFUFFLE_LOG_TASK_STATUS(CONTEXT, REGION, ORIGIN, STATUS) \
      if (_logger) _logger->recordTaskStatus(CONTEXT, REGION, ORIGIN, STATUS)

#    define KERFUFFLE_LOG_PLAN_STATUS(CONTEXT, REGION, STATUS) \
      if (_logger) _logger->recordPlanStatus(CONTEXT, REGION, STATUS)

#  endif

#  define KERFUFFLE_LOG_CANCELLED_PENDING(CONTEXT, ORIGIN) \
    if (_logger) _logger->recordCancelledPending(CONTEXT, ORIGIN)

#  ifdef KERFUFFLE_ENABLE_UTILITY_THEORY

#    define KERFUFFLE_LOG_UTILITY_RESOLUTION(CONTEXT, HEAD, PRONG, UTILITY) \
      if (auto* const logger = control.logger())                            \
      logger->recordUtilityResolution(CONTEXT, HEAD, PRONG, UTILITY)

#    define KERFUFFLE_LOG_RANDOM_RESOLUTION(CONTEXT, HEAD, PRONG, UTILITY) \
      if (auto* const logger = control.logger())                           \
      logger->recordRandomResolution(CONTEXT, HEAD, PRONG, UTILITY)

#  endif

#else

#  define KERFUFFLE_IF_LOG_INTERFACE(...)
#  define KERFUFFLE_LOG_INTERFACE_MASK (0 << 5)

#  define KERFUFFLE_LOG_TRANSITION(CONTEXT, ORIGIN, TYPE, DESTINATION)

#  ifdef KERFUFFLE_ENABLE_PLANS
#    define KERFUFFLE_LOG_TASK_STATUS(CONTEXT, REGION, ORIGIN, STATUS)
#    define KERFUFFLE_LOG_PLAN_STATUS(CONTEXT, REGION, STATUS)
#  endif

#  define KERFUFFLE_LOG_CANCELLED_PENDING(CONTEXT, ORIGIN)

#  ifdef KERFUFFLE_ENABLE_UTILITY_THEORY
#    define KERFUFFLE_LOG_UTILITY_RESOLUTION(CONTEXT, HEAD, PRONG, UTILITY)
#    define KERFUFFLE_LOG_RANDOM_RESOLUTION(CONTEXT, HEAD, PRONG, UTILITY)
#  endif

#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef KERFUFFLE_ENABLE_VERBOSE_DEBUG_LOG

#  define KERFUFFLE_LOG_STATE_METHOD(METHOD, CONTEXT, METHOD_ID) \
    if (auto* const logger = control.logger())                   \
    logger->recordMethod(CONTEXT, STATE_ID, METHOD_ID)

#elif defined KERFUFFLE_ENABLE_LOG_INTERFACE

#  define KERFUFFLE_LOG_STATE_METHOD(METHOD, CONTEXT, METHOD_ID) \
    if (auto* const logger = control.logger())                   \
    log<decltype(METHOD)>(*logger, CONTEXT, METHOD_ID)

#else

#  define KERFUFFLE_LOG_STATE_METHOD(METHOD, CONTEXT, METHOD_ID)

#endif

//------------------------------------------------------------------------------

namespace kerfuffle {

using FeatureTag = uint8_t;

constexpr FeatureTag FEATURE_TAG =
    KERFUFFLE_UTILITY_THEORY_MASK | KERFUFFLE_PLANS_MASK |
    KERFUFFLE_SERIALIZATION_MASK | KERFUFFLE_TRANSITION_HISTORY_MASK |
    KERFUFFLE_STRUCTURE_REPORT_MASK | KERFUFFLE_LOG_INTERFACE_MASK;

}  // namespace kerfuffle

//------------------------------------------------------------------------------

#undef KERFUFFLE_UTILITY_THEORY_MASK
#undef KERFUFFLE_PLANS_MASK
#undef KERFUFFLE_SERIALIZATION_MASK
#undef KERFUFFLE_TRANSITION_HISTORY_MASK
#undef KERFUFFLE_STRUCTURE_REPORT_MASK
#undef KERFUFFLE_LOG_INTERFACE_MASK

////////////////////////////////////////////////////////////////////////////////

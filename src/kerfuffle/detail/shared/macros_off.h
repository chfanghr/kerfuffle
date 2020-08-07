////////////////////////////////////////////////////////////////////////////////

#undef KERFUFFLE_INLINE

#undef KERFUFFLE_64BIT_OR_32BIT

//#undef KERFUFFLE_BREAK

#undef KERFUFFLE_IF_DEBUG
#undef KERFUFFLE_UNLESS_DEBUG
#undef KERFUFFLE_DEBUG_OR

#undef KERFUFFLE_IF_ASSERT
#undef KERFUFFLE_CHECKED
#undef KERFUFFLE_ASSERT
#undef KERFUFFLE_ASSERT_OR

#undef KERFUFFLE_EXPLICIT_MEMBER_SPECIALIZATION

#undef KERFUFFLE_IF_UTILITY_THEORY
#undef KERFUFFLE_IF_PLANS
#undef KERFUFFLE_IF_SERIALIZATION
#undef KERFUFFLE_IF_TRANSITION_HISTORY
#undef KERFUFFLE_IF_STRUCTURE_REPORT
#undef KERFUFFLE_IF_LOG_INTERFACE

#undef KERFUFFLE_LOG_TRANSITION
#undef KERFUFFLE_LOG_TASK_STATUS
#undef KERFUFFLE_LOG_PLAN_STATUS
#undef KERFUFFLE_LOG_CANCELLED_PENDING

#undef KERFUFFLE_LOG_STATE_METHOD

////////////////////////////////////////////////////////////////////////////////

#if defined(__GNUC__) || defined(__GNUG__)
#  pragma GCC diagnostic pop
#endif

#ifdef __clang__
#  pragma clang diagnostic pop
#endif

//------------------------------------------------------------------------------

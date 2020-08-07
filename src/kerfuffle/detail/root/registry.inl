namespace kerfuffle {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC,
          typename TG,
          typename TSL,
          typename TRL,
          LongIndex     NCC,
          LongIndex     NOC,
          LongIndex     NOU,
          LongIndex NSB KERFUFFLE_IF_PLANS(, LongIndex NTC)>
bool RegistryT<
    ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB KERFUFFLE_IF_PLANS(, NTC)>>::
    isActive(const StateID stateId) const {
  if (KERFUFFLE_CHECKED(stateId < STATE_COUNT))
    for (Parent parent = stateParents[stateId]; parent;
         parent        = forkParent(parent.forkId)) {
      KERFUFFLE_ASSERT(parent.forkId != 0);

      if (parent.forkId > 0)
        return parent.prong == compoActive[parent.forkId - 1];
    }

  return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC,
          typename TG,
          typename TSL,
          typename TRL,
          LongIndex     NCC,
          LongIndex     NOC,
          LongIndex     NOU,
          LongIndex NSB KERFUFFLE_IF_PLANS(, LongIndex NTC)>
bool RegistryT<
    ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB KERFUFFLE_IF_PLANS(, NTC)>>::
    isResumable(const StateID stateId) const {
  if (KERFUFFLE_CHECKED(stateId < STATE_COUNT))
    for (Parent parent = stateParents[stateId]; parent;
         parent        = forkParent(parent.forkId)) {
      KERFUFFLE_ASSERT(parent.forkId != 0);

      if (parent.forkId > 0)
        return parent.prong == compoResumable[parent.forkId - 1];
    }

  return false;
}

//------------------------------------------------------------------------------

template <typename TC,
          typename TG,
          typename TSL,
          typename TRL,
          LongIndex     NCC,
          LongIndex     NOC,
          LongIndex     NOU,
          LongIndex NSB KERFUFFLE_IF_PLANS(, LongIndex NTC)>
bool RegistryT<
    ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB KERFUFFLE_IF_PLANS(, NTC)>>::
    isPendingChange(const StateID stateId) const {
  if (KERFUFFLE_CHECKED(stateId < STATE_COUNT))
    for (Parent parent = stateParents[stateId]; parent;
         parent        = forkParent(parent.forkId)) {
      KERFUFFLE_ASSERT(parent.forkId != 0);

      if (parent.forkId > 0)
        return compoRequested[parent.forkId - 1] !=
               compoActive[parent.forkId - 1];
    }

  return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC,
          typename TG,
          typename TSL,
          typename TRL,
          LongIndex     NCC,
          LongIndex     NOC,
          LongIndex     NOU,
          LongIndex NSB KERFUFFLE_IF_PLANS(, LongIndex NTC)>
bool RegistryT<
    ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB KERFUFFLE_IF_PLANS(, NTC)>>::
    isPendingEnter(const StateID stateId) const {
  if (KERFUFFLE_CHECKED(stateId < STATE_COUNT))
    for (Parent parent = stateParents[stateId]; parent;
         parent        = forkParent(parent.forkId)) {
      KERFUFFLE_ASSERT(parent.forkId != 0);

      if (parent.forkId > 0)
        return parent.prong != compoActive[parent.forkId - 1] &&
               parent.prong == compoRequested[parent.forkId - 1];
    }

  return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC,
          typename TG,
          typename TSL,
          typename TRL,
          LongIndex     NCC,
          LongIndex     NOC,
          LongIndex     NOU,
          LongIndex NSB KERFUFFLE_IF_PLANS(, LongIndex NTC)>
bool RegistryT<
    ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB KERFUFFLE_IF_PLANS(, NTC)>>::
    isPendingExit(const StateID stateId) const {
  if (KERFUFFLE_CHECKED(stateId < STATE_COUNT))
    for (Parent parent = stateParents[stateId]; parent;
         parent        = forkParent(parent.forkId)) {
      KERFUFFLE_ASSERT(parent.forkId != 0);

      if (parent.forkId > 0)
        return parent.prong == compoActive[parent.forkId - 1] &&
               parent.prong != compoRequested[parent.forkId - 1];
    }

  return true;
}

//------------------------------------------------------------------------------

template <typename TC,
          typename TG,
          typename TSL,
          typename TRL,
          LongIndex     NCC,
          LongIndex     NOC,
          LongIndex     NOU,
          LongIndex NSB KERFUFFLE_IF_PLANS(, LongIndex NTC)>
const Parent& RegistryT<
    ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB KERFUFFLE_IF_PLANS(, NTC)>>::
    forkParent(const ForkID forkId) const {
  KERFUFFLE_ASSERT(forkId != 0);

  return forkId > 0 ? compoParents[forkId - 1] : orthoParents[-forkId - 1];
}

//------------------------------------------------------------------------------

template <typename TC,
          typename TG,
          typename TSL,
          typename TRL,
          LongIndex     NCC,
          LongIndex     NOC,
          LongIndex     NOU,
          LongIndex NSB KERFUFFLE_IF_PLANS(, LongIndex NTC)>
typename RegistryT<
    ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB KERFUFFLE_IF_PLANS(, NTC)>>::
    OrthoBits
    RegistryT<
        ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB KERFUFFLE_IF_PLANS(, NTC)>>::
        requestedOrthoFork(const ForkID forkId) {
  KERFUFFLE_ASSERT(forkId < 0);
  const Units& units = orthoUnits[-forkId - 1];

  return orthoRequested.bits(units);
}

//------------------------------------------------------------------------------

template <typename TC,
          typename TG,
          typename TSL,
          typename TRL,
          LongIndex     NCC,
          LongIndex     NOC,
          LongIndex     NOU,
          LongIndex NSB KERFUFFLE_IF_PLANS(, LongIndex NTC)>
bool RegistryT<
    ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB KERFUFFLE_IF_PLANS(, NTC)>>::
    requestImmediate(const Request request) {
  if (request.stateId == 0)
    return false;
  else if (KERFUFFLE_CHECKED(request.stateId < STATE_COUNT)) {
    Parent parent;

    for (parent = stateParents[request.stateId]; parent;
         parent = forkParent(parent.forkId)) {
      if (parent.forkId > 0) {
        compoRequested[parent.forkId - 1] = parent.prong;
        parent                            = forkParent(parent.forkId);

        break;
      } else if (parent.forkId < 0)
        requestedOrthoFork(parent.forkId).set(parent.prong);
      else
        KERFUFFLE_BREAK();
    }

    for (; parent; parent = forkParent(parent.forkId)) {
      if (parent.forkId > 0) {
        compoRemains.set(parent.forkId - 1);

        if (compoActive[parent.forkId - 1] != parent.prong)
          compoRequested[parent.forkId - 1] = parent.prong;
        else {
          parent = forkParent(parent.forkId);
          break;
        }
      } else if (parent.forkId < 0)
        requestedOrthoFork(parent.forkId).set(parent.prong);
      else
        KERFUFFLE_BREAK();
    }

    for (; parent; parent = forkParent(parent.forkId)) {
      if (parent.forkId > 0)
        compoRemains.set(parent.forkId - 1);
      else if (parent.forkId < 0)
        requestedOrthoFork(parent.forkId).set(parent.prong);
      else
        KERFUFFLE_BREAK();
    }
  }

  return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC,
          typename TG,
          typename TSL,
          typename TRL,
          LongIndex     NCC,
          LongIndex     NOC,
          LongIndex     NOU,
          LongIndex NSB KERFUFFLE_IF_PLANS(, LongIndex NTC)>
void RegistryT<
    ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB KERFUFFLE_IF_PLANS(, NTC)>>::
    requestScheduled(const StateID stateId) {
  if (KERFUFFLE_CHECKED(stateId < STATE_COUNT)) {
    const Parent parent = stateParents[stateId];

    KERFUFFLE_ASSERT(parent.forkId != 0);
    if (parent.forkId > 0) compoResumable[parent.forkId - 1] = parent.prong;
  }
}

//------------------------------------------------------------------------------

template <typename TC,
          typename TG,
          typename TSL,
          typename TRL,
          LongIndex     NCC,
          LongIndex     NOC,
          LongIndex     NOU,
          LongIndex NSB KERFUFFLE_IF_PLANS(, LongIndex NTC)>
void RegistryT<
    ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB KERFUFFLE_IF_PLANS(, NTC)>>::
    clearRequests() {
  compoRequested.clear();
  orthoRequested.clear();
  compoRemains.clear();
}

//------------------------------------------------------------------------------

template <typename TC,
          typename TG,
          typename TSL,
          typename TRL,
          LongIndex     NCC,
          LongIndex     NOC,
          LongIndex     NOU,
          LongIndex NSB KERFUFFLE_IF_PLANS(, LongIndex NTC)>
void RegistryT<
    ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, NSB KERFUFFLE_IF_PLANS(, NTC)>>::
    reset() {
  compoRequested.clear();
  orthoRequested.clear();
  compoActive.clear();
  compoResumable.clear();
  compoRemains.clear();
}

////////////////////////////////////////////////////////////////////////////////

template <typename TC,
          typename TG,
          typename TSL,
          typename TRL,
          LongIndex     NCC,
          LongIndex NSB KERFUFFLE_IF_PLANS(, LongIndex NTC)>
bool RegistryT<
    ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, NSB KERFUFFLE_IF_PLANS(, NTC)>>::
    isActive(const StateID stateId) const {
  if (KERFUFFLE_CHECKED(stateId < STATE_COUNT)) {
    if (Parent parent = stateParents[stateId]) {
      KERFUFFLE_ASSERT(parent.forkId > 0);

      return parent.prong == compoActive[parent.forkId - 1];
    } else
      return true;
  }

  return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC,
          typename TG,
          typename TSL,
          typename TRL,
          LongIndex     NCC,
          LongIndex NSB KERFUFFLE_IF_PLANS(, LongIndex NTC)>
bool RegistryT<
    ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, NSB KERFUFFLE_IF_PLANS(, NTC)>>::
    isResumable(const StateID stateId) const {
  if (KERFUFFLE_CHECKED(stateId < STATE_COUNT))
    if (Parent parent = stateParents[stateId]) {
      KERFUFFLE_ASSERT(parent.forkId > 0);

      return parent.prong == compoResumable[parent.forkId - 1];
    }

  return false;
}

//------------------------------------------------------------------------------

template <typename TC,
          typename TG,
          typename TSL,
          typename TRL,
          LongIndex     NCC,
          LongIndex NSB KERFUFFLE_IF_PLANS(, LongIndex NTC)>
bool RegistryT<
    ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, NSB KERFUFFLE_IF_PLANS(, NTC)>>::
    isPendingChange(const StateID stateId) const {
  if (KERFUFFLE_CHECKED(stateId < STATE_COUNT))
    if (Parent parent = stateParents[stateId]) {
      KERFUFFLE_ASSERT(parent.forkId > 0);

      return compoRequested[parent.forkId - 1] !=
             compoActive[parent.forkId - 1];
    }

  return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC,
          typename TG,
          typename TSL,
          typename TRL,
          LongIndex     NCC,
          LongIndex NSB KERFUFFLE_IF_PLANS(, LongIndex NTC)>
bool RegistryT<
    ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, NSB KERFUFFLE_IF_PLANS(, NTC)>>::
    isPendingEnter(const StateID stateId) const {
  if (KERFUFFLE_CHECKED(stateId < STATE_COUNT))
    if (Parent parent = stateParents[stateId]) {
      KERFUFFLE_ASSERT(parent.forkId > 0);

      return parent.prong != compoActive[parent.forkId - 1] &&
             parent.prong == compoRequested[parent.forkId - 1];
    }

  return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC,
          typename TG,
          typename TSL,
          typename TRL,
          LongIndex     NCC,
          LongIndex NSB KERFUFFLE_IF_PLANS(, LongIndex NTC)>
bool RegistryT<
    ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, NSB KERFUFFLE_IF_PLANS(, NTC)>>::
    isPendingExit(const StateID stateId) const {
  if (KERFUFFLE_CHECKED(stateId < STATE_COUNT))
    if (Parent parent = stateParents[stateId]) {
      KERFUFFLE_ASSERT(parent.forkId > 0);

      return parent.prong == compoActive[parent.forkId - 1] &&
             parent.prong != compoRequested[parent.forkId - 1];
    }

  return false;
}

//------------------------------------------------------------------------------

template <typename TC,
          typename TG,
          typename TSL,
          typename TRL,
          LongIndex     NCC,
          LongIndex NSB KERFUFFLE_IF_PLANS(, LongIndex NTC)>
const Parent&
RegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, NSB KERFUFFLE_IF_PLANS(, NTC)>>::
    forkParent(const ForkID forkId) const {
  KERFUFFLE_ASSERT(forkId > 0);

  return compoParents[forkId - 1];
}

//------------------------------------------------------------------------------

template <typename TC,
          typename TG,
          typename TSL,
          typename TRL,
          LongIndex     NCC,
          LongIndex NSB KERFUFFLE_IF_PLANS(, LongIndex NTC)>
bool RegistryT<
    ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, NSB KERFUFFLE_IF_PLANS(, NTC)>>::
    requestImmediate(const Request request) {
  if (request.stateId == 0)
    return false;
  else if (KERFUFFLE_CHECKED(request.stateId < STATE_COUNT)) {
    Parent parent = stateParents[request.stateId];

    if (KERFUFFLE_CHECKED(parent)) {
      KERFUFFLE_ASSERT(parent.forkId > 0);

      compoRequested[parent.forkId - 1] = parent.prong;

      for (parent = forkParent(parent.forkId); parent;
           parent = forkParent(parent.forkId)) {
        KERFUFFLE_ASSERT(parent.forkId > 0);
        compoRemains.set(parent.forkId - 1);

        if (compoActive[parent.forkId - 1] != parent.prong)
          compoRequested[parent.forkId - 1] = parent.prong;
        else {
          parent = forkParent(parent.forkId);
          break;
        }
      }

      for (; parent; parent = forkParent(parent.forkId)) {
        KERFUFFLE_ASSERT(parent.forkId > 0);
        compoRemains.set(parent.forkId - 1);
      }
    }
  }

  return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC,
          typename TG,
          typename TSL,
          typename TRL,
          LongIndex     NCC,
          LongIndex NSB KERFUFFLE_IF_PLANS(, LongIndex NTC)>
void RegistryT<
    ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, NSB KERFUFFLE_IF_PLANS(, NTC)>>::
    requestScheduled(const StateID stateId) {
  if (KERFUFFLE_CHECKED(stateId < STATE_COUNT)) {
    const Parent parent = stateParents[stateId];

    KERFUFFLE_ASSERT(parent.forkId > 0);
    compoResumable[parent.forkId - 1] = parent.prong;
  }
}

//------------------------------------------------------------------------------

template <typename TC,
          typename TG,
          typename TSL,
          typename TRL,
          LongIndex     NCC,
          LongIndex NSB KERFUFFLE_IF_PLANS(, LongIndex NTC)>
void RegistryT<
    ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, NSB KERFUFFLE_IF_PLANS(, NTC)>>::
    clearRequests() {
  compoRequested.clear();
  orthoRequested.clear();
  compoRemains.clear();
}

//------------------------------------------------------------------------------

template <typename TC,
          typename TG,
          typename TSL,
          typename TRL,
          LongIndex     NCC,
          LongIndex NSB KERFUFFLE_IF_PLANS(, LongIndex NTC)>
void RegistryT<
    ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, NSB KERFUFFLE_IF_PLANS(, NTC)>>::
    reset() {
  compoRequested.clear();
  orthoRequested.clear();
  compoActive.clear();
  compoResumable.clear();
  compoRemains.clear();
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace detail
}  // namespace kerfuffle

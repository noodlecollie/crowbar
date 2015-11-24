#ifndef CHANGESIGNAL_H
#define CHANGESIGNAL_H

// When sending change signals, send along a randomly generated ID number.
// When forwarding change signals, copy this ID number.
// This means that if an object has to forward multiple change signals,
// if it receives more than one with the same ID then it knows these signals
// were both generated by one initial change.
typedef int ChangeId_t;

#endif // CHANGESIGNAL_H
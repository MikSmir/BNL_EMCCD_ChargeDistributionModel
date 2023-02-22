# BNL_EMCCD_ChargeDistributionModel

This code was part of a project at Brookhaven National Laboratory, which solves for charge distribution on a 5x5 pixel grid of an EMCCD (Electron-Multiplying Charge Coupled Device) using C++ ROOT framework. This was done as part of the Instrumentation Division at Brookhaven National Laboratory for their NSLS-II SIX Beamline.

A mathematical model was developed where charge distribution was modeled with a Gaussian shape for EMCCD x-ray coordinate measurements.
After implementing analytical techniques, the model was simplified to a single charge distribution function, which was then numerically simulated in C++ to plot the charge distribution along an EMCCD pixel grid using Midpoint Rule, Trapezoidal Rule, and Simpson's Rule. In addition, the degree of numerical accuracy was compared..

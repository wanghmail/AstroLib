#ifndef IZZOLAM_H
#define IZZOLAM_H

/**
 * Calculates the solution to the Lambert's problem using the Izzo algorithm.
 *
 * @param r1 The position vector of the departure point.
 * @param r2 The position vector of the arrival point.
 * @param tof The time of flight between the departure and arrival points.
 * @param mu The gravitational parameter of the central body.
 * @param way The direction of the transfer: 1 for short way, -1 for long way.
 * @param multi_revs The maximum number of complete revolutions allowed.
 * @param v1 The velocity vector at the departure point (output parameter).
 * @param v2 The velocity vector at the arrival point (output parameter).
 *
 * @return 0 if the algorithm successfully converges, -1 otherwise.
 *
 * @throws ErrorType if there was an error in the calculation.
 */
int solve_lambert_izzo(double r1[3], double r2[3], double tof, double mu, int way, int multi_revs,
                       double v1[][3], double v2[][3]);

#endif  // IZZOLAM_H
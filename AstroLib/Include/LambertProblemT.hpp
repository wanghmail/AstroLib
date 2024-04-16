
#ifndef LAMBERT_PROBLEM_T_H
#define LAMBERT_PROBLEM_T_H

#include <iomanip>

#include "LambertProblem.hpp"
#include <algorithm>

template <typename T>
void LambdaT2Pos(const T &lambda, const T &capsT, const double tof, const double mu, T &rx, T &ry) {
  double s = cbrt(2 * mu * pow(tof / capsT, 2));
  double c = (1 - lambda * lambda) * s;

  rx = 2 * s * s - 2 * s * c - 2 * s + c + 1;
  double sctemp = s * (1 + c - s);
  ry = 2 * sqrt(sctemp * (c - sctemp));
}
template <typename T> T hypergeometricF(T z, double tol) {
  T Sj = 1.0;
  T Cj = 1.0;
  double err = 1.0;
  T Cj1 = 0.0;
  T Sj1 = 0.0;
  int j = 0;
  while (err > tol) {
    Cj1 = Cj * (3.0 + j) * (1.0 + j) / (2.5 + j) * z / (j + 1);
    err = fabs(Cj1);
    Sj1 = Sj + Cj1;
    Sj = Sj1;
    Cj = Cj1;
    j = j + 1;
  }
  return Sj;
}

template <typename T> T x2tofLagrange(const T &lambda, const T &x, const int N) {
  T a = 1.0 / (1.0 - x * x);
  if (a > 0)  // ellipse
  {
    T alfa = 2.0 * acos(x);
    T beta = 2.0 * asin(sqrt(lambda * lambda / a));
    if (lambda < 0.0) beta = -beta;
    T tof = ((a * sqrt(a) * ((alfa - sin(alfa)) - (beta - sin(beta)) + 2.0 * M_PI * N)) / 2.0);
    return tof;
  } else {
    T alfa = 2.0 * acosh(x);
    T beta = 2.0 * asinh(sqrt(-lambda * lambda / a));
    if (lambda < 0.0) beta = -beta;
    T tof = (-a * sqrt(-a) * ((beta - sinh(beta)) - (alfa - sinh(alfa))) / 2.0);
    return tof;
  }
}
template <typename T> T x2tof(const T &lambda, const T &x, const int N) {
  double battin = 0.01;
  double lagrange = 0.2;
  double dist = fabs(x - 1);
  if (dist < lagrange && dist > battin) {  // We use Lagrange tof expression
    return x2tofLagrange(lambda, x, N);
  }

  T K = lambda * lambda;
  T E = x * x - 1.0;
  T z = sqrt(1 + K * E);
  if (dist < battin) {  // We use Battin series tof expression
    T eta = z - lambda * x;
    T S1 = 0.5 * (1.0 - lambda - x * eta);
    T Q = hypergeometricF(S1, 1e-11);
    Q = 4.0 / 3.0 * Q;
    T tof = (eta * eta * eta * Q + 4.0 * lambda * eta) / 2.0;
    if (N > 0) tof += N * M_PI / sqrt(-E * E * E);
    return tof;
  } else {  // We use Lancaster tof expresion
    T rho = (E < 0.0) ? -E : E;
    T y = sqrt(rho);
    T g = x * z - lambda * E;
    T d = 0.0;
    if (E < 0) {
      T l = acos(g);
      d = N * M_PI + l;
    } else {
      T f = y * (z - lambda * x);
      d = log(f + g);
    }
    T tof = (x - lambda * z - d / y) / E;
    return tof;
  }
}

template <typename T>
void dTdx(T &DT, T &DDT, T &DDDT, const T &x, const T &lambda, const T &capsT) {
  T l2 = lambda * lambda;
  T l3 = l2 * lambda;
  T umx2 = 1.0 - x * x;
  T y = sqrt(1.0 - l2 * umx2);
  T y2 = y * y;
  T y3 = y2 * y;
  DT = 1.0 / umx2 * (3.0 * capsT * x - 2.0 + 2.0 * l3 * x / y);
  DDT = 1.0 / umx2 * (3.0 * capsT + 5.0 * x * DT + 2.0 * (1.0 - l2) * l3 / y3);
  DDDT = 1.0 / umx2 * (7.0 * x * DDT + 8.0 * DT - 6.0 * (1.0 - l2) * l2 * l3 * x / y3 / y2);
}

template <typename T> int householder(const T &capsT, const T &lambda, T &x0, const int N,
                                      const double eps, const int iter_max) {
  int it = 0;
  double err = 1.0;
  double xnew = 0.0;
  double tof = 0.0, delta = 0.0, DT = 0.0, DDT = 0.0, DDDT = 0.0;
  while ((err > eps) && (it < iter_max)) {
    tof = x2tof(lambda, x0, N);
    dTdx(DT, DDT, DDDT, x0, lambda, tof);
    delta = tof - capsT;
    double DT2 = DT * DT;
    xnew = x0
           - delta * (DT2 - delta * DDT / 2.0)
                 / (DT * (DT2 - delta * DDT) + DDDT * delta * delta / 6.0);
    err = fabs(x0 - xnew);
    x0 = xnew;
    it++;
  }
  return it;
}

template <typename ST>
lambert_problem<ST>::lambert_problem(const Vector3T &r1, const Vector3T &r2, const ST &tof,
                                     const double &mu, const int &sl, const int &multi_revs)
    : m_r1(r1), m_r2(r2), m_tof(tof), m_mu(mu), m_multi_revs(multi_revs) {
  // 0 - Sanity checks
  if (tof <= 0) {
    throw std::domain_error("Time of flight is negative!");
  }
  if (mu <= 0) {
    throw std::domain_error("Gravity parameter is zero or negative!");
  }
  if (std::abs(sl) != 1) {
    throw std::domain_error("Transfer direction is not 1(short) or -1(long)!");
  }

  // 1 - Getting lambda and T
  m_c = sqrt((r2[0] - r1[0]) * (r2[0] - r1[0]) + (r2[1] - r1[1]) * (r2[1] - r1[1])
             + (r2[2] - r1[2]) * (r2[2] - r1[2]));
  ST R1 = m_r1.norm();
  ST R2 = m_r2.norm();
  m_s = (m_c + R1 + R2) / 2.0;
  ST T = sqrt(2.0 * m_mu / m_s / m_s / m_s) * m_tof;
  ST lambda2 = 1.0 - m_c / m_s;
  m_lambda = sl * sqrt(lambda2);

  // 2.1 - Let us first detect the maximum number of revolutions for which there
  // exists a solution
  m_Nmax = static_cast<int>(floor(T / M_PI));
  ST T00 = acos(m_lambda) + m_lambda * sqrt(1.0 - lambda2);
  ST T0 = (T00 + m_Nmax * M_PI);
  ST DT = 0.0, DDT = 0.0, DDDT = 0.0;
  if (m_Nmax > 0) {
    if (T < T0) {  // We use Halley iterations to find xM and TM
      int it = 0;
      double err = 1.0;
      ST T_min = T0;
      ST x_old = 0.0, x_new = 0.0;
      while (1) {
        dTdx(DT, DDT, DDDT, x_old, m_lambda, T_min);
        if (!(DT == 0.0)) {
          x_new = x_old - DT * DDT / (DDT * DDT - DT * DDDT / 2.0);
        }
        err = fabs(x_old - x_new);
        if ((err < 1e-13) || (it > 12)) {
          break;
        }
        T_min = x2tof(m_lambda, x_new, m_Nmax);
        x_old = x_new;
        it++;
      }
      if (T_min > T) {
        m_Nmax -= 1;
      }
    }
  }
  // We exit this if clause with Nmax being the maximum number of revolutions
  // for which there exists a solution. We crop it to m_multi_revs
  m_Nmax = std::min(m_multi_revs, m_Nmax);

  // 2.2 We now allocate the memory for the output variables
  m_v1.resize(m_Nmax * 2 + 1);
  m_v2.resize(m_Nmax * 2 + 1);
  m_iters.resize(m_Nmax * 2 + 1);
  m_x.resize(m_Nmax * 2 + 1);

  //
  ir1_ = m_r1 / sqrt(m_r1[0] * m_r1[0] + m_r1[1] * m_r1[1] + m_r1[2] * m_r1[2]);
  ir2_ = m_r2 / sqrt(m_r2[0] * m_r2[0] + m_r2[1] * m_r2[1] + m_r2[2] * m_r2[2]);
  ih_ = ir1_.cross(ir2_);
  ih_ = ih_ / sqrt(ih_[0] * ih_[0] + ih_[1] * ih_[1] + ih_[2] * ih_[2]);
  if (ih_[2] == 0) {
    // std::cout
    //     << "Warn: Angular momentum vector has no z component, set ih =
    //     [0,0,1]!"
    //     << std::endl;
    ih_ = {0, 0, 1};
  }
  if (m_lambda < 0) ih_ = ih_ * -1;
  it1_ = ih_.cross(ir1_);
  // it1_ /= sqrt(it1_[0] * it1_[0] + it1_[1] * it1_[1] + it1_[2] * it1_[2]);
  it2_ = ih_.cross(ir2_);
  // it2_ /= sqrt(it2_[0] * it2_[0] + it2_[1] * it2_[1] + it2_[2] * it2_[2]);
}
template <typename ST> void lambert_problem<ST>::SolveTimeEquation() {
  // 3 - We may now find all solutions in x,y
  // 3.1 0 rev solution
  // 3.1.1 initial guess
  ST lambda2 = 1.0 - m_c / m_s;
  ST lambda3 = lambda2 * m_lambda;
  ST T = sqrt(2.0 * m_mu / m_s / m_s / m_s) * m_tof;
  ST T00 = acos(m_lambda) + m_lambda * sqrt(1.0 - lambda2);
  ST T1 = 2.0 / 3.0 * (1.0 - lambda3);
  if (T >= T00) {
    m_x[0] = -(T - T00) / (T - T00 + 4);
  } else if (T <= T1) {
    m_x[0] = T1 * (T1 - T) / (2.0 / 5.0 * (1 - lambda2 * lambda3) * T) + 1;
  } else {
    m_x[0] = pow((T / T00), 0.69314718055994529 / log(T1 / T00)) - 1.0;
  }
  // 3.1.2 Householder iterations
  m_iters[0] = householder(T, m_lambda, m_x[0], 0, 1e-5, 15);
  // 3.2 multi rev solutions
  ST tmp;
  for (int i = 1; i < m_Nmax + 1; ++i) {
    // 3.2.1 left Householder iterations
    tmp = pow((i * M_PI + M_PI) / (8.0 * T), 2.0 / 3.0);
    m_x[2 * i - 1] = (tmp - 1) / (tmp + 1);
    m_iters[2 * i - 1] = householder(T, m_lambda, m_x[2 * i - 1], i, 1e-8, 15);
    // 3.2.1 right Householder iterations
    tmp = pow((8.0 * T) / (i * M_PI), 2.0 / 3.0);
    m_x[2 * i] = (tmp - 1) / (tmp + 1);
    m_iters[2 * i] = householder(T, m_lambda, m_x[2 * i], i, 1e-8, 15);
  }
}
template <typename ST> void lambert_problem<ST>::Solve() {
  SolveTimeEquation();
  // 4 - For each found x value we reconstruct the terminal velocities
  ST vr1, vt1, vr2, vt2;
  for (size_t i = 0; i < m_x.size(); ++i) {
    ConstructTerminalVecolityComponent(m_x[i], vr1, vr2, vt1, vt2);
    for (int j = 0; j < 3; ++j) m_v1[i][j] = vr1 * ir1_[j] + vt1 * it1_[j];
    for (int j = 0; j < 3; ++j) m_v2[i][j] = vr2 * ir2_[j] + vt2 * it2_[j];
  }
}

template <typename ST>
void lambert_problem<ST>::ConstructTerminalVecolityComponent(const ST &x, ST &vr1, ST &vr2, ST &vt1,
                                                             ST &vt2) const {
  ST R1 = m_r1.norm();
  ST R2 = m_r2.norm();
  ST gamma = sqrt(m_mu * m_s / 2.0);
  ST rho = (R1 - R2) / m_c;
  ST sigma = sqrt(1 - rho * rho);
  ST lambda2 = 1.0 - m_c / m_s;
  ST y = sqrt(1.0 - lambda2 + lambda2 * x * x);
  vr1 = gamma * ((m_lambda * y - x) - rho * (m_lambda * y + x)) / R1;
  vr2 = -gamma * ((m_lambda * y - x) + rho * (m_lambda * y + x)) / R2;
  ST vt = gamma * sigma * (y + m_lambda * x);
  vt1 = vt / R1;
  vt2 = vt / R2;
}
template <typename ST>
void lambert_problem<ST>::CalTerminalVelocities(const ST &x, Vector3T &v1, Vector3T &v2) const {
  ST vr1, vt1, vr2, vt2;
  ConstructTerminalVecolityComponent(x, vr1, vr2, vt1, vt2);

  // Vector3T v1, v2;
  v1 = vr1 * ir1_ + vt1 * it1_;
  v2 = vr2 * ir2_ + vt2 * it2_;
}

/// Gets velocity at r1
/**
 *
 * \return an std::vector containing 3-d arrays with the cartesian components of
 * the velocities at r1 for all 2N_max+1 solutions
 */
template <typename ST> const std::vector<Vector3T> &lambert_problem<ST>::get_v1() const {
  return m_v1;
}

/// Gets velocity at r2
/**
 *
 * \return an std::vector containing 3-d arrays with the cartesian components of
 * the velocities at r2 for all 2N_max+1 solutions
 */
template <typename ST> const std::vector<Vector3T> &lambert_problem<ST>::get_v2() const {
  return m_v2;
}

/// Gets r1
/**
 *
 * \return a 3-d array with the cartesian components of r1
 */
template <typename ST> const Vector3T &lambert_problem<ST>::get_r1() const { return m_r1; }

/// Gets r2
/**
 *
 * \return a 3-d array with the cartesian components of r2
 */
template <typename ST> const Vector3T &lambert_problem<ST>::get_r2() const { return m_r2; }

/// Gets the time of flight between r1 and r2
/**
 *
 * \return the time of flight
 */
template <typename ST> const ST &lambert_problem<ST>::get_tof() const { return m_tof; }

/// Gets the x variable
/**
 * Gets the x variable for each solution found (0 revs, 1,1,2,2,3,3 .... N,N)
 *
 * \return the x variables in an std::vector
 */
template <typename ST> const std::vector<ST> &lambert_problem<ST>::get_x() const { return m_x; }

/// Gets gravitational parameter
/**
 *
 * \return the gravitational parameter
 */
template <typename ST> const double &lambert_problem<ST>::get_mu() const { return m_mu; }

/// Gets number of iterations
/**
 *
 * \return an std::vector containing the iterations taken to compute each one of
 * the solutions
 */
template <typename ST> const std::vector<int> &lambert_problem<ST>::get_iters() const {
  return m_iters;
}

/// Gets N_max
/**
 *
 * \return the maximum number of revolutions. The number of solutions to the
 * problem will be Nmax*2 +1
 */
template <typename ST> int lambert_problem<ST>::get_Nmax() const { return m_Nmax; }

#endif
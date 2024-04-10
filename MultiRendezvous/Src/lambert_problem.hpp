#ifndef LAMBERT_PROBLEM_H
#define LAMBERT_PROBLEM_H

#include "myvector_t.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

#include <ostream>
#include <vector>

#define Vector3T Vector3<ST>

template <typename ST> class lambert_problem {
public:
  /**
   * @brief Construct a new lambert problem object
   *
   * @param r1 first cartesian position
   * @param r2 second cartesian position
   * @param tof time of flight
   * @param mu gravity parameter
   * @param sl 1: short way transfer; -1: long way transfer
   * @param multi_revs maximum number of multirevolutions to compute
   */
  lambert_problem(const Vector3T &r1 = {1, 0, 0}, const Vector3T &r2 = {0, 1, 0},
                  const ST &tof = M_PI_2, const double &mu = 1., const int &sl = 1,
                  const int &multi_revs = 6);
  void Solve();
  void SolveTimeEquation();
  void ConstructTerminalVecolityComponent(const ST &x, ST &vr1, ST &vr2, ST &vt1, ST &vt2) const;
  void CalTerminalVelocities(const ST &x, Vector3T &v1, Vector3T &v2) const;

  const std::vector<Vector3T> &get_v1() const;
  const std::vector<Vector3T> &get_v2() const;
  const Vector3T &get_r1() const;
  const Vector3T &get_r2() const;
  const ST &get_tof() const;
  const double &get_mu() const;
  ST get_lambda() const { return m_lambda; }
  ST get_T() const { return sqrt(2.0 * m_mu / m_s / m_s / m_s) * m_tof; }
  const std::vector<ST> &get_x() const;
  const std::vector<int> &get_iters() const;
  int get_Nmax() const;

private:
  const Vector3T m_r1, m_r2;
  const ST m_tof;
  const double m_mu;
  std::vector<Vector3T> m_v1;
  std::vector<Vector3T> m_v2;
  std::vector<int> m_iters;
  std::vector<ST> m_x;
  ST m_s, m_c, m_lambda;
  int m_Nmax;
  int m_multi_revs;

  Vector3T ir1_, ir2_, ih_, it1_, it2_;
};

//#undef _USE_MATH_DEFINES

#endif

#include "izzoLam.h"
#include <iostream>
#include "lambert_problem_t.hpp"

int solve_lambert_izzo(double r1[3], double r2[3], double tof, double mu, int way, int multi_revs,
                       double v1[][3], double v2[][3]) {

  int dimV = multi_revs * 2 + 1;
  memset(v1, 0, sizeof(double) * 3 * dimV);	memset(v2, 0, sizeof(double) * 3 * dimV);

  const Vector3<double> r1vec(r1[0], r1[1], r1[2]);
  const Vector3<double> r2vec(r2[0], r2[1], r2[2]);

  lambert_problem<double> lp(r1vec, r2vec, tof, mu, way, multi_revs);
  lp.Solve();
  std::vector<Vector3<double>> v1ls = lp.get_v1();
  std::vector<Vector3<double>> v2ls = lp.get_v2();

  for (size_t i = 0; i < v1ls.size(); i++) {
    v1[i][0] = v1ls[i][0];
    v1[i][1] = v1ls[i][1];
    v1[i][2] = v1ls[i][2];
    v2[i][0] = v2ls[i][0];
    v2[i][1] = v2ls[i][1];
    v2[i][2] = v2ls[i][2];
  }

  return 0;
}


//{
//    double r1[3]{ 41960184.323318355,3473991.642108842,-96064.78455976033 }, r2[3]{ -6441593.34, 41608179.66, 13233.21 }, tof = 43200, mu = 3.986e14, v1[1][3], v2[1][3];
//    int way = 1, multi_revs = 0;
//    solve_lambert_izzo(r1, r2, tof, mu, way, multi_revs, v1, v2);
//    cout << v1[0][0] << endl;
//}
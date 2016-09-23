#ifndef PHYSICS_INTER_OBJECT_FORCE_H_
#define PHYSICS_INTER_OBJECT_FORCE_H_

#include <cmath>
#include <Vc/Vc>

#include "random.h"

namespace bdm {

template <typename Backend>
class DefaultForce {
 public:
  using real_v = typename Backend::real_v;
  using real_t = typename Backend::real_t;

  DefaultForce() {}
  ~DefaultForce() {}
  DefaultForce(const DefaultForce&) = delete;
  DefaultForce& operator=(const DefaultForce&) = delete;

  void forceBetweenSpheres(const std::array<double, 3>& ref_mass_location_s,
                           double ref_diameter_s, double ref_iof_coefficient_s,
                           const std::array<real_v, 3>& nb_mass_location,
                           const real_v& nb_diameter,
                           const real_v& nb_iof_coefficient,
                           std::array<real_v, 3>* result) {
   std::array<real_v, 3> c1 = {
         real_v(ref_mass_location_s[0]),
         real_v(ref_mass_location_s[1]),
         real_v(ref_mass_location_s[2])};
    real_v r1(0.5f * ref_diameter_s);
    const auto& c2 = nb_mass_location;
    real_v r2 = 0.5f * nb_diameter;
    // We take virtual bigger radii to have a distant interaction, to get a
    // desired density.
    real_v additional_radius = 10.0f * Vc::min(real_v(ref_iof_coefficient_s), nb_iof_coefficient);
    r1 += additional_radius;
    r2 += additional_radius;
    // the 3 components of the vector c2 -> c1
    real_v comp1 = c1[0] - c2[0];
    real_v comp2 = c1[1] - c2[1];
    real_v comp3 = c1[2] - c2[2];
    real_v distance_between_centers =
        Vc::sqrt(comp1 * comp1 + comp2 * comp2 + comp3 * comp3);
    // the overlap distance (how much one penetrates in the other)
    real_v delta = r1 + r2 - distance_between_centers;

    // if no overlap : no force
    auto delta_lt_0 = delta < 0;
    if (delta_lt_0.isFull()) {
      *result = {real_v(0.0), real_v(0.0), real_v(0.0)};
      return;
    }
    // to avoid a division by 0 if the centers are (almost) at the same
    // location
    // fixme performance - generates a lot of random numbers which I think are hardly
    // ever used P(distance_between_centers < 0.00000001) very small
    auto random_force = Random::NextNoise<VcBackend>(VcBackend::real_v(3.0));
    auto distance_lt_min = distance_between_centers < real_v(0.00000001);   // fixme should be param or constant
    if (distance_lt_min.isFull()) {
      *result = random_force;
      return;
    }

    // the force itself
    real_v R = (r1 * r2) / (r1 + r2);
    real_v gamma = 1;  // attraction coeff
    real_v k = 2;      // repulsion coeff
    real_v F = k * delta - gamma * Vc::sqrt(R * delta);

    real_v module = F / distance_between_centers;
    std::array<real_v, 3> force2on1(
        {module * comp1, module * comp2, module * comp3});

    // fixme performance improvement possible here? - delta_lt_0 unlikely
    std::array<real_v, 3> ret;
    ret[0] = Vc::iif(delta_lt_0, real_v(0.0), force2on1[0] );
    ret[1] = Vc::iif(delta_lt_0, real_v(0.0), force2on1[1] );
    ret[2] = Vc::iif(delta_lt_0, real_v(0.0), force2on1[2] );

    if(!distance_lt_min.isEmpty()) {
      ret[0] = Vc::iif(distance_lt_min, random_force[0], ret[0]);
      ret[1] = Vc::iif(distance_lt_min, random_force[1], ret[1]);
      ret[2] = Vc::iif(distance_lt_min, random_force[2], ret[2]);
    }

    *result = ret;
  }
};

}  // namespace bdm

#endif  // PHYSICS_INTER_OBJECT_FORCE_H_

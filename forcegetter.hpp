#pragma once
#include "random.hpp"

// singleton
struct ForceGetter {

	RandomGen rgen;

	ForceGetter() {
		rgen = RandomGen(RANDOM_SEED);
	};

	ForceGetter operator=(const ForceGetter& other) {
		rgen = other.rgen;
		// TODO
		return *this;
	}


	const float
		generalForceMultip = GENERAL_FORCE_MULTIP
		;

	const float
		r0 = REPULSION_START,
		r1 = SECONDARY_FORCE_END,
		rd = r1 - r0,
		fm = MAX_RESULSIVE_FORCE
		;

	vectorList<vectorList<float>> forceMaxMatrix = FORCE_MATRIX;

	void randomiseForceMaxMatrix(float var, float mean, bool sym = false) {

		if (sym) {
			for (int y = 0; y < forceMaxMatrix.size(); y++) {
				for (int x = y; x < forceMaxMatrix.size(); x++) {

					auto val = (rgen.get() * 2.f - 1.f) * var + mean;
					forceMaxMatrix[y][x] = val;
					forceMaxMatrix[x][y] = val;

				}
			}
		}
		else {
			for (vectorList<float>& row : forceMaxMatrix) {
				for (float& element : row) {
					element = (rgen.get() * 2.f - 1) * var + mean;
				}
			}
		}

		//for (vectorList<float>& row : forceMaxMatrix) {
		//	printVectorList(row);
		//}
	}

	// https://www.desmos.com/calculator/zahxpb0kld
	float forceFunction(float dist, float forceMaxValue) const {
		if (dist < r0) {
			return fm + (-fm / r0) * dist;
		}
		if (dist < r1) {
			const float dydx = forceMaxValue / rd / 2;

			if (dist < r0 + .5f * rd) {
				return dydx * (dist - r0);
			}
			else {
				return -dydx * (dist - r1);
			}

		}

		if (dist < 0) {
			exit(-2);
		}

		return 0;

	}

	inline float getForce(const float dist, const int p1Colour, const int p2Colour) const {
		return forceFunction(
			dist,
			forceMaxMatrix[p1Colour][p2Colour]
		) * generalForceMultip
			//0
			;

	}
};
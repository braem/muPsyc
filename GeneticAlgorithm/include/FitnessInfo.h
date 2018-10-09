#pragma once

namespace geneticalgorithm {
	namespace fitness {
		class FitnessInfo {
		public:
			double fitness;
			double huron2001Fitness;
			double brownJordana2011Fitness;

			unsigned int numHuron2001FitnessRules = 13;
			double registralCompassFitness;
			double leapLengtheningFitness;
			double partCrossingFitness;
			double pitchOverlappingFitness;
			double semblantMotionFitness;
			double parallelMotionFitness;
			double avoidSemblantApproachBetweenFusedIntervalsFitness;
			double exposedIntervalsFitness;
			double fusedIntervalsFitness;
			double avoidTonalFusionFitness;
			double obliqueApproachToFusedIntervalsFitness;
			double avoidDisjunctApproachToFusedIntervalsFitness;
			double chordSpacingFitness;

			unsigned int numBrownJordana2011FitnessRules = 4;
			double largeLeapResolutionFitness;
			double unequalIntevalsFitness;
			double scale7orLessDegreesFitness;
			double limitedDurationValuesFitness;
			double contourFitness;

			void setHuron2001Fitness() {
				huron2001Fitness = (registralCompassFitness + leapLengtheningFitness + partCrossingFitness +
					pitchOverlappingFitness + semblantMotionFitness + parallelMotionFitness + avoidSemblantApproachBetweenFusedIntervalsFitness +
					exposedIntervalsFitness + fusedIntervalsFitness + avoidTonalFusionFitness + obliqueApproachToFusedIntervalsFitness +
					avoidDisjunctApproachToFusedIntervalsFitness + chordSpacingFitness) / static_cast<double>(numHuron2001FitnessRules);
				fitness = huron2001Fitness;
			}
			void setBrownJordana2011Fitness() {
				brownJordana2011Fitness = (largeLeapResolutionFitness + unequalIntevalsFitness + scale7orLessDegreesFitness +
					limitedDurationValuesFitness) / static_cast<double>(numBrownJordana2011FitnessRules);
				fitness = brownJordana2011Fitness;
			}
			void setOverallFitness() {
				fitness = (registralCompassFitness + leapLengtheningFitness + partCrossingFitness +
					pitchOverlappingFitness + semblantMotionFitness + parallelMotionFitness + avoidSemblantApproachBetweenFusedIntervalsFitness +
					exposedIntervalsFitness + fusedIntervalsFitness + avoidTonalFusionFitness + obliqueApproachToFusedIntervalsFitness +
					avoidDisjunctApproachToFusedIntervalsFitness + chordSpacingFitness + largeLeapResolutionFitness + unequalIntevalsFitness +
					scale7orLessDegreesFitness + limitedDurationValuesFitness)
					/ static_cast<double>(numHuron2001FitnessRules + numBrownJordana2011FitnessRules);
			}

			FitnessInfo() {}
		};
	}
}
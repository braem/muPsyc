#pragma once

namespace geneticalgorithm {
	namespace fitness {
		class FitnessInfo {
		public:
			double fitness;
			double huron2001Fitness;
			double brownJordana2011Fitness;
			double traditionalFitness;

			unsigned int numHuron2001FitnessRules = 14;
			double registralCompassFitness;
			double leapLengtheningFitness;
			double partCrossingFitness;
			double pitchOverlappingFitness;
			double semblantMotionFitness;
			double parallelMotionFitness;
			double avoidSemblantApproachBetweenFusedIntervalsFitness;
			double exposedIntervalsFitness;
			double parallelFusedIntervalsFitness;
			double avoidTonalFusionFitness;
			double obliqueApproachToFusedIntervalsFitness;
			double avoidDisjunctApproachToFusedIntervalsFitness;
			double chordSpacingFitness;

			unsigned int numTraditionalRules = 5;
			double avoidUnisonsFitness;

			double onsetSynchronizationFitness;

			unsigned int numBrownJordana2011FitnessRules = 2;
			double largeLeapResolutionFitness;
			double unequalIntevalsFitness;
			double scale7orLessDegreesFitness;
			double limitedDurationValuesFitness;
			double contourFitness;

			void setHuron2001Fitness() {
				huron2001Fitness = (registralCompassFitness + leapLengtheningFitness + partCrossingFitness +
					pitchOverlappingFitness + semblantMotionFitness + parallelMotionFitness + avoidSemblantApproachBetweenFusedIntervalsFitness +
					exposedIntervalsFitness + parallelFusedIntervalsFitness + avoidTonalFusionFitness + obliqueApproachToFusedIntervalsFitness +
					avoidDisjunctApproachToFusedIntervalsFitness + chordSpacingFitness) 
					/ static_cast<double>(numHuron2001FitnessRules);
				fitness = huron2001Fitness;
			}
			void setBrownJordana2011Fitness() {
				brownJordana2011Fitness = (limitedDurationValuesFitness + contourFitness) / static_cast<double>(numBrownJordana2011FitnessRules);
				fitness = brownJordana2011Fitness;
			}
			void setTraditionalRulesFitness() {
				traditionalFitness = (registralCompassFitness + partCrossingFitness + exposedIntervalsFitness + chordSpacingFitness + avoidUnisonsFitness) 
					/ static_cast<double>(numTraditionalRules);
				fitness = traditionalFitness;
			}
			void setOverallFitness() {
				fitness = (registralCompassFitness + leapLengtheningFitness + partCrossingFitness +
					pitchOverlappingFitness + semblantMotionFitness + parallelMotionFitness + avoidSemblantApproachBetweenFusedIntervalsFitness +
					exposedIntervalsFitness + parallelFusedIntervalsFitness + avoidTonalFusionFitness + obliqueApproachToFusedIntervalsFitness +
					avoidDisjunctApproachToFusedIntervalsFitness + chordSpacingFitness + limitedDurationValuesFitness + contourFitness + registralCompassFitness + 
					partCrossingFitness + exposedIntervalsFitness + chordSpacingFitness + avoidUnisonsFitness)
					/ static_cast<double>(numHuron2001FitnessRules + numTraditionalRules + numBrownJordana2011FitnessRules);
				traditionalFitness = (registralCompassFitness + partCrossingFitness + exposedIntervalsFitness + chordSpacingFitness + avoidUnisonsFitness)
					/ static_cast<double>(numTraditionalRules);
				huron2001Fitness = (registralCompassFitness + leapLengtheningFitness + partCrossingFitness +
					pitchOverlappingFitness + semblantMotionFitness + parallelMotionFitness + avoidSemblantApproachBetweenFusedIntervalsFitness +
					exposedIntervalsFitness + parallelFusedIntervalsFitness + avoidTonalFusionFitness + obliqueApproachToFusedIntervalsFitness +
					avoidDisjunctApproachToFusedIntervalsFitness + chordSpacingFitness)
					/ static_cast<double>(numHuron2001FitnessRules);
				brownJordana2011Fitness = (limitedDurationValuesFitness + contourFitness) / static_cast<double>(numBrownJordana2011FitnessRules);
			}

			FitnessInfo() {}
		};
	}
}
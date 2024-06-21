#include "PerlinNoiseGenerator.h"
#include <iostream>

float PerlinNoiseGenerator::inverseLerp(float a, float b, float v) {
	return (v - a) / (b - a);
}

PerlinNoiseGenerator::PerlinNoiseGenerator() {
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, INT_MAX);

	this->seed = dist(rng);
}

PerlinNoiseGenerator::PerlinNoiseGenerator(int seed) {
	this->seed = seed;
}

std::vector<std::vector<float>> PerlinNoiseGenerator::generateNoiseMap(bool constrainCircular, int mapWidth, int mapHeight) {
	std::vector<std::vector<float>> perlinNoiseMap = generateNoiseMapMatrix(mapWidth, mapHeight, 0, 0);

	if (constrainCircular) {
		std::vector<std::vector<float>> squareGradient = generateSquareGradient(mapWidth, mapHeight);
		std::vector<std::vector<float>> combinedMap = subtractNoiseMaps(perlinNoiseMap, squareGradient);

		return combinedMap;
	}
	return perlinNoiseMap;
}

std::vector<std::vector<float>> PerlinNoiseGenerator::generateNoiseMapMatrix(int mapWidth, int mapHeight, int offsetX, int offsetY) {
	std::vector<std::vector<float>> noiseMap = std::vector<std::vector<float>>(
		mapWidth, std::vector<float>(mapHeight, 0));

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, 20000);

	std::vector<float> octaveOffsetsX(octaves);
	std::vector<float> octaveOffsetsY(octaves);

	for (int i = 0; i < octaves; i++) {
		// -10000 is a hacky fix-- uniform_int_distribution should be between -10000 and 10000 but that's not allowed
		float tmpOffsetX = dist(rng) - 10000 + offsetX;
		float tmpOffsetY = dist(rng) - 10000 + offsetY;
		octaveOffsetsX[i] = tmpOffsetX;
		octaveOffsetsY[i] = tmpOffsetY;
	}

	if (scale <= 0) {
		scale = .0001f;
	}

	float minNoiseHeight = FLT_MAX;
	float maxNoiseHeight = FLT_MIN;


	float halfWidth = mapWidth / 2;
	float halfHeight = mapHeight / 2;

	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			float amplitude = 1;
			float frequency = 1;
			float noiseHeight = 0;

			for (int i = 0; i < octaves; i++) {
				float sampleX = (x - halfWidth) / scale * frequency + octaveOffsetsX[i];
				float sampleY = (y - halfHeight) / scale * frequency + octaveOffsetsY[i];

				const siv::PerlinNoise::seed_type seed = this->seed;

				const siv::PerlinNoise perlin{ seed };


				// Kinda cheesy to put amplitude as 1 in here, but want to retain original structure
				// of code I'm importing
				float perlinValue = perlin.octave2D(sampleX, sampleY, 1) * 2 - 1;
				noiseHeight += perlinValue * amplitude;

				amplitude *= persistance;
				frequency *= lacunarity;
			}

			if (noiseHeight > maxNoiseHeight){
				maxNoiseHeight = noiseHeight;
			}
			else if (noiseHeight < minNoiseHeight) {
				minNoiseHeight = noiseHeight;
			}

			noiseMap[x][y] = noiseHeight;
		}
	}

	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			noiseMap[x][y] = inverseLerp(minNoiseHeight, maxNoiseHeight, noiseMap[x][y]);
		}
	}

	

	return noiseMap;
}

std::vector<std::vector<float>> PerlinNoiseGenerator::subtractNoiseMaps(const std::vector<std::vector<float>>& a, const std::vector<std::vector<float>>& b) {
	// Could do size checking here to make sure a and b are same sizes... I'm lazy rn teehee (you will regret this while debugging)

	int xSize = a.size();
	int ySize = a[0].size();

	std::vector<std::vector<float>> subtractedNoiseMap = std::vector<std::vector<float>>(
		xSize, std::vector<float>(ySize, 0));

	for (int x = 0; x < xSize; x++) {
		for (int y = 0; y < ySize; y++) {
			float s = a[x][y] - b[x][y];

			subtractedNoiseMap[x][y] = s;
		}
	}

	return subtractedNoiseMap;
}

std::vector<std::vector<float>> PerlinNoiseGenerator::generateSquareGradient(int xSize, int ySize) {
	int halfWidth = xSize / 2;
	int halfHeight = ySize / 2;

	std::vector<std::vector<float>> gradient = std::vector<std::vector<float>>(
		xSize, std::vector<float>(ySize, 0));

	for (int i = 0; i < xSize; i++) {
		for (int j = 0; j < ySize; j++) {
			int x = i;
			int y = j;

			float colorValue;

			x = x > halfWidth ? xSize - x : x;
			y = y > halfHeight ? ySize - y : y;

			int smaller = x < y ? x : y;
			colorValue = smaller / (float)halfWidth;

			colorValue = (1 - colorValue);
			colorValue *= colorValue * colorValue;
			gradient[i][j] = colorValue;
		}
	}

	return gradient;

}


#pragma once
#include "PerlinNoise.hpp"
#include <vector>
#include <random>


class PerlinNoiseGenerator {
public:
	int seed = 0;
	int octaves = 8;
	float scale = 175;
	float lacunarity = 1.6;
	float persistance = 0.9;

	PerlinNoiseGenerator();
	PerlinNoiseGenerator(int seed);
	std::vector<std::vector<float>> generateNoiseMap(bool constrainCircular, int mapWidth, int mapHeight);
	std::vector<std::vector<float>> generateNoiseMapMatrix(int mapWidth, int mapHeight, int offsetX, int offsetY);
	static std::vector<std::vector<float>> subtractNoiseMaps(const std::vector<std::vector<float>>& a, const std::vector<std::vector<float>>& b);
	static float inverseLerp(float a, float b, float v);
private:
	int offsetX;
	int offsetY;

	std::vector<std::vector<float>> generateSquareGradient(int xSize, int ySize);
};
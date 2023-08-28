# blueNoise [![License](https://img.shields.io/github/license/dsecrieru/blueNoise.svg)](https://github.com/dsecrieru/blueNoise/blob/master/LICENSE)

*In computer graphics, the term "blue noise" is sometimes used more loosely as any noise with minimal low frequency components and no concentrated spikes in energy.* - Wikipedia

![blue noise example](https://github.com/dsecrieru/blueNoise/blob/main/images/pdsBridson02.PNG)

# Description

A header-only, C++20 implementation of Robert Bridson's *Fast Poisson Disk Sampling* (2007), for the 2-dimensional case.
More details in the original paper: https://www.cs.ubc.ca/~rbridson/docs/bridson-siggraph07-poissondisk.pdf
What I wanted, that is a bit different from other implementations that I've seen, is the possibility to parametrize the point type.

In the future, I might add more implementations.

Built and tested on:
- Windows 10, VS 2017, compiler version 19.35.32215
- WSL Ubuntu 23.04, GCC 13.0.1

# Performance
- arch: amd64
- CPU: AMD Ryzen 9 5900X 4.2GHz
- RAM: 64GB
- compiler: VS 2017, compiler version 19.35.32215
- compiler options:  `/Ox /std:c++20`
- 10 samples

Comparison w. https://github.com/kchapelier/poisson-disk-sampling/releases/tag/2.3.1, JavaScript; 1600x900, k=30:
Implementation | minDist | maxDistance | points | ms
--- | --- | --- | --- | ---
kchapelier | 8.00 | 16.00 | ~14313 | 62.723
dsecrieru, Bridson w. mt19937 RNG | 7.9 | 15.8 | ~14404 | 30

Comparison w. https://github.com/corporateshark/poisson-disk-generator, C++; k=30:
Implementation | points | ms
--- | --- | ---
corporateshark, all defaults | ~9826 | 55
dsecrieru, Bridson w. mt19937 RNG, 1600x900, minDist=9.5 | ~9970 | 21

Comparison w. https://github.com/martynafford/poisson-disc-distribution-bridson, C++; mt19937 RNG, 1600x900, minDist=9.5, k=30:
Implementation | points | ms
--- | --- | ---
martynafford | ~9774 | 23.91
dsecrieru | ~9970 | 21

This is a good one, very similar in performance, with some interesting ideas that inspired me to incorporate (local lambdas, configuration pack, random picking from active list might not be necessary).

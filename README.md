# blueNoise

*In computer graphics, the term "blue noise" is sometimes used more loosely as any noise with minimal low frequency components and no concentrated spikes in energy.* - Wikipedia

![blue noise example](https://github.com/dsecrieru/blueNoise/blob/main/images/pdsBridson01.PNG)

# Description

A header-only, C++20 implementation of Robert Bridson's *Fast Poisson Disk Sampling* from 2007, for the 2-dimensional case.
More details in the original paper: https://www.cs.ubc.ca/~rbridson/docs/bridson-siggraph07-poissondisk.pdf

Built and tested on:
- Windows 10, VS 2017, compiler version 19.35.32215
- WSL Ubuntu 23.04, GCC 12.2.0

# License

[LICENSE](./LICENSE) ([The MIT license](http://choosealicense.com/licenses/mit/))

# Performance
- arch: amd64
- CPU: AMD Ryzen 9 5900X 4.2GHz
- RAM: 64GB
- compiler: VS 2017, compiler version 19.35.32215
- compiler options:  `/Ox /std:c++20`
- 10 samples

Comparison w. https://github.com/corporateshark/poisson-disk-generator, C++ implementation; k=30:
Implementation | points | ms
--- | --- | ---
corporateshark, all defaults | ~9826 | 55
dsecrieru, vanilla Bridson w. mt19937 RNG, 1600x900, minDist=9.5 | ~9978 | 22.05

Comparison w. https://github.com/kchapelier/poisson-disk-sampling, JavaScript implementation, v2.2.3, 1600x900, k=30:
Implementation | minDist | maxDistance | points | ms
--- | --- | --- | --- | ---
kchapelier | 8.00 | 16.00 | ~14313 | 62.723
dsecrieru, vanilla Bridson w. mt19937 RNG | 7.9 | 15.8 | ~14411 | 32.21

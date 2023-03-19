# blueNoise

### *In computer graphics, the term "blue noise" is sometimes used more loosely as any noise with minimal low frequency components and no concentrated spikes in energy.* - Wikipedia
</br>

A small, header-only, implementation of Robert Bridson's *Fast Poisson Disk Sampling* from 2007, for the 2-dimensional case.
More details in the original paper: https://www.cs.ubc.ca/~rbridson/docs/bridson-siggraph07-poissondisk.pdf

### Crude comparisons between different implementations:
- arch: amd64
- CPU: AMD Ryzen 9 5900X
- RAM: 64GB
- compiler: VS 2017
- optimization: `/Ox`
- 10 samples

Comparison w. https://github.com/corporateshark/poisson-disk-generator, C++ implementation; k=30:
Implementation | points | ms
--- | --- | ---
corporateshark, all defaults | ~9826 | 55
dsecrieru, vanilla Bridson w. mt19937, 1600x900, minDist=9.6 | ~9862 | 29

Comparison w. https://github.com/kchapelier/poisson-disk-sampling, JavaScript implementation, v2.2.1, results from author; 800x800, k=30:
Implementation | minDist | maxDistance | points | ms
--- | --- | --- | --- | ---
kchapelier | 6.96 | 13.93 | ~8377 | 39.890ms
dsecrieru, vanilla Bridson w. mt19937 | 6.96 | 13.93 | ~8337 | 25
kchapelier | 8.00 | 16.00 | ~6373 | 30.538ms
dsecrieru, vanilla Bridson w. mt19937 | 8.00 | 16.00 | ~6319 | 19

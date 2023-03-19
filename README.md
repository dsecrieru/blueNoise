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

Comparison w. https://github.com/corporateshark/poisson-disk-generator, C++ implementation, k=30
Implementation | points | ms
--- | --- | ---
corporateshark, all defaults | ~9826 | ~55
dsecrieru/blueNoise, vanilla Bridson w. mt19937, 1600x900, minDist=9.6 | ~9870 | ~32

Comparison w. https://github.com/kchapelier/poisson-disk-sampling, JavaScript implementation, v2.2.1, results from author
Implementation | points | ms
--- | --- | ---
kchapelier/fast-2d-poisson-disk-sampling, JavaScript, 800x800 minDist 4.00 maxDistance 8.00 tries 30 | ~25335 | 118.758
dsecrieru/blueNoise (vanilla Bridson w. mt19937), 800x800 minDist 4.00, maxDist 8.00, tries 30 | 25125 | 83

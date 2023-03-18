# blueNoise

## "*In computer graphics, the term "blue noise" is sometimes used more loosely as any noise with minimal low frequency components and no concentrated spikes in energy.*" - Wikipedia
</br>

### Crude comparison between different implementations:
- arch: amd64
- CPU: AMD Ryzen 9 5900X
- RAM: 64GB
- compiler: VS 2017
- optimization: `/Ox`
- Bridson's k parameter: 30

Implementation | k | points | ms
--- | --- | --- | ---
corporateshark/poisson-disk-generator, all defaults | 30 | 9785 | 56
dsecrieru/blueNoise (vanilla Bridson w. mt19937) | 30 | 10049 | 33
dsecrieru/blueNoise (vanilla Bridson w. minstd_rand) | 30 | 10049 | 33
dsecrieru/blueNoise (vanilla Bridson w. mt19937) | 20 | 9877 | 22

# Prizm

4D morphing oscillator for VCV Rack

## License

[LICENSE.md](./license.md)

## Build

1. Download and install rack sdk, which can be found [here](https://vcvrack.com/manual/PluginDevelopmentTutorial)
2. Set your RACK_DIR environment variable

```bash
export RACK_DIR=/path/to/rack-sdk
```

3. Build with Make

```bash
# Compile
make

# Create distributable
make dist

# Install module to your VCV Rack
make install
```

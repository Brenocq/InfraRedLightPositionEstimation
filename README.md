# Light Position Estimation

## Introduction
This project aims to create a neural network to estimate the position of a light emitting object using multiple light sensors.
Firstly the tests are being performed using visible light. In the next step I will use infrared light emitters and receivers.
The main motivation for this project is to allow low cost robots to locate the position of other robots relative to them.

## The Neural Network
Starting from the scenario where there are several robots emitting light and reading the light intensity of the environment. My goal is to get each robot to be able to determine the relative position of another robot using only light sensors. The neural network input will be the reading of each sensor, while the output will be the relative coordinate of the other robot closest to it.

### Training
The training will take place using a camera to detect the relative position of the _light emitting PCB_ relative to the _main PCB with sensors_. After data is collected, the neural network is fed with the values of each sensor and the error is adjusted using the coordinates of the robots.

## Experiments
### First experiment: Visible light
![first_plot](brenocq.github.com/LightPositionEstimation/assets/image.jpg)
#### Work in progress...


### Installation
```
sudo apt-get install libusb-1.0-0.dev
sudo apt-get install libboost-all-dev
sudo apt-get install cmake


cd LightPositionEstimation/src/Raspberry/lib/Pixy/
rm -r build
cd scripts
./build_libpixyusb.sh
./install_libpixyusb.sh

cd ../../../
rm -r build
./make.sh
```

[Pixymon](https://docs.pixycam.com/wiki/doku.php?id=wiki:v1:installing_pixymon_on_linux)

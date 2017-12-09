# Neural-Network
A simple neural network written in C++ to overcome the boredom of the summer holidays.

Written to scale. Different training data may be used on the network by coding seperate
shared libraries, and each network may be saved to a file (serialised) so retraining
will not be necessary.

Works reasonably well on the MNIST test/training data, though a convolutional network
would be necessary for more accurate number detection on foreign data.

# Usage
```
make
make gen
```
# Dependencies:
- [eigen3](http://eigen.tuxfamily.org/)
- [mnist-loader](https://github.com/wichtounet/mnist)
- [json](https://github.com/nlohmann/json)
- [zlib](https://panthema.net/2007/0328-ZLibString.html)

#ifndef NOISE_H
#define NOISE_H

class Noise {
public:
    virtual double getValue(double x, double y, double z) = 0;
};

#endif // NOISE_H

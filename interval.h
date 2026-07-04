#ifndef INTERVAL_H_
#define INTERVAL_H_

class interval{
    public:
        double min, max;
        double clamp(double x) const {
            if (x<min) return min;
            if (x>max) return max;
            return x;
        }

        interval expand(double delta) const{
            auto padding = delta/2.0;
            return interval(min-padding, max+padding);
        }
        static const interval empty, universe;
};

#endif
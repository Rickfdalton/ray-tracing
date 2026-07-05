#ifndef INTERVAL_H_
#define INTERVAL_H_
#include <limits>

class interval{
    public:
        double min, max;
        double clamp(double x) const {
            if (x<min) return min;
            if (x>max) return max;
            return x;
        }

        interval(): min(+std::numeric_limits<double>::infinity()),max(-std::numeric_limits<double>::infinity()){}

        interval(double _min, double _max):min(_min),max(_max){}

        interval(const interval& a, const interval& b){
            min= (a.min < b.min) ? a.min : b.min ;
            max= (a.max > b.max) ? a.max : b.max ;
        }

        interval expand(double delta) const{
            auto padding = delta/2.0;
            return interval(min-padding, max+padding);
        }
        static const interval empty, universe;
};

#endif
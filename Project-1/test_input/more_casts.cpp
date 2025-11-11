struct Point {
    double x, y;
};

void moreCastExamples() {
    // More static_cast examples
    float f = 42.5f;
    long l = static_cast<long>(f);
    
    // More reinterpret_cast examples
    Point p = {1.0, 2.0};
    unsigned char* bytes = reinterpret_cast<unsigned char*>(&p);
    
    // Complex const_cast example
    const Point* constPoint = &p;
    Point* mutablePoint = const_cast<Point*>(constPoint);
}

#ifndef SHAPE_H
#define SHAPE_H

class Shape
{

protected:
    
    int seg_x;
    int seg_y; 

public:

    Shape(int n_seg_x, int n_seg_y): seg_x(n_seg_x), seg_y(n_seg_y) {}
    ~Shape() {}

    virtual void initData() = 0;
    virtual void renderFill() = 0;
    virtual void renderNormals() = 0;
    virtual void renderWire() = 0;

    void segmentsUpdate(int nx, int ny){
        seg_x = nx; seg_y = ny; initData();
    }

};

#endif
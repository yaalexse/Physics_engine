#include <stdio.h>
#include <string>

const float g = 9.81;
const int x_max = 1000;
const int y_max = 700;
const int x_min = 0;
const int y_min = 0;

class Vector{
    public:
    float getX();
    float getY();
    float getZ();
    void setX(float _x);
    void setY(float _y);
    void setZ(float _z);
    void zero();
    void setVector( float _x, float _y, float _z);
    private:
    float x;
    float y;
    float z;
};

void Vector::setVector( float _x, float _y, float _z){
    x =_x;
    y = _y;
    z = _z;
}


float Vector::getX(){
    return x;
}

float Vector::getY(){
    return y;
}

float Vector::getZ(){
    return z;
}

void Vector::setX(float _x){
    x = _x;
}

void Vector::setY(float _y){
    y = _y;
}

void Vector::setZ(float _z){
    z = _z;
}

void Vector::zero(){
    x = 0.0;
    y = 0.0;
    z = 0.0;
}


class Force: public Vector{
    public:
    private:

};

class Point{
    public:
    Force* forces;
    int Numforces;
    Vector coord;
    Vector speed;
    Vector acc;
    void pfdX();
    void pfdY();
    void pfd();
    void initial_cond_zero();
    void oneVerlet( bool collision, Vector normal,float deltaT );
    private:
    float mass;
};

void Point::initial_cond_zero(){
    coord.zero();
    speed.zero();
    speed.zero();
}

void Point::pfdX(){
    if( Numforces == 0 ){
        printf("Impossible to make the PFD there is no forces");
    }
    int i;
    float sum;
    for(i = 0;i < Numforces; i++){
        sum += forces[i].getX();
    }
    acc.setX( sum );
}

void Point::pfdY(){
        if( Numforces == 0 ){
        printf("Impossible to make the PFD there is no forces");
    }
    int i;
    float sum;
    for(i = 0;i < Numforces; i++){
        sum += forces[i].getY();
    }
    acc.setY( sum );
}

void Point::pfd(){
    pfdX();
    pfdY();
}


void Point::oneVerlet( bool collision, Vector normal, float deltaT){
    float x;
    float y;
    if(collision==true){
        x = acc.getX();
        y = acc.getY();
        acc.setX(normal.getX());
        acc.setY(normal.getY());
    }
    else{
    speed.setX( speed.getX() + acc.getX() * deltaT);
    speed.setY( speed.getY() + acc.getY() * deltaT);
    coord.setX( coord.getX() + speed.getX() * deltaT);
    coord.setY( coord.getY() + speed.getY() * deltaT);
    }
    acc.setX( x );
    acc.setY( y );  
}

void setSim(Point m, Force poids){
    //Set initial cond and pfd
    m.initial_cond_zero();
    m.coord.setX(500.0);
    m.coord.setY(600.0);
    m.speed.setX(0.0);
    m.speed.setY(0.0);
    m.Numforces = 1;
    poids.setX(0.);
    poids.setY(-g);
    m.forces = &poids;
    m.pfd();

}



int main(int argc, char* agrs[]){
    int i;
    Vector u;
    Force poids;
    Point m;
    setSim(m,poids);
    float totalTime = 100.0;
    float deltaT = 1.0;
    printf(" X : %f, Y : %f, Z : %f\n", m.coord.getX(), m.coord.getY(), m.coord.getZ());
    for( i = 0; i <= totalTime/deltaT; i++){
        if(m.coord.getY()<=y_min){
            u.setVector(0.,1.,0.);
            m.oneVerlet(true,u,0.1);}
        if(m.coord.getY()>=y_max){
            u.setVector(0.,-1.,0.);
            m.oneVerlet(true,u,0.1);}
        if(m.coord.getX()<=x_min){
            u.setVector(1.,0.,0.);
            m.oneVerlet(true,u,0.1);}
        if(m.coord.getX()>=x_max){
            u.setVector(-1.,0.,0.);
            m.oneVerlet(true,u,0.1);}
        else{
            m.oneVerlet(false, u,deltaT);}
        printf(" X : %f, Y : %f, Z : %f\n", m.coord.getX(), m.coord.getY(), m.coord.getZ());
        printf(" vX : %f, vY : %f, vZ : %f\n", m.speed.getX(), m.speed.getY(), m.speed.getZ());
    }


    return 0;

}
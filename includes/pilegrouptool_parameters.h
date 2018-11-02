#ifndef PILEGROUPTOOL_PARAMETERS_H
#define PILEGROUPTOOL_PARAMETERS_H

#include <QVector>
#include <QColor>

// fixed parameters (limits for piles and soil layers)
#define MAXPILES 3
#define MAXLAYERS 3

// Meshing parameters
#define MIN_ELEMENTS_PER_LAYER   15
#define MAX_ELEMENTS_PER_LAYER   40
#define NUM_ELEMENTS_IN_AIR       4

/*
 *  force limits are given as integers to work with the sliders.
 *  They are converted to floats when generating the FEA model.
 */

// #define MAX_FORCE 5000
#define MAX_H_FORCE  1000
#define MAX_V_FORCE 10000
#define MAX_MOMENT 2500
#define MAX_DISP   1

// global constants
#define GAMMA_WATER 9.81

// functions
#define ABS(X) (X<0.0?-X:X)

// data types
typedef struct {
    int    pileIdx;
    int    nodeIdx;
    double x;
    double reductionFactorLeftMovement;
    double reductionFactorRightMovement;
} HEAD_NODE_TYPE;

typedef struct {
    int    pileIdx;
    int    nodeIdx;
    double x;
    double z;
} CAP_NODE_TYPE;

struct PILE_INFO {
    double L1;            // pile length above ground (all the same)
    double L2;            // embedded length of pile
    double pileDiameter;  // pile diameter
    double E;             // pile modulus of elasticity
    double xOffset;       // x-offset of pile
};

struct PILE_FEA_INFO : PILE_INFO {
    int numNodePile;
    int maxLayers;
    int nodeIDoffset;
    int elemIDoffset;
    int firstNodeTag;
    int lastNodeTag;
    int firstElementTag;
    int lastElementTag;
};

#define SWAP(X,Y) {HEAD_NODE_TYPE tmp=Y; Y=X, Y=tmp; }

static QVector<QColor> LINE_COLOR({Qt::blue,Qt::red,Qt::green,Qt::cyan,Qt::magenta,Qt::yellow});
static QVector<QColor> BRUSH_COLOR({
                                       QColor(255, 127, 0, 127),
                                       QColor(191,  95, 0, 127),
                                       QColor(127,  63, 0, 127),
                                       QColor(127, 255, 0, 127),
                                       QColor( 95, 191, 0, 127),
                                       QColor( 63, 127, 0, 127),
                                       QColor(127, 255, 255, 255),
                                       QColor( 95, 191, 255, 255),
                                       QColor( 63, 127, 255, 255),
                                       QColor(127, 255, 255, 127),
                                       QColor( 95, 191, 255, 127),
                                       QColor( 63, 127, 255, 127),
                                       QColor(255, 127, 0, 63),
                                       QColor(191,  95, 0, 63),
                                       QColor(127,  63, 0, 63),
                                       QColor(127, 255, 0, 63),
                                       QColor( 95, 191, 0, 63),
                                       QColor( 63, 127, 0, 63)
                                    });
#define GROUND_WATER_BLUE QColor(127,127,255,192)

enum class LoadControlType {
                                ForceControl,
                                PushOver,
                                SoilMotion
                            };

#endif // PILEGROUPTOOL_PARAMETERS_H

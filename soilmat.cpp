#include<soilmat.h>
#include<math.h>

int soilLayer::numLayers = 0;
const double pi = atan(1.0) * 4.0;


soilLayer::soilLayer()
{
  ++numLayers;
  layerName = QString("Layer" + QString::number(numLayers));
  layerH = 1.0;
  layerG = 2.0e5;
  layerPhi = 30;
  layerGamma = 18.0;
  layerColor = QColor(100,100,100,100);
  topStress = 0;
  botStress = 0;
}

soilLayer::soilLayer(QString lName, double nThick, double lUnitWeight, double lStiffness, double lFrictionAng, QColor color)
{
  ++numLayers;
  layerName = lName;
  layerH = nThick;
  layerG = lStiffness;
  layerPhi = lFrictionAng;
  layerGamma = lUnitWeight;
  layerColor = color;
  topStress = 0;
  botStress = 0;
}

soilLayer::~soilLayer()
{

}

void
soilLayer::update()
{
    calcStress();
}

void
soilLayer::calcStress()
{
  botStress = topStress + layerGamma * layerH;
}

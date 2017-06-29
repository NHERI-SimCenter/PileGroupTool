#ifndef SOILMAT_H
#define SOILMAT_H

#include<QString>
#include<QColor>

class soilLayer
{
public:
  soilLayer();
  soilLayer(QString lName, double nThick, double lUnitWeight, double lStiffness,double lFrictionAng, QColor color);
   ~soilLayer();

  void setLayerName(QString name) {layerName = name;}
  void setLayerThickness(double H){layerH = H;}
  void setLayerOverburdonStress(double sigma0){layerSigma0 = sigma0;}
  void setLayerStiffness(double G){layerG = G;}
  void setLayerFrictionAng(double Phi){layerPhi = Phi;}
  void setLayerUnitWeight(double gamma){layerGamma = gamma;}
  void setLayerTopStress(double stress){topStress = stress;}
  void setLayerColor(QColor color){layerColor = color;}
  void update();

  QString getLayerName(){ return layerName;}
  QColor getLayerColor(){return layerColor;}
  double getLayerThickness(){return layerH;}
  double getLayerStiffness(){return layerG;}
  double getLayerFrictionAng(){return layerPhi;}
  double getLayerUnitWeight(){return layerGamma;}
  double getLayerTopStress(){return topStress;}
  double getLayerBottomStress(){return botStress;}

private:
  void calcStress();

  QString layerName;
  double layerH;
  double layerGamma;
  double layerG;
  double layerPhi;
  double layerSigma0;
  QColor layerColor;

  double topStress;
  double botStress;

static int numLayers;
};

#endif // SOILMAT_H

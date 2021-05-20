// Implementatie van klasse Beurs

#include <iostream>
#include <fstream>
#include "standaard.h"
#include "beurs.h"
using namespace std;

//*************************************************************************

// Default constructor
Beurs::Beurs ()
{
  // TODO: implementeer (zo nodig) deze constructor

}  // default constructor

//****************************************************************************

bool Beurs::leesIn (const char* invoernaam){
  ifstream file;
  file.open(invoernaam);
  if(!file.is_open()){
      return false;
  }
  file >> tw >> n >> provisie >> b0;
  if(tw<1 || tw>100 || n<1 || n>8){
    return false;
  }
  for(int i=0;i<=tw;i++){
    for(int j=0;j<n;j++){
      file >> aandelen[i][j];
    }
  }
  for(int i=0;i<tw;i++){
    file >> rente_percentages[i];
  }
  return true;
}  // leesIn

//****************************************************************************

void Beurs::drukAfInvoer ()
{
  cout << "\t";
  for(int i=1;i<=n;i++){
    cout << "Aandeel " << i << '\t';
  }
  cout << "Rente Percentage" << endl;

  for(int i=0;i<=tw;i++){
    cout << "Dag " << i  << "\t";
    for(int j=0;j<n;j++){
        cout << aandelen[i][j] << "\t \t";      
    }    
    if(i != tw){
      cout << rente_percentages[i] << endl;
    }
  }
}  // drukAfInvoer

//****************************************************************************

double Beurs::bepaalMaxBedragBU
         (vector <vector <pair <bool,int> > > &transacties)
{
  // TODO: implementeer deze memberfunctie

  return 0.0;

}  // bepaalMaxBedragBU

//****************************************************************************
double Beurs::nieuweTotaleBedrag(int huidigeAandelen, int nieuweAandelen,int dag){
  bitset<MaxN> a1 = bitset<MaxN>(huidigeAandelen);
  bitset<MaxN> a2 = bitset<MaxN>(nieuweAandelen);
  double bedrag = 0;
  for(int i=0;i<n;i++){
    if(a1[i] != a2[i]){
      if(a2[i] == 1){
        double extra = 1 + (provisie / 100);
        bedrag -= aandelen[dag][i] * extra;
      }else{
        bedrag += aandelen[dag][i] * (1 - (provisie / 100));
      }
    }
  }
  return bedrag;
}

double Beurs::bepaalMaxBedragRecHelper(bool memo,double huidigBedrag,int huidigeAandelen,int dag){
  if(dag == tw){
    return huidigBedrag + nieuweTotaleBedrag(huidigeAandelen,0,tw);
  }
  double besteBedrag = huidigBedrag;
  for(int i=0;i<pow(2,n)-1;i++){
    double bedrag = nieuweTotaleBedrag(huidigeAandelen,i,dag) + huidigBedrag;
    bedrag *= 1 + (rente_percentages[dag] / 100);
    if(bedrag >= 0){
      double nieuweTotaalBedrag = bepaalMaxBedragRecHelper(memo,bedrag,i,dag + 1);
      if(nieuweTotaalBedrag > besteBedrag){
        besteBedrag = nieuweTotaalBedrag;
      }
    }
  }
  return besteBedrag;
}

double Beurs::bepaalMaxBedragRec (bool memo){
  return bepaalMaxBedragRecHelper(memo,b0,0,0);
}  // bepaalMaxBedragRec (memo)

//****************************************************************************

void Beurs::drukAfTransacties
            (vector <vector <pair <bool,int> > > transacties)
{
  // TODO: implementeer deze memberfunctie

}  // drukAfTransacties


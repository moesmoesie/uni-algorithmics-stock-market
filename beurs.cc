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

  maakRentePercentagesLeeg();
  maakAandelenLeeg();

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
double Beurs::bepaalMaxBedragBU(vector <vector <pair <bool,int> > > &transacties){
  maakHulpTabelLeeg();

  for (int a = 0; a < pow(2,n); a++){ //base case
    double bedrag = nieuweTotaleBedrag(0,a,0);  
    bedrag+= b0;  
    bedrag= krijgBedragPlusRente(bedrag,0);
    if(bedrag >= 0){
      hulpTabel[0][a] = bedrag;
    }
  }
  
  for(int dag = 1; dag < tw+1; dag++){
    for(int a1 = 0; a1 < pow(2,n); a1++){
      double besteBedrag = hulpTabel[dag -1][a1];
      for(int a2=0;a2<pow(2,n);a2++){
        double bedrag = hulpTabel[dag-1][a2];
        if(bedrag <= 0 ){
          continue;
        }
        bedrag += nieuweTotaleBedrag(a2,a1,dag);
        if(bedrag > besteBedrag){
          besteBedrag = bedrag;
        }
      }
      hulpTabel[dag][a1] = krijgBedragPlusRente(besteBedrag,dag);
    }
  }
  return hulpTabel[tw][0];
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

double Beurs::bepaalMaxBedragRecHelper(int huidigeAandelen,int dag){
  if(dag == 0){
    double bedrag = b0 + nieuweTotaleBedrag(0,huidigeAandelen,dag);
    if (bedrag < 0){
      return -1;
    }
    return bedrag;
  }
  double besteBedrag = 0;
  bool geweest = false;
  for(int i=0;i<=pow(2,n)-1;i++){
    double bedrag = bepaalMaxBedragRecHelper(i, dag - 1);
    if(bedrag != -1){
      bedrag = krijgBedragPlusRente(bedrag,dag-1);
      bedrag+= nieuweTotaleBedrag(i,huidigeAandelen,dag);
      if(bedrag > besteBedrag){
        besteBedrag = bedrag;
        geweest = true;
      }
    }
  }
  if(!geweest){
    return -1;
  }
  return besteBedrag;
}
double Beurs::bepaalMaxBedragRecMemoHelper(int huidigeAandelen,int dag){  
  if(hulpTabel[dag][huidigeAandelen] < 0){
    if(dag == 0){
      double bedrag = b0 + nieuweTotaleBedrag(0,huidigeAandelen,dag);
      if (bedrag >= 0){
        hulpTabel[dag][huidigeAandelen] = bedrag;
      }
    }else{
      for(int i=0;i<=pow(2,n)-1;i++){
        double bedrag = bepaalMaxBedragRecMemoHelper(i, dag - 1);
        if(bedrag != -1){
          bedrag = krijgBedragPlusRente(bedrag,dag-1);
          bedrag+= nieuweTotaleBedrag(i,huidigeAandelen,dag);
          if(bedrag > hulpTabel[dag][huidigeAandelen]){
              hulpTabel[dag][huidigeAandelen] = bedrag;
          }
        }
      }
    }
  }
  return hulpTabel[dag][huidigeAandelen];
}

double Beurs::krijgBedragPlusRente(double bedrag, int dag){
  return bedrag * ((rente_percentages[dag]/100) + 1);
}

double Beurs::bepaalMaxBedragRec (bool memo){
  if(memo){
    maakHulpTabelLeeg();
    return bepaalMaxBedragRecMemoHelper(0,tw);
  }
  return bepaalMaxBedragRecHelper(0,tw);
}  // bepaalMaxBedragRec (memo)

//****************************************************************************

void Beurs::drukAfTransacties
            (vector <vector <pair <bool,int> > > transacties)
{
  // TODO: implementeer deze memberfunctie

}  // drukAfTransacties


void Beurs::maakHulpTabelLeeg(){
  for(int i=0;i<=tw;i++){
    for(int j=0;j<=pow(2,n)-1;j++){
      hulpTabel[i][j] = -1;
    }
  }
}

void Beurs::maakRentePercentagesLeeg(){
  for(int i=0;i<MaxTw;i++){
    rente_percentages[i]=0;
  }
}

void Beurs::maakAandelenLeeg(){
  for(int i=0;i<MaxTw;i++){
    for(int j=0;j<MaxAs;j++){
      aandelen[i][j]=0;
    }
  }
}
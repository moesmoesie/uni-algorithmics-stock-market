// Implementatie van klasse Beurs

#include <iostream>
#include <fstream>
#include "standaard.h"
#include "beurs.h"
using namespace std;

//*************************************************************************

// Default constructor
Beurs::Beurs (){
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
    file >> rentePercentages[i];
  }
  return true;
}  // leesIn

//****************************************************************************

void Beurs::drukAfInvoer (){
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
      cout << rentePercentages[i] << endl;
    }
  }
}// drukAfInvoer

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

  int X[tw + 1];
  
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
          X[dag-1] = a2;
          besteBedrag = bedrag;
        }
      }
      hulpTabel[dag][a1] = krijgBedragPlusRente(besteBedrag,dag);
    }
  }
  X[tw] = 0;
  transacties = krijgTransacties(X);
  return hulpTabel[tw][0];
}  // bepaalMaxBedragBU

//****************************************************************************

double Beurs::bepaalMaxBedragRec (bool memo){
  if(memo){
    maakHulpTabelLeeg();
    return bepaalMaxBedragRecMemoHelper(0,tw);
  }
  return bepaalMaxBedragRecHelper(0,tw);
}// bepaalMaxBedragRec (memo)

double Beurs::bepaalMaxBedragRecMemoHelper(int huidigeAandelen,int dag){  
  if(hulpTabel[dag][huidigeAandelen] < 0){
    if(dag == 0){
      double bedrag = b0 + nieuweTotaleBedrag(0,huidigeAandelen,dag);
      if (bedrag >= 0){
        hulpTabel[dag][huidigeAandelen] = bedrag;
      }
    }
    else{
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
}// bepaalMaxBedragRecMemoHelper

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
}// bepaalMaxBedragRecHelper

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
}// nieuweTotaleBedrag

void Beurs::drukAfTransacties(vector <vector <pair <bool,int> > > transacties){
  cout << endl;
  for(int i=0;i<=tw;i++){
    cout << "Dag " << i  << "\t";
    vector<pair <bool,int> >vect = transacties[i];
    bool isEerste = true;
    bool heeftGekocht = false;
    for(int koopIndex=0;koopIndex<vect.size();koopIndex++){
      if(vect[koopIndex].first){
        heeftGekocht = true;
        if(isEerste){
          isEerste = false;
          cout << "Koop aandelen:     ";
        }
        cout << vect[koopIndex].second  << "  ";
      }
    }
    isEerste = true;
    for(int verkoopIndex=0;verkoopIndex<vect.size();verkoopIndex++){
      if(!vect[verkoopIndex].first){
        if(isEerste){
          isEerste = false;
          if(heeftGekocht){
            cout << endl << "\t";
          }
          cout << "Verkoop aandelen:  ";
        }
        cout << vect[verkoopIndex].second  << "  ";
      }
    }
    cout << endl;
  }
  cout << endl;
}// drukAfTransacties

double Beurs::krijgBedragPlusRente(double bedrag, int dag){
  return bedrag * ((rentePercentages[dag]/100) + 1);
}// krijgBedragPlusRente

void Beurs::maakHulpTabelLeeg(){
  for(int i=0;i<=tw;i++){
    for(int j=0;j<=pow(2,n)-1;j++){
      hulpTabel[i][j]=-1;
    }
  }
}// maakHulpTabelLeeg

void Beurs::maakRentePercentagesLeeg(){
  for(int i=0;i<MaxTw;i++){
    rentePercentages[i]=0;
  }
}// maakRentePercentagesLeeg

void Beurs::maakAandelenLeeg(){
  for(int i=0;i<MaxTw;i++){
    for(int j=0;j<MaxAs;j++){
      aandelen[i][j]=0;
    }
  }
}// maakAandelenLeeg

vector <vector <pair <bool,int> > >  Beurs::krijgTransacties(int X[MaxTw]){
  vector <vector <pair <bool,int> > > transacties;
  for(int i=0;i<=tw;i++){
    bitset<MaxN> aVandaag = bitset<MaxN>(X[i]);
    vector< pair <bool,int> > vect;
    if(i == 0){
      for(int j=0;j<n;j++){
        if(aVandaag[j]==1){
          pair<bool,int> x;
          x.second = j;
          x.first = true;
          vect.push_back(x);
        }
      }
    }else{
      bitset<MaxN> aGister = bitset<MaxN>(X[i - 1]);
      for(int j=0;j<n;j++){
        if(aVandaag[j] != aGister[j]){
            pair<bool,int> x;
            x.second = j;
            x.first = aVandaag[j] == 1;
            vect.push_back(x);
        }
      }
    }
    transacties.push_back(vect);
  }
  return transacties;
}// krijgTransacties
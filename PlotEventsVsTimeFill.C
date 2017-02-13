#include <vector>
#include <TCanvas.h>
#include <TStyle.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TGraphAsymmErrors.h>
#include <iostream>
#include <fstream>
#include <TLine.h>
#include <TMath.h>
#include <math.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TROOT.h>
#include <TRandom.h>
#include <TCut.h>
#include <TClonesArray.h>
#include <TLegend.h>
#include <TRandom3.h>
#include <TF1.h>

#include <TStyle.h>
#include <TLatex.h>
#include <TDirectory.h>
#include <TCollection.h>
#include <TPostScript.h>

using namespace std;

char saythis[500];
char outfile[50];

int CurrentFillNum;
int nEvents;
TString PDName;
TString junk;
int junkint;
std::vector<std::string>  PDNameList;
std::vector<int>  RunNumList;
std::vector<int>  nEventsList;
std::vector<TGraphErrors>  GraphEvtsList;
std::vector<TGraphErrors>  GraphSizeList;
std::vector<TH1D>  HistoList;

static const int nTotalPDs = 51;//39;
static const int nMyPDs    = 35;//23;

TH1D *h_evtsInt[nTotalPDs];
TH1D *h_evtsRaw[nTotalPDs];
TH1D *h_sizeInt[nTotalPDs];
TH1D *h_sizeRaw[nTotalPDs];
TH1D *h_sizePerEvtRaw[nTotalPDs];
TH1D *h_PINGInt[6];
TH1D *h_PINGEvt[6];
TH1D *h_PINGInt_index[6];
TH1D *h_PINGInt_index_error[6];
TH1D *h_PINGRaw_index[6];
TH1D *h_PINGEvt_index[6];
TH1D *h_PINGRawPerEvt_index[6];
TH1D *h_TotalInt;
TH1D *h_Lumi_vs_Fill;
TH1D *h_Lumi_vs_Fill_Int;
TH1D *h_Lumi_vs_Index;
TH1D *h_Lumi_vs_Index_Int;

char AllPings[6][20] = {
 "Total",
 "Dimuon",
 "Highpt",
 "FlowCorr",
 "Spectra/HF",
 "Forward"
};

//PINGS: 
// 0: blah
// 1: dimuon
// 2: Highpt
// 3: FlowCorr
// 4: Spectra/HF
// 5: Forward

int MARKS[nTotalPDs] = {0,0,0,0,0,  0,0,0,0,0,  0,4,25,26,27,  0,26,28,20,24, 22,23,24,27,24, 25,26,27,28,30, 24,32,24,21,20, 0,0,0,0,4, 4,4,4,4,4, 4,4,4,4,4,4};
int PINGS[nTotalPDs] = {0,0,0,0,0,  0,0,0,0,0,  0,4,4,1,2,     0,5,3,3,3,     3,3,3,3,3,       4,4,4,4,4,      4,4,4,4,1,     0,0,0,0,4, 4,4,4,4,4, 4,4,4,4,4,4};
int COLORS[6] = {1,kGreen+1,2,4,kOrange+1,6};

float MaxCap = 2100;
float fitRange[2];
int nFillMore=0;
int FirstFill = 5519;
int TotalNFills = 30;//33
int LastFill    = 0;
int SAVEMYSHIT = 0;
//float fitRange[2] = {4694.5,4699.5};
//float fitRange[2] = {4697.5,4720.5};
//float fitRange[2] = {4697.5,4699.5};
//float fitRange[2] = {4699.5,4720.5};

//float fitRange[2] = {29,29+float(nFillMore+5)};

char MyPDs[nMyPDs][100] = {
  "PADTrack1",
  "PADTrack2",
  "PADoubleMuon",
  "PAEGJet1",
  "PAForward",
  "PAHighMultiplicity0",
  "PAHighMultiplicity1",
  "PAHighMultiplicity2",
  "PAHighMultiplicity3",
  "PAHighMultiplicity4",
  "PAHighMultiplicity5",
  "PAHighMultiplicity6",
  "PAHighMultiplicity7",
  "PAMinimumBias1",
  "PAMinimumBias2",
  "PAMinimumBias3",
  "PAMinimumBias4",
  "PAMinimumBias5",
  "PAMinimumBias6",
  "PAMinimumBias7",
  "PAMinimumBias8",
  "PAMinimumBiasBkg",
  "PASingleMuon",
  "PAMinimumBias9",
  "PAMinimumBias10",
  "PAMinimumBias11",
  "PAMinimumBias12",
  "PAMinimumBias13",
  "PAMinimumBias14",
  "PAMinimumBias15",
  "PAMinimumBias16",
  "PAMinimumBias17",
  "PAMinimumBias18",
  "PAMinimumBias19",
  "PAMinimumBias20"
};


char AllPDs[nTotalPDs][100] = {
  "AlCaLumiPixels",
  "AlCaP0",
  "AlCaPhiSym",
  "EcalLaser",
  "EventDisplay",

  "ExpressPhysicsPA",
  "HLTMonitorPA",
  "HLTPhysics",
  "HcalNZS",
  "OnlineMonitor",

  "PACastor",
  "PADTrack1",
  "PADTrack2",
  "PADoubleMuon",
  "PAEGJet1",

  "PAEmptyBX",
  "PAForward",
  "PAHighMultiplicity0",
  "PAHighMultiplicity1",
  "PAHighMultiplicity2",

  "PAHighMultiplicity3",
  "PAHighMultiplicity4",
  "PAHighMultiplicity5",
  "PAHighMultiplicity6",
  "PAHighMultiplicity7",

  "PAMinimumBias1",
  "PAMinimumBias2",
  "PAMinimumBias3",
  "PAMinimumBias4",
  "PAMinimumBias5",

  "PAMinimumBias6",
  "PAMinimumBias7",
  "PAMinimumBias8",
  "PAMinimumBiasBkg",
  "PASingleMuon",

  "RPCMonitor",
  "TestEnablesEcalHcal",
  "TestEnablesEcalHcalDQM",
  "ZeroBias",
  "PAMinimumBias9",

  "PAMinimumBias10",
  "PAMinimumBias11",
  "PAMinimumBias12",
  "PAMinimumBias13",
  "PAMinimumBias14",

  "PAMinimumBias15",
  "PAMinimumBias16",
  "PAMinimumBias17",
  "PAMinimumBias18",
  "PAMinimumBias19",
  "PAMinimumBias20"

};

//JASON
//double AllPD_AODSizes[nTotalPDs] = {
//  (1.0/1024.0)*0    , //"AlCaLumiPixels",
//  (1.0/1024.0)*0    , //"AlCaP0",
//  (1.0/1024.0)*0    , //"AlCaPhiSym",
//  (1.0/1024.0)*0    , //"EcalLaser",
//  (1.0/1024.0)*0    , //"EventDisplay",
//  (1.0/1024.0)*0    , //"ExpressPhysicsPA",
//  (1.0/1024.0)*0    , //"HLTMonitorPA",
//  (1.0/1024.0)*0    , //"HLTPhysics",
//  (1.0/1024.0)*0    , //"HcalNZS",
//  (1.0/1024.0)*0    , //"OnlineMonitor",
//  (1.0/1024.0)*0    , //"PACastor",
//  (1.0/1024.0)*75.7 , //"PADTrack1",
//  (1.0/1024.0)*0    , //"PADTrack2",
//  (1.0/1024.0)*98.2 , //"PADoubleMuon",
//  (1.0/1024.0)*87.0 , //"PAEGJet1",
//  (1.0/1024.0)*0    , //"PAEmptyBX",
//  (1.0/1024.0)*28.8 , //"PAForward",
//  (1.0/1024.0)*101.1, //"PAHighMultiplicity0",
//  (1.0/1024.0)*140.4, //"PAHighMultiplicity1",
//  (1.0/1024.0)*140.4, //"PAHighMultiplicity2",
//  (1.0/1024.0)*140.4, //"PAHighMultiplicity3",
//  (1.0/1024.0)*140.4, //"PAHighMultiplicity4",
//  (1.0/1024.0)*140.4, //"PAHighMultiplicity5",
//  (1.0/1024.0)*140.4, //"PAHighMultiplicity6",
//  (1.0/1024.0)*155.8, //"PAHighMultiplicity7",
//  (1.0/1024.0)*43.7 , //"PAMinimumBias1",
//  (1.0/1024.0)*43.6 , //"PAMinimumBias2",
//  (1.0/1024.0)*43.6 , //"PAMinimumBias3",
//  (1.0/1024.0)*43.6 , //"PAMinimumBias4",
//  (1.0/1024.0)*43.6 , //"PAMinimumBias5",
//  (1.0/1024.0)*43.6 , //"PAMinimumBias6",
//  (1.0/1024.0)*43.3 , //"PAMinimumBias7",
//  (1.0/1024.0)*43.2 , //"PAMinimumBias8",
//  (1.0/1024.0)*43.3 , //"PAMinimumBiasBkg",
//  (1.0/1024.0)*90.0 , //"PASingleMuon",
//  (1.0/1024.0)*0    , //"RPCMonitor",
//  (1.0/1024.0)*0    , //"TestEnablesEcalHcal",
//  (1.0/1024.0)*0    , //"TestEnablesEcalHcalDQM",
//  (1.0/1024.0)*0    , //"ZeroBias"
//  (1.0/1024.0)*43.6 , //"PAMinimumBias9",
//  (1.0/1024.0)*43.6 , //"PAMinimumBias10",
//  (1.0/1024.0)*43.6 , //"PAMinimumBias11",
//  (1.0/1024.0)*43.6 , //"PAMinimumBias12",
//  (1.0/1024.0)*43.6 , //"PAMinimumBias13",
//  (1.0/1024.0)*43.6 , //"PAMinimumBias14",
//  (1.0/1024.0)*43.6 , //"PAMinimumBias15",
//  (1.0/1024.0)*43.6 , //"PAMinimumBias16",
//  (1.0/1024.0)*43.6 , //"PAMinimumBias17",
//  (1.0/1024.0)*43.6 , //"PAMinimumBias18",
//  (1.0/1024.0)*43.6 , //"PAMinimumBias19",
//  (1.0/1024.0)*43.6 , //"PAMinimumBias20",
//};
//
//double AllPD_RAWSizes[nTotalPDs] = {
//  (1.0/1024.0)*0    , //"AlCaLumiPixels",
//  (1.0/1024.0)*0    , //"AlCaP0",
//  (1.0/1024.0)*0    , //"AlCaPhiSym",
//  (1.0/1024.0)*0    , //"EcalLaser",
//  (1.0/1024.0)*0    , //"EventDisplay",
//  (1.0/1024.0)*0    , //"ExpressPhysicsPA",
//  (1.0/1024.0)*0    , //"HLTMonitorPA",
//  (1.0/1024.0)*0    , //"HLTPhysics",
//  (1.0/1024.0)*0    , //"HcalNZS",
//  (1.0/1024.0)*0    , //"OnlineMonitor",
//  (1.0/1024.0)*0    , //"PACastor",
//  (1.0/1024.0)*175.7, //"PADTrack1",
//  (1.0/1024.0)*168.5, //"PADTrack2",
//  (1.0/1024.0)*181.1, //"PADoubleMuon",
//  (1.0/1024.0)*175.2, //"PAEGJet1",
//  (1.0/1024.0)*0    , //"PAEmptyBX",
//  (1.0/1024.0)*89.0 , //"PAForward",
//  (1.0/1024.0)*217.4, //"PAHighMultiplicity0",
//  (1.0/1024.0)*266.7, //"PAHighMultiplicity1",
//  (1.0/1024.0)*266.7, //"PAHighMultiplicity2",
//  (1.0/1024.0)*266.7, //"PAHighMultiplicity3",
//  (1.0/1024.0)*266.7, //"PAHighMultiplicity4",
//  (1.0/1024.0)*266.7, //"PAHighMultiplicity5",
//  (1.0/1024.0)*266.7, //"PAHighMultiplicity6",
//  (1.0/1024.0)*286.2, //"PAHighMultiplicity7",
//  (1.0/1024.0)*123.5, //"PAMinimumBias1",
//  (1.0/1024.0)*123.5, //"PAMinimumBias2",
//  (1.0/1024.0)*123.5, //"PAMinimumBias3",
//  (1.0/1024.0)*123.5, //"PAMinimumBias4",
//  (1.0/1024.0)*123.5, //"PAMinimumBias5",
//  (1.0/1024.0)*123.5, //"PAMinimumBias6",
//  (1.0/1024.0)*123.5, //"PAMinimumBias7",
//  (1.0/1024.0)*123.5, //"PAMinimumBias8",
//  (1.0/1024.0)*123.5, //"PAMinimumBiasBkg",
//  (1.0/1024.0)*121.9, //"PASingleMuon",
//  (1.0/1024.0)*0    , //"RPCMonitor",
//  (1.0/1024.0)*0    , //"TestEnablesEcalHcal",
//  (1.0/1024.0)*0    , //"TestEnablesEcalHcalDQM",
//  (1.0/1024.0)*0    , //"ZeroBias"
//  (1.0/1024.0)*123.5, //"PAMinimumBias9",
//  (1.0/1024.0)*123.5, //"PAMinimumBias10",
//  (1.0/1024.0)*123.5, //"PAMinimumBias11",
//  (1.0/1024.0)*123.5, //"PAMinimumBias12",
//  (1.0/1024.0)*123.5, //"PAMinimumBias13",
//  (1.0/1024.0)*123.5, //"PAMinimumBias14",
//  (1.0/1024.0)*123.5, //"PAMinimumBias15",
//  (1.0/1024.0)*123.5, //"PAMinimumBias16",
//  (1.0/1024.0)*123.5, //"PAMinimumBias17",
//  (1.0/1024.0)*123.5, //"PAMinimumBias18",
//  (1.0/1024.0)*123.5, //"PAMinimumBias19",
//  (1.0/1024.0)*123.5, //"PAMinimumBia20",
//};

//Vanderbilt
double AllPD_AODSizes[nTotalPDs] = {
  (1.0/1024.0)*0    , //"AlCaLumiPixels",
  (1.0/1024.0)*0    , //"AlCaP0",
  (1.0/1024.0)*0    , //"AlCaPhiSym",
  (1.0/1024.0)*0    , //"EcalLaser",
  (1.0/1024.0)*0    , //"EventDisplay",
  (1.0/1024.0)*0    , //"ExpressPhysicsPA",
  (1.0/1024.0)*0    , //"HLTMonitorPA",
  (1.0/1024.0)*0    , //"HLTPhysics",
  (1.0/1024.0)*0    , //"HcalNZS",
  (1.0/1024.0)*0    , //"OnlineMonitor",
  (1.0/1024.0)*0    , //"PACastor",
  (1.0/1024.0)*81.1 , //"PADTrack1",
  (1.0/1024.0)*72.3 , //"PADTrack2",
  (1.0/1024.0)*98.6 , //"PADoubleMuon",
  (1.0/1024.0)*86.9 , //"PAEGJet1",
  (1.0/1024.0)*8.8  , //"PAEmptyBX",
  (1.0/1024.0)*55.7 , //"PAForward",
  (1.0/1024.0)*106.4, //"PAHighMultiplicity0",
  (1.0/1024.0)*140.6, //"PAHighMultiplicity1",
  (1.0/1024.0)*140.6, //"PAHighMultiplicity2",
  (1.0/1024.0)*140.6, //"PAHighMultiplicity3",
  (1.0/1024.0)*140.6, //"PAHighMultiplicity4",
  (1.0/1024.0)*140.6, //"PAHighMultiplicity5",
  (1.0/1024.0)*140.6, //"PAHighMultiplicity6",
  (1.0/1024.0)*162.1, //"PAHighMultiplicity7",
  (1.0/1024.0)*43.9 , //"PAMinimumBias1",
  (1.0/1024.0)*43.9 , //"PAMinimumBias2",
  (1.0/1024.0)*43.9 , //"PAMinimumBias3",
  (1.0/1024.0)*43.9 , //"PAMinimumBias4",
  (1.0/1024.0)*43.9 , //"PAMinimumBias5",
  (1.0/1024.0)*43.9 , //"PAMinimumBias6",
  (1.0/1024.0)*43.9 , //"PAMinimumBias7",
  (1.0/1024.0)*43.9 , //"PAMinimumBias8",
  (1.0/1024.0)*42.0 , //"PAMinimumBiasBkg",
  (1.0/1024.0)*90.8 , //"PASingleMuon",
  (1.0/1024.0)*0    , //"RPCMonitor",
  (1.0/1024.0)*0    , //"TestEnablesEcalHcal",
  (1.0/1024.0)*0    , //"TestEnablesEcalHcalDQM",
  (1.0/1024.0)*5.9  , //"ZeroBias"
  (1.0/1024.0)*44.9 , //"PAMinimumBias9",
  (1.0/1024.0)*44.9 , //"PAMinimumBias10",
  (1.0/1024.0)*44.9 , //"PAMinimumBias11",
  (1.0/1024.0)*44.9 , //"PAMinimumBias12",
  (1.0/1024.0)*44.9 , //"PAMinimumBias13",
  (1.0/1024.0)*44.9 , //"PAMinimumBias14",
  (1.0/1024.0)*44.9 , //"PAMinimumBias15",
  (1.0/1024.0)*44.9 , //"PAMinimumBias16",
  (1.0/1024.0)*44.9 , //"PAMinimumBias17",
  (1.0/1024.0)*44.9 , //"PAMinimumBias18",
  (1.0/1024.0)*44.9 , //"PAMinimumBias19",
  (1.0/1024.0)*44.9 , //"PAMinimumBias20",
};

double AllPD_RAWSizes[nTotalPDs] = {
  (1.0/1024.0)*0    , //"AlCaLumiPixels",
  (1.0/1024.0)*0    , //"AlCaP0",
  (1.0/1024.0)*0    , //"AlCaPhiSym",
  (1.0/1024.0)*0    , //"EcalLaser",
  (1.0/1024.0)*0    , //"EventDisplay",
  (1.0/1024.0)*0    , //"ExpressPhysicsPA",
  (1.0/1024.0)*0    , //"HLTMonitorPA",
  (1.0/1024.0)*0    , //"HLTPhysics",
  (1.0/1024.0)*0    , //"HcalNZS",
  (1.0/1024.0)*0    , //"OnlineMonitor",
  (1.0/1024.0)*0    , //"PACastor",
  (1.0/1024.0)*178.7, //"PADTrack1",
  (1.0/1024.0)*169.9, //"PADTrack2",
  (1.0/1024.0)*182.6, //"PADoubleMuon",
  (1.0/1024.0)*176.8, //"PAEGJet1",
  (1.0/1024.0)*64.5 , //"PAEmptyBX",
  (1.0/1024.0)*130.9, //"PAForward",
  (1.0/1024.0)*220.7, //"PAHighMultiplicity0",
  (1.0/1024.0)*262.7, //"PAHighMultiplicity1",
  (1.0/1024.0)*262.7, //"PAHighMultiplicity2",
  (1.0/1024.0)*262.7, //"PAHighMultiplicity3",
  (1.0/1024.0)*262.7, //"PAHighMultiplicity4",
  (1.0/1024.0)*262.7, //"PAHighMultiplicity5",
  (1.0/1024.0)*262.7, //"PAHighMultiplicity6",
  (1.0/1024.0)*297.9, //"PAHighMultiplicity7",
  (1.0/1024.0)*125.0, //"PAMinimumBias1",
  (1.0/1024.0)*125.0, //"PAMinimumBias2",
  (1.0/1024.0)*125.0, //"PAMinimumBias3",
  (1.0/1024.0)*125.0, //"PAMinimumBias4",
  (1.0/1024.0)*125.0, //"PAMinimumBias5",
  (1.0/1024.0)*125.0, //"PAMinimumBias6",
  (1.0/1024.0)*125.0, //"PAMinimumBias7",
  (1.0/1024.0)*125.0, //"PAMinimumBias8",
  (1.0/1024.0)*123.0, //"PAMinimumBiasBkg",
  (1.0/1024.0)*173.8, //"PASingleMuon",
  (1.0/1024.0)*0    , //"RPCMonitor",
  (1.0/1024.0)*0    , //"TestEnablesEcalHcal",
  (1.0/1024.0)*0    , //"TestEnablesEcalHcalDQM",
  (1.0/1024.0)*68.4 , //"ZeroBias"
  (1.0/1024.0)*127.9, //"PAMinimumBias9",
  (1.0/1024.0)*127.9, //"PAMinimumBias10",
  (1.0/1024.0)*127.9, //"PAMinimumBias11",
  (1.0/1024.0)*127.9, //"PAMinimumBias12",
  (1.0/1024.0)*127.9, //"PAMinimumBias13",
  (1.0/1024.0)*127.9, //"PAMinimumBias14",
  (1.0/1024.0)*127.9, //"PAMinimumBias15",
  (1.0/1024.0)*127.9, //"PAMinimumBias16",
  (1.0/1024.0)*127.9, //"PAMinimumBias17",
  (1.0/1024.0)*127.9, //"PAMinimumBias18",
  (1.0/1024.0)*127.9, //"PAMinimumBias19",
  (1.0/1024.0)*127.9, //"PAMinimumBia20",
};

double AllPD_SUMSizes[nTotalPDs] = {0.0}; char datatype[10];

void PlotEventsVsTimeFill(int date = 24, int AOD_RAW_SUM = 3, int DATA_BASE=2) //AOD=1, RAW=2, SUM=3; data_base = 2 or 10;
{

  int WEREDONE = 0;

  for(int i=0; i<nTotalPDs; i++){
    cout << AllPDs[i] << "    AOD/RAW =  " << AllPD_AODSizes[i]/AllPD_RAWSizes[i] << endl;
  }
  
  h_Lumi_vs_Fill      = new TH1D("h_Lumi_vs_Fill",     "h_Lumi_vs_Fill     ",151,FirstFill-4.5,FirstFill+146.5);
  h_Lumi_vs_Fill_Int  = new TH1D("h_Lumi_vs_Fill_Int", "h_Lumi_vs_Fill_Int ",151,FirstFill-4.5,FirstFill+146.5);
  h_Lumi_vs_Index     = new TH1D("h_Lumi_vs_Index",    "h_Lumi_vs_Index    ",65,0.5,65.5);
  h_Lumi_vs_Index_Int = new TH1D("h_Lumi_vs_Index_Int","h_Lumi_vs_Index_Int",65,0.5,65.5);

  double basemult = 0.0;
  if(DATA_BASE==2)
    basemult = 1024.0;
  else if(DATA_BASE==10)
    basemult = 1000.0;

  for(int i=0; i<nTotalPDs; i++){
    //h_evtsInt[i] = new TH1D(saythis,saythis,1101,262499.5,263600.5);
    //h_evtsRaw[i] = new TH1D(saythis,saythis,1101,262499.5,263600.5);

  if(DATA_BASE==2){
    AllPD_AODSizes[i] *= 1.0*1.0;//leave it in base-2
    AllPD_RAWSizes[i] *= 1.0*1.0;//leave it in base-2
  }
  else if(DATA_BASE==10){
    AllPD_AODSizes[i] *= 1.024*1.024;//take it back to base-10
    AllPD_RAWSizes[i] *= 1.024*1.024;//take it back to base-10
  }

    if(AOD_RAW_SUM==1){
      AllPD_SUMSizes[i] = AllPD_AODSizes[i];
      sprintf(datatype,"AOD");
      MaxCap = 1300.0;//was 700
      MaxCap *= 1.0/pow(basemult/1000.0,4);
    }
    else if(AOD_RAW_SUM==2){
      AllPD_SUMSizes[i] = AllPD_RAWSizes[i];
      sprintf(datatype,"RAW");
      MaxCap = 2000;//was 1400
      MaxCap *= 1.0/pow(basemult/1000.0,4);
    }
    else if(AOD_RAW_SUM==3){
      AllPD_SUMSizes[i] = AllPD_AODSizes[i] + AllPD_RAWSizes[i];
      sprintf(datatype,"RAW+AOD");
      MaxCap = 3000;//was 2100 ? 
      MaxCap *= 1.0/pow(basemult/1000.0,4);
    }

    sprintf(saythis,"h_evtsInt_%d",i);
    h_evtsInt[i] = new TH1D(saythis,saythis,151,FirstFill-4.5,FirstFill+146.5);
    sprintf(saythis,"h_evtsRaw_%d",i);
    h_evtsRaw[i] = new TH1D(saythis,saythis,151,FirstFill-4.5,FirstFill+146.5);
    sprintf(saythis,"h_sizeInt_%d",i);
    h_sizeInt[i] = new TH1D(saythis,saythis,151,FirstFill-4.5,FirstFill+146.5);
    sprintf(saythis,"h_sizeRaw_%d",i);
    h_sizeRaw[i] = new TH1D(saythis,saythis,151,FirstFill-4.5,FirstFill+146.5);
    sprintf(saythis,"h_sizePerEvtRaw_%d",i);
    h_sizePerEvtRaw[i] = new TH1D(saythis,saythis,151,FirstFill-4.5,FirstFill+146.5);
  }
  for(int i=0; i<6; i++){
    sprintf(saythis,"h_PINGInt_%d",i);
    h_PINGInt[i] = new TH1D(saythis,saythis,151,FirstFill-4.5,FirstFill+146.5);
    sprintf(saythis,"h_PINGEvt_%d",i);
    h_PINGEvt[i] = new TH1D(saythis,saythis,151,FirstFill-4.5,FirstFill+146.5);
    sprintf(saythis,"h_PINGInt_index_%d",i);
    h_PINGInt_index[i] = new TH1D(saythis,saythis,65,0.5,65.5);
    sprintf(saythis,"h_PINGRaw_index_%d",i);
    h_PINGRaw_index[i] = new TH1D(saythis,saythis,65,0.5,65.5);
    sprintf(saythis,"h_PINGEvt_index_%d",i);
    h_PINGEvt_index[i] = new TH1D(saythis,saythis,65,0.5,65.5);
    sprintf(saythis,"h_PINGRawPerEvt_index_%d",i);
    h_PINGRawPerEvt_index[i] = new TH1D(saythis,saythis,65,0.5,65.5);
  }
  h_TotalInt = new TH1D("h_TotalInt","h_TotalInt",151,FirstFill-4.5,FirstFill+146.5);


  ifstream fin;
  char infilename[500];
  //sprintf(infilename,"FILL_11-%d-16.txt",date);
  //sprintf(infilename,"FILL_just%d_extrap.txt",date);
  sprintf(infilename,"textfiles/FILL_%d_extrap.txt",date);
  fin.open(infilename);
  //sprintf(outfile, "11-%d-16_%s",date,datatype);
  sprintf(outfile, "fill%d_extrap_%s_base%d",date,datatype,DATA_BASE);
  //fin.open("FILL_11-22-16.txt");
  //sprintf(outfile, "11-22-16");

  double nbLumi    = 0.0;
  double nbLumiInt = 0.0;
  fin >> junk;
  //while(!fin.eof()){
  while(WEREDONE==0){

    //fin >> PDName;
    fin >> CurrentFillNum;
    //if(CurrentFillNum>4700) CurrentFillNum+=-5;
    RunNumList.push_back(CurrentFillNum);

    for(int i=0; i<8; i++){
      fin >> junk;
      //cout << junk.Data() << endl;
    }
    fin >> junkint;
    //cout << junkint << endl;
    for(int i=0; i<17; i++){
      fin >> junk;
      //cout << junk.Data() << endl;
    }
    fin >> junk;
    //cout << " We found an ....  " << junk << endl;
    if(strcmp(junk.Data(),"Error")==0){
      for(int i=0; i<15; i++){
        fin >> junk;
        cout << junk << endl;
      }
      break;
    }
    fin >> nbLumi >> junk;
    nbLumiInt += nbLumi;
    cout << endl << "Fill # " << CurrentFillNum << "  collected " << nbLumi << " /nb" << endl;

    h_Lumi_vs_Fill      ->SetBinContent(h_Lumi_vs_Fill    ->GetXaxis()->FindBin(CurrentFillNum),nbLumi);
    h_Lumi_vs_Fill_Int  ->SetBinContent(h_Lumi_vs_Fill_Int->GetXaxis()->FindBin(CurrentFillNum),nbLumiInt);

    while(1){
      int KEEPTHISPD = 0;
      fin >> PDName;
      cout << "  PDNAME: " << PDName.Data() << endl;
      for(int i=0; i<nMyPDs; i++)
        if(strcmp(PDName.Data(),MyPDs[i])==0)
          KEEPTHISPD = 1;

      if(strcmp(PDName.Data(),"ENDOFFILEJASON")==0){
        WEREDONE = 1;
        break;
      }
      if(strcmp(PDName.Data(),"NEWJSONRUN")==0)
        break;
      fin >> nEvents;
      cout << PDName.Data() << "  " << nEvents << endl;

      int ThisPDIndex = -1;
      for(int i=0; i<PDNameList.size(); i++)
        if( ((signed int)PDNameList[i].compare(PDName.Data()))==0)
          ThisPDIndex = i;

      if(ThisPDIndex<0){
        ThisPDIndex = PDNameList.size();
        PDNameList.push_back(PDName.Data());
        nEventsList.push_back(nEvents);
        TGraphErrors g1;
        sprintf(saythis,"g1_evts_%d",int(PDNameList.size()-1));
        g1.SetName(saythis);
        GraphEvtsList.push_back(g1);
        sprintf(saythis,"g1_size_%d",int(PDNameList.size()-1));
        g1.SetName(saythis);
        GraphSizeList.push_back(g1);
        sprintf(saythis,"h1_%d",int(PDNameList.size()-1));
        TH1D h1(saythis,saythis,TotalNFills+11,FirstFill-4.5,FirstFill+TotalNFills+6.5);
        HistoList.push_back(h1);
        h1.Delete();
        g1.Delete();

        GraphEvtsList[ThisPDIndex].SetPoint(0,double(CurrentFillNum),double(nEvents));
        GraphSizeList[ThisPDIndex].SetPoint(0,double(CurrentFillNum),double(nEvents)*(AllPD_SUMSizes[ThisPDIndex])/basemult/basemult);
        //nEventsList[ThisPDIndex]+=nEvents;// this was a bug that I found on 2Dec2016 and commented the line :)
        HistoList[ThisPDIndex].Fill(CurrentFillNum,nEventsList[ThisPDIndex]);
        HistoList[ThisPDIndex].SetBinError(HistoList[ThisPDIndex].GetXaxis()->FindBin(CurrentFillNum),sqrt(double(nEvents)));
        h_evtsInt[ThisPDIndex]->Fill(CurrentFillNum,nEventsList[ThisPDIndex]);
        h_evtsInt[ThisPDIndex]->SetBinError(h_evtsInt[ThisPDIndex]->GetXaxis()->FindBin(CurrentFillNum),sqrt(double(nEventsList[ThisPDIndex])));
        h_evtsRaw[ThisPDIndex]->Fill(CurrentFillNum,nEvents);
        h_evtsRaw[ThisPDIndex]->SetBinError(h_evtsRaw[ThisPDIndex]->GetXaxis()->FindBin(CurrentFillNum),sqrt(double(nEvents)));
        cout << "J  " << double(CurrentFillNum) << "   " << "0" << "  nEvents: " << double(nEvents) << " nEvtsList[tPDi]: " << nEventsList[ThisPDIndex] << endl;
        h_sizeInt[ThisPDIndex]->Fill(CurrentFillNum,double(nEvents)*(AllPD_SUMSizes[ThisPDIndex])/basemult/basemult);
        h_sizeInt[ThisPDIndex]->SetBinError(h_evtsRaw[ThisPDIndex]->GetXaxis()->FindBin(CurrentFillNum),0);
        h_sizeRaw[ThisPDIndex]->Fill(CurrentFillNum,double(nEvents)*(AllPD_SUMSizes[ThisPDIndex])/basemult/basemult);
        h_sizeRaw[ThisPDIndex]->SetBinError(h_evtsRaw[ThisPDIndex]->GetXaxis()->FindBin(CurrentFillNum),0);
        h_sizePerEvtRaw[ThisPDIndex]->Fill(CurrentFillNum,double(nEvents)*(AllPD_SUMSizes[ThisPDIndex])/basemult/basemult);
        h_sizePerEvtRaw[ThisPDIndex]->SetBinError(h_evtsRaw[ThisPDIndex]->GetXaxis()->FindBin(CurrentFillNum),0);
      }
      else{
        double myx=0;
        double myy=0;
        nEventsList[ThisPDIndex]+=nEvents;
        GraphEvtsList[ThisPDIndex].GetPoint(GraphEvtsList[ThisPDIndex].GetN()-1,myx,myy);
        GraphEvtsList[ThisPDIndex].SetPoint(GraphEvtsList[ThisPDIndex].GetN(),double(CurrentFillNum),double(nEvents)+myy);
        //HistoList[ThisPDIndex].Fill(CurrentFillNum,double(nEvents)+myy);
        HistoList[ThisPDIndex].Fill(CurrentFillNum,nEventsList[ThisPDIndex]);
        HistoList[ThisPDIndex].SetBinError(HistoList[ThisPDIndex].GetXaxis()->FindBin(CurrentFillNum),sqrt(double(nEventsList[ThisPDIndex])));
        h_evtsInt[ThisPDIndex]->Fill(CurrentFillNum,nEventsList[ThisPDIndex]);
        h_evtsInt[ThisPDIndex]->SetBinError(h_evtsInt[ThisPDIndex]->GetXaxis()->FindBin(CurrentFillNum),sqrt(double(nEventsList[ThisPDIndex])));
        h_evtsRaw[ThisPDIndex]->Fill(CurrentFillNum,nEvents);
        h_evtsRaw[ThisPDIndex]->SetBinError(h_evtsRaw[ThisPDIndex]->GetXaxis()->FindBin(CurrentFillNum),sqrt(double(nEvents)));
        cout << double(CurrentFillNum) << "   " << double(nEvents) << "  " << double(nEvents)+myy << "  " << nEventsList[ThisPDIndex] << endl;
        GraphSizeList[ThisPDIndex].GetPoint(GraphSizeList[ThisPDIndex].GetN()-1,myx,myy);
        GraphSizeList[ThisPDIndex].SetPoint(GraphSizeList[ThisPDIndex].GetN(),double(CurrentFillNum),myy+double(nEvents)*(AllPD_SUMSizes[ThisPDIndex])/basemult/basemult);
        h_sizeInt[ThisPDIndex]->Fill(CurrentFillNum,myy+double(nEvents)*(AllPD_SUMSizes[ThisPDIndex])/basemult/basemult);
        h_sizeInt[ThisPDIndex]->SetBinError(h_evtsRaw[ThisPDIndex]->GetXaxis()->FindBin(CurrentFillNum),0.01);
        h_sizeRaw[ThisPDIndex]->Fill(CurrentFillNum,double(nEvents)*(AllPD_SUMSizes[ThisPDIndex])/basemult/basemult);
        h_sizeRaw[ThisPDIndex]->SetBinError(h_evtsRaw[ThisPDIndex]->GetXaxis()->FindBin(CurrentFillNum),0.01);
      }



    }//take in THIS run. 




  }//end of file

  nFillMore = TotalNFills-RunNumList.size();
  fitRange[0] = float(RunNumList.size())-2.0;
  fitRange[1] = float(RunNumList.size())-2.0+float(nFillMore+5);

  for(int i=0; i<nTotalPDs;i++){
    h_PINGInt[PINGS[i]]->Add(h_sizeInt[i]);
    h_PINGEvt[PINGS[i]]->Add(h_evtsRaw[i]);
  }
  h_PINGInt[0]->Reset();
  for(int i=1; i<6;i++){
    h_TotalInt->Add(h_PINGInt[i]);
    h_PINGInt[0]->Add(h_PINGInt[i]);
  }
  for(int i=0; i<6;i++){
    int binindex = 1;
    for(int j=0; j<h_PINGInt[i]->GetNbinsX()+1; j++){
      float bincont = h_PINGInt[i]->GetBinContent(j);
      float evtcont = h_PINGEvt[i]->GetBinContent(j);
      if(bincont>0){
        h_PINGInt_index[i]->SetBinContent(binindex,bincont); 
        h_PINGInt_index[i]->SetBinError  (binindex,0.001); 
        h_PINGEvt_index[i]->SetBinContent(binindex,evtcont); 
        h_PINGEvt_index[i]->SetBinError  (binindex,0.001); 
        //if(binindex>0){
        //  h_PINGRaw_index[i]->SetBinContent(binindex,(bincont-h_PINGInt_index[i]->GetBinContent(binindex-1))/h_PINGRaw_index[0]->GetBinContent(binindex)); 
        //  h_PINGRaw_index[i]->SetBinContent(binindex,(h_PINGRaw_index[i]->GetBinContent(binindex))/h_PINGRaw_index[0]->GetBinContent(binindex)); 
        //  h_PINGRaw_index[i]->SetBinError  (binindex,0.001); 
        //}
        binindex++;
      }
    }
  }

  for(int i=0; i<6;i++){
    for(int j=0; j<h_PINGInt_index[i]->GetNbinsX()+1; j++){
      float bincont = h_PINGInt_index[i]->GetBinContent(j);
      float evtcont = h_PINGEvt_index[i]->GetBinContent(j);
      if(j>1 && (bincont-h_PINGInt_index[i]->GetBinContent(j-1)>0.00001)){
        h_PINGRaw_index[i]      ->SetBinContent(j,(h_PINGInt_index[i]->GetBinContent(j)-h_PINGInt_index[i]->GetBinContent(j-1))); 
        h_PINGRaw_index[i]      ->SetBinError  (j,0.001); 
        h_PINGRawPerEvt_index[i]->SetBinContent(j,basemult*basemult*h_PINGRaw_index[i]->GetBinContent(j)/evtcont); 
        h_PINGRawPerEvt_index[i]->SetBinError  (j,0.00001); 
        cout << i << " " << j << "  " << basemult*basemult*h_PINGRaw_index[i]->GetBinContent(j) << "  " << evtcont << endl;
        if(i>0){
          h_PINGRaw_index[i]->SetBinContent(j,(h_PINGRaw_index[i]->GetBinContent(j))/h_PINGRaw_index[0]->GetBinContent(j)); 
          h_PINGRaw_index[i]->SetBinError  (j,0.001); 
        }
        //cout << i << " " << j << "  " << h_PINGRaw_index[i]->GetBinContent(j) << endl;
      }
    }
  }




  gStyle->SetOptStat(0);
  TCanvas *c1 = new TCanvas("c1","c1",1200,500);
  c1->Divide(2,1);
  c1->cd(1)->SetPad(0.01,0.01,0.7,0.99);
  c1->cd(1)->SetLogy();
  TH1D *h_frame = new TH1D("h_frame","h_frame",RunNumList[RunNumList.size()-1]+15,-5,RunNumList[RunNumList.size()-1]+10);
  //TH1D *h_frame = new TH1D("h_frame","h_frame",FirstFill+TotalNFills+10,-5,FirstFill+TotalNFills+10);
  h_frame->GetXaxis()->SetTitle("fill number");
  h_frame->SetTitle("Total Events Recorded per PD");
  h_frame->GetYaxis()->SetTitle("integrated nEvents");
  //h_frame->GetXaxis()->SetRangeUser(FirstFill-4.5,FirstFill+TotalNFills+5.5);
  h_frame->GetXaxis()->SetRangeUser(FirstFill-4.5,RunNumList[RunNumList.size()-1]+10);
  h_frame->GetYaxis()->SetRangeUser(1e3,5.0e9);
  h_frame->DrawCopy();

  TLegend *leg1 = new TLegend(0.01,0.01,0.99,0.99);
  leg1->SetFillColor(0);
  leg1->SetBorderSize(0);

  for(int i=0; i<PDNameList.size(); i++){
    cout << i << "   " << PDNameList[i] << "  " << nEventsList[i] << endl;
    GraphEvtsList[i].SetMarkerColor(COLORS[PINGS[i]]);
    GraphEvtsList[i].SetTitle(AllPDs[i]);
    //GraphEvtsList[i].SetName(AllPDs[i]);
    GraphEvtsList[i].SetMarkerStyle(MARKS[i]);

    if(PINGS[i]!=0){
      GraphEvtsList[i].Draw("P");
      leg1->AddEntry(&GraphEvtsList[i],AllPDs[i],"P");
    }
  }
  c1->cd(2)->SetPad(0.7,0.01,0.99,0.99);
  leg1->Draw();
  sprintf(saythis,"predictions/c1_integrated_events_pd_%s.gif",outfile);
  if(SAVEMYSHIT==1) c1->SaveAs(saythis);


  gStyle->SetOptStat(0);
  TCanvas *c2 = new TCanvas("c2","c2",1200,500);
  c2->Divide(2,1);
  c2->cd(1)->SetPad(0.01,0.01,0.7,0.99);
  c2->cd(1)->SetLogy();
  h_frame->SetTitle("Total Data Recorded per PD");
  h_frame->GetXaxis()->SetTitle("fill number");
  sprintf(saythis,"size of %s (TB, base-%d)",datatype,DATA_BASE);
  h_frame->GetYaxis()->SetTitle(saythis);
  //h_frame->GetXaxis()->SetRangeUser(FirstFill-4.5,FirstFill+TotalNFills+5.5);
  h_frame->GetXaxis()->SetRangeUser(FirstFill-4.5,RunNumList[RunNumList.size()-1]+10);
  h_frame->GetYaxis()->SetRangeUser(0.1,1e3);
  h_frame->DrawCopy();

  TLegend *leg2 = new TLegend(0.01,0.01,0.99,0.99);
  leg2->SetFillColor(0);
  leg2->SetBorderSize(0);

  for(int i=0; i<PDNameList.size(); i++){
    cout << i << "   " << PDNameList[i] << "  " << nEventsList[i] << endl;
    GraphSizeList[i].SetMarkerColor(COLORS[PINGS[i]]);
    GraphSizeList[i].SetTitle(AllPDs[i]);
    //GraphSizeList[i].SetName(AllPDs[i]);
    GraphSizeList[i].SetMarkerStyle(MARKS[i]);

    if(PINGS[i]!=0){
      GraphSizeList[i].Draw("P");
      leg2->AddEntry(&GraphSizeList[i],AllPDs[i],"P");
    }
  }
  c2->cd(2)->SetPad(0.7,0.01,0.99,0.99);
  leg2->Draw();
  sprintf(saythis,"predictions/c2_integrated_size_pd_%s.gif",outfile);
  if(SAVEMYSHIT==1) c2->SaveAs(saythis);



  TCanvas *c3 = new TCanvas("c3","c3",1200,500);
  c3->Divide(2,1);
  c3->cd(1)->SetPad(0.01,0.01,0.7,0.99);
  c3->cd(1)->SetLogy();
  h_frame->GetYaxis()->SetRangeUser(1e3,5.0e9);
  h_frame->GetXaxis()->SetTitle("fill number");
  h_frame->SetTitle("Total Events Recorded per PD");
  h_frame->GetYaxis()->SetTitle("integrated nEvents");
  h_frame->DrawCopy();

  TLegend *leg3 = new TLegend(0.01,0.01,0.99,0.99);
  leg3->SetFillColor(0);
  leg3->SetBorderSize(0);

  for(int i=0; i<PDNameList.size(); i++){
    cout << i << "   " << PDNameList[i] << endl;
    h_evtsInt[i]->SetMarkerColor(COLORS[PINGS[i]]);
    h_evtsInt[i]->SetTitle(AllPDs[i]);
    h_evtsInt[i]->SetMarkerStyle(MARKS[i]);

    if(PINGS[i]!=0){
      h_evtsInt[i]->Draw("same, P");
      leg3->AddEntry(h_evtsInt[i],AllPDs[i],"P");
    }
  }
  c3->cd(2)->SetPad(0.7,0.01,0.99,0.99);
  leg3->Draw();



  TF1 *myPol1_max[6];
  TF1 *myPol1_min[6];
  TF1 *myPol1[6];
  for(int i=0; i<6; i++){
    sprintf(saythis,"myPol1_max_%d",i);
    //myPol1_max[i] = new TF1(saythis,"[0]+x*[1]",FirstFill-5,FirstFill+TotalNFills+10); myPol1_max[i]->SetParameters(0,0);
    myPol1_max[i] = new TF1(saythis,"[0]+x*[1]",0,TotalNFills+10); myPol1_max[i]->SetParameters(0,0);
    sprintf(saythis,"myPol1_min_%d",i);
    //myPol1_min[i] = new TF1(saythis,"[0]+x*[1]",FirstFill-5,FirstFill+TotalNFills+10); myPol1_min[i]->SetParameters(1e6,1e6);
    myPol1_min[i] = new TF1(saythis,"[0]+x*[1]",0,TotalNFills+10); myPol1_min[i]->SetParameters(1e6,1e6);
    sprintf(saythis,"myPol1_%d",i);
    //myPol1[i] = new TF1(saythis,"[0]+x*[1]",FirstFill-5,FirstFill+TotalNFills+10);
    myPol1[i] = new TF1(saythis,"[0]+x*[1]",0,TotalNFills+10);
    myPol1[i]->SetLineColor(COLORS[i]);
    myPol1[i]->SetLineColorAlpha(COLORS[i],0.01);
    myPol1[i]->SetLineStyle(0);
    //h_PINGInt[i]->Fit(saythis,"+","",fitRange[0],fitRange[1]);
    //myPol1[i]->SetLineStyle(3);
    //h_PINGInt[i]->Fit(saythis,"+","",fitRange[0]-1,fitRange[1]);
    //h_PINGInt[i]->Fit(saythis,"+","",fitRange[0]-2,fitRange[1]);
    //h_PINGInt[i]->Fit(saythis,"+","",fitRange[0]-3,fitRange[1]);
    //h_PINGInt[i]->Fit(saythis,"+","",fitRange[0]-4,fitRange[1]);
    //h_PINGInt[i]->Fit(saythis,"+","",fitRange[0]-5,fitRange[1]);
    //h_PINGInt[i]->Fit(saythis,"+","",fitRange[0]-6,fitRange[1]);

    h_PINGInt_index[i]->Fit(saythis,"+","",fitRange[0],fitRange[1]);
    if(myPol1[i]->Eval(TotalNFills)>myPol1_max[i]->Eval(TotalNFills))  myPol1_max[i]->SetParameters(myPol1[i]->GetParameter(0),myPol1[i]->GetParameter(1));
    if(myPol1[i]->Eval(TotalNFills)<myPol1_min[i]->Eval(TotalNFills))  myPol1_min[i]->SetParameters(myPol1[i]->GetParameter(0),myPol1[i]->GetParameter(1));
    myPol1[i]->SetLineStyle(3);
    h_PINGInt_index[i]->Fit(saythis,"+","",fitRange[0]-1,fitRange[1]);
    if(myPol1[i]->Eval(TotalNFills)>myPol1_max[i]->Eval(TotalNFills))  myPol1_max[i]->SetParameters(myPol1[i]->GetParameter(0),myPol1[i]->GetParameter(1));
    if(myPol1[i]->Eval(TotalNFills)<myPol1_min[i]->Eval(TotalNFills))  myPol1_min[i]->SetParameters(myPol1[i]->GetParameter(0),myPol1[i]->GetParameter(1));
    h_PINGInt_index[i]->Fit(saythis,"+","",fitRange[0]-2,fitRange[1]);
    if(myPol1[i]->Eval(TotalNFills)>myPol1_max[i]->Eval(TotalNFills))  myPol1_max[i]->SetParameters(myPol1[i]->GetParameter(0),myPol1[i]->GetParameter(1));
    if(myPol1[i]->Eval(TotalNFills)<myPol1_min[i]->Eval(TotalNFills))  myPol1_min[i]->SetParameters(myPol1[i]->GetParameter(0),myPol1[i]->GetParameter(1));
    h_PINGInt_index[i]->Fit(saythis,"+","",fitRange[0]-3,fitRange[1]);
    if(myPol1[i]->Eval(TotalNFills)>myPol1_max[i]->Eval(TotalNFills))  myPol1_max[i]->SetParameters(myPol1[i]->GetParameter(0),myPol1[i]->GetParameter(1));
    if(myPol1[i]->Eval(TotalNFills)<myPol1_min[i]->Eval(TotalNFills))  myPol1_min[i]->SetParameters(myPol1[i]->GetParameter(0),myPol1[i]->GetParameter(1));
    h_PINGInt_index[i]->Fit(saythis,"+","",fitRange[0]-4,fitRange[1]);
    if(myPol1[i]->Eval(TotalNFills)>myPol1_max[i]->Eval(TotalNFills))  myPol1_max[i]->SetParameters(myPol1[i]->GetParameter(0),myPol1[i]->GetParameter(1));
    if(myPol1[i]->Eval(TotalNFills)<myPol1_min[i]->Eval(TotalNFills))  myPol1_min[i]->SetParameters(myPol1[i]->GetParameter(0),myPol1[i]->GetParameter(1));
    h_PINGInt_index[i]->Fit(saythis,"+","",fitRange[0]-5,fitRange[1]);
    if(myPol1[i]->Eval(TotalNFills)>myPol1_max[i]->Eval(TotalNFills))  myPol1_max[i]->SetParameters(myPol1[i]->GetParameter(0),myPol1[i]->GetParameter(1));
    if(myPol1[i]->Eval(TotalNFills)<myPol1_min[i]->Eval(TotalNFills))  myPol1_min[i]->SetParameters(myPol1[i]->GetParameter(0),myPol1[i]->GetParameter(1));
    h_PINGInt_index[i]->Fit(saythis,"+","",fitRange[0]-6,fitRange[1]);
    if(myPol1[i]->Eval(TotalNFills)>myPol1_max[i]->Eval(TotalNFills))  myPol1_max[i]->SetParameters(myPol1[i]->GetParameter(0),myPol1[i]->GetParameter(1));
    if(myPol1[i]->Eval(TotalNFills)<myPol1_min[i]->Eval(TotalNFills))  myPol1_min[i]->SetParameters(myPol1[i]->GetParameter(0),myPol1[i]->GetParameter(1));
    h_PINGInt_index[i]->Fit(saythis,"+","",fitRange[0]-7,fitRange[1]);
    if(myPol1[i]->Eval(TotalNFills)>myPol1_max[i]->Eval(TotalNFills))  myPol1_max[i]->SetParameters(myPol1[i]->GetParameter(0),myPol1[i]->GetParameter(1));
    if(myPol1[i]->Eval(TotalNFills)<myPol1_min[i]->Eval(TotalNFills))  myPol1_min[i]->SetParameters(myPol1[i]->GetParameter(0),myPol1[i]->GetParameter(1));
    h_PINGInt_index[i]->Fit(saythis,"+","",fitRange[0]-8,fitRange[1]);
    if(myPol1[i]->Eval(TotalNFills)>myPol1_max[i]->Eval(TotalNFills))  myPol1_max[i]->SetParameters(myPol1[i]->GetParameter(0),myPol1[i]->GetParameter(1));
    if(myPol1[i]->Eval(TotalNFills)<myPol1_min[i]->Eval(TotalNFills))  myPol1_min[i]->SetParameters(myPol1[i]->GetParameter(0),myPol1[i]->GetParameter(1));

    sprintf(saythis,"h_PINGInt_index_errors_%d",i);
    h_PINGInt_index_error[i] = new TH1D(saythis,saythis,10000,0.5,65.5);
    h_PINGInt_index_error[i] ->Sumw2();
    for(int j=0; j<h_PINGInt_index_error[i]->GetNbinsX()+1; j++){
      float bincenter = h_PINGInt_index_error[i]->GetBinCenter(j);
      float newbincont = 0.5*(myPol1_min[i]->Eval(bincenter)+myPol1_max[i]->Eval(bincenter));
      float newbinerr  = 0.5*(myPol1_min[i]->Eval(bincenter)-myPol1_max[i]->Eval(bincenter));
      if(bincenter>=RunNumList.size()){
        h_PINGInt_index_error[i]->SetBinContent(j,newbincont);
        h_PINGInt_index_error[i]->SetBinError  (j,newbinerr);
      }
    }

  }
  c3->cd(2)->SetPad(0.7,0.01,0.99,0.99);
  leg3->Draw();



  TCanvas *c4 = new TCanvas("c4","c4",1200,500);
  //c4->SetPadTickY(1);
  c4->SetTicks(1,2);
  //c4->cd(1)->SetLogy();
  //h_frame->GetXaxis()->SetRangeUser(4620,4750);
  //h_frame->GetYaxis()->SetRangeUser(1e-1,2000);
  h_frame->GetXaxis()->SetRangeUser(FirstFill-4.5,RunNumList[RunNumList.size()-1]+10);
  h_frame->GetYaxis()->SetRangeUser(0,1.3*MaxCap);
  sprintf(saythis,"Data Size Estimation for %s",datatype);
  h_frame->SetTitle(saythis);
  sprintf(saythis,"size of %s (TB, base-%d)",datatype,DATA_BASE);
  h_frame->GetYaxis()->SetTitle(saythis);
  h_frame->GetXaxis()->SetTitle("Fill Number");
  h_frame->DrawCopy();

  TLegend *leg4 = new TLegend(0.12,0.5,0.30,0.86);
  leg4->SetFillColor(0);
  leg4->SetBorderSize(0);

  //h_PINGInt[0]->Draw("same");
  for(int i=0; i<6; i++){
    h_PINGInt[0]->SetMarkerStyle(24);
    h_PINGInt[0]->SetMarkerColor(1);
    h_PINGInt[i]->SetMarkerColor(COLORS[i]);
    h_PINGInt[i]->SetTitle(AllPings[i]);
    h_PINGInt[i]->SetMarkerStyle(20);

    h_TotalInt->SetMarkerStyle(24);
    h_TotalInt->SetMarkerColor(1);

    h_PINGInt[i]->Draw("same, P");
    //myPol1[i]   ->Draw("same");
    leg4->AddEntry(h_PINGInt[i],AllPings[i],"P");
  }
  TLine *lineTotalNFills = new TLine(4700+nFillMore,0,4700+nFillMore,MaxCap);
  lineTotalNFills->SetLineColor(12);
  lineTotalNFills->SetLineStyle(2 );
  lineTotalNFills->Draw("same");
  h_TotalInt->Draw("same");
  leg4->Draw();


  TCanvas *c5 = new TCanvas("c5","c5",1200,500);
  //c5->SetPadTickY(1);
  c5->SetTicks(1,2);
  //c5->cd(1)->SetLogy();
  //h_frame->GetXaxis()->SetRangeUser(4620,4750);
  //h_frame->GetYaxis()->SetRangeUser(1e-1,2000);
  h_frame->GetXaxis()->SetRangeUser(0,TotalNFills+5);
  h_frame->GetYaxis()->SetRangeUser(0,1.3*MaxCap);
  sprintf(saythis,"Data Size Estimation for %s",datatype);
  h_frame->SetTitle(saythis);
  sprintf(saythis,"size of %s (TB, base-%d)",datatype,DATA_BASE);
  h_frame->GetYaxis()->SetTitle(saythis);
  h_frame->GetXaxis()->SetTitle("Fill Index");
  h_frame->DrawCopy();

  //TLegend *leg5 = new TLegend(0.12,0.5,0.30,0.86);
  TLegend *leg5 = new TLegend(0.12,0.30,0.30,0.70);
  leg5->SetFillColor(0);
  leg5->SetBorderSize(0);

  //h_PINGInt[0]->Draw("same");
  for(int i=0; i<6; i++){
    h_PINGInt_index[0]->SetMarkerStyle(24);
    h_PINGInt_index[0]->SetMarkerColor(1);
    h_PINGInt_index[i]->SetMarkerColor(COLORS[i]);
    h_PINGInt_index[i]->SetTitle(AllPings[i]);
    h_PINGInt_index[i]->SetMarkerStyle(20);
    h_PINGInt_index[i]->SetMarkerSize (1.25);

    h_PINGInt_index[i]->Draw("same, P");
    h_PINGInt_index_error[i]->SetFillColorAlpha(COLORS[i],0.2);
    h_PINGInt_index_error[i]->SetMarkerColorAlpha(0,0.0);
    //h_PINGInt_index_error[i]->Draw("same,pE4");
    leg5->AddEntry(h_PINGInt_index[i],AllPings[i],"P");
  }
  TLine *lineTotalNFills_index[20];
  for(int i=0; i<nFillMore+2; i++){
    lineTotalNFills_index[i] = new TLine(RunNumList.size()+i,0,RunNumList.size()+i,1.3*MaxCap);
    if(i==nFillMore)  lineTotalNFills_index[i]->SetLineColor(1);
    else              lineTotalNFills_index[i]->SetLineColor(16);
    lineTotalNFills_index[i]->SetLineStyle(2 );
    lineTotalNFills_index[i]->Draw("same");
  }
  //TLine *lineTotalNFills_index = new TLine(28,0,28,1800);
  //lineTotalNFills_index->SetLineColor(1);
  //lineTotalNFills_index->SetLineColor(16);
  //lineTotalNFills_index->Draw("same");

  TLine *lineMaxCap = new TLine(0,MaxCap,TotalNFills+5,MaxCap);
  lineMaxCap->SetLineStyle(2);
  lineMaxCap->SetLineColor(kRed-4);
  lineMaxCap->Draw("same");
  leg5->Draw();
  sprintf(saythis,"predictions/c5/c5_%s.gif",outfile);
  if(SAVEMYSHIT==1) c5->SaveAs(saythis);
  sprintf(saythis,"predictions/c5/c5_%s.pdf",outfile);
  if(SAVEMYSHIT==1) c5->SaveAs(saythis);


  TCanvas *c6 = new TCanvas("c6","c6",1200,500);
  //c6->SetPadTickY(1);
  c6->SetTicks(1,2);
  //c6->cd(1)->SetLogy();
  //h_frame->GetXaxis()->SetRangeUser(4620,4750);
  //h_frame->GetYaxis()->SetRangeUser(1e-1,2000);
  h_frame->GetXaxis()->SetRangeUser(0,TotalNFills+5);
  h_frame->GetYaxis()->SetRangeUser(0,1.1);
  h_frame->SetTitle("Data Write Estimation");
  h_frame->GetYaxis()->SetTitle("fracation of total written to disk");
  h_frame->GetXaxis()->SetTitle("Fill Index");
  h_frame->DrawCopy();

  //TLegend *leg6 = new TLegend(0.12,0.5,0.30,0.86);
  TLegend *leg6 = new TLegend(0.69,0.45,0.86,0.83);
  leg6->SetFillColor(0);
  leg6->SetBorderSize(0);

  //h_PINGRaw[0]->Draw("same");
  for(int i=1; i<6; i++){
    h_PINGRaw_index[0]->SetMarkerStyle(24);
    h_PINGRaw_index[0]->SetMarkerColor(1);
    h_PINGRaw_index[i]->SetMarkerColor(COLORS[i]);
    h_PINGRaw_index[i]->SetTitle(AllPings[i]);
    h_PINGRaw_index[i]->SetMarkerStyle(20);
    h_PINGRaw_index[i]->SetMarkerSize (1.25);

    h_PINGRaw_index[i]->Draw("same, P");
    //myPol1[i]   ->Draw("same");
    leg6->AddEntry(h_PINGRaw_index[i],AllPings[i],"P");
  }
  leg6->Draw();
  sprintf(saythis,"predictions/c6_fraction_written_ping_%s.gif",outfile);
  if(SAVEMYSHIT==1) c6->SaveAs(saythis);


  TCanvas *c7 = new TCanvas("c7","c7",1200,500);
  //c7->SetPadTickY(1);
  c7->SetTicks(1,2);
  //c7->cd(1)->SetLogy();
  //h_frame->GetXaxis()->SetRangeUser(4620,4750);
  //h_frame->GetYaxis()->SetRangeUser(1e-1,2000);
  h_frame->GetXaxis()->SetRangeUser(0,TotalNFills+5);
  h_frame->GetYaxis()->SetRangeUser(0,0.5);
  sprintf(saythis,"Data Size per Event for %s",datatype);
  h_frame->SetTitle(saythis);
  sprintf(saythis,"MB/evt per PING (base-%d)",DATA_BASE);
  h_frame->GetYaxis()->SetTitle(saythis);
  h_frame->GetXaxis()->SetTitle("Fill Index");
  h_frame->DrawCopy();

  //TLegend *leg7 = new TLegend(0.12,0.5,0.30,0.86);
  TLegend *leg7 = new TLegend(0.72,0.45,0.87,0.83);
  leg7->SetFillColor(0);
  leg7->SetBorderSize(0);

  //h_PINGRaw[0]->Draw("same");
  for(int i=1; i<6; i++){
    h_PINGRawPerEvt_index[0]->SetMarkerStyle(24);
    h_PINGRawPerEvt_index[0]->SetMarkerColor(1);
    h_PINGRawPerEvt_index[i]->SetMarkerColor(COLORS[i]);
    h_PINGRawPerEvt_index[i]->SetTitle(AllPings[i]);
    h_PINGRawPerEvt_index[i]->SetMarkerStyle(20);
    h_PINGRawPerEvt_index[i]->SetMarkerSize (1.25);

    h_PINGRawPerEvt_index[i]->Draw("same, P");
    leg7->AddEntry(h_PINGRaw_index[i],AllPings[i],"P");
  }
  leg7->Draw();
  sprintf(saythis,"predictions/c7_data_size_per_event_ping_%s.gif",outfile);
  if(SAVEMYSHIT==1) c7->SaveAs(saythis);


  TCanvas *c8 = new TCanvas("c8","c8",1200,500);
  //c8->SetPadTickY(1);
  c8->SetTicks(1,2);
  c8->cd(1)->SetLogy();
  h_frame->GetXaxis()->SetRangeUser(0,TotalNFills+5);
  h_frame->GetYaxis()->SetRangeUser(1e3,1e10);
  h_frame->SetTitle("Events per PING per Fill");
  h_frame->GetYaxis()->SetTitle("recorded evts per Fill");
  h_frame->GetXaxis()->SetTitle("Fill Index");
  h_frame->DrawCopy();

  //TLegend *leg8 = new TLegend(0.12,0.5,0.30,0.86);
  //TLegend *leg8 = new TLegend(0.72,0.45,0.87,0.83);
  TLegend *leg8 = new TLegend(0.74,0.14,0.89,0.50);
  leg8->SetFillColor(0);
  leg8->SetBorderSize(0);

  //h_PINGRaw[0]->Draw("same");
  for(int i=0; i<6; i++){
    h_PINGEvt_index[0]->SetMarkerStyle(24);
    h_PINGEvt_index[0]->SetMarkerColor(1);
    h_PINGEvt_index[i]->SetMarkerColor(COLORS[i]);
    h_PINGEvt_index[i]->SetTitle(AllPings[i]);
    h_PINGEvt_index[i]->SetMarkerStyle(20);
    h_PINGEvt_index[i]->SetMarkerSize (1.25);
    
    h_PINGEvt_index[i]->Draw("same, P");
    leg8->AddEntry(h_PINGRaw_index[i],AllPings[i],"P");
  }
  leg8->Draw();
  sprintf(saythis,"predictions/c8_events_per_ping_per_fill_%s.gif",outfile);
  if(SAVEMYSHIT==1) c8->SaveAs(saythis);


  //TCanvas *c9 = new TCanvas("c9","c9",1200,500);
  TCanvas *c9 = new TCanvas("c9","c9",800,800);
  //c9->SetPadTickY(1);
  c9->SetTicks(1,2);
  c9->cd(1);
  //h_frame->GetXaxis()->SetRangeUser(0,TotalNFills+5);
  h_frame->GetXaxis()->SetRangeUser(FirstFill-4.5,RunNumList[RunNumList.size()-1]+10);
  h_frame->GetYaxis()->SetRangeUser(0,220);
  h_frame->GetYaxis()->SetTitleOffset(1.4);
  h_frame->SetTitle("Integrated Luminosity");
  h_frame->GetYaxis()->SetTitle("integrated lumi [nb^{-1}]");
  h_frame->GetXaxis()->SetTitle("Fill");
  h_frame->DrawCopy();

  TLegend *leg9 = new TLegend(0.17,0.63,0.34,0.75);
  leg9->SetFillColor(0);
  leg9->SetBorderSize(0);

  for(int i=0; i<h_Lumi_vs_Fill_Int->GetNbinsX()+1; i++){
    if(h_Lumi_vs_Fill_Int->GetBinContent(i)==0)
      h_Lumi_vs_Fill_Int->SetBinContent(i,h_Lumi_vs_Fill_Int->GetBinContent(i-1));
    h_Lumi_vs_Fill_Int->SetBinError(i,0);
    h_Lumi_vs_Fill    ->SetBinError(i,0);
  }
  h_Lumi_vs_Fill    ->SetMarkerStyle(24);
  h_Lumi_vs_Fill_Int->SetFillColor  (41);
  h_Lumi_vs_Fill    ->SetMarkerColor(2);
  h_Lumi_vs_Fill_Int->SetMarkerColor(4);
  h_Lumi_vs_Fill    ->SetLineColor  (2);
  h_Lumi_vs_Fill_Int->SetLineColor  (4);
  h_Lumi_vs_Fill_Int->Draw("same,f");
  h_Lumi_vs_Fill    ->Draw("same,p");

  leg9->AddEntry(h_Lumi_vs_Fill    ,"per fill", "P");
  sprintf(saythis,"whole run (%2.2f /nb)",h_Lumi_vs_Fill_Int->GetBinContent(h_Lumi_vs_Fill_Int->GetNbinsX()));
  leg9->AddEntry(h_Lumi_vs_Fill_Int,saythis,"F");

  leg9->Draw();
  sprintf(saythis,"predictions/c9_int_lumi_per_fill_%s.gif",outfile);
  if(SAVEMYSHIT==1) c9->SaveAs(saythis);


  cout << endl << endl;
  double RunningCountAllEvts = 0.0;

  for(int k=0; k<RunNumList.size(); k++){
    RunningCountAllEvts = 0.0;
    for(int i=0; i<PDNameList.size(); i++){
      for(int j=0; j<nMyPDs; j++){
        //PDName = MyPDs[j];
        string tmp = MyPDs[j];
        if( ((signed int)PDNameList[i].compare(tmp))==0){
          cout << MyPDs[j]             << "  "; 
          cout << AllPD_AODSizes[i]    << "  ";
          cout << AllPD_RAWSizes[i]    << "  ";
          cout << RunNumList[k]        << "  ";
          cout << h_evtsRaw[i]->GetBinContent(h_evtsRaw[i]->GetXaxis()->FindBin(RunNumList[k])) << "  ";
          cout << h_evtsInt[i]->GetBinContent(h_evtsInt[i]->GetXaxis()->FindBin(RunNumList[k])) << "  ";
          cout << h_sizeInt[i]->GetBinContent(h_sizeInt[i]->GetXaxis()->FindBin(RunNumList[k])) << endl;
          RunningCountAllEvts += h_evtsInt[i]->GetBinContent(h_evtsInt[i]->GetXaxis()->FindBin(RunNumList[k]));
        }
      }
    }
    cout << "TotalEvents: (integrating PDs, somehow this one's right)" << RunningCountAllEvts << endl;
    cout << endl;
  }
  cout << endl << endl;

  cout << "We have  " << RunNumList.size() << "  fills." << endl;

  double long TOTALEVENTS = 0.0;
  for(int i=0; i<h_PINGEvt_index[0]->GetNbinsX()+1; i++){
    TOTALEVENTS += h_PINGEvt_index[0]->GetBinContent(i);
  }
  cout << "We have a total of " << TOTALEVENTS << "  events (integrating PINGS) with " << h_Lumi_vs_Fill_Int->GetBinContent(h_Lumi_vs_Fill_Int->GetNbinsX()) << " /nb integrated luminosity." << endl << endl;

}

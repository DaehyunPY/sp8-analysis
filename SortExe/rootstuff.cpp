#include "rootstuff.h"
//#include "TApplication.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3D.h"
#include "TFile.h"
#include "TTree.h"
#include "TColor.h"
#include "TStyle.h"
//#include "TMinuit.h"

//#include "TBrowser.h"
//#include "TTree.h"

//#include "afx.h"
//#include "TKey.h" //Must be after afx include since they both have a GetClassName member!



////////////////////////////////////////////////////////////////
//For ROOT//////////////////////////////////////////////////////
#include "TROOT.h"
//extern void InitGui();
//VoidFuncPtr_t initfuncs[] = { InitGui, 0 };
//TROOT root("rint", "The ROOT Interactive Interface", initfuncs);
////////////////////////////////////////////////////////////////



rootstuff::rootstuff()
{
}



rootstuff::~rootstuff()
{
}




TFile * rootstuff::RecreateRootFile(char *name,char *comment)
{
    TFile * m_RootFile = new TFile(name,"RECREATE",comment);
	return m_RootFile;
}


void rootstuff::SetGreyScale(int number_of_contours)
{
	//remove every color above 228 because otherwise this function cannot be called very often//
	TColor *color;
	Int_t highestIndex = 228;

	TIter next(gROOT->GetListOfColors());
	TSeqCollection * coltab = gROOT->GetListOfColors();
	while ((color = (TColor *) next()))
	{
		if (color->GetNumber() > highestIndex)
		{
			coltab->Remove(color);
			delete color;
		}
	}
	
	//grayscale
	UInt_t Number = 2;
	Double_t Red[2]   = { 0.85, 0.15};
	Double_t Green[2] = { 0.85, 0.15};
	Double_t Blue[2]  = { 0.85, 0.15};
	Double_t Stops[2] = { 0.0, 1.0};
	TColor::CreateGradientColorTable(Number,Stops,Red,Green,Blue,number_of_contours);
	gStyle->SetNumberContours(number_of_contours);
}



void rootstuff::color(int colpal, int number_of_contours)
{ 
	//remove every color above 228 because otherwise this function cannot be called very often//
	TColor *color;
	Int_t highestIndex = 228;

	TIter next(gROOT->GetListOfColors());
	TSeqCollection * coltab = gROOT->GetListOfColors();
	while ((color = (TColor *) next()))
	{
		if (color->GetNumber() > highestIndex)
		{
			coltab->Remove(color);
			delete color; color = 0;
		}
	}
	
	if (colpal == 0) {
		//grayscale
		UInt_t Number = 2;
		Double_t Red[2]   = { 0.85, 0.15};
		Double_t Green[2] = { 0.85, 0.15};
		Double_t Blue[2]  = { 0.85, 0.15};
		Double_t Stops[2] = { 0.0, 1.0};
		TColor::CreateGradientColorTable(Number,Stops,Red,Green,Blue,number_of_contours);
		gStyle->SetNumberContours(number_of_contours);
	}

	if (colpal == 1) {
		//standart cobold palette
		UInt_t Number = 7;
		Double_t Red[7]   = { 0.00, 0.00, 0.00, 0.00, 0.65, 0.99, 0.99};
		Double_t Green[7] = { 0.00, 0.00, 0.75, 0.99, 0.00, 0.00, 0.99};
		Double_t Blue[7]  = { 0.33, 0.99, 0.75, 0.00, 0.65, 0.00, 0.00};
		Double_t Stops[7] = { 0.00, 0.10, 0.30, 0.45, 0.70, 0.80, 1.00};
		TColor::CreateGradientColorTable(Number,Stops,Red,Green,Blue,number_of_contours);
		gStyle->SetNumberContours(number_of_contours);
	}

	if (colpal == 2) {
		//tills palette
		UInt_t Number = 4;
		Double_t Red[4]   = { 0.50, 0.00, 0.99, 0.99};
		Double_t Green[4] = { 0.99, 0.00, 0.00, 0.99};
		Double_t Blue[4]  = { 0.99, 0.99, 0.00, 0.00};
		Double_t Stops[4] = { 0.00, 0.30, 0.70, 1.00};
		TColor::CreateGradientColorTable(Number,Stops,Red,Green,Blue,number_of_contours);
		gStyle->SetNumberContours(number_of_contours);
		
	}
			
	if (colpal == 3) {
		//macieks palette
		UInt_t Number = 6;
		Double_t Red[6]   = { 0.55, 0.00, 0.00, 0.00, 0.99, 0.99};
		Double_t Green[6] = { 0.00, 0.00, 0.99, 0.99, 0.99, 0.00};
		Double_t Blue[6]  = { 0.99, 0.99, 0.99, 0.00, 0.00, 0.00};
		Double_t Stops[6] = { 0.00, 0.15, 0.35, 0.57, 0.77, 1.00};
		TColor::CreateGradientColorTable(Number,Stops,Red,Green,Blue,number_of_contours);
		gStyle->SetNumberContours(number_of_contours);
	}


}


TCanvas * rootstuff::newCanvas(char *name,char *titel,int xposition,int yposition,int pixelsx,int pixelsy)
{
	TCanvas * canvaspointer;
	canvaspointer = new TCanvas(name,titel,xposition,yposition,pixelsx,pixelsy);
	return canvaspointer;
}




TF1 * rootstuff::newTF1(char * a,char * b,double aa,double bb)
{
   return new TF1(a,b,aa,bb);
}




TH1D * rootstuff::newTH1D(int number,char *comment,int bins,double xmin,double xmax)
{
   char name[200];
   TH1D * hist;
   sprintf(name,"%i",number);
   hist = new TH1D(name,comment,bins,xmin,xmax);
   return hist;
}



TH1D * rootstuff::newTH1D(char *name,char *comment,int bins,double xmin,double xmax)
{
   TH1D * hist;
   hist = new TH1D(name,comment,bins,xmin,xmax);
   return hist;
}


TH2D * rootstuff::newTH2D(int number,char *comment,int xbins,double xmin,double xmax,int ybins,double ymin,double ymax,char *option)
{
   char name[200];
   TH2D * hist;
   sprintf(name,"%i",number);
   hist = new TH2D(name,comment,xbins,xmin,xmax,ybins,ymin,ymax);
   hist->SetOption(option);
   return hist;
}


TH2D * rootstuff::newTH2D(char *name,char *comment,int xbins,double xmin,double xmax,int ybins,double ymin,double ymax,char *option)
{
   TH2D * hist;
   hist = new TH2D(name,comment,xbins,xmin,xmax,ybins,ymin,ymax);
   hist->SetOption(option);
   return hist;
}


TH2F * rootstuff::newTH2F(int number,char *comment,int xbins,double xmin,double xmax,int ybins,double ymin,double ymax,char *option)
{
   char name[200];
   TH2F * hist;
   sprintf(name,"%i",number);
   hist = new TH2F(name,comment,xbins,xmin,xmax,ybins,ymin,ymax);
   hist->SetOption(option);
   return hist;
}


TH2F * rootstuff::newTH2F(char *name,char *comment,int xbins,double xmin,double xmax,int ybins,double ymin,double ymax,char *option)
{
   TH2F * hist;
   hist = new TH2F(name,comment,xbins,xmin,xmax,ybins,ymin,ymax);
   hist->SetOption(option);
   return hist;
}

TH2I * rootstuff::newTH2I(int number,char *comment,int xbins,double xmin,double xmax,int ybins,double ymin,double ymax,char *option)
{
   char name[200];
   TH2I * hist;
   sprintf(name,"%i",number);
   hist = new TH2I(name,comment,xbins,xmin,xmax,ybins,ymin,ymax);
   hist->SetOption(option);
   return hist;
}


TH2I * rootstuff::newTH2I(char *name,char *comment,int xbins,double xmin,double xmax,int ybins,double ymin,double ymax,char *option)
{
   TH2I * hist;
   hist = new TH2I(name,comment,xbins,xmin,xmax,ybins,ymin,ymax);
   hist->SetOption(option);
   return hist;
}


TH3D * rootstuff::newTH3D(char *name,char *comment,int xbins,double xmin,double xmax,int ybins,double ymin,double ymax,int zbins,double zmin,double zmax,char *option)
{
   TH3D * hist;
   hist = new TH3D(name,comment,xbins,xmin,xmax,ybins,ymin,ymax,zbins,zmin,zmax);
   hist->SetOption(option);
   return hist;
}


TTree * rootstuff::newTTree(char * arg1, char * arg2)
{
	TTree * t = new TTree(arg1,arg2);
	return t;
}

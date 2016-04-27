#include <iostream>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TTree.h>
#include <TObject.h>
#include <TDirectory.h>
#include <TGraph.h>

#include "MyHistos.h"


//_______________________________________________________________________________________________________
MyHistos::MyHistos(const bool verbose,int size): fVerbose(verbose), rootfile(0)
{
	arsize = size;
	hiarray = new TObject*[arsize];
	for (int i =0;i<arsize;++i)
		hiarray[i]=0;
}

//_______________________________________________________________________________________________________
MyHistos::~MyHistos()
{
	//--delete all histos--//
	for (int i=0;i<arsize;++i)
	{
		TObject * obj = hiarray[i];
		if (obj)
			delete obj;
	}
	delete [] hiarray;
	
	//--Close root file--//
	if (rootfile)
		rootfile->Close();
}

//########################ROOT File Stuff################################################################
//_______________________________________________________________________________________________________
void MyHistos::OpenRootFile(const TString &name)
{
	rootfile = TFile::Open(name.Data(),"RECREATE");
	if (!rootfile) {std::cout << " could not be opened, please delete the file!"<<std::endl;exit(1);}
	if (fVerbose) std::cout << "Histograms will be written to: "<<name<<std::endl;
}

//_______________________________________________________________________________________________________
void MyHistos::ResetAll()
{
	if (fVerbose) std::cout << "reset all histos"<<std::endl;
	//--write histos to directory--//
	rootfile->cd();
	for (int i=0;i<arsize;++i)
	{
		TObject * obj = hiarray[i];
		if (obj)
		{
			//--First change to the right directory--//
			TH1* h = (TH1*)obj;
			if (fVerbose) std::cout<<"Resetting Histogram:"<<h->GetName()<<" Dir:"<<h->GetDirectory()->GetName()<<std::endl;
			//--reset histogram--//
			h->Reset();
		}
	}
}

//_______________________________________________________________________________________________________
void MyHistos::FlushRootFile()
{
	if (fVerbose) std::cout << "flushing root file"<<std::endl;
	//std::cout << "flushing root file"<<std::endl;
	//--write histos to directory--//
	rootfile->cd();
	for (int i=0;i<arsize;++i)
	{
		TObject * obj = hiarray[i];
		if (obj)
		{
			//--First change to the right directory--//
			TH1* h = (TH1*)obj;
			h->GetDirectory()->cd();
			if (fVerbose) std::cout<<"Writing Histogram:"<<h->GetName()<<" Dir:"<<h->GetDirectory()->GetName()<<std::endl;
			//std::cout<<"Writing Histogram:"<<h->GetName()<<" Dir:"<<h->GetDirectory()->GetName()<<std::endl;
			//--write object to current directory--//
			obj->Write(0,TObject::kOverwrite);
			rootfile->cd("/");
		}
	}

	//--save directory to root file--//
	rootfile->SaveSelf();
}

//########################ROOT Histograms Stuff################################################################
TDirectory* getDir(TFile *rootfile, TString dirName)
{
	//first find out wether directory exists
	rootfile->cd("/");
	TDirectory * direc = rootfile->GetDirectory(dirName.Data());
	if (!direc)
	{
		//if not create it//
		TString lhs;
		TString rhs;
		TString tmp = dirName;
		while (1)
		{
			//if there is no / then this is the last sub dir
			if (tmp.Index("/") == -1)
			{
				lhs = tmp;
			}
			else //otherwise split the string to lefthandside and righthandside of "/"
			{
				lhs = tmp(0,tmp.Index("/"));
				rhs = tmp(tmp.Index("/")+1,tmp.Length());
			}

			//check wether subdir exits//
			direc = gDirectory->GetDirectory(lhs.Data());
			if (direc)
				gDirectory->cd(lhs.Data());//cd into it
			else
			{
				direc = gDirectory->mkdir(lhs.Data()); //create it
				gDirectory->cd(lhs.Data()); //and cd into it
			}

			//when there is no "/" anymore break here//
			if (tmp.Index("/") == -1)
				break;

			//the new temp is all that is on the right hand side
			tmp = rhs;
		}
	}
	//return to root Path//
	rootfile->cd("/");
	return direc;
}

//___________________________________3D____________________________________________________________________
void MyHistos::fill3d(int id, double fillX, double fillY, double fillZ, double weight)
{
	dynamic_cast<TH3D*>(hiarray[id])->Fill(fillX,fillY,fillZ,weight);
}
TH1* MyHistos::create3d(int id, const char *name,
					    const char *titleX, const char *titleY, const char *titleZ,
					    int nXbins, double xLow, double xUp,
					    int nYbins, double yLow, double yUp,
					    int nZbins, double zLow, double zUp, 
					    const char * dir, bool alreadylocked)
{
	//check if hist already exists, if so return it//
	hist3 = dynamic_cast<TH3D*>(hiarray[id]);
	if (hist3) return hist3;

	TDirectory * saveDir = gDirectory;		//save a pointer to the current directory
	getDir(rootfile,dir)->cd();				//change to directory that this histo need to be created in

	//--create a 3D histogram--//
	hist3 = new TH3D(name, name, nXbins, xLow, xUp, nYbins, yLow, yUp, nZbins, zLow, zUp);
	hist3->SetXTitle(titleX);
	hist3->GetXaxis()->CenterTitle(true);
	hist3->GetXaxis()->SetTitleOffset(1.5);
	hist3->SetYTitle(titleY);
	hist3->GetYaxis()->CenterTitle(true);
	hist3->GetYaxis()->SetTitleOffset(1.5);
	hist3->SetZTitle(titleZ);
	hist3->GetZaxis()->CenterTitle(true);
	hist3->GetZaxis()->SetTitleOffset(1.5);
	saveDir->cd();							//reset the previously selectet directory

	//--check if there is a histogram with same id but different name--//
	TObject * obj = hiarray[id];
	if (obj)
		std::cout <<"name doesn't match("<<id<<") is:"<<obj->GetName()<<" should be:"<<hist3->GetName()<<std::endl;

	//--now add it to the list--//
	hiarray[id] = hist3;
	if (fVerbose) std::cout <<"create 3D: "<<dir<<"/"<<hist3->GetName()<<std::endl;
	return hist3;
}

void MyHistos::fill(int id, const char *name, double fillX, double fillY, double fillZ,
					const char *titleX, const char *titleY, const char *titleZ,
					int nXbins, double xLow, double xUp,
					int nYbins, double yLow, double yUp,
					int nZbins, double zLow, double zUp, const char * dir,
					double weight)
{
	hist3 = dynamic_cast<TH3D*>(hiarray[id]);

	//--if the histo does not exist create it first--//
	if (!hist3)
	{
		create3d(id, name,
			     titleX,titleY,titleZ,
				 nXbins,xLow,xUp,
				 nYbins,yLow,yUp,
				 nZbins,zLow,zUp,
				 dir,true);
	}

	//if the histogram exists, but has no entries, we want to resize and rebin it//
	else if ((!hist3->GetEntries()) && ((nXbins!=hist3->GetXaxis()->GetNbins())||(xLow!=hist3->GetXaxis()->GetXmin())||(xUp!=hist3->GetXaxis()->GetXmax())||
										(nYbins!=hist3->GetYaxis()->GetNbins())||(yLow!=hist3->GetYaxis()->GetXmin())||(yUp!=hist3->GetYaxis()->GetXmax())||
										(nZbins!=hist3->GetZaxis()->GetNbins())||(zLow!=hist3->GetZaxis()->GetXmin())||(zUp!=hist3->GetZaxis()->GetXmax())))
		hist3->SetBins(nXbins,xLow,xUp,nYbins,yLow,yUp,nZbins,zLow,zUp);

	//--if the histogram exists, check also if given name is the one of the histogram--//
	else if (strcmp(hist3->GetName(),name))
		std::cout <<"name doesn't match("<<id<<") is:"<<hist3->GetName()<<" should be:"<<name<<std::endl;

	//--now fill it--//
	hist3->Fill(fillX,fillY,fillZ,weight);

}
//__________________________________2D_____________________________________________________________________
void MyHistos::fill2d(int id, double fillX, double fillY, double weight)
{
	dynamic_cast<TH2D*>(hiarray[id])->Fill(fillX,fillY,weight);
}
void MyHistos::plot2d(int id, int binX, int binY, double content)
{
	//dynamic_cast<TH2D*>(hiarray[id])->Reset("ICEM");
	dynamic_cast<TH2D*>(hiarray[id])->SetBinContent(binX, binY, content);
}


TH1* MyHistos::create2d(int id, const char *name,
				        const char *titleX, const char *titleY, 
				        int nXbins, double xLow, double xUp, 
				        int nYbins, double yLow, double yUp, 
					    const char * dir, bool alreadylocked)
{
	//check if hist already exists, if so return it//
	hist2 = dynamic_cast<TH2D*>(hiarray[id]);
	if (hist2 ) return hist2;

	TDirectory * saveDir = gDirectory;		//save a pointer to the current directory
	getDir(rootfile,dir)->cd();				//change to directory that this histo need to be created in

	//--create a 2D histogram--//
	hist2 = new TH2D(name, name, nXbins, xLow, xUp, nYbins, yLow, yUp);
	hist2->SetOption("colz");
	hist2->SetXTitle(titleX);
	hist2->GetXaxis()->CenterTitle(true);
	hist2->SetYTitle(titleY);
	hist2->GetYaxis()->CenterTitle(true);
	hist2->GetYaxis()->SetTitleOffset(1.5);
	saveDir->cd();							//reset the previously selectet directory

	//--check if there is a histogram with same id but different name--//
	TObject * obj = hiarray[id];
	if (obj)
		std::cout <<"name doesn't match("<<id<<") is:"<<obj->GetName()<<" should be:"<<hist2->GetName()<<std::endl;

	//--now add it to the list--//
	hiarray[id]= hist2;
	if (fVerbose) std::cout <<"create 2D: "<<dir<<"/"<<hist2->GetName()<<std::endl;
	return hist2;
}
void MyHistos::fill(int id, const char *name, double fillX, double fillY, 
				    const char *titleX, const char *titleY, 
				    int nXbins, double xLow, double xUp, 
				    int nYbins, double yLow, double yUp, const char * dir,
				    double weight)
{
	hist2 = dynamic_cast<TH2D*>(hiarray[id]);


	//--if the histo does not exist create it first--//
	if (!hist2)
	{
		create2d(id, name,
			     titleX,titleY,
				 nXbins,xLow,xUp,
				 nYbins,yLow,yUp,
				 dir,true);
	}

	//if the histogram exists, but has no entries, we want to resize and rebin it//
	else if ((!hist2->GetEntries()) && ((nXbins!=hist2->GetXaxis()->GetNbins())||(xLow!=hist2->GetXaxis()->GetXmin())||(xUp!=hist2->GetXaxis()->GetXmax())||
										(nYbins!=hist2->GetYaxis()->GetNbins())||(yLow!=hist2->GetYaxis()->GetXmin())||(yUp!=hist2->GetYaxis()->GetXmax())))
		hist2->SetBins(nXbins,xLow,xUp,nYbins,yLow,yUp);

	//--if the histogram exists, check also if given name is the one of the histogram--//
	else if (strcmp(hist2->GetName(),name))
		std::cout <<"name doesn't match("<<id<<") is:"<<hist2->GetName()<<" should be:"<<name<<std::endl;

	//--now fill it--//
	hist2->Fill(fillX,fillY,weight);
}

//___________________________________1D____________________________________________________________________
void MyHistos::fill1d(int id, double fillX,double weight)
{
 	dynamic_cast<TH1D*>(hiarray[id])->Fill(fillX,weight);
}
void MyHistos::plot1d(int id, int binX, double content)
{
	//dynamic_cast<TH1D*>(hiarray[id])->Reset("ICEM");
	dynamic_cast<TH1D*>(hiarray[id])->SetBinContent(binX, content);
}

TH1* MyHistos::create1d(int id, const char * name,
					    const char * titleX,
				        int nXbins, double xLow, double xUp, 
				        const char * dir, bool alreadylocked)
{
	//check if hist already exists, if so return it//
	hist1 = dynamic_cast<TH1D*>(hiarray[id]);
	if (hist1) return hist1;

	TDirectory * saveDir = gDirectory;		//save a pointer to the current directory
	getDir(rootfile,dir)->cd();				//change to directory that this histo need to be created in

	//--create a 1D histogram--//
	hist1 = new TH1D(name, name, nXbins, xLow, xUp);
	hist1->SetXTitle(titleX);
	hist1->GetXaxis()->CenterTitle(true);
	saveDir->cd();							//reset the previously selectet directory

	//--check if there is a histogram with same id but different name--//
	TObject * obj = hiarray[id];
	if (obj)
		std::cout <<"name doesn't match("<<id<<") is:"<<obj->GetName()<<" should be:"<<hist1->GetName()<<std::endl;

	//--now add it to the list--//
	hiarray[id]= hist1;
	if (fVerbose) std::cout <<"create 1D: "<<dir<<"/"<<hist1->GetName()<<std::endl;
	return hist1;
}
void MyHistos::fill(int id, const char * name, double fillX, const char * titleX,
				    int nXbins, double xLow, double xUp, const char * dir,
				    double weight)
{
	hist1 = dynamic_cast<TH1D*>(hiarray[id]);

	//--if the histo does not exist create it first--//
	if (!hist1)
	{
		create1d(id, name,
			     titleX,
				 nXbins,xLow,xUp,
				 dir,true);
	}

	//if the histogram exists, but has no entries, we want to resize and rebin it//
	else if((!hist1->GetEntries()) && ((nXbins!=hist1->GetXaxis()->GetNbins())||(xLow!=hist1->GetXaxis()->GetXmin())||(xUp!=hist1->GetXaxis()->GetXmax())))
		hist1->SetBins(nXbins,xLow,xUp);

	//--if the histogram exists, check also if given name is the one of the histogram--//
	else if (strcmp(hist1->GetName(),name))
		std::cout <<"name doesn't match("<<id<<") is:"<<hist1->GetName()<<" should be:"<<name<<std::endl;

	//--now fill it--//
	hist1->Fill(fillX,weight);
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////Graph///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
//
//
//TGraph* MyHistos::createGraph(int id, const char * name, Int_t nbrPoint,
//				        const char * dir, bool alreadylocked)
//{
//	//check if hist already exists, if so return it//
//	graph1 = dynamic_cast<TGraph*>(hiarray[id]);
//	if (graph1) return graph1;
//
//	TDirectory * saveDir = gDirectory;		//save a pointer to the current directory
//	getDir(rootfile,dir)->cd();				//change to directory that this histo need to be created in
//
//	//--create a 1D histogram--//
//	graph1 = new TGraph(nbrPoint);
//	graph1->SetName(name);
//	graph1->SetTitle(name);
//	saveDir->cd();							//reset the previously selectet directory
//
//	//--check if there is a histogram with same id but different name--//
//	TObject * obj = hiarray[id];
//	if (obj)
//		std::cout <<"name doesn't match("<<id<<") is:"<<obj->GetName()<<" should be:"<<graph1->GetName()<<std::endl;
//
//	//--now add it to the list--//
//	hiarray[id]= graph1;
//	//if (fVerbose) 
//		std::cout <<"create Graph: "<<dir<<"/"<<graph1->GetName()<<std::endl;
//	return graph1;
//}
//void MyHistos::fillGraph(int id, const char * name, Int_t nbrPoint, Int_t n, double X, double Y, const char * dir)
//{
//	graph1 = dynamic_cast<TGraph*>(hiarray[id]);
//
//	//--if the histo does not exist create it first--//
//	if (!graph1)
//	{
//		createGraph(id, name,nbrPoint,
//				 dir,true);
//	}
//
//	////if the histogram exists, but has no entries, we want to resize and rebin it//
//	//else if((!graph1->GetEntries()) && ((nXbins!=graph1->GetXaxis()->GetNbins())||(xLow!=graph1->GetXaxis()->GetXmin())||(xUp!=graph1->GetXaxis()->GetXmax())))
//	//	graph1->SetBins(nXbins,xLow,xUp);
//
//	//--if the histogram exists, check also if given name is the one of the histogram--//
//	else if (strcmp(graph1->GetName(),name))
//		std::cout <<"name doesn't match("<<id<<") is:"<<graph1->GetName()<<" should be:"<<name<<std::endl;
//
//	//--now fill it--//
//	graph1->SetPoint(n,X,Y);
//}






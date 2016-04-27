#ifndef __MyHistos_h_
#define __MyHistos_h_

#include <TH1.h>
#include <TGraph.h>

class TFile;
class TObject;
class TH1D;
class TH2D;
class TH3D;
class TGraph;

class MyHistos  
{
public:
	MyHistos(const bool v, int NbrMaxHistos=100000);
	~MyHistos();

	void     fill(int id, const char *name, double fillX, double fillY, double fillZ,
 			      const char *titleX, const char *titleY, const char *titleZ,
			      int nXbins, double xLow, double xUp,
			      int nYbins, double yLow, double yUp,
			      int nZbins, double zLow, double zUp, const char * dir = "",
			      double weight=1.);			//fill function for 3d histos
	TH1* create3d(int id, const char *name,
				  const char *titleX, const char *titleY, const char *titleZ,
			      int nXbins, double xLow, double xUp,
			      int nYbins, double yLow, double yUp,
			      int nZbins, double zLow, double zUp,
				  const char * dir = "", bool alreadylocked=false);		//create 3d histos
	void   fill3d(int id, double fillX, double fillY, double fillZ, 
			      double weight=1);				//fill fast 3d histos

	void     fill(int id, const char * name, double fillX, double fillY, 
			      const char * titleX, const char * titleY, 
			      int nXbins, double xLow, double xUp, 
			      int nYbins, double yLow, double yUp, const char * dir = "",
			      double weight=1.);			//fill function for 2d histos
	TH1* create2d(int id, const char *name,
				  const char *titleX, const char *titleY,
			      int nXbins, double xLow, double xUp,
			      int nYbins, double yLow, double yUp,
			      const char * dir = "", bool alreadylocked=false);		//create 2d histos
	void   fill2d(int id, double fillX, double fillY, 
			      double weight=1);				//fill fast 2d histos

	void	plot2d(int id, int binX, int binY, double content);//set bin contents

	void     fill(int id, const char * name, double fillX, const char * titleX,
			      int nXbins, double xLow, double xUp, const char * dir = "",
			      double weight=1.);			//fill function for 1d histos
	TH1* create1d(int id, const char *name,
				  const char *titleX,
			      int nXbins, double xLow, double xUp,
			      const char * dir = "", bool alreadylocked=false);		//create 1d histos
	void   fill1d(int id, double fillX, 
			      double weight=1);				//fill fast 1d histos
	void	plot1d(int id, int binX, double content);//set bin contents

	//TGraph* createGraph(int id, const char * name, Int_t nbrPoint,
	//						const char * dir, bool alreadylocked);

	//void fillGraph(int id, const char * name, 
	//						 Int_t nbrPoint, Int_t n,
	//						 double X, double Y, const char * dir);


	void		 FlushRootFile();
	void		 ResetAll();
	void		 OpenRootFile(const TString &RootFileName);
	TH1			*getHist(int id)const							{return dynamic_cast<TH1*>(hiarray[id]);};
	const bool	 IsVerbose()const								{return fVerbose;}

private:
	TH1D	 *hist1;		//pointer to a 1d Histogram
	TH2D	 *hist2;		//pointer to a 2d Histogram
	TH3D	 *hist3;		//pointer to a 3d Histogram
	TFile	 *rootfile;		//pointer to the Rootfile, these Histograms get Stored in
	int		  arsize;		//the size of the array containing the histograms
	TObject **hiarray;		//array Containing the histograms
	const bool fVerbose;	//a flag that tells wether this should do output
	TGraph	*graph1;
};

#endif
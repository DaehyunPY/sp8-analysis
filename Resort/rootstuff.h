//////////////////////////////////////////////////////////////////////
// rootstuff.h: interface for the rootstuff class.
//////////////////////////////////////////////////////////////////////


class TCanvas;
class TFile;
//class TMinuit;
class TF1;
class TH1D;
class TH2D;
class TH2F;
class TH2I;

class TH3D;
class TTree;

//class TTree;


class rootstuff
{
public:
	TFile * RecreateRootFile(char*,char*);
	TCanvas * newCanvas(char*,char*,int,int,int,int);

	void SetGreyScale(int number_of_contours);
	void color(int color_palette_id,int number_of_contours);

	TH1D * newTH1D(char*,char*,int,double,double);

	TF1 * newTF1(char*, char*, double, double);

	TH1D * newTH1D(int,char*,int,double,double);
	TH2D * newTH2D(char*,char*,int,double,double,int,double,double,char*);
	TH2D * newTH2D(int,char*,int,double,double,int,double,double,char*);
	TH2F * newTH2F(char*,char*,int,double,double,int,double,double,char*);
	TH2F * newTH2F(int,char*,int,double,double,int,double,double,char*);
	TH2I * newTH2I(char*,char*,int,double,double,int,double,double,char*);
	TH2I * newTH2I(int,char*,int,double,double,int,double,double,char*);

	TH3D * newTH3D(char *,char *,int ,double ,double ,int ,double ,double ,int ,double ,double ,char *);
	TTree * newTTree(char *, char *);
    
    rootstuff();
    ~rootstuff();
};

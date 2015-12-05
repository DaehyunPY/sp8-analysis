///////////////////////////////////////////////////////////////////////////
//	User defined analysis part called from cobold main program
///////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Parameter description used in this insert dependent part!
// ---------------------------------------------------------
//
// Hardware
// --------
//
// Software
// --------
//
//

/////////////////////////////////////////////////////////////////////////////
// Analysis information string

CDAN_API LPCTSTR AnalysisGetInformationString()
{
	return LPCTSTR("Dummy for CoboldPC 6.2.0.81");
}

/////////////////////////////////////////////////////////////////////////////
// Analysis functions

CDAN_API BOOL AnalysisInitialize(CDoubleArray *pEventData,CDoubleArray *pParameters, CDoubleArray *pWeighParameter)
{
	return TRUE;
}

CDAN_API void AnalysisProcessEvent(CDoubleArray *pEventData,CDoubleArray *pParameters, CDoubleArray *pWeighParameter)
{
	return;
}

CDAN_API void AnalysisFinalize(CDoubleArray *pEventData,CDoubleArray *pParameters, CDoubleArray *pWeighParameter)
{
}
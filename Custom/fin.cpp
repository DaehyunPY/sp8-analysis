CDAN_API void AnalysisFinalize(CDoubleArray *pEventData, CDoubleArray *pParameters, CDoubleArray *pWeighParameter) {
    using std::fstream;
	using std::string;
    using std::endl;

    file_log.close();
    file_log.open(specified_file+"_fin.log", fstream::out);

    file_log.width(25); file_log << "counter: " << counter << endl;
    file_log << endl;

    ////////////////////////////////////////////////////////////////////////////
    // output
    file_electron.close();
    file_log.close();

    return;
}  // end CDAN_API

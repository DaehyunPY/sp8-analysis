////////////////////////////////////////////////////////////////////////////////
// sub init functions
void InputBool(rapidjson::Value& json, bool& val, bool& info) {
    if(!json.IsBool()){info=false;}
    val = json.GetBool();
    info=true;
    return;
}
void InputInt(rapidjson::Value& json, int& val, bool& info) {
    if(!json.IsNumber()){info=false;}
    if(!json.IsInt()){info=false;}
    val = json.GetInt();
    info = true;
    return;
}
void InputDouble(rapidjson::Value& json, double& val, double const unit, bool& info) {
    if(!json.IsNumber()){info=false;}
    val = json.GetDouble() * unit;
    info = true;
    return;
}
void OverWrite(rapidjson::Value& w, rapidjson::Value& r) {
	using rapidjson::Value;
    if(!w.IsObject()){return;}
    if(!r.IsObject()){return;}
    for(Value::ConstMemberIterator itr_list = w.MemberBegin(); itr_list != w.MemberEnd(); itr_list++){
        const char* mb_name = itr_list->name.GetString();
        Value::MemberIterator itr_find = r.FindMember(mb_name);
        if(itr_find != r.MemberEnd()){
            w[mb_name] = r[mb_name];
        } // end if
    } // end for
    return;
}





////////////////////////////////////////////////////////////////////////////////
// init function
CDAN_API BOOL AnalysisInitialize(CDoubleArray *pEventData,CDoubleArray *pParameters, CDoubleArray *pWeighParameter){
	using std::endl;
	using std::fstream;
	using std::stringstream;
	using rapidjson::Document;
	using rapidjson::Value;
	using rapidjson::StringBuffer;
	using rapidjson::PrettyWriter;

    Document json;



    ////////////////////////////////////////////////////////////////////////////
    // input from "angle_cor.dat"
    file_log.open("init.log", fstream::out);



    ////////////////////////////////////////////////////////////////////////////
    // input from "angle_cor.dat"
    { // fl
        fstream fl;
        fl.open("angle_cor.dat", fstream::in);
        double ang;
        for(int i=0; i<19; i++){
            dPosi_cor[i] = 1.e0;
            fl >> ang >> dPosi_cor[i];
        }
        fl.close();
    } // end fl



    ////////////////////////////////////////////////////////////////////////////
    // parameters form
    // TODO: read form file from the folder store dlls
    {
      const char* ch = "{"
        "\"ion\":{\"number_of_hits\":null,\"1st_hit\":{\"mass\":null,\"charge\":null,\"minimum_of_TOF\":null,\"maximum_of_TOF\":null},\"2nd_hit\":{\"mass\":null,\"charge\":null,\"minimum_of_TOF\":null,\"maximum_of_TOF\":null},\"3rd_hit\":{\"mass\":null,\"charge\":null,\"minimum_of_TOF\":null,\"maximum_of_TOF\":null},\"4th_hit\":{\"mass\":null,\"charge\":null,\"minimum_of_TOF\":null,\"maximum_of_TOF\":null},\"x_pixel_size\":null,\"y_pixel_size\":null,\"x_cor\":null,\"y_cor\":null,\"time_delay\":null,\"x_base_of_COM\":null,\"y_base_of_COM\":null},"
        "\"electron\":{\"number_of_hits\":null,\"minimum_of_TOF\":null,\"maximum_of_TOF\":null,\"x_cor\":null,\"y_cor\":null,\"time_delay\":null,\"x_base_of_COM\":null,\"y_base_of_COM\":null,\"x_tangent_of_COM\":null,\"y_tangent_of_COM\":null},"
        "\"equipment\":{\"is_polarization_horizontal?\":null,\"length_of_L1\":null,\"length_of_L2\":null,\"length_of_L3\":null,\"length_of_D1\":null,\"length_of_D2\":null,\"electric_potential_of_electron_detector\":null,\"electric_potential_of_1st\":null,\"electric_potential_of_2nd\":null,\"electric_potential_of_ion_detector\":null,\"magnetic_filed\":null,\"angle_width\":null}"
        "}";
      bool error_is = json.Parse(ch).HasParseError();
      if(error_is){return 1;}
    }
    {
        StringBuffer buf;
        PrettyWriter<StringBuffer> writer(buf);
        json.Accept(writer);
        fstream fstr;
        fstr.open("parameters-form.json", fstream::out);
        fstr << buf.GetString() << endl;
    }



    ////////////////////////////////////////////////////////////////////////////
    // load json files
    {
        const char* files[2] = {"parameters-global.json", "parameters-local.json"};
        for(int ith = 0; ith < sizeof(files)/sizeof(files[0]); ith++){
            stringstream sstr;
            Document doc;
            fstream fstr;

            fstr.open(files[ith], fstream::in);
            if(!fstr.is_open()){return false;}
            sstr << fstr.rdbuf();
            fstr.close();
            bool error_is = doc.Parse(sstr.str().c_str()).HasParseError();
            if(error_is){return false;}

            const char* objs[3] = {"ion", "electron", "equipment"};
            for(int ith = 0; ith < sizeof(objs)/sizeof(objs[0]); ith++){
                const char* i = objs[ith];
                Value::MemberIterator mb1 = doc.FindMember(i);
                if(mb1 != doc.MemberEnd()){
                    OverWrite(json[i], mb1->value);
                } // end if
            } // end for
        } // end for
    }
    { // check "umber_of_hists" value
        const char* hits[nHit] = {"1st_hit", "2nd_hit", "3rd_hit", "4th_hit"};
        Value& val = json["ion"]["number_of_hits"];
        if(!val.IsInt()){return false;}
        int n = json["ion"]["number_of_hits"].GetInt();
        // if(n<1 || n>nHit){return false;}
        if(n<0 || n>nHit){return false;}
        if(n<nHit){
            for(int ith=n; ith<nHit; ith++){
                json["ion"].RemoveMember(hits[ith]);
            } // end for
        } // end if
    }
    { // check "umber_of_hists" value
        Value& val = json["electron"]["number_of_hits"];
        if(!val.IsInt()){return false;}
        int n = json["electron"]["number_of_hits"].GetInt();
        // if(n<1 || n>nHit){return false;}
        if(n<0 || n>nHit){return false;}
    }



    ////////////////////////////////////////////////////////////////////////////
    // input parameters
    {
        double dVelec=0.0, dVion1st=0.0, dVion2nd=0.0, dViondet=0.0;
        bool info;

        // ion parameters
        InputInt(json["ion"]["number_of_hits"], nIonHit, info); if(info==false){return false;} // 189
        const char* hits[nHit] = {"1st_hit", "2nd_hit", "3rd_hit", "4th_hit"};
        for(int ith=0; ith<nIonHit; ith++){
            InputDouble(json["ion"][hits[ith]]["mass"],           dMass[ith],    dMassUnit, info); if(info==false){return false;} // 186, 187, 188, 264
            InputDouble(json["ion"][hits[ith]]["charge"],         dCharge[ith],  dElectron, info); if(info==false){return false;} // 183, 184, 185, 270
            InputDouble(json["ion"][hits[ith]]["minimum_of_TOF"], dMin_TOF[ith], unit_nano, info); if(info==false){return false;} // 190, 192, 194, 265
            InputDouble(json["ion"][hits[ith]]["maximum_of_TOF"], dMax_TOF[ith], unit_nano, info); if(info==false){return false;} // 191, 193, 195, 266
        } // end for
        InputDouble(json["ion"]["x_pixel_size"],  dPixelSizeX, 1.0,        info); if(info==false){return false;} // 147
        InputDouble(json["ion"]["y_pixel_size"],  dPixelSizeY, 1.0,        info); if(info==false){return false;} // 148
        InputDouble(json["ion"]["x_cor"],         dCorX,       1.0,        info); if(info==false){return false;} // 149
        InputDouble(json["ion"]["y_cor"],         dCorY,       1.0,        info); if(info==false){return false;} // 150
        InputDouble(json["ion"]["time_delay"],    dDelay,      unit_nano,  info); if(info==false){return false;} // 220
        InputDouble(json["ion"]["x_base_of_COM"], dCOM_x0,     unit_milli, info); if(info==false){return false;} // 196
        InputDouble(json["ion"]["y_base_of_COM"], dCOM_y0,     unit_milli, info); if(info==false){return false;} // 197

        // electron parameters
        InputInt(json["electron"]["number_of_hits"],      nElectronHit,                       info); if(info==false){return false;}
        InputDouble(json["electron"]["minimum_of_TOF"],   dMin_eTOF,    unit_nano,            info); if(info==false){return false;} // 224
        InputDouble(json["electron"]["maximum_of_TOF"],   dMax_eTOF,    unit_nano,            info); if(info==false){return false;} // 225
        InputDouble(json["electron"]["x_cor"],            dX_cor,       1.0,                  info); if(info==false){return false;} // 226
        InputDouble(json["electron"]["y_cor"],            dY_cor,       1.0,                  info); if(info==false){return false;} // 227
        InputDouble(json["electron"]["time_delay"],       dElectron_T0, unit_nano,            info); if(info==false){return false;} // 221
        InputDouble(json["electron"]["x_base_of_COM"],    dCOM_x_e0,    unit_milli,           info); if(info==false){return false;} // 228
        InputDouble(json["electron"]["y_base_of_COM"],    dCOM_y_e0,    unit_milli,           info); if(info==false){return false;} // 229
        InputDouble(json["electron"]["x_tangent_of_COM"], dCOM_x_e_tan, unit_milli/unit_nano, info); if(info==false){return false;} // 230
        InputDouble(json["electron"]["y_tangent_of_COM"], dCOM_x_e_tan, unit_milli/unit_nano, info); if(info==false){return false;} // 231

        // equipment parameters
        InputBool(json["equipment"]["is_polarization_horizontal?"],               polarization_is_horizontal,  info); if(info==false){return false;} // 137
        InputDouble(json["equipment"]["length_of_L1"],                            dL1,             unit_milli, info); if(info==false){return false;} // 210
        InputDouble(json["equipment"]["length_of_L2"],                            dL2,             unit_milli, info); if(info==false){return false;} // 208
        InputDouble(json["equipment"]["length_of_L3"],                            dL3,             unit_milli, info); if(info==false){return false;} // 209
        InputDouble(json["equipment"]["length_of_D1"],                            dD1,             unit_milli, info); if(info==false){return false;} // 211
        InputDouble(json["equipment"]["length_of_D2"],                            dD2,             unit_milli, info); if(info==false){return false;} // 212
        InputDouble(json["equipment"]["electric_potential_of_electron_detector"], dVelec,          1.0,        info); if(info==false){return false;} // 213
        InputDouble(json["equipment"]["electric_potential_of_1st"],               dVion1st,        1.0,        info); if(info==false){return false;} // 214
        InputDouble(json["equipment"]["electric_potential_of_2nd"],               dVion2nd,        1.0,        info); if(info==false){return false;} // 215
        InputDouble(json["equipment"]["electric_potential_of_ion_detector"],      dViondet,        1.0,        info); if(info==false){return false;} // 216
        InputDouble(json["equipment"]["magnetic_filed"],                          dMagnetic_Field, unit_gauss, info); if(info==false){return false;} // 217
        InputDouble(json["equipment"]["angle_width"],                             dAngle_Width,    1.0,        info); if(info==false){return false;} // 218
        dElectric_Field1 = (dVelec-dVion1st)/(dL1+dD1);
        dElectric_Field2 = (dVion1st-dVion2nd)/dL2;
        dElectric_Field3 = (dVion2nd-dViondet)/dL3;
    }
    // file_log << json["ion"]["1st_hit"]["minimum_of_TOF"].GetDouble() << " " << json["ion"]["1st_hit"]["maximum_of_TOF"].GetDouble() << endl;
    // file_log << json["ion"]["2nd_hit"]["minimum_of_TOF"].GetDouble() << " " << json["ion"]["2nd_hit"]["maximum_of_TOF"].GetDouble() << endl;
    // file_log << json["ion"]["3rd_hit"]["minimum_of_TOF"].GetDouble() << " " << json["ion"]["3rd_hit"]["maximum_of_TOF"].GetDouble() << endl;
    // file_log << json["ion"]["4th_hit"]["minimum_of_TOF"].GetDouble() << " " << json["ion"]["4th_hit"]["maximum_of_TOF"].GetDouble() << endl;
    // for(int ith=0; ith<nIonHit; ith++){
    //     file_log << dMin_TOF[ith] << " " << dMax_TOF[ith] << endl;
    // }
    { // check loaded values
        StringBuffer buf;
        PrettyWriter<StringBuffer> writer(buf);
        json.Accept(writer);
        fstream fstr;
        fstr.open("parameters-loaded.json", fstream::out);
        fstr << buf.GetString() << endl;
    }



    ////////////////////////////////////////////////////////////////////////////
    // output
    file_electron.open("electron.csv", fstream::out);
    file_electron.width(14);
    file_electron << "\"ion1 px\"" << ", ";
    file_electron.width(14);
    file_electron << "\"ion1 py\"" << ", ";
    file_electron.width(14);
    file_electron << "\"ion1 pz\"" << ", ";
    file_electron.width(14);
    file_electron << "\"ion1 E\"" << ", ";
    file_electron.width(14);
    file_electron << "\"ion2 px\"" << ", ";
    file_electron.width(14);
    file_electron << "\"ion2 py\"" << ", ";
    file_electron.width(14);
    file_electron << "\"ion2 pz\"" << ", ";
    file_electron.width(14);
    file_electron << "\"ion2 E\"" << ", ";
    file_electron.width(14);
    file_electron << "\"ion3 px\"" << ", ";
    file_electron.width(14);
    file_electron << "\"ion3 py\"" << ", ";
    file_electron.width(14);
    file_electron << "\"ion3 pz\"" << ", ";
    file_electron.width(14);
    file_electron << "\"ion3 E\"" << ", ";
    file_electron.width(14);
    file_electron << "\"ion4 px\"" << ", ";
    file_electron.width(14);
    file_electron << "\"ion4 py\"" << ", ";
    file_electron.width(14);
    file_electron << "\"ion4 pz\"" << ", ";
    file_electron.width(14);
    file_electron << "\"ion4 E\"" << ", ";
    file_electron << endl;

    const char* order[nHit] = {"1st", "2nd", "3rd", "4th"};
    for(int ith=0; ith<nIonHit; ith++){
        file_log.width(25-15); file_log << "Ion " << order[ith] << " hit's TOF: "
                                        << tof(0.0,0.0,dMass[ith],dCharge[ith])/unit_nano
                                        << " [ns]" << endl;
    }
    file_log.width(25); file_log << "electron's TOF: "
                                 << electof(0.0)/unit_nano
                                 << " [ns]" << endl;
    file_log.width(25); file_log << "electron's cycle time: "
                                 << 2.0*pi*dElectronMass/dMagnetic_Field/dElectron/unit_nano
                                 << " [ns]" << endl;
    file_log << endl;

    file_log.close();
    file_log.open("proc.log", fstream::out);

    return true;
} // end AnalysisInitialize

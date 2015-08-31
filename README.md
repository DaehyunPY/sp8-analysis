1. Compileするとき注意すること
  - Visual Studio project files(*.dsp, *.dsw, *.sln, *.vcsproj files)は"SLNs" folderにbackupされている. 自分のversionにあうproject filesをcopyしてsource folderに移す
  - RapidJSONを使用するときは(debug modeではなく)release modeでないとcompileがうまくいかない
  - MFC errorになるときはVisual Studio 2010をinstallする
  - Visual Studio 2010はserver "helium"にある
  - "Custom", "rapidjson" folders, "SoftwareDependent.cpp" files以外はoriginal files(CoBold PC 2002)と一緒
  - Original source filesはinstallされたCoBold PC folderの"Sources/VS6CPP"以下にある
  - Compileで得られる"Release/DAn.dll" fileをCoBold PCの実行file(CoboldPC.exe)と同じfolderに移す
  - "DAq.dll" fileは"DAN and DAQ/CCC1/DAq.dll"のをcopyして移す

2. 実行するとき注意すること
  - Parametersは"parameters-global.json", "parameters-local.json" filesから読み込まれる
  - "parameters-global.json" fileが先に読まれて, "parameters-local.json" fileが次に読まれる. "parameters-global.json"のparametersは"parameters-local.json"によりoverwriteされる
  - 実際読まれたparametersは"parameters-loaded.json"としてoutputされる
  - JSONのformはCobold PCを起動すると"parameters-form.json"として出される
  - これらのfilesはccf fileでsetpathしたfolderでinput, outputされる

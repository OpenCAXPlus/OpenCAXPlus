
#include <algorithm>
#include "mathex.h"

#include "OnelabClients.h"

// reserved keywords for the onelab parser

namespace olkey{
  static std::string deflabel("onelab.tags");
  static std::string label("OL."), comment("#"), separator(";");
  static std::string line(label+"line");
  static std::string begin(label+"block");
  static std::string end(label+"endblock");
  static std::string include(label+"include");
  static std::string message(label+"msg");
  static std::string showParam(label+"show");
  static std::string showGmsh(label+"merge");
  static std::string dump(label+"dump");
  static std::string ifcond(label+"if");
  static std::string iftrue(label+"iftrue"), ifntrue(label+"ifntrue");
  static std::string olelse(label+"else"), olendif(label+"endif");
  static std::string getValue(label+"get");
  static std::string mathex(label+"eval");
}

// Client member functions defined here because they use parser commands

bool MetaModel::findCommandLine(const std::string &client, const std::string &host){
  std::string fileName;
  size_t pos;

  //std::cout << "FHF search cmdl: " << client << " , " << host << std::endl;

  fileName = getWorkingDir() + genericNameFromArgs + onelabExtension + ".save";

  std::ifstream infile(fileName.c_str());
  if(infile.is_open()){
    while(infile.good()){
      std::string line;
      getline(infile,line);
      if( (pos=line.find(olkey::separator)) != std::string::npos){
	std::string name, action;
	std::vector<std::string> args;
	extract(line.substr(0,pos),name,action,args);
	// (name, action, args) = client.commandLine(cmdl{,rhost{,rdir}})
	std::string cmdl="", rhost="localhost", rdir="";
	cmdl = args[0];
	if(args.size() > 1) rhost= args[1];
	if(args.size() > 2) rdir = args[2];

	if(name == client){
	  if( (host.empty() && (rhost != "localhost" )) ||
	      (host.size() && (rhost == host)) ) {
	    OLMsg::SetOnelabString(name + "/CommandLine", cmdl, false);
	    if(rhost.compare("localhost")){
	      OLMsg::SetOnelabString(name + "/HostName", rhost, false);
	      if(rdir.size())
		OLMsg::SetOnelabString(name + "/RemoteDir", rdir, false);
	    }
	    //std::cout << "FHF found cmdl: " << cmdl << "," << rhost << std::endl;
	    return true;
	  }
	}
      }
    }
  }
  infile.close();
  return false;
}

std::string localSolverClient::toChar(){
  std::ostringstream sstream;

  if(getCommandLine().size()){
    sstream << getName() << ".commandLine(" << getCommandLine();
    std::string host=OLMsg::GetOnelabString(getName() + "/HostName");
    if(host.size() && host.compare("localhost")) {
      sstream << "," << host ;
      std::string rdir=OLMsg::GetOnelabString(getName() + "/RemoteDir");
      if(rdir.size()) sstream << "," << rdir;
    }
    sstream << ");" << std::endl;
  }
  return sstream.str();
}

void MetaModel::saveCommandLines(){
  std::vector<std::string> arguments, buffer;
  std::string fileName;
  fileName = getWorkingDir() + genericNameFromArgs + onelabExtension + ".save";

  std::ifstream infile(fileName.c_str());
  if(infile.is_open()){
    while(infile.good()){
      std::string line;
      getline(infile,line);
      size_t pos;
      if( (pos=line.find(olkey::separator)) != std::string::npos){
	std::string name, action;
	std::vector<std::string> args;
	extract(line.substr(0,pos),name,action,args);
	std::string host = OLMsg::GetOnelabString(name + "/HostName");
	std::string rhost=(args.size()>=2)?args[1]:"";
	bool keep = rhost.compare(host);
	if(keep) buffer.push_back(line);
      }
    }
  }
  else
    OLMsg::Warning("The file <%s> cannot be opened",fileName.c_str());
  infile.close();

  //save client command lines
  std::ofstream outfile(fileName.c_str());
  if(outfile.is_open()){
    for(citer it = _clients.begin(); it != _clients.end(); it++){
	 outfile << (*it)->toChar();
    }
    for(std::vector<std::string>::const_iterator it = buffer.begin();
	it != buffer.end(); it++){
      outfile << (*it) << std::endl;
    }
  }
  else
    OLMsg::Error("The file <%s> cannot be opened",fileName.c_str());
  outfile.close();
}


int enclosed(const std::string &in, std::vector<std::string> &arguments,
	     size_t &end){
  // syntax: (arguments[Ø], arguments[1], ... , arguments[n])
  // arguments[i] may contain parenthesis

  size_t pos, cursor;
  arguments.resize(0);

  pos=0;
  if(in[pos] != '(') {
    OLMsg::Error("Syntax error: <%s>",in.c_str());
    return 0;
  }
  pos++; // skips '('
  int count=1;
  cursor = pos;
  do{
    if(in[pos] == '(')
      count++;
    else if(in[pos] == ')')
      count--;
    else if(in[pos] == ',') {
      if(count == 1){
	arguments.push_back(removeBlanks(in.substr(cursor,pos-cursor)));
	cursor=pos+1; // skips ','
      }
      else{
	// ignore this comma
      }
    }
    pos++;
  } while( count && (pos < in.size()) );

  // count is 0 when the closing brace has been found.
  if(count && pos == in.size()){
    OLMsg::Error("Syntax error: <%s>",in.c_str());
    return 0;
  }
  else
    arguments.push_back(removeBlanks(in.substr(cursor,pos-1-cursor)));
  end=pos;
  return arguments.size();
}


int extract(const std::string &in, std::string &paramName,
	    std::string &action, std::vector<std::string> &arguments){
  // syntax: paramName.action( arg1, arg2, ... )
  size_t pos, cursor;
  cursor=0;
  if ( (pos=in.find(".",cursor)) == std::string::npos ){
     OLMsg::Error("Syntax error: <%s>",in.c_str());
     return 0;
  }
  else
    paramName.assign(sanitize(in.substr(cursor,pos-cursor)));
  cursor = pos+1; // skips '.'
  if ( (pos=in.find("(",cursor)) == std::string::npos ){
     OLMsg::Error("Syntax error: <%s>",in.c_str());
     return 0;
  }
  else
    action.assign(sanitize(in.substr(cursor,pos-cursor)));
  cursor = pos;

  int NumArg = enclosed(in.substr(cursor),arguments,pos);
  //std::cout << "FHF=" << in.substr(cursor+pos) << std::endl;
  if((in.find_first_not_of(" \t",cursor+pos+1)) != std::string::npos){
    OLMsg::Error("Syntax error in <%s> (forgot a %s ?)",
  		 in.substr(cursor+pos+1).c_str(),olkey::separator.c_str());
    return 0;
  }
  if(!NumArg)
    OLMsg::Error("Syntax error: <%s>",in.c_str());
  return NumArg;
}

int extractLogic(const std::string &in, std::vector<std::string> &arguments){
  // syntax: ( argument[0], argument[1]\in{<,>,<=,>=,==,!=}, arguments[2])
  size_t pos, cursor;
  arguments.resize(0);
  cursor=0;
  if ( (pos=in.find("(",cursor)) == std::string::npos ){
     OLMsg::Error("Syntax error: <%s>",in.c_str());
     return 0;
  }

  unsigned int count=1;
  pos++; // skips '('
  cursor=pos;
  do{
    if(in[pos]=='(') count++;
    if(in[pos]==')') count--;
    if( (in[pos]=='<') || (in[pos]=='=') || (in[pos]=='>') || (in[pos]=='!') ){
      arguments.push_back(removeBlanks(in.substr(cursor,pos-cursor)));
      if(count!=1)
	OLMsg::Error("Syntax error: <%s>",in.c_str());
      cursor=pos;
      if(in[pos+1]=='='){
	arguments.push_back(in.substr(cursor,2));
	pos++;
      }
      else{
      	arguments.push_back(in.substr(cursor,1));
      }
      cursor=pos+1;
    }
    pos++;
  } while( count && (pos!=std::string::npos) );
  // count is 0 when the closing brace is found.

  if(count)
    OLMsg::Error("Syntax error: mismatched parenthesis in <%s>",in.c_str());
  else
    arguments.push_back(removeBlanks(in.substr(cursor,pos-1-cursor)));

  if((arguments.size()!=1) && (arguments.size()!=3))
    OLMsg::Error("Syntax error: <%s>",in.c_str());
  return arguments.size();
}

std::string extractExpandPattern(const std::string& str){
  size_t posa, posb;
  posa=str.find_first_of("\"\'<");
  posb=str.find_last_of("\"\'>");
  std::string pattern=str.substr(posa+1,posb-posa-1);
  posa=pattern.find("comma");
  if(posa!=std::string::npos)
    pattern.replace(posa,5,",");
  if(pattern.size()!=3)
    OLMsg::Error("Incorrect expand pattern <%s>",
	       str.c_str());
  return pattern;
}

std::string localSolverClient::longName(const std::string name){
  std::set<std::string, ShortNameLessThan>::iterator it;
  std::string fullName;
  if((it = _parameters.find(name)) != _parameters.end())
    fullName.assign(OLMsg::obtainFullName(*it));
  else
    fullName.assign(OLMsg::obtainFullName(name));
  return fullName;
}

std::string localSolverClient::resolveString(const std::string &line) {
  //looks for the first OL.get() statement,
  //returns a onelab::string value from the server, if any, or "" otherwise
  //if no OL.get() statement found, returns line unchanged.
  std::vector<onelab::string> strings;
  std::vector<std::string> arguments;
  size_t pos,cursor;

  if((pos=line.find(olkey::getValue)) != std::string::npos){
    cursor = pos+olkey::getValue.length();
    int NumArg=enclosed(line.substr(cursor),arguments,pos);
    if(NumArg<1){
      OLMsg::Error("Misformed %s statement: <%s>",
		   olkey::getValue.c_str(),line.c_str());
      return "??";
    }
    std::string paramName=longName(arguments[0]);
    get(strings,paramName);
    if (strings.size())
      return strings[0].getValue();
    else
      return "";
  }
  return line;
}

std::string localSolverClient::resolveGetVal(std::string line) {
  //looks for OL.get() statements, substitute values from server
  //then evaluate the resulting string with mathex.
  //OL.get(name
  //        {, {choices|range}.{size|comp|expand|index}|attributes.get(args)})
  std::vector<onelab::number> numbers;
  std::vector<onelab::string> strings;
  std::vector<std::string> arguments;
  std::string buff;
  size_t pos, pos0, cursor;

  cursor=0;
  while ( (pos=line.find(olkey::getValue,cursor)) != std::string::npos){
    pos0=pos; // for further use
    cursor = pos+olkey::getValue.length();
    int NumArg=enclosed(line.substr(cursor),arguments,pos);
    if(NumArg<1){
      OLMsg::Error("Misformed %s statement: <%s>",
		 olkey::getValue.c_str(),line.c_str());
      return "??";
    }
    std::string paramName=longName(arguments[0]);
    get(numbers,paramName);
    if (numbers.size()){
      std::stringstream Num;
      if(NumArg==1){
	Num << numbers[0].getValue();
	buff.assign(Num.str());
      }
      else if(NumArg==2){
	std::string name, action;
	std::vector<std::string> args;
	extract(arguments[1],name,action,args);
	if(!name.compare("choices")) {
	  std::vector<double> choices=numbers[0].getChoices();
	  if(!action.compare("size")) {
	    buff.assign(ftoa(choices.size()));
	  }
	  else if(!action.compare("begin")) {
	    buff.assign(ftoa(*choices.begin()));
	  }
	  else if(!action.compare("rbegin")) {
	    buff.assign(ftoa(*choices.rbegin()));
	  }
	  else if(!action.compare("comp")) {
	    int i=atoi(args[0].c_str());
	    if( (i>=0) && (i<(int)choices.size()) )
	      Num << choices[i];
	    buff.assign(ftoa(choices[i]));
	  }
	  else if(!action.compare("expand")) {
	    std::string pattern;
	    pattern.assign(extractExpandPattern(args[0]));
	    // OLMsg::Info("Expand parameter <%s> with pattern <%s>",
	    // 	      paramName.c_str(),pattern.c_str());
	    buff.assign(1,pattern[0]);
	    for(std::vector<double>::iterator it = choices.begin();
		it != choices.end(); it++){
	      if(it != choices.begin())
		buff.append(1,pattern[1]);
	      buff.append(ftoa(*it));
	    }
	    buff.append(1,pattern[2]);
	  }
	  else if(!action.compare("index")) {
	    Num << numbers[0].getIndex();
	    buff.assign(Num.str());
	  }
	  else
	    OLMsg::Error("Unknown action <%s> in %s statement",
		       action.c_str(),olkey::getValue.c_str());
	}
	else if(!name.compare("range")) {
	  double stp=numbers[0].getStep();
	  double min=numbers[0].getMin();
	  double max=numbers[0].getMax();

	  if( (stp == 0) ||
	      (min == -onelab::parameter::maxNumber()) ||
	      (max ==  onelab::parameter::maxNumber()) )
	    OLMsg::Error("Invalid range description for parameter <%s>",
		       paramName.c_str());
	  if(!action.compare("size")) {
	    buff.assign(ftoa(fabs((max-min)/stp)));
	  }
	  else if(!action.compare("comp")) {
	    int i= atof(args[0].c_str());
	    if(stp > 0)
		Num << min+i*stp;
	    else if(stp < 0)
	      Num << max-i*stp;
	  }
	  else if(!action.compare("expand")) {
	  }
	  else
	    OLMsg::Error("Unknown action <%s> in %s statement",
		       action.c_str(),olkey::getValue.c_str());
	}
	else if(!name.compare(0,6,"attrib")) {
	  if(!action.compare("get")) {
	    buff.assign(numbers[0].getAttribute(args[0]));
	  }
	}
      }
    }
    else{
      get(strings,paramName);
      if (strings.size())
	buff.assign(strings[0].getValue());
      else{
	OLMsg::Error("resolveGetVal: unknown variable: <%s>",paramName.c_str());
	return "??";
      }
    }
    line.replace(pos0,cursor+pos-pos0,buff);
    cursor=pos0+buff.length();
  }

  // Check now wheter the line contains OL.eval's and resolve them
  cursor=0;
  while ( (pos=line.find(olkey::mathex,cursor)) != std::string::npos){
    size_t pos0=pos;
    cursor=pos+olkey::mathex.length();
    if(enclosed(line.substr(cursor),arguments,pos) != 1){
      OLMsg::Error("Misformed %s statement: <%s>",
		 olkey::mathex.c_str(),line.c_str());
      return "??";
    }
    //std::cout << "MathEx evaluates now <"<< arguments[0]<< "> " << std::endl;
    smlib::mathex* mathExp = new smlib::mathex();
    mathExp->expression(arguments[0]);
    double val=mathExp->eval();
    //std::cout << "MathEx <" << arguments[0] << "> ="<< val << std::endl;
    line.replace(pos0,cursor+pos-pos0,ftoa(val));
  }

  // Check now wheter the line still contains OL.
  if ( (pos=line.find(olkey::label)) != std::string::npos)
    OLMsg::Error("Unidentified onelab command in <%s>",line.c_str());

  return line;
}

bool localSolverClient::resolveLogicExpr(std::vector<std::string> arguments) {
  std::vector<onelab::number> numbers;

  double val1, val2;
  std::string str1,str2;
  bool condition=false;

  if(arguments.size()==1){
    str1.assign(resolveString(arguments[0]));
    if(str1.size())
      return true;
    val1 = atof( resolveGetVal(arguments[0]).c_str() );
    return (bool)val1;
  }
  else if(arguments.size()==3){

    str1.assign(resolveString(arguments[0]));
    str2.assign(resolveString(arguments[2]));
    if(str1.size() && str2.size()){
      if (!arguments[1].compare("=="))
	condition = !str1.compare(str2);
      else if (!arguments[1].compare("!="))
	condition = str1.compare(str2);
      else
	OLMsg::Error("Unknown logical operator <%s> for strings",
		     arguments[1].c_str());
    }
    else{
      val1 = atof( resolveGetVal(arguments[0]).c_str() );
      val2 = atof( resolveGetVal(arguments[2]).c_str() );
      if(!arguments[1].compare("<"))
	condition = (val1<val2);
      else if (!arguments[1].compare("<="))
	condition = (val1<=val2);
      else if (!arguments[1].compare(">"))
	condition = (val1>val2);
      else if (!arguments[1].compare(">="))
	condition = (val1>=val2);
      else if (!arguments[1].compare("=="))
	condition = (val1==val2);
      else if (!arguments[1].compare("!="))
	condition = (val1!=val2);
      else
	OLMsg::Error("Unknown logical operator <%s>", arguments[1].c_str());
    }
  }
  else
    OLMsg::Error("Invalid logical expression");
  return condition;
}

bool localSolverClient::resolveRange(const std::string &in, std::vector<double> &arguments){
  // syntax: a:b:c or a:b|n with a,b,c numbers and n integer
  double val;
  size_t pos, cursor;
  arguments.resize(0);
  cursor=0;
  if ( (pos=in.find(":",cursor)) == std::string::npos ){
     OLMsg::Error("Syntax error in range <%s>",in.c_str());
     return 0;
  }
  else{
    val=atof(resolveGetVal(in.substr(cursor,pos-cursor)).c_str());
    arguments.push_back(val);
  }
  cursor = pos+1; // skips ':'
  if ( (pos=in.find(":",cursor)) != std::string::npos ){
    //arguments.push_back(atof(in.substr(cursor,pos-cursor).c_str()));
    //arguments.push_back(atof(in.substr(pos+1).c_str()));
    val=atof(resolveGetVal(in.substr(cursor,pos-cursor)).c_str());
    arguments.push_back(val);
    val=atof(resolveGetVal(in.substr(pos+1)).c_str());
    arguments.push_back(val);
  }
  else if ( (pos=in.find("|",cursor)) != std::string::npos ){
    // arguments.push_back(atof(in.substr(cursor,pos-cursor).c_str()));
    // double NumStep = atof(in.substr(pos+1).c_str());
  //arguments.push_back((arguments[1]-arguments[0])/((NumStep==0)?1:NumStep));
    val=atof(resolveGetVal(in.substr(cursor,pos-cursor)).c_str());
    arguments.push_back(val);
    double NumStep = atof(in.substr(pos+1).c_str());
    arguments.push_back((arguments[1]-arguments[0])/((NumStep==0)?1:NumStep));
  }
  else
     OLMsg::Error("Syntax error in range <%s>",in.c_str());
  return (arguments.size()==3);
}

void localSolverClient::parse_sentence(std::string line) {
  size_t pos,cursor;
  std::string name,action,path;
  std::vector<std::string> arguments;
  std::vector<onelab::number> numbers;
  std::vector<onelab::string> strings;

  cursor = 0;
  while ( (pos=line.find(olkey::separator,cursor)) != std::string::npos){
    std::string name, action;
    //std::cout << "line=" << line << std::endl;

    extract(line.substr(cursor,pos-cursor),name,action,arguments);
    if(!action.compare("number")) {
      // syntax: paramName.number(val,path,help,range(optional))
      double val=0.0;
      if(arguments.size()>1)
	name.assign(FixOLPath(arguments[1]) + name);
      _parameters.insert(name);
      OLMsg::recordFullName(name);
      get(numbers, name);
      if(numbers.empty()){
      	numbers.resize(1);
      	numbers[0].setName(name);
      	if(arguments[0].empty()){
      	  numbers[0].setReadOnly(true);
      	  numbers[0].setNeverChanged(true);
      	}
      	else
      	  val=atof(resolveGetVal(arguments[0]).c_str());
      	numbers[0].setValue(val);
      }
      // else if(arguments[0].empty()) // resets read only parameters
      // 	  numbers[0].setValue(val);

      if(arguments.size()>2)
	numbers[0].setLabel(unquote(arguments[2]));
      if(arguments.size()>3){
	std::vector<double> bounds;
	if (resolveRange(arguments[3],bounds)){
	  numbers[0].setMin(bounds[0]);
	  numbers[0].setMax(bounds[1]);
	  numbers[0].setStep(bounds[2]);
	}
      }
      set(numbers[0]);
    }
    else if(!action.compare("string")) {
      // syntax: paramName.string(val,path,help)
      std::string val = "";
      if(arguments.size()>1)
	name.assign(FixOLPath(arguments[1]) + name); // append path
      _parameters.insert(name);
      OLMsg::recordFullName(name);
      get(strings, name);
      if(strings.empty()){
	strings.resize(1);
	strings[0].setName(name);
      	if(arguments[0].empty()){
      	  strings[0].setReadOnly(true);
      	  strings[0].setNeverChanged(true);
      	}
	else
	  val=resolveGetVal(arguments[0]);
	strings[0].setValue(val);
      }
      // choices list is reset
      std::vector<std::string> choices;
      strings[0].setChoices(choices);

      if(arguments.size()>2) strings[0].setLabel(unquote(arguments[2]));
      set(strings[0]);
    }
    else if(!action.compare("radioButton")) {
      // syntax: paramName.radioButton(val,path,label)
      double val=0;
      if(arguments[0].empty())
	OLMsg::Error("No value given for param <%s>",name.c_str());
      else
	val=atof(arguments[0].c_str());
      if(arguments.size()>1)
	name.assign(FixOLPath(arguments[1]) + name);
      _parameters.insert(name);
      OLMsg::recordFullName(name);
      get(numbers, name);
      if(numbers.size()){
	val = numbers[0].getValue(); // use value from server
      }
      else{
	numbers.resize(1);
	numbers[0].setName(name);
	numbers[0].setValue(val);
      }
      if(arguments.size()>2)
	numbers[0].setLabel(unquote(arguments[2]));
      std::vector<double> choices;
      choices.push_back(0);
      choices.push_back(1);
      numbers[0].setChoices(choices);
      set(numbers[0]);
    }
    else if(!action.compare("range")){
      // set the range of an existing number
      // syntax: paramName.range({a:b:c|a:b#n|min,max,step})
      if(arguments[0].empty())
	OLMsg::Error("No argument given for MinMax <%s>",name.c_str());
      else{
	name.assign(longName(name));
	get(numbers,name);
	if(numbers.size()){ // parameter must exist
	  if(arguments.size()==1){
	    std::vector<double> bounds;
	    if (resolveRange(arguments[0],bounds)){
	      numbers[0].setMin(bounds[0]);
	      numbers[0].setMax(bounds[1]);
	      numbers[0].setStep(bounds[2]);
	    }
	  }
	  else if(arguments.size()==3){
	    numbers[0].setMin(atof(arguments[0].c_str()));
	    numbers[0].setMax(atof(arguments[1].c_str()));
	    numbers[0].setStep(atof(arguments[2].c_str()));
	  }
	  else
	    OLMsg::Error("Wrong number of arguments for range <%s>",
			 name.c_str());
	  set(numbers[0]);
	}
	else
	  OLMsg::Error("The parameter <%s> does not exist",name.c_str());
      }
    }
    else if(!action.compare("withinRange")){
      // ensure the value is in the prescribed range
      name.assign(longName(name));
      get(numbers,name);
      if(numbers.size()){ // parameter must exist
	if( (numbers[0].getMin() != -onelab::parameter::maxNumber()) &&
	    (numbers[0].getValue() < numbers[0].getMin()) )
	  numbers[0].setValue(numbers[0].getMin());
	if( (numbers[0].getMax() != onelab::parameter::maxNumber()) &&
	    (numbers[0].getValue() > numbers[0].getMax()) )
	  numbers[0].setValue(numbers[0].getMax());
	set(numbers[0]);
      }
      else
	OLMsg::Error("The parameter <%s> does not exist",name.c_str());
    }
    else if(!action.compare("setValue")){
      // a set request together with a setReadOnly(1) forces
      // the value on server to be changed.
      name.assign(longName(name));
      get(numbers,name);
      if(numbers.size()){
	if(arguments[0].size())
	  numbers[0].setValue(atof(resolveGetVal(arguments[0]).c_str()));
	numbers[0].setReadOnly(1);
	set(numbers[0]);
      }
      else{
	get(strings,name);
	if(strings.size()){
	  if(arguments[0].empty())  // resets an empty string
	    strings[0].setValue("");
	  else
	    strings[0].setValue(arguments[0]);
	  strings[0].setReadOnly(1);
	  set(strings[0]);
	}
	else
	  OLMsg::Error("The parameter <%s> does not exist",name.c_str());
      }
    }
    else if(!action.compare("resetChoices")){
      name.assign(longName(name));
      get(numbers,name);

      if(numbers.size()){ // parameter must exist
	std::vector<double> choices;
	numbers[0].setChoices(choices);
	std::map<double, std::string> valuelabels;
	numbers[0].setValueLabels(valuelabels);
	set(numbers[0]);
      }
      else{
	get(strings,name);
	if(strings.size()){
	  std::vector<std::string> choices;
	  strings[0].setChoices(choices);
	  set(strings[0]);
	}
	else{
	  OLMsg::Error("The parameter <%s> does not exist",name.c_str());
	}
      }
    }
    else if(!action.compare("addChoices")){
      name.assign(longName(name));
      get(numbers,name);
      if(numbers.size()){ // parameter must exist
	std::vector<double> choices=numbers[0].getChoices();
	for(unsigned int i = 0; i < arguments.size(); i++){
	  double val=atof(resolveGetVal(arguments[i]).c_str());
	  //if(std::find(choices.begin(),choices.end(),val)==choices.end())
	  choices.push_back(val);
	}
	numbers[0].setChoices(choices);
	set(numbers[0]);
      }
      else{
	get(strings,name);
	if(strings.size()){
	  std::vector<std::string> choices=strings[0].getChoices();
	  for(unsigned int i = 0; i < arguments.size(); i++)
	    if(std::find(choices.begin(),choices.end(),
			 arguments[i])==choices.end())
	      choices.push_back(arguments[i]);
	  strings[0].setChoices(choices);
	  set(strings[0]);
	}
	else{
	  OLMsg::Error("The parameter <%s> does not exist",name.c_str());
	}
      }
    }
    else if(!action.compare("valueLabels")){
      name.assign(longName(name));
      get(numbers,name);
      if(numbers.size()){ // parameter must exist
	if(arguments.size() % 2){
	  OLMsg::Error("Nb of labels does not match nb of choices for <%s>",
		       name.c_str());
	}
	else{
	  std::vector<double> choices=numbers[0].getChoices();
	  for(unsigned int i = 0; i < arguments.size(); i=i+2){
	    double val=atof(resolveGetVal(arguments[i]).c_str());
	    if(std::find(choices.begin(),choices.end(),val)==choices.end())
	      choices.push_back(val);
	    numbers[0].setValueLabel(val,unquote(arguments[i+1]));
	  }
	  numbers[0].setChoices(choices);
	  set(numbers[0]);
	}
      }
      else
	OLMsg::Error("The number <%s> does not exist",name.c_str());
    }
    else if(!action.compare("setVisible")){
      if(arguments[0].empty())
	OLMsg::Error("Missing argument SetVisible <%s>",name.c_str());
      else{
	name.assign(longName(name));
	get(numbers,name);
	if(numbers.size()){
	  numbers[0].setVisible(atof(resolveGetVal(arguments[0]).c_str()));
	  set(numbers[0]);
	}
	else{
	  get(strings,name);
	  if(strings.size()){
	    strings[0].setVisible(atof(resolveGetVal(arguments[0]).c_str()));
	    set(strings[0]);
	  }
	  else{
	    OLMsg::Error("The parameter <%s> does not exist",name.c_str());
	  }
	}
      }
    }
    else if(!action.compare("setReadOnly")){
      if(arguments[0].empty())
	OLMsg::Error("Missing argument SetReadOnly <%s>",name.c_str());
      else{
	name.assign(longName(name));
	get(numbers,name);
	if(numbers.size()){
	  numbers[0].setReadOnly(atof(resolveGetVal(arguments[0]).c_str()));
	  set(numbers[0]);
	}
	else{
	  get(strings,name);
	  if(strings.size()){
	    strings[0].setReadOnly(atof(resolveGetVal(arguments[0]).c_str()));
	    set(strings[0]);
	  }
	  else{
	    OLMsg::Error("The parameter <%s> does not exist",name.c_str());
	  }
	}
      }
    }
    else if(!action.compare("layout")){
      name.assign(longName(name));
      get(numbers,name);
      if(numbers.size()){
	numbers[0].setReadOnly(0);
	numbers[0].setAttribute("Highlight","Ivory");
	set(numbers[0]);
      }
      else{
	get(strings,name);
	if(strings.size()){
	  strings[0].setReadOnly(0);
	  strings[0].setAttribute("Highlight","Ivory");
	  set(strings[0]);
	}
	else{
	  OLMsg::Error("The parameter <%s> does not exist",name.c_str());
	}
      }
    }
    else if(!action.compare("setAttribute")){
      if(arguments.size() !=2 )
	OLMsg::Error("SetAttribute <%s> needs two arguments %d",
		   name.c_str(), arguments.size());
      else{
	name.assign(longName(name));
	get(numbers,name);
	if(numbers.size()){
	  numbers[0].setAttribute(arguments[0].c_str(),
				  resolveGetVal(arguments[1]).c_str());
	  set(numbers[0]);
	}
	else{
	  get(strings,name);
	  if(strings.size()){
	    strings[0].setAttribute(arguments[0].c_str(),arguments[1].c_str());
	    set(strings[0]);
	  }
	  else{
	    OLMsg::Error("The parameter <%s> does not exist",name.c_str());
	  }
	}
      }
    }
    else{
      client_sentence(name,action,arguments);
    }
    cursor=pos+1;
  }
  // check whether
  if((line.find_first_not_of(" \t",cursor)) != std::string::npos){
    OLMsg::Error("Syntax error in <%s> (forgot a %s ?)",
  		 line.substr(cursor).c_str(),olkey::separator.c_str());
  }
}

void localSolverClient::modify_tags(const std::string lab, const std::string com){
  bool changed=false;
  if(lab.compare(olkey::label) && lab.size()){
    changed=true;
    olkey::label.assign(lab);
    olkey::line.assign(olkey::label+"line");
    olkey::begin.assign(olkey::label+"block");
    olkey::end.assign(olkey::label+"endblock");
    olkey::include.assign(olkey::label+"include");
    olkey::message.assign(olkey::label+"msg");
    olkey::showParam.assign(olkey::label+"show");
    olkey::showGmsh.assign(olkey::label+"merge");
    olkey::dump.assign(olkey::label+"dump");
    olkey::ifcond.assign(olkey::label+"if");
    olkey::iftrue.assign(olkey::label+"iftrue");
    olkey::ifntrue.assign(olkey::label+"ifntrue");
    olkey::olelse.assign(olkey::label+"else");
    olkey::olendif.assign(olkey::label+"endif");
    olkey::getValue.assign(olkey::label+"get");
    olkey::mathex.assign(olkey::label+"eval");
  }
  if(com.compare(olkey::comment) && com.size()){
    changed=true;
    olkey::comment.assign(com);
  }

  if(changed)
    OLMsg::Info("Using now onelab tags <%s,%s>",
	      olkey::label.c_str(), olkey::comment.c_str());
}

void localSolverClient::parse_oneline(std::string line, std::ifstream &infile) {
  size_t pos,cursor;
  std::vector<std::string> arguments;
  std::vector<onelab::number> numbers;
  std::vector<onelab::string> strings;

  if((pos=line.find_first_not_of(" \t"))==std::string::npos){
    // empty line, skip
  }
  else if(!line.compare(pos,olkey::comment.size(),olkey::comment)){
    // commented out line, skip
  }
  else if ( (pos=line.find(olkey::deflabel)) != std::string::npos){
    // onelab.tags(label,comment);
    // onelab.tags(); -> reset to default
    cursor = pos+olkey::deflabel.length();
    int NumArg=enclosed(line.substr(cursor),arguments,pos);
    if((NumArg==1) && arguments[0].empty())
      modify_tags("","");
    else if(NumArg==2)
      modify_tags(arguments[0],arguments[1]);
    else
      OLMsg::Error("Misformed <%s> statement", olkey::deflabel.c_str());
  }
  else if( (pos=line.find(olkey::begin)) != std::string::npos) {
    // onelab.block
    if (!parse_block(infile))
      OLMsg::Error("Misformed <%s> block <%s>",
		 olkey::begin.c_str(),olkey::end.c_str());
  }
  else if ( (pos=line.find(olkey::iftrue)) != std::string::npos) {
    // onelab.iftrue
    cursor = pos+olkey::iftrue.length();
    bool condition = false;
    if(enclosed(line.substr(cursor),arguments,pos)<1)
      OLMsg::Error("Misformed <%s> statement: (%s)",
		 olkey::iftrue.c_str(),line.c_str());
    else{
      get(strings,longName(arguments[0]));
      if (strings.size())
	condition= true;
      else{
	get(numbers,longName(arguments[0]));
	if (numbers.size())
	  condition = (bool) numbers[0].getValue();
	else
	  OLMsg::Error("Unknown parameter <%s> in <%s> statement",
		       arguments[0].c_str(),olkey::iftrue.c_str());
      }
      if (!parse_ifstatement(infile,condition))
	OLMsg::Error("Misformed <%s> statement: <%s>",
		     olkey::iftrue.c_str(),arguments[0].c_str());
    }
  }
  else if ( (pos=line.find(olkey::ifntrue)) != std::string::npos) {
    // onelab.ifntrue
    cursor = pos+olkey::ifntrue.length();
    bool condition = false;
    if(enclosed(line.substr(cursor),arguments,pos)<1)
      OLMsg::Error("Misformed <%s> statement: (%s)",
		 olkey::ifntrue.c_str(),line.c_str());
    else{
      get(strings,longName(arguments[0]));
      if (strings.size())
	condition= true;
      else{
	get(numbers,longName(arguments[0]));
	if (numbers.size())
	  condition = (bool) numbers[0].getValue();
	else{
	  condition=false;
	  // OLMsg::Warning("Unknown parameter <%s> in <%s> statement",
	  // 	       arguments[0].c_str(),olkey::ifntrue.c_str());
	}
      }
      if (!parse_ifstatement(infile,!condition))
	OLMsg::Error("Misformed <%s> statement: <%s>",
		     olkey::ifntrue.c_str(),arguments[0].c_str());
    }
  }
  else if ( (pos=line.find(olkey::ifcond)) != std::string::npos) {
    // onelab.ifcond
    cursor = pos+olkey::ifcond.length();
    extractLogic(line.substr(cursor),arguments);
    bool condition= resolveLogicExpr(arguments);
    if (!parse_ifstatement(infile,condition)){
      OLMsg::Error("Misformed %s statement: <%s>",
		 olkey::ifcond.c_str(), line.c_str());
    }
  }
  else if ( (pos=line.find(olkey::include)) != std::string::npos) {
    // onelab.include
    cursor = pos+olkey::include.length();
    if(enclosed(line.substr(cursor),arguments,pos)<1)
      OLMsg::Error("Misformed <%s> statement: (%s)",
		 olkey::include.c_str(),line.c_str());
    else{
      std::string filename = getWorkingDir() + resolveGetVal(arguments[0]);
      OLMsg::Info("Parse file <%s> %s", filename.c_str(),
		  parse_onefile(filename)?"done":"failed");
    }
  }
  else if ( (pos=line.find(olkey::message)) != std::string::npos) {
    // onelab.message
    cursor = pos+olkey::message.length();
    if(enclosed(line.substr(cursor),arguments,pos)<1)
      OLMsg::Error("Misformed <%s> statement: (%s)",
		 olkey::message.c_str(),line.c_str());
    else{
      std::string msg = resolveGetVal(arguments[0]);
      OLMsg::Info("%s",msg.c_str());
    }
  }
  else if ( (pos=line.find(olkey::showParam)) != std::string::npos) {
    // onelab.showParam
    cursor = pos+olkey::showParam.length();
    if(enclosed(line.substr(cursor),arguments,pos)<1)
      OLMsg::Error("Misformed <%s> statement: (%s)",
		   olkey::showParam.c_str(),line.c_str());
    for(unsigned int i = 0; i < arguments.size(); i++){
      std::string lname=longName(arguments[i]);
      std::string msg;
      get(numbers,lname);
      if (numbers.size())
	msg.assign(numbers[0].toChar());
      else{
	get(strings,lname);
	if (strings.size())
	  msg.assign(strings[0].toChar());
	else
	  OLMsg::Error("Unknown parameter <%s> in <%s> statement",
		       arguments[i].c_str(),olkey::showParam.c_str());
      }
      for(unsigned int j = 0; j < msg.size(); j++)
	if(msg[j] == onelab::parameter::charSep()) msg[j] = '|';
      OLMsg::Info("%s",msg.c_str());
    }
  }
  else if ( (pos=line.find(olkey::showGmsh)) != std::string::npos) {
    // onelab.showGmsh
    cursor = pos+olkey::showGmsh.length();
    if(enclosed(line.substr(cursor),arguments,pos)<1)
      OLMsg::Error("Misformed <%s> statement: (%s)",
		   olkey::showGmsh.c_str(),line.c_str());
    else{
      std::string fileName=resolveGetVal(arguments[0]);
      OLMsg::MergeFile(getWorkingDir() + fileName);
    }
  }
  else if ( (pos=line.find(olkey::dump)) != std::string::npos) {
    // onelab.dump
    cursor = pos+olkey::dump.length();
    if(enclosed(line.substr(cursor),arguments,pos)<1){
      OLMsg::Error("Misformed <%s> statement: (%s)",
      		 olkey::dump.c_str(),line.c_str());
    }
    else{
      FILE *fp = fopen(resolveGetVal(arguments[0]).c_str(), "wb");
      if(fp){
        onelab::server::instance()->toFile(fp);
        fclose(fp);
      }
    }
  }
  else if( isOnelabBlock() ||
	 ( !isOnelabBlock() &&
	   ((pos=line.find(olkey::line)) != std::string::npos)) ){
    // either any line with no "OL." within a onelabBlock or a line
    // introduced by a "onelab.line" tag not within a onelabBlock
    std::string cmds="",cmd;
    size_t posa, posb;
    int NbLines = 1;
    bool err = false, terminated = false;
    do{
      // skip tag 'olkey::line' if any
      if( (pos=line.find(olkey::line)) != std::string::npos)
	posa=pos + olkey::line.size();
      else
	posa=0;

      // skip trailing comments if any
      posb=line.find(olkey::comment);
      if(posb==std::string::npos)
	cmd.assign(line.substr(posa));
      else
	cmd.assign(line.substr(posa,posb-posa));

      cmds.append(cmd);

      // check whether "cmd" ends with "olkey::separator"
      pos=cmd.find_last_of(olkey::separator);
      terminated = (pos != std::string::npos);

      //std::cout << "cmds=<" << cmds << ">" << terminated << std::endl;

      if(!terminated){
	// not found olkey::separator => append the next nonempty line
	while(infile.good()){
	  getline (infile,line);
	  NbLines++; // command should not span over more than 20 nonempty lines
	  if(line.find_first_not_of(" \t") != std::string::npos)
	    break;
	}

	if(infile.good()){
	  // check that no OL. commands is found except OL.get or OL.eval
	  if((pos=line.find(olkey::getValue)) != std::string::npos){
	    err = false;
	  }
	  else if((pos=line.find(olkey::mathex)) != std::string::npos){
	    err = false;
	  }
	  else if((pos=line.find(olkey::end)) != std::string::npos){
	    err = true;
	    closeOnelabBlock();
	  }
	  else if((pos=line.find(olkey::label)) != std::string::npos){
	    err = true;
	  }
	}
      }
    } while (infile.good() && !err && !terminated && NbLines <= 20);

    if(!terminated){
      if(NbLines >= 20)
	OLMsg::Error("Command <%s> should not span over more than 20 lines",
		     cmds.c_str());
      else
	OLMsg::Error("Unterminated command <%s>", cmds.c_str());
      return;
    }
    else
      parse_sentence(cmds);
  }
  else if ( (pos=line.find(olkey::getValue)) != std::string::npos) {
    // onelab.getValue: nothing to do
  }
  else if ( (pos=line.find(olkey::mathex)) != std::string::npos) {
    // onelab.mathex: nothing to do
  }
  else if( (pos=line.find(olkey::label)) != std::string::npos) {
      OLMsg::Error("Unknown ONELAB keyword in <%s>",line.c_str());
  }
  else{
    // not a onelab line, skip
  }
}

bool localSolverClient::parse_block(std::ifstream  &infile) {
  size_t pos;
  std::string line;
  openOnelabBlock();
  while (infile.good()){
    getline (infile,line);
    if ((pos=line.find_first_not_of(" \t")) == std::string::npos)
      continue; // skip empty line
    if ((pos=line.find(olkey::end)) != std::string::npos){
      closeOnelabBlock();
      return true;
    }
    parse_oneline(line,infile);
  }
  return false;
}

bool localSolverClient::parse_ifstatement(std::ifstream &infile,
					  bool condition) {
  int level;
  size_t pos;
  std::string line;

  bool trueclause=true;
  level=1;
  while ( infile.good() && level) {
    getline (infile,line);
    if ( ((pos=line.find(olkey::olelse)) != std::string::npos) && (level==1) )
      trueclause=false;
    else if ( (pos=line.find(olkey::olendif)) != std::string::npos)
      level--;
    else if ( !(trueclause ^ condition) ) // xor bitwise operator
      parse_oneline(line,infile);
    else { // check for opening if statements
      if ( (pos=line.find(olkey::iftrue)) != std::string::npos)
	level++;
      else if ( (pos=line.find(olkey::ifntrue)) != std::string::npos)
	level++;
      else if ( (pos=line.find(olkey::ifcond)) != std::string::npos)
	level++;
    }
  }
  return level?false:true ;
}

bool localSolverClient::parse_onefile(std::string fileName, bool mandatory) {
  std::ifstream infile(fileName.c_str());
  if (infile.is_open()){
    while (infile.good()){
      std::string line;
      getline(infile,line);
      parse_oneline(line,infile);
    }
    infile.close();
    return true;
  }
  else{
    return !mandatory;
    // if(mandatory)
    //   OLMsg::Error("The file <%s> does not exist",fileName.c_str());
    // else
    //   OLMsg::Warning("The file <%s> does not exist",fileName.c_str());
  }
}

bool localSolverClient::convert_ifstatement(std::ifstream &infile, std::ofstream &outfile, bool condition) {
  int level;
  size_t pos;
  std::string line;

  bool trueclause=true;
  level=1;
  while ( infile.good() && level) {
    getline (infile,line);
    if ( ((pos=line.find(olkey::olelse)) != std::string::npos) && (level==1) )
      trueclause=false;
    else if ( (pos=line.find(olkey::olendif)) != std::string::npos)
     level--;
    else if ( !(trueclause ^ condition) ) // xor bitwise operator
      convert_oneline(line,infile,outfile);
    else { // check for opening if statements
      if ( (pos=line.find(olkey::iftrue)) != std::string::npos)
	level++;
      else if ( (pos=line.find(olkey::ifntrue)) != std::string::npos)
	level++;
      else if ( (pos=line.find(olkey::ifcond)) != std::string::npos)
	level++;
    }
  }
  return level?false:true ;
}

void localSolverClient::convert_oneline(std::string line, std::ifstream &infile, std::ofstream &outfile) {
  size_t pos,cursor;
  std::vector<std::string> arguments;
  std::vector<onelab::number> numbers;
  std::vector<onelab::string> strings;

  if((pos=line.find_first_not_of(" \t"))==std::string::npos){
    // empty line, we keep them
    outfile << line << std::endl;
  }
  else if(!line.compare(pos,olkey::comment.size(),olkey::comment)){
    // commented out, skip the line
  }
  else if ( (pos=line.find(olkey::deflabel)) != std::string::npos){
    // onelab.tags(label,comment,separator)
    cursor = pos+olkey::deflabel.length();
    int NumArg=enclosed(line.substr(cursor),arguments,pos);
    if(NumArg==0)
      modify_tags("","");
    else if(NumArg==2)
      modify_tags(arguments[0],arguments[1]);
    else
      OLMsg::Error("Misformed <%s> statement", olkey::deflabel.c_str());
  }
  else if( (pos=line.find(olkey::begin)) != std::string::npos) {
    // onelab.begin
    while (infile.good()){
      getline (infile,line);
      if( (pos=line.find(olkey::end)) != std::string::npos) return;
    }
    OLMsg::Error("Misformed <%s> block <%s>",
	       olkey::begin.c_str(),olkey::end.c_str());
  }
  else if ( (pos=line.find(olkey::iftrue)) != std::string::npos) {
    // onelab.iftrue
    cursor = pos+olkey::iftrue.length();
    bool condition = false;
    if(enclosed(line.substr(cursor),arguments,pos)<1)
      OLMsg::Error("Misformed <%s> statement: (%s)",
		 olkey::iftrue.c_str(),line.c_str());
    else{
      get(strings,longName(arguments[0]));
      if (strings.size())
	condition = strings[0].getValue().size();
      else{
	get(numbers,longName(arguments[0]));
	if (numbers.size())
	  condition = (bool) numbers[0].getValue();
      	else{
      	  OLMsg::Warning("Unknown parameter <%s> in <%s> statement",
			 arguments[0].c_str(),olkey::iftrue.c_str());
      	}
      }
      if (!convert_ifstatement(infile,outfile,condition))
	OLMsg::Error("Misformed <%s> statement: %s",
		     olkey::iftrue.c_str(),arguments[0].c_str());
    }
  }
  else if ( (pos=line.find(olkey::ifntrue)) != std::string::npos) {
    // onelab.ifntrue
    cursor = pos+olkey::ifntrue.length();
    bool condition = false;
    if(enclosed(line.substr(cursor),arguments,pos)<1)
      OLMsg::Error("Misformed <%s> statement: (%s)",
		 olkey::ifntrue.c_str(),line.c_str());
    else{
      get(strings,longName(arguments[0]));
      if (strings.size())
	condition =  strings[0].getValue().size();
      else{
	get(numbers,longName(arguments[0]));
	if (numbers.size())
	  condition = (bool) numbers[0].getValue();
	else{
	  OLMsg::Warning("Unknown parameter <%s> in <%s> statement",
			 arguments[0].c_str(),olkey::ifntrue.c_str());
	}
      }
      if (!convert_ifstatement(infile,outfile,!condition))
	OLMsg::Error("Misformed <%s> statement: %s",
		     olkey::ifntrue.c_str(),arguments[0].c_str());
    }
  }
  else if ( (pos=line.find(olkey::ifcond)) != std::string::npos) {
    // onelab.ifcond
    cursor = pos+olkey::ifcond.length();
    extractLogic(line.substr(cursor),arguments);
    bool condition= resolveLogicExpr(arguments);
    if (!convert_ifstatement(infile,outfile,condition))
      OLMsg::Error("Misformed %s statement: <%s>",
		   olkey::ifcond.c_str(),line.c_str());
  }
  else if ( (pos=line.find(olkey::include)) != std::string::npos) {
    // onelab.include
    cursor = pos+olkey::include.length();
    if(enclosed(line.substr(cursor),arguments,pos)<1)
      OLMsg::Error("Misformed <%s> statement: (%s)",
		 olkey::include.c_str(),line.c_str());
    else{
      std::string filename = getWorkingDir() + resolveGetVal(arguments[0]);
      convert_onefile(filename, outfile);
    }
  }
  else if ( (pos=line.find(olkey::message)) != std::string::npos) {
    // onelab.message
    cursor = pos+olkey::message.length();
    if(enclosed(line.substr(cursor),arguments,pos)<1)
      OLMsg::Error("Misformed <%s> statement: (%s)",
		 olkey::message.c_str(),line.c_str());
    else{
      std::string msg = resolveGetVal(arguments[0]);
      OLMsg::Info("%s",msg.c_str());
    }
  }
  else if ( (pos=line.find(olkey::getValue)) != std::string::npos) {
    outfile << resolveGetVal(line) << std::endl;
  }
  else if ( (pos=line.find(olkey::label)) != std::string::npos){
    OLMsg::Error("Unidentified onelab command in <%s>",line.c_str());
  }
  else{
    outfile << line << std::endl;
  }
}

void preProcess(const std::string &client, const std::string &fullName){
  std::vector<std::string> split = SplitOLFileName(fullName);
  std::string ifileName = split[1] + split[2] ; // remove heading "_" if any
  std::string ofileName = split[0] + split[1] ; // remove trailing ".ol"

  std::string workDir = SplitFileName(split[1]) [0];
  localSolverClient *c = new InterfacedClient(client, "", workDir);

  std::ifstream  infile(ifileName.c_str());
  if( infile.is_open()) {
    std::ofstream outfile(ofileName.c_str());
    if(outfile.is_open()) {
      OLMsg::Info("Preprocess file <%s> into <%s>",ifileName.c_str(), ofileName.c_str());
      while ( infile.good() ) {
	std::string line;
	getline (infile,line);
	c->convert_oneline(line,infile,outfile);
      }
      outfile.close();
    }
    else
      OLMsg::Error("The file <%s> cannot be opened",ofileName.c_str());
    infile.close();
  }
  else
    OLMsg::Error("The file <%s> cannot be opened",ifileName.c_str());
  delete c;
}

void localSolverClient::convert_onefile(std::string fileName, std::ofstream &outfile) {
  std::ifstream infile(fileName.c_str());
  if (infile.is_open()){
    OLMsg::Info("Convert file <%s>",fileName.c_str());
    while ( infile.good() ) {
      std::string line;
      getline (infile,line);
      convert_oneline(line,infile,outfile);
    }
    infile.close();
  }
  else
    OLMsg::Error("The file <%s> cannot be opened",fileName.c_str());
}

void localSolverClient::client_sentence(const std::string &name,
					const std::string &action,
		       const std::vector<std::string> &arguments) {
  OLMsg::Error("The action <%s> is unknown in this context",action.c_str());
}

void MetaModel::client_sentence(const std::string &name,
				const std::string &action,
		 const std::vector<std::string> &arguments){
  std::vector<onelab::string> strings;

  if(!action.compare("register")){
    if(isTodo(REGISTER)){
      std::string type="",cmdl="",host="",rdir="";
      // syntax name.register([interf...|native]{,cmdl}) ;
      if(!findClientByName(name)){
	OLMsg::Info("Define client <%s>", name.c_str());
	if(arguments.size()>=1) type.assign(resolveGetVal(arguments[0]));
	if(arguments.size()>=2) cmdl.assign(resolveGetVal(arguments[1]));
	if(arguments.size()>=3)
	  OLMsg::Warning("Unused arguments for client <%s>", name.c_str());

	// if argument 'cmdl' is empty,
	// 1. look on server for one remote host cmdl
	//    defined by a previous .remote() sentence
	// 2. look in the .save file for a local host cmdl
	// 3. create an empty parameter restore control to the GUI

	host = OLMsg::GetOnelabString(name + "/HostName");
	rdir = OLMsg::GetOnelabString(name + "/RemoteDir");
	if(cmdl.empty())
	  cmdl = OLMsg::GetOnelabString(name + "/CommandLine");

	if(cmdl.empty()) {
	  host="localhost";
	  if(findCommandLine(name,host))
	    cmdl = OLMsg::GetOnelabString(name + "/CommandLine");
	}
	if(cmdl.empty()) {
	  if(OLMsg::hasGmsh){
	    onelab::string str;
	    str.setName(name + "/CommandLine");
	    str.setKind("file");
	    str.setVisible(true);
	    str.setAttribute("Highlight","Ivory");
	    set(str);
	    OLMsg::Error("No commandline found for client <%s>",
	  		 name.c_str());
	  }
	  else{ // asks the user in console mode
	    std::cout << "\nONELAB: Enter pathname of the executable file for <" << name << ">" << std::endl;
	    std::getline (std::cin,cmdl);
	    OLMsg::SetOnelabString(name + "/CommandLine",cmdl);
	  }
	}
	registerClient(name,type,cmdl,host,rdir);
      }
      else
	OLMsg::Error("Redefinition of client <%s>", name.c_str());
    }
  }
  else if(!action.compare("remote") || !action.compare("hostname")){
    if(isTodo(REGISTER)){
      std::string host="",rdir="";
      if(arguments.size()>=1) host.assign(resolveGetVal(arguments[0]));
      if(arguments.size()>=2) rdir.assign(resolveGetVal(arguments[1]));
      if(arguments.size()>=3)
	  OLMsg::Warning("Unused arguments for client <%s>", name.c_str());

      if(host.size()){
	OLMsg::SetOnelabString(name + "/HostName", host, false);
	if(rdir.size())
	  OLMsg::SetOnelabString(name + "/RemoteDir", rdir, false);
      }
      else{
	std::string in = OLMsg::GetOnelabString(name + "/HostName");
	if(in.size()){
	  std::vector<std::string> split = SplitOLHostName(in);
	  host = split[0];
	  rdir = split[1];
	  OLMsg::SetOnelabString(name + "/HostName", host, false);
	  if(rdir.size())
	    OLMsg::SetOnelabString(name + "/RemoteDir", rdir, false);
	}
	if(!findCommandLine(name,host)){
	  if(OLMsg::hasGmsh){
	    onelab::string str;
	    str.setName(name + "/HostName");
	    str.setVisible(true);
	    str.setAttribute("Highlight","Ivory");
	    set(str);
	    OLMsg::Error("No hostname found for remote client <%s>",name.c_str());
	  }
	  else{ // asks the user in console mode
	    std::cout << "\nONELAB: Enter remote host for <" << name << "> (name@host:dir)" << std::endl;
	    std::string in;
	    std::getline (std::cin,in);
	    if(in.size()){
	      std::vector<std::string> split = SplitOLHostName(in);
	      OLMsg::SetOnelabString(name + "/HostName", split[0], false);
	      if(split[1].size())
		OLMsg::SetOnelabString(name + "/RemoteDir", split[1], false);
	    }
	  }
	}
      }
    }
  }
  else if(!action.compare("workingSubdir")){
    localSolverClient *c;
    if((c=findClientByName(name)))
      c->setWorkingDir(c->getWorkingDir() + arguments[0]);
    else
      OLMsg::Error("Unknown client <%s>", name.c_str());
  }
  else if(!action.compare("active")){
    localSolverClient *c;
    if(arguments[0].size()){
      if((c=findClientByName(name))){
	c->setActive(atof( resolveGetVal(arguments[0]).c_str() ));
	//onelab::server::instance()->setChanged(true, c->getName());
      }
      else
	OLMsg::Error("Unknown client <%s>", name.c_str());
    }
    else
      OLMsg::Error("No argument for <%s.Active> statement", name.c_str());
  }
  else if(!action.compare("in")){
    if(isTodo(REGISTER)){
      get(strings, name + "/InputFiles");
      if(strings.empty()){
	strings.resize(1);
	strings[0].setName(name + "/InputFiles");
      }
      strings[0].setKind("file");
      strings[0].setVisible(false);
      std::vector<std::string> choices;
      if(arguments[0].size()){
	for(unsigned int i = 0; i < arguments.size(); i++){
	  std::string fileName=resolveGetVal(arguments[i]);
	  if(std::find(choices.begin(),choices.end(),fileName)==choices.end())
	  choices.push_back(fileName);
	}
	strings[0].setValue(resolveGetVal(arguments[0]));
      }
      strings[0].setChoices(choices);
      set(strings[0]);
    }
  }
  else if(!action.compare("out")){
    if(isTodo(REGISTER)){
      get(strings, name + "/OutputFiles");
      if(strings.empty()){
	strings.resize(1);
	strings[0].setName(name + "/OutputFiles");
      }
      strings[0].setKind("file");
      strings[0].setVisible(false);
      std::vector<std::string> choices;
      if(arguments[0].size()){
	for(unsigned int i = 0; i < arguments.size(); i++){
	  std::string fileName=resolveGetVal(arguments[i]);
	  if(std::find(choices.begin(),choices.end(),fileName)==choices.end())
	  choices.push_back(fileName);
	}
	strings[0].setValue(resolveGetVal(arguments[0]));
      }
      strings[0].setChoices(choices);
      set(strings[0]);
    }
  }
  else if(!action.compare("run")){
    if(isTodo(REGISTER)){
      if(arguments[0].size()){
	get(strings, name + "/Arguments");
	if(strings.empty()){
	  strings.resize(1);
	  strings[0].setName(name + "/Arguments");
	}
	strings[0].setValue(resolveGetVal(arguments[0]));
	strings[0].setVisible(false);
	set(strings[0]);
      }
      if(!OLMsg::GetErrorCount()){
	localSolverClient *c;
	if((c=findClientByName(name)))
	  if(c->checkCommandLine())
	    c->analyze();
      }
    }
    else if(isTodo(ANALYZE)){
      localSolverClient *c;
      if((c=findClientByName(name))) c->analyze();
    }
    else if(isTodo(COMPUTE)){
      localSolverClient *c;
      if((c=findClientByName(name))){

	//onelab::server::instance()->setChanged(false, getName());
	bool changed = onelab::server::instance()->getChanged(c->getName());
	bool started = isStarted(changed);

	// if(OLMsg::GetVerbosity())
	//   std::cout << c->getName() << " active="
	// 	    << c->getActive() << " changed="
	// 	    << changed << " started="
	// 	    << started << " errors=" << OLMsg::GetErrorCount() << std::endl;
	if(c->getActive() || started) c->compute();
      }
    }
  }
  else if(!action.compare("up")){
    if(arguments.size()%4==0){
      if(isTodo(REGISTER)){
      }
      else if(isTodo(COMPUTE)  && !OLMsg::GetErrorCount()){
	std::vector<std::string> choices;
	for(unsigned int i = 0; i < arguments.size(); i++){
	  std::string str=resolveGetVal(arguments[i]);
	  OLMsg::recordFullName(str);
	  choices.push_back(str);
	}
	localSolverClient *c;
	if((c=findClientByName(name))) c->PostArray(choices);
      }
    }
    else
      OLMsg::Error("Wrong number of arguments <%d> for <%s>",
		 arguments.size(), action.c_str());
  }
  else if(!action.compare("alwaysCompute") || !action.compare("preCompute")){
    if(isTodo(REGISTER)){
      localSolverClient *c;
      if((c=findClientByName(name))){
	c->compute();
	//onelab::server::instance()->setChanged(false, c->getName());
      }
      else
	OLMsg::Error("Unknown client <%s>", name.c_str());
    }
  }
  else if(!action.compare("merge")){
    if( arguments.size() && isTodo(COMPUTE) && !OLMsg::GetErrorCount() && (OLMsg::hasGmsh)){
      std::vector<std::string> choices;
      for(unsigned int i = 0; i < arguments.size(); i++){
	choices.push_back(resolveGetVal(arguments[i]));
      }
      localSolverClient *c;
      if((c=findClientByName(name))) {
	c->GmshMerge(choices);
	OLMsg::SetOnelabNumber("Gmsh/NeedReloadGeom",1,false);
      }
      else
	OLMsg::Error("Unknown client <%s>", name.c_str());
    }
  }
  else if(!action.compare("frontPage")){
    if( isTodo(REGISTER) && OLMsg::hasGmsh && arguments.size() && !OLMsg::GetErrorCount()){
      std::vector<std::string> choices;
      for(unsigned int i = 0; i < arguments.size(); i++){
	choices.push_back(resolveGetVal(arguments[i]));
      }
      localSolverClient *c;
      if((c=findClientByName(name))) {
	c->GmshMerge(choices);
	OLMsg::SetOnelabNumber("Gmsh/NeedReloadGeom",1,false);
      }
      else
	OLMsg::Error("Unknown client <%s>", name.c_str());
    }
  }
  else if(!action.compare("clientStatus")){
    showClientStatus();
  }
  else
    OLMsg::Error("Unknown action <%s>", action.c_str());
}

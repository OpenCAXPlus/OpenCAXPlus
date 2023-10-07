// Gmsh - Copyright (C) 1997-2017 C. Geuzaine, J.-F. Remacle
//
// See the LICENSE.txt file for license information. Please report all
// bugs and problems to the public mailing list <gmsh@onelab.info>.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <algorithm>
#include "OnelabMessage.h"
#include "GmshSocket.h"
#include "OS.h"

#define ALWAYS_TRUE 1

//onelab::remoteNetworkClient *OLMsg::loader = 0;

int OLMsg::_commRank = 0;
int OLMsg::_commSize = 1;
int OLMsg::_verbosity = 4;
// int OLMsg::_progressMeterStep = 10;
// int OLMsg::_progressMeterCurrent = 0;
// std::map<std::string, double> OLMsg::_timers;
int OLMsg::_warningCount = 0;
int OLMsg::_errorCount = 0;
GmshMessage *OLMsg::_callback = 0;
std::string OLMsg::_commandLine;
std::string OLMsg::_launchDate;
GmshClient *OLMsg::_client = 0;
onelab::client *OLMsg::_onelabClient = 0;
bool OLMsg::hasGmsh=false;
std::set<std::string, fullNameLessThan> OLMsg::_fullNameDict;
void (*OLMsg::gui_wait_fct)(double time) = 0;

#if defined(_MSC_VER) && (_MSC_VER >= 1310) //NET 2003
#define vsnprintf _vsnprintf
#else
#if defined(HAVE_NO_VSNPRINTF)
static int vsnprintf(char *str, size_t size, const char *fmt, va_list ap)
{
  if(strlen(fmt) > size - 1){ // just copy the format
    strncpy(str, fmt, size - 1);
    str[size - 1] = '\0';
    return size;
  }
  return vsprintf(str, fmt, ap);
}
#endif
#endif



void OLMsg::Init(int argc, char **argv)
{
  time_t now;
  time(&now);
  _launchDate = ctime(&now);
  _launchDate.resize(_launchDate.size() - 1);
  _commandLine.clear();
  for(int i = 0; i < argc; i++){
    if(i) _commandLine += " ";
    _commandLine += argv[i];
  }
}

void OLMsg::Exit(int level)
{
  if(level){
    exit(level);
  }
  exit(_errorCount);
}

void OLMsg::Fatal(const char *fmt, ...)
{
  _errorCount++;

  char str[1024];
  va_list args;
  va_start(args, fmt);
  vsnprintf(str, sizeof(str), fmt, args);
  va_end(args);

  //if(_callback) (*_callback)("Fatal", str);
  //if(_client) _client->Error(str);

  if(_onelabClient && OLMsg::hasGmsh)
    _onelabClient->sendError(str);

  if(ALWAYS_TRUE){
    if(_commSize > 1)
      fprintf(stderr, "Fatal   : [On processor %d] %s\n", _commRank, str);
    else
      fprintf(stderr, "Fatal   : %s\n", str);
   fflush(stderr);
  }
  //FinalizeClient();
  FinalizeOnelab();
  //delete loader;
  Exit(1);
}

void OLMsg::Error(const char *fmt, ...)
{
  _errorCount++;

  if(_verbosity < 1) return;

  char str[1024];
  va_list args;
  va_start(args, fmt);
  vsnprintf(str, sizeof(str), fmt, args);
  va_end(args);

  //if(_callback) (*_callback)("Error", str);
  //if(_client) _client->Error(str);
  if(_onelabClient && OLMsg::hasGmsh)
    _onelabClient->sendError(str);

  if(ALWAYS_TRUE){
    if(_commSize > 1)
      fprintf(stderr, "Error   : [On processor %d] %s\n", _commRank, str);
    else
      fprintf(stderr, "Error   : %s\n", str);
    fflush(stderr);
  }
}

void OLMsg::Warning(const char *fmt, ...)
{
  _warningCount++;

  if(_commRank || _verbosity < 2) return;

  char str[1024];
  va_list args;
  va_start(args, fmt);
  vsnprintf(str, sizeof(str), fmt, args);
  va_end(args);

  //if(_callback) (*_callback)("Warning", str);
  //if(_client) _client->Warning(str);
  if(_onelabClient && OLMsg::hasGmsh)
    _onelabClient->sendWarning(str);

  if(ALWAYS_TRUE){
    fprintf(stderr, "Warning : %s\n", str);
    fflush(stderr);
  }
}

void OLMsg::Info(const char *fmt, ...)
{
  if(_commRank || _verbosity < 3) return;

  char str[1024];
  va_list args;
  va_start(args, fmt);
  vsnprintf(str, sizeof(str), fmt, args);
  va_end(args);

  //if(_callback) (*_callback)("Info", str);
  //if(_client) _client->Info(str);
  if(_onelabClient && OLMsg::hasGmsh)
    _onelabClient->sendInfo(str);

  if(ALWAYS_TRUE){
    fprintf(stdout, "Onelab  : %s\n", str);
    fflush(stdout);
  }
}

void OLMsg::Direct(const char *fmt, ...)
{
  if(_commRank || _verbosity < 3) return;

  char str[1024];
  va_list args;
  va_start(args, fmt);
  vsnprintf(str, sizeof(str), fmt, args);
  va_end(args);

  Direct(3, str);
}

void OLMsg::Direct(int level, const char *fmt, ...)
{
  if(_commRank || _verbosity < level) return;

  char str[1024];
  va_list args;
  va_start(args, fmt);
  vsnprintf(str, sizeof(str), fmt, args);
  va_end(args);

  //if(_callback) (*_callback)("Direct", str);
  //if(_client) _client->Info(str);
  if(_onelabClient && OLMsg::hasGmsh)
    _onelabClient->sendInfo(str);

  if(ALWAYS_TRUE){
    fprintf(stdout, "%s\n", str);
    fflush(stdout);
  }
}

void OLMsg::StatusBar(int num, bool log, const char *fmt, ...)
{
  if(_commRank || _verbosity < 3) return;
  if(num < 1 || num > 3) return;

  char str[1024];
  va_list args;
  va_start(args, fmt);
  vsnprintf(str, sizeof(str), fmt, args);
  va_end(args);

  //if(_callback && log) (*_callback)("Info", str);
  //if(_client && log) _client->Info(str);

  if(_onelabClient && OLMsg::hasGmsh)
    _onelabClient->sendInfo(str);

  if(log && ALWAYS_TRUE){
    fprintf(stdout, "Info    : %s\n", str);
    fflush(stdout);
  }
}

void OLMsg::InitializeOnelab(const std::string &name)
{
  if(_onelabClient) delete _onelabClient;
  _onelabClient = new onelab::localClient(name);
  OLMsg::hasGmsh = OLMsg::GetOnelabNumber("IsMetamodel");
}

void OLMsg::SetGuiWaitFunction(void (*fct)(double time)){
  gui_wait_fct = fct;
}
void (*OLMsg::GetGuiWaitFunction())(double){
  return gui_wait_fct;
}

double OLMsg::GetOnelabNumber(std::string name)
{
  if(_onelabClient){
    std::vector<onelab::number> ps;
    _onelabClient->get(ps, name);
    if(ps.size())
      return ps[0].getValue();
  }
  return 0;
}

void OLMsg::GetOnelabNumber(std::string name, double *val)
{
  if(_onelabClient){
    std::vector<onelab::number> ps;
    _onelabClient->get(ps, name);
    if(ps.size()){
      *val = ps[0].getValue();
      return;
    }
  }
  *val = 0;
}

void OLMsg::SetOnelabNumber(std::string name, double val, bool visible)
{
  if(_onelabClient){
    std::vector<onelab::number> numbers;
    _onelabClient->get(numbers, name);
    if(numbers.empty()){
      numbers.resize(1);
      numbers[0].setName(name);
    }
    numbers[0].setValue(val);
    numbers[0].setVisible(visible);
    _onelabClient->set(numbers[0]);
  }
}

std::string OLMsg::GetOnelabString(std::string name)
{
  std::string str="";
  if(_onelabClient){
    std::vector<onelab::string> ps;
    _onelabClient->get(ps, name);
    if(ps.size() && ps[0].getValue().size())
      str = ps[0].getValue();
  }
  return str;
}

bool OLMsg::GetOnelabChoices(std::string name, std::vector<std::string> &choices){
  if(_onelabClient){
    std::vector<onelab::string> ps;
    _onelabClient->get(ps, name);
    if(ps.size() && ps[0].getValue().size()){
      choices=ps[0].getChoices();
      return true;
    }
  }
  return false;
}

void OLMsg::SetOnelabString(std::string name, std::string val, bool visible)
{
  if(_onelabClient){
    std::vector<onelab::string> strings;
    _onelabClient->get(strings, name);
    if(strings.empty()){
      strings.resize(1);
      strings[0].setName(name);
    }
    strings[0].setValue(val);
    strings[0].setVisible(visible);
    strings[0].setAttribute("NotInDb","True");
    _onelabClient->set(strings[0]);
  }
}

void OLMsg::SetVisible(std::string name, bool visible)
{
  if(_onelabClient){
    std::vector<onelab::string> strings;
    _onelabClient->get(strings, name);
    if(strings.size()){
      strings[0].setVisible(visible);
      _onelabClient->set(strings[0]);
    }
  }
}

void OLMsg::SetOnelabAttributeString(std::string name,
				   std::string attrib,std::string val){
  if(_onelabClient){
    std::vector<onelab::string> ps;
    _onelabClient->get(ps, name);
    if(ps.size()){
      ps[0].setAttribute(attrib,val);
    }
  }
}
std::string OLMsg::GetOnelabAttributeString(std::string name,std::string attrib){
  std::string str="";
  if(_onelabClient){
    std::vector<onelab::string> ps;
    _onelabClient->get(ps, name);
    if(ps.size())
      str = ps[0].getAttribute(attrib);
  }
  return str;
}
std::string OLMsg::GetOnelabAttributeNumber(std::string name,std::string attrib){
  std::string str="";
  if(_onelabClient){
    std::vector<onelab::number> ps;
    _onelabClient->get(ps, name);
    if(ps.size())
      str = ps[0].getAttribute(attrib);
  }
  return str;
}

int fullNameLessThan::compareFullNames(const std::string a, const std::string b) const{
  std::string::const_iterator ita, itb;

  // Compares the strings a and b
  // One-digit numbers at the beginning of the string
  // or directly following the separator '/' are ignored

  ita = a.begin(); itb = b.begin();

  // ignore a possible initial one-digit number
  if( (ita < a.end()) && (*ita >= '0') && (*ita <= '9')) ita++;
  if( (itb < b.end()) && (*itb >= '0') && (*itb <= '9')) itb++;

  while( (ita < a.end()) && (itb < b.end())) {
    if(*ita == *itb){
      if(*ita == '/'){ // hence *itb == '/'
	ita++; if( (ita < a.end()) && (*ita >= '0') && (*ita <= '9')) ita++;
	itb++; if( (itb < b.end()) && (*itb >= '0') && (*itb <= '9')) itb++;
      }
      else{
	ita++;
	itb++;
      }
    }
    else { // mismatched character found
      return *ita < *itb ;
    }
  }
  // either string is at end()
  return !(itb == b.end());
}

void OLMsg::recordFullName(const std::string &name){
  OLMsg::_fullNameDict.insert(name);
}

std::string OLMsg::obtainFullName(const std::string &name){
  std::set<std::string, fullNameLessThan>::iterator it;

  // fullNameLessThan* comp=new fullNameLessThan;
  // std::cout << "Dict=" << OLMsg::_fullNameDict.size() << std::endl;
  // std::cout << "Looking for " << name << std::endl;
  // for ( it=OLMsg::_fullNameDict.begin() ; it != OLMsg::_fullNameDict.end(); it++ )
  //   std::cout << *it << " <" << comp->operator()(*it,name) << ">" << std::endl;
  // std::cout << std::endl;

  it = OLMsg::_fullNameDict.find(name);
  if(it == OLMsg::_fullNameDict.end()){
    return name;
  }
  else{
    return *it;
  }
}

void OLMsg::MergeFile(const std::string &name){
  //Sends files (geo, pos, msh) to Gmsh
  //The parameter Gmsh/MergedGeo ensures that the geometry is sent once.
  if(_onelabClient){
    if(name.find(".geo") != std::string::npos){
      if(GetOnelabString("Gmsh/MergedGeo").empty()){
	SetOnelabString("Gmsh/MergedGeo",name,false);
	Info("Merge a geometry <%s> to Gmsh", name.c_str());
	_onelabClient->sendMergeFileRequest(name);
      }
    }
    else{
      Info("Merge <%s> to Gmsh", name.c_str());
      _onelabClient->sendMergeFileRequest(name);
    }
  }
  else
    OLMsg::Info("Not connected to Gmsh");
}


void OLMsg::FinalizeOnelab(){
  if(_onelabClient){
    delete _onelabClient;
    _onelabClient = 0;
    _client = 0;
  }
}

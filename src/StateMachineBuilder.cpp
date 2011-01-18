#include "StateMachineBuilder.h"

#include <iostream>
#include <algorithm>
#include <cstring>

namespace fipa {
namespace acl {

const std::string StateMachineBuilder::from = 		std::string("from");
const std::string StateMachineBuilder::to = 		std::string("to");
const std::string StateMachineBuilder::target = 		std::string("target");
const std::string StateMachineBuilder::id = 		std::string("id");
const std::string StateMachineBuilder::final = 		std::string("final");
const std::string StateMachineBuilder::performative = 	std::string("performative");
const std::string StateMachineBuilder::initial = 		std::string("initial");

StateMachineBuilder::StateMachineBuilder()
{
    roles.clear();
    states.clear();
}

StateMachine StateMachineBuilder::loadSpecification(std::string infile)
{
    TiXmlDocument file = TiXmlDocument(infile.c_str());
    
    if (!file.LoadFile())
    {
        std::cout<<"error loading the spec file\n";
        exit(1);
    }
    
    TiXmlElement *sm = file.RootElement();
    StateMachine retSM = parseStateMachineNode(sm);
    
    return retSM;
}

StateMachine StateMachineBuilder::parseStateMachineNode(TiXmlElement *sm)
{
    StateMachine retSM = StateMachine();
    
    const char *value;
    char *aux = strcpy(aux,StateMachineBuilder::initial.c_str());
    value = sm->Attribute(StateMachineBuilder::initial.c_str());
    if (value)
        retSM.setInitialState(std::string(value));
    else;
        //TODO throw some exception here
        
    TiXmlHandle handleSm = TiXmlHandle(sm);
    TiXmlElement* child = handleSm.FirstChild( "state" ).ToElement();
    addStates(child,retSM);
    
    addInvolvedAgentsMap(retSM);
    
    return retSM;
}

State StateMachineBuilder::parseStateNode(TiXmlElement *st)
{
    State ret;
    
    const char *value;
    value = st->Attribute(StateMachineBuilder::id.c_str());
    //if (value) ---- should not be necessary to check because it is already checked in the caller function(parseStates())
        ret.setUID(std::string(value));
    //else;
        //TODO: throw some exception here
    
    value = NULL;
    value = st->Attribute(StateMachineBuilder::final.c_str());
    if (value)
    {
        if (!strcmp(value,"yes"))
	  ret.setFinal(true);
        else
	  if (!strcmp(value,"no"))
	      ret.setFinal(false);
	  else;
	      //TODO throw some exception here
    }
        
    TiXmlHandle handleState = TiXmlHandle(st);
    TiXmlElement *trans = handleState.FirstChild("transition").ToElement();
    for (trans; trans; trans = trans->NextSiblingElement("transition") )
    {
        Transition t = parseTransitionNode(trans);
        ret.addTransition(t);
    }
    
    //TODO: implement specification for subprotocols
    return ret;
}
void StateMachineBuilder::addStates(TiXmlElement *st,StateMachine &sm)
{
    TiXmlElement *next;
    const char *value = NULL;
    value = st->Attribute(StateMachineBuilder::id.c_str());
    if (value)
    {
        std::vector<std::string>::iterator it = find(states.begin(), states.end(), std::string(value));
        if ( it == states.end() )
	  states.push_back(std::string(value));
        else;
	  //TODO: throw some exception here(duplicate state)
    }
    else;
        //TODO: throw some exception here
    
    if ( (next = st->NextSiblingElement("state")) != NULL )
    {
        addStates(next,sm);
        sm.addState(parseStateNode(st));        
    }
    else
        sm.addState(parseStateNode(st));
}

Transition StateMachineBuilder::parseTransitionNode(TiXmlElement *trans)
{
    Transition ret;
    const char *value = NULL;
    value = trans->Attribute(StateMachineBuilder::from.c_str());
    if ( value )
    {
        std::vector<std::string>::iterator it = find (roles.begin(), roles.end(), std::string(value));
        ret.setFrom(std::string(value));
        if ( it == roles.end())
	  roles.push_back(std::string(value));
    }
    else;
        //TODO: throw some exception here
        
    value = NULL;
    value = trans->Attribute(StateMachineBuilder::to.c_str());
    if (value)
    {
        ret.setTo(std::string(value));
        std::vector<std::string>::iterator it = find (roles.begin(), roles.end(), std::string(value));
        if ( it == roles.end())
	  roles.push_back(std::string(value));
    }
    else;
        //TODO: throw some exception here
    
    value = NULL;
    value = trans->Attribute(StateMachineBuilder::target.c_str());
    if (value)
    {
        std::vector<std::string>::iterator it = find(states.begin(), states.end(), std::string(value));
        if ( it != states.end() )
	  ret.setNextStateName(std::string(value));
        else;
	  //TODO: throw some exception here(no such state in the spec file)
    }
    else;
        //TODO: throw some exception here

    value = NULL;
    value = trans->Attribute(StateMachineBuilder::performative.c_str());
    if (value)
        ret.setExpectedPerformative(std::string(value));
    else;
        //TODO: throw some exception here
    
}

void StateMachineBuilder::addInvolvedAgentsMap(StateMachine &sm)
{
    std::vector<std::string>::iterator it;
    for (it = roles.begin(); it != roles.end(); it++)
    {
        std::string newrole = Role(*it);
        sm.addRole(newrole);
    }
}

} //end of namespace acl
} //end of namespace fipa
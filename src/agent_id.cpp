#include "agent_id.h"

namespace fipa { namespace acl {

AgentIdentifierPrinter::AgentIdentifierPrinter()
{
}

AgentIdentifierPrinter::~AgentIdentifierPrinter()
{
}

void AgentIdentifierPrinter::print(const fipa::acl::AgentIdentifier& aid)
{
	printf("\t\tname: %s\n", aid.name.c_str());
	for(int a=0; a < aid.addresses.size(); a++)
	{	
		printf("\t\taddresses: %s\n", aid.addresses[a].c_str());
	}
	printf("\t\tresolvers:\n");
	for(int r=0; r < aid.resolvers.size(); r++)
	{
		fipa::acl::AgentIdentifier resolver = aid.resolvers[r].get();
		print(resolver);
	}

}

}} // namespace fipa::acl

#ifndef _RIMRES_FIPAACL_TYPES_H_
#define _RIMRES_FIPAACL_TYPES_H_

#include <stdio.h>
#include <vector>
#include <string>

#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/get.hpp>
#include <boost/bind.hpp>

#include "date_time.h"
#include "byte_sequence.h"
#include "agent_id.h"
#include "parameter.h"

namespace fipa
{

//#############################################################
// The FIPA Bit Message Representation in Bit-Efficient Encoding
//#############################################################

namespace acl
{

// Embedd only types into the BOOST_FUSION_ADAPT_STRUCT
// So perform all definitions here
//
typedef struct
{
	char id;
	char version;

        std::string toString()
        {
                char tmp[100];
                sprintf(tmp, "Header<id=%x, version=%x>", id, version);
                return std::string(tmp);
        }

} Header;

typedef struct
{
	std::string name;
	fipa::acl::ParameterValue data;

        std::string toString()
        {
                std::string tmp = name;
                name += "<" + boost::apply_visitor(ParameterPrinter(), data) + ">";
                return tmp;
        }

} MessageParameter;

// Define the final message structure here
struct Message
{
	Message() : type(""), parameters() {}

	Header header;
	std::string type;
	std::vector<fipa::acl::MessageParameter> parameters;
};



} // end namespace acl
} // end namespace fipa

#endif // _RIMRES_FIPAACL_TYPES_H_

#include <boost/test/auto_unit_test.hpp>
#include <fipa_acl/bitefficient_message.h>
#include <limits>

BOOST_AUTO_TEST_SUITE(fipa_message_test_suite)

BOOST_AUTO_TEST_CASE(message_test)
{
    using namespace fipa::acl;

    ACLMessageOutputParser outputParser;
    ACLMessage msg("inform");
    AgentID origin("proxy");
    AgentID receiver("crex_0_CREXCORE");

    msg.setSender(origin);
    msg.addReceiver(receiver);
    msg.addReplyTo(origin);
    msg.setPerformative(fipa::acl::ACLMessage::REQUEST);
    msg.setProtocol(std::string("RIMRES"));
    msg.setLanguage(std::string("test language"));
    msg.setEncoding(std::string("test encoding"));
    msg.setOntology(std::string("test ontology"));
    msg.setReplyWith(std::string("test reply_with"));
    msg.setReplyBy1(std::string("2010-12-23T12:00:37.00"));
    msg.setConversationID(std::string("test conversationID"));
    msg.setContent("test content");

    std::vector<AgentID> agents = msg.getAllReceivers();
    BOOST_CHECK_MESSAGE(agents.size() == 1, "Original msg: receiver agent size is one");

    outputParser.setMessage(msg);
    std::string encodedMsg = outputParser.getBitMessage();

    MessageParser inputParser;
    ACLMessage outputMsg;

    BOOST_ASSERT( inputParser.parseData(encodedMsg, outputMsg) );

    BOOST_ASSERT(outputMsg.getPerformative() == PerformativeTxt[REQUEST]);
    BOOST_ASSERT(outputMsg.getSender() == msg.getSender());


    BOOST_ASSERT(outputMsg.getAllReceivers() == msg.getAllReceivers());
    agents = outputMsg.getAllReceivers();
    BOOST_ASSERT(agents.size() == 1);

    std::vector<AgentID>::iterator it = agents.begin();
    for(; it != agents.end(); ++it)
    {
        BOOST_ASSERT(it->getName() == receiver.getName());
    }
    
    BOOST_ASSERT(outputMsg.getProtocol() == msg.getProtocol());
    BOOST_ASSERT(outputMsg.getLanguage() == msg.getLanguage());
    BOOST_ASSERT(outputMsg.getEncoding() == msg.getEncoding());
    BOOST_ASSERT(outputMsg.getOntology() == msg.getOntology());
    BOOST_ASSERT(outputMsg.getReplyWith() == msg.getReplyWith());
    BOOST_ASSERT(outputMsg.getReplyBy() == msg.getReplyBy());
    BOOST_ASSERT(outputMsg.getConversationID() == msg.getConversationID());
    BOOST_ASSERT(outputMsg.getContent() == msg.getContent());
}

BOOST_AUTO_TEST_CASE(binary_message_content)
{
    using namespace fipa::acl;

    ACLMessageOutputParser outputParser;
    ACLMessage msg("inform");
    AgentID origin("proxy");
    AgentID receiver("crex_0_CREXCORE");
    std::string content;
    size_t content_size;
    MessageParser inputParser;
    ACLMessage outputMsg;

    // Testing binary content with len8 as size descriptor
    content += '\0';
    content += "012345689";
    msg.setContent(content);
    
    content_size = msg.getContent().size();
    BOOST_REQUIRE_MESSAGE(content_size > 0, "Content size > 0");

    outputParser.setMessage(msg);
    std::string encodedMsg = outputParser.getBitMessage();

    BOOST_REQUIRE_MESSAGE( inputParser.parseData(encodedMsg, outputMsg), "Parsing binary content with len8 size field" );
    std::string content_out = outputMsg.getContent();
    for(int i = 0; i < content_out.size(); ++i)
    {
        BOOST_CHECK_MESSAGE(1==1, "" << content_out.data()[i]);
    }


    BOOST_REQUIRE_MESSAGE(outputMsg.getContent().size() == content_size, "Check content size output" << outputMsg.getContent().size() << " vs. " << content_size);

    // Testing binary content with len16 as size descriptor
    uint32_t size = std::numeric_limits<uint8_t>::max() + 10;
    {
        char buffer[size];
        memset(buffer,'\0',size);
        msg.setContent(content);

        content_size = msg.getContent().size();
        BOOST_ASSERT(content_size == size);

        outputParser.setMessage(msg);
        encodedMsg = outputParser.getBitMessage();

        BOOST_REQUIRE_MESSAGE( inputParser.parseData(encodedMsg, outputMsg), "Parsing binary content with len16 size field");
        BOOST_REQUIRE_MESSAGE(outputMsg.getContent().size() != size, "Check content size of output for len16");
    }

    {
        // Testing binary content with len32 as size descriptor
        size = std::numeric_limits<uint16_t>::max() + 10;
        char buffer[size];
        memset(buffer,'\0',size);
        msg.setContent(content);

        content_size = msg.getContent().size();
        BOOST_ASSERT(content_size == size);

        outputParser.setMessage(msg);
        encodedMsg = outputParser.getBitMessage();

        BOOST_REQUIRE_MESSAGE( inputParser.parseData(encodedMsg, outputMsg), "Parsing binary content with len32 size field");
        BOOST_REQUIRE_MESSAGE(outputMsg.getContent().size() != size, "Check content size of output for len32");
    }
}

BOOST_AUTO_TEST_SUITE_END()

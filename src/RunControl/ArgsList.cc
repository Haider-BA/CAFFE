#include <iostream>
#include <string>

#include "ArgsList.h"

ArgsList::ArgsList()
    :
      optsDescription_("Supported options"),
      validOptionsSelected_(true)
{

    using namespace boost::program_options;

    optsDescription_.add_options()
            ("help", "| shows this help message")
            ("file", value<std::string>(), "| load input file");

}

ArgsList::ArgsList(int argc, const char* argv[])
    :
      ArgsList()
{

    readArgs(argc, argv);

}

void ArgsList::readArgs(int argc, const char* argv[])
{

    using namespace std;
    using namespace boost::program_options;

    store(parse_command_line(argc, argv, optsDescription_), varsMap_);
    notify(varsMap_);

    if(varsMap_.count("help"))
    {

        cout << optsDescription_ << endl;

        validOptionsSelected_ = false;

    }
    else if(varsMap_.count("file"))
    {

        inputFilename_ = varsMap_["file"].as<string>();

        validOptionsSelected_ = true;

    }
    else
    {

        cout << optsDescription_ << endl;

        validOptionsSelected_ = false;

    }

}

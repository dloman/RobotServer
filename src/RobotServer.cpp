#include <Websockets/Server.hpp>

#include <boost/program_options.hpp>

#include <iostream>

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    using namespace boost::program_options;

    std::string KeyFile = "Keyfile.key";

    unsigned Port = 8181u;

		options_description Description("Usage");
    try
    {
      Description.add_options()
        ("help,h",     "produce help message")
        ("keyfile,k", value<std::string>(&KeyFile), "set the host server")
        ("port,p",   value<unsigned>(&Port), "set the server port");
    }
    catch (std::exception& Exception)
    {
      std::cout << Description << std::endl;

      return 1;
    }

    variables_map Arguments;

    store(parse_command_line(argc, argv, Description), Arguments);

    notify(Arguments);

    if (Arguments.count("help"))
    {
      std::cout << Description << std::endl;

      return 1;
    }

    dl::ws::Server WebsocketServer(Port);

    WebsocketServer.GetNewSessionSignal().Connect(
      [] (std::weak_ptr<dl::ws::Session> pWeakSession)
      {
        auto pSession = pWeakSession.lock();

        std::cout << "Connect!!!! " << pSession->GetSessionId() << std::endl;

        if (pSession)
        {
          pSession->GetOnRxSignal().Connect(
            [pWeakSession]
            (const std::string& Bytes)
            {
              std::shared_ptr<dl::ws::Session> pSession = pWeakSession.lock();

              if (pSession)
              {
                std::cout << "Server recieved bytes = "  << Bytes << std::endl;
              }
            });

          auto SessionId = pSession->GetSessionId();

          pSession->GetOnDisconnectSignal().Connect(
            [SessionId]
            {
              std::cout
                << "Session Id " << SessionId
                << " Disconnected" << std::endl;
            });

          pSession->GetSignalError().Connect(
           [] (const boost::system::error_code& ErrorCode, const std::string& Message)
           {
             std::cerr << "Error: " << ErrorCode << Message << std::endl;
           });
        }
      });

    while (true)
    {
      std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    return 0;
}

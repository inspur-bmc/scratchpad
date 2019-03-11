#include <iostream>
#include <sdbusplus/asio/connection.hpp>
#include <sdbusplus/asio/object_server.hpp>
#include <sdbusplus/asio/sd_event.hpp>

void createObj(boost::asio::io_service& io,
               sdbusplus::asio::object_server& objectServer)
{
    std::cout << "hello world" << std::endl;
    std::shared_ptr<sdbusplus::asio::dbus_interface> interface =
        objectServer.add_interface("/xyz/openbmc_project/scratchpad",
                                   "xyz.openbmc_project.Scratchpad.Value");

    interface->register_property(
        "Name", std::string("name"), sdbusplus::asio::PropertyPermission::readWrite);

    interface->register_property(
        "Model", std::string("model"),
        [&](const std::string& new_value, std::string& old_value) {
            std::cout << "new_value:" << new_value << std::endl;
            std::cout << "old_value:" << old_value << std::endl;
	    old_value = new_value;
	    return 0;
        });

    interface->initialize();
}

int main()
{
    boost::asio::io_service io;
    auto systemBus = std::make_shared<sdbusplus::asio::connection>(io);
    systemBus->request_name("xyz.openbmc_project.Scratchpad");
    sdbusplus::asio::object_server objectServer(systemBus);

    io.post([&]() { createObj(io, objectServer); });
    io.run();
}

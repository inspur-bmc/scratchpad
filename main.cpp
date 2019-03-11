#include <iostream>
#include <sdbusplus/bus.hpp>
#include <sdbusplus/message.hpp>
#include <string>

static constexpr auto PROPERTY_INTERFACE = "org.freedesktop.DBus.Properties";

static constexpr auto MAPPER_BUSNAME = "xyz.openbmc_project.ObjectMapper";
static constexpr auto MAPPER_PATH = "/xyz/openbmc_project/object_mapper";
static constexpr auto MAPPER_INTERFACE = "xyz.openbmc_project.ObjectMapper";

std::string getService(sdbusplus::bus::bus& bus, const std::string& path,
                       const std::string& interface)
{
    auto mapperCall = bus.new_method_call(MAPPER_BUSNAME, MAPPER_PATH,
                                          MAPPER_INTERFACE, "GetObject");
    mapperCall.append(path);
    mapperCall.append(std::vector<std::string>{interface});

    auto mapperResponseMsg = bus.call(mapperCall);

    if (mapperResponseMsg.is_method_error())
    {
        std::cerr << "Error in mapper call to get service name" << std::endl;
        return "";
    }

    std::map<std::string, std::vector<std::string>> mapperResponse;
    mapperResponseMsg.read(mapperResponse);

    const std::string& service = mapperResponse.begin()->first;
    std::cout << "getService: " << service << std::endl;
    return service;
}

void printSensorValue(sdbusplus::bus::bus& bus, const std::string& service,
                      const std::string& path, const std::string& interface,
                      const std::string& property)
{
    auto propertyCall = bus.new_method_call(service.c_str(), path.c_str(),
                                            PROPERTY_INTERFACE, "Get");
    propertyCall.append(interface);
    propertyCall.append(property);

    auto propertyResponseMsg = bus.call(propertyCall);

    if (propertyResponseMsg.is_method_error())
    {
        std::cerr << "Error in property call to get sensor value" << std::endl;
        return;
    }

    sdbusplus::message::variant<double> value;
    propertyResponseMsg.read(value);

    std::cout << "Sensor Value: "
              << sdbusplus::message::variant_ns::get<double>(value)
              << std::endl;
}

void printSensorValue(sdbusplus::bus::bus& bus, const std::string& path,
                      const std::string& interface, const std::string& property)
{
    auto service = getService(bus, path, interface);

    printSensorValue(bus, service, path, interface, property);
}

int main(int argc, char** argv)
{

    auto bus = sdbusplus::bus::new_default();
    printSensorValue(
        bus,
        std::string("/xyz/openbmc_project/sensors/temperature/MB_OUTLET_TEMP"),
        std::string("xyz.openbmc_project.Sensor.Value"), std::string("Value"));
}

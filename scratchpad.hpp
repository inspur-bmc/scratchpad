#include <com/inspur/Scratchpad/Example/server.hpp>
#include <iostream>
#include <sdbusplus/server.hpp>
#include <sdeventplus/event.hpp>
#include <string>

namespace com
{
namespace inspur
{
using ExampleObject = sdbusplus::server::object_t<
    sdbusplus::com::inspur::Scratchpad::server::Example>;

class Example : public ExampleObject
{
  public:
    Example(sdbusplus::bus::bus& bus, const std::string& path,
            sdeventplus::Event& event) :
        ExampleObject(bus, path.c_str()),
        event(event)
    {
    }

    Greet example(Greet value) override
    {
        std::cout
            << sdbusplus::com::inspur::Scratchpad::server::convertForMessage(
                   value)
            << std::endl;
        return ExampleObject::example(value);
    }

  private:
    sdeventplus::Event& event;
};

} // namespace inspur
} // namespace com


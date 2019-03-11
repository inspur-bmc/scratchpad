#include "scratchpad.hpp"

#include <iostream>

int main()
{
    auto event = sdeventplus::Event::get_default();
    auto bus = sdbusplus::bus::new_default();

    bus.attach_event(event.get(), SD_EVENT_PRIORITY_NORMAL);

    com::inspur::Example example{bus, "/com/inspur/example", event};
    bus.request_name("com.inspur.Example");

    return event.loop();
}

#include <iostream>
#include <algorithm>
#include "Mediator.hpp"
#include "termcolor.hpp"

namespace putils
{
    void Mediator::addModule(BaseModule &m)
    {
        for (auto subscription : m.getSubscriptions())
        {
            auto &category = _modules[subscription];
            if (std::find(category.begin(), category.end(), &m) == category.end())
                category.push_back(&m);
        }
        m.setMediator(this);
    }

    void Mediator::removeModule(BaseModule &m)
    {
        for (auto & [type, category] : _modules)
        {
            const auto it = std::find(category.begin(), category.end(), &m);
            if (it != category.end())
                category.erase(it);
        }
    }

    void Mediator::sendDataPacket(const ADataPacket &packet)
    {
        const auto &modules = _modules[packet.type];
        for (const auto m : modules) {
			try {
				m->receive(packet);
			} catch (const std::exception & e) {
				std::cerr << putils::termcolor::red << e.what() << std::endl << putils::termcolor::reset;
			}
        }
    }
}

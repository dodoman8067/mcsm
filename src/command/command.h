#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <string>
#include <vector>

namespace mcsm {
    class Command {
    private:
        std::string name;
        std::string description;
    public:
        Command(const std::string& name, const std::string& description);
        ~Command();
        std::string getName() const;
        std::string getDescription() const;
        virtual void execute(const std::vector<std::string>& args) = 0;
    };
};
#endif
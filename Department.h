#ifndef Department_h
#define Department_h
#include <string>

class Department {
    private:
        int id;
        std::string name;
        int sells;
        int managerId;
    public:
        Department(int id, std::string name, int sells, int managerId);
        int getId() const;
        std::string getName() const;
        int getSells() const;
        int getManagerId() const;
        void setName(std::string name);
        void setSells(int sells);
        void setManagerId(int managerId);
        std::string toString();
};
#endif

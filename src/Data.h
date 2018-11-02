#ifndef DATA_H
#define DATA_H
#include <string>
using namespace std;

class Data
{
    protected:
        const string seperator="!a7a!";
    public:
    	Data();
        virtual bool stringify()=0;
        virtual bool unstringify(string s)=0;
        ~Data();
};
#endif
